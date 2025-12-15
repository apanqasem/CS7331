// mpi_summa.c
// 2D block-distributed SUMMA with nonblocking row/col Ibcast (double-buffered).
// Build: mpicc -O2 -std=c11 mpi_summa.c -o mpi_summa
// Run:   mpirun -np <P> ./mpi_summa [N] [Prow] [Pcol]
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

static inline size_t idx(int r, int c, int ld) { return (size_t)r * (size_t)ld + (size_t)c; }

static double* xmalloc(size_t n) {
    double *p = (double*)malloc(n * sizeof(double));
    if (!p) { fprintf(stderr, "malloc failed for %zu doubles\n", n); MPI_Abort(MPI_COMM_WORLD, 1); }
    return p;
}

// Same initialization rule used before, but computed from global (i,j)
static inline double valA(long long gi, long long gj) { return (double)(((gi + gj) % 100)) / 10.0; }
static inline double valB(long long gi, long long gj) { return (double)((((gi - gj) % 100) + 100) % 100) / 10.0; }

static void init_local_tiles(double *Aloc, double *Bloc, int bs, int prow, int pcol, int Prow, int Pcol) {
    // Local tile corresponds to global rows [prow*bs : (prow+1)*bs) and cols [pcol*bs : (pcol+1)*bs)
    long long row0 = (long long)prow * bs;
    long long col0 = (long long)pcol * bs;
    for (int i = 0; i < bs; ++i) {
        for (int j = 0; j < bs; ++j) {
            Aloc[idx(i,j,bs)] = valA(row0 + i, col0 + j);
            Bloc[idx(i,j,bs)] = valB(row0 + i, col0 + j);
        }
    }
}

static void zero_tile(double *C, int bs) {
    for (int i = 0; i < bs*bs; ++i) C[i] = 0.0;
}

static void dgemm_local(const double *A, const double *B, double *C, int bs) {
    // Simple i-j-k triple loop for the tile (bs x bs)
    for (int i = 0; i < bs; ++i) {
        for (int k = 0; k < bs; ++k) {
            double aik = A[idx(i,k,bs)];
            for (int j = 0; j < bs; ++j) {
                C[idx(i,j,bs)] += aik * B[idx(k,j,bs)];
            }
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int world, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Parse inputs
    int N = (argc > 1) ? atoi(argv[1]) : 8192;
    int Prow = 0, Pcol = 0;
    if (argc > 3) {
        Prow = atoi(argv[2]);
        Pcol = atoi(argv[3]);
        if (Prow * Pcol != world) {
            if (rank == 0) fprintf(stderr, "Error: Prow*Pcol must equal world size.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    } else {
        // Choose near-square grid
        int s = (int)(sqrt((double)world) + 0.5);
        while (s > 1 && world % s != 0) --s;
        Prow = s;
        Pcol = world / s;
    }
    if (rank == 0) {
        printf("SUMMA: N=%d, P=%d (%dx%d grid)\n", N, world, Prow, Pcol);
    }

    if (N % Prow != 0 || N % Pcol != 0) {
        if (rank == 0) fprintf(stderr, "Require N %% Prow == 0 and N %% Pcol == 0.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Create 2D Cartesian communicator (no wrap)
    int dims[2] = {Prow, Pcol};
    int periods[2] = {0, 0};
    MPI_Comm cart;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cart);

    int coords[2];
    MPI_Cart_coords(cart, rank, 2, coords);
    int my_row = coords[0], my_col = coords[1];

    // Create row and column communicators
    MPI_Comm row_comm, col_comm;
    MPI_Comm_split(cart, my_row, my_col, &row_comm);
    MPI_Comm_split(cart, my_col, my_row, &col_comm);

    // Local block size
    const int bs_r = N / Prow;
    const int bs_c = N / Pcol;
    if (bs_r != bs_c) {
        if (rank == 0) fprintf(stderr, "This simple demo expects square tiles: N/Prow == N/Pcol.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    const int bs = bs_r;

    // Allocate local A(pr,pc), B(pr,pc), C(pr,pc)
    double *Aloc = xmalloc((size_t)bs * bs);
    double *Bloc = xmalloc((size_t)bs * bs);
    double *Cloc = xmalloc((size_t)bs * bs);
    init_local_tiles(Aloc, Bloc, bs, my_row, my_col, Prow, Pcol);
    zero_tile(Cloc, bs);

    // Double buffers for broadcasted tiles
    double *Abuf[2] = { xmalloc((size_t)bs * bs), xmalloc((size_t)bs * bs) };
    double *Bbuf[2] = { xmalloc((size_t)bs * bs), xmalloc((size_t)bs * bs) };

    // Requests for nonblocking broadcasts
    MPI_Request reqA[2] = { MPI_REQUEST_NULL, MPI_REQUEST_NULL };
    MPI_Request reqB[2] = { MPI_REQUEST_NULL, MPI_REQUEST_NULL };

    // Pre-post broadcasts for step 0 into buffer 0
    int buf = 0;
    if (my_col == 0) {
        // Root for A in this row is column=0 (first step)
        for (int i = 0; i < bs*bs; ++i) Abuf[buf][i] = Aloc[i]; // my tile A(row,0)
    }
    if (my_row == 0) {
        for (int i = 0; i < bs*bs; ++i) Bbuf[buf][i] = Bloc[i]; // my tile B(0,col)
    }

    // Nonblocking broadcasts within row and column communicators
    MPI_Ibcast(Abuf[buf], bs*bs, MPI_DOUBLE, /*root=*/0, row_comm, &reqA[buf]);
    MPI_Ibcast(Bbuf[buf], bs*bs, MPI_DOUBLE, /*root=*/0, col_comm, &reqB[buf]);

    // SUMMA loop over k panels
    for (int k = 0; k < Pcol; ++k) {
        // Wait until current A(row,k) and B(k,col) tiles are available
        MPI_Wait(&reqA[buf], MPI_STATUS_IGNORE);
        MPI_Wait(&reqB[buf], MPI_STATUS_IGNORE);

        // Compute with current buffers
        dgemm_local(Abuf[buf], Bbuf[buf], Cloc, bs);

        // Prepare next step (k+1) if any
        int next = buf ^ 1;
        if (k + 1 < Pcol) {
            // If I'm the row owner of the next A tile (col == k+1), copy my local A(row,k+1)
            if (my_col == (k + 1)) {
                for (int i = 0; i < bs*bs; ++i) Abuf[next][i] = Aloc[i];
            }
            // If I'm the col owner of the next B tile (row == k+1), copy my local B(k+1,col)
            if (my_row == (k + 1)) {
                for (int i = 0; i < bs*bs; ++i) Bbuf[next][i] = Bloc[i];
            }

            // Nonblocking broadcasts for step k+1:
            // In row_comm, the root rank (relative) is 'k+1'
            // In col_comm, the root rank (relative) is 'k+1'
            MPI_Ibcast(Abuf[next], bs*bs, MPI_DOUBLE, /*root=*/k+1, row_comm, &reqA[next]);
            MPI_Ibcast(Bbuf[next], bs*bs, MPI_DOUBLE, /*root=*/k+1, col_comm, &reqB[next]);
        }

        // Rotate buffers
        buf ^= 1;
    }

    // Simple checksum for sanity (sum of my local C). Rank 0 prints global sum.
    double local_sum = 0.0;
    for (int i = 0; i < bs*bs; ++i) local_sum += Cloc[i];
    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, cart);
    if (rank == 0) {
        printf("Checksum(C) across all ranks: %.6f\n", global_sum);
    }

    free(Aloc); free(Bloc); free(Cloc);
    free(Abuf[0]); free(Abuf[1]); free(Bbuf[0]); free(Bbuf[1]);
    MPI_Comm_free(&row_comm);
    MPI_Comm_free(&col_comm);
    MPI_Comm_free(&cart);
    MPI_Finalize();
    return 0;
}
