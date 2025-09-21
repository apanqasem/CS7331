// mpi_mm.c
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

static double* alloc_matrix(size_t elems) {
    double *M = (double*)malloc(elems * sizeof(double));
    if (!M) { fprintf(stderr, "Allocation failed\n"); MPI_Abort(MPI_COMM_WORLD, 1); }
    return M;
}

static void init_matrices(double *A, double *B, int N) {
    // Only called on root for A,B initialization
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i*N + j] = (double)((i + j) % 100) / 10.0;
            B[i*N + j] = (double)((i - j + 100) % 100) / 10.0;
        }
    }
}

static void local_matmul(const double *Ablk, const double *B, double *Cblk, int N, int rows) {
    // Compute rows of C corresponding to Ablk
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0.0;
            for (int k = 0; k < N; ++k) {
                sum += Ablk[i*N + k] * B[k*N + j];
            }
            Cblk[i*N + j] = sum;
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N = (argc > 1) ? atoi(argv[1]) : 1024;
    if (N % world_size != 0) {
        if (rank == 0) fprintf(stderr, "For simplicity, require N %% world_size == 0.\n");
        MPI_Finalize();
        return 1;
    }

    int rows_per_rank = N / world_size;

    double *Aroot = NULL;
    double *B = alloc_matrix((size_t)N * N);
    double *Ablk = alloc_matrix((size_t)rows_per_rank * N);
    double *Cblk = alloc_matrix((size_t)rows_per_rank * N);
    double *Croot = NULL;

    if (rank == 0) {
        Aroot = alloc_matrix((size_t)N * N);
        Croot = alloc_matrix((size_t)N * N);
        init_matrices(Aroot, B, N);
    }

    // Broadcast B to all ranks
    MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Scatter rows of A to each rank
    MPI_Scatter(
        (rank == 0) ? Aroot : NULL, rows_per_rank*N, MPI_DOUBLE,
        Ablk, rows_per_rank*N, MPI_DOUBLE,
        0, MPI_COMM_WORLD
    );

    // Local multiply
    local_matmul(Ablk, B, Cblk, N, rows_per_rank);

    // Gather rows of C back to root
    MPI_Gather(
        Cblk, rows_per_rank*N, MPI_DOUBLE,
        (rank == 0) ? Croot : NULL, rows_per_rank*N, MPI_DOUBLE,
        0, MPI_COMM_WORLD
    );

    if (rank == 0) {
        // Simple checksum
        double chk = 0.0;
        for (long long i = 0; i < (long long)N*N; ++i) chk += Croot[i];
        printf("MPI: N=%d, ranks=%d, Checksum: %.6f\n", N, world_size, chk);
    }

    free(B);
    free(Ablk);
    free(Cblk);
    if (rank == 0) { free(Aroot); free(Croot); }

    MPI_Finalize();
    return 0;
}
