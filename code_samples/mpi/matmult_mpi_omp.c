// mpi_omp_scatter_gather.c
// Hybrid MPI + OpenMP: row-wise A scatter, full B broadcast, row-wise C gather.
// Build: mpicc -O2 -fopenmp -std=c11 mpi_omp_scatter_gather.c -o mpi_omp_sg
// Run:   mpirun -np <P> ./mpi_omp_sg [N] [threads] [tol]
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

static double* xmalloc(size_t n) {
    double *p = (double*)malloc(n * sizeof(double));
    if (!p) { fprintf(stderr, "alloc failed for %zu doubles\n", n); MPI_Abort(MPI_COMM_WORLD, 1); }
    return p;
}

static void init_mats(double *A, double *B, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[(size_t)i*N + j] = (double)((i + j) % 100) / 10.0;
            B[(size_t)i*N + j] = (double)((((i - j) % 100) + 100) % 100) / 10.0;
        }
    }
}

static void zero_mat(double *M, size_t elems) {
    #pragma omp parallel for
    for (long long i = 0; i < (long long)elems; ++i) M[i] = 0.0;
}

// Local C_blk = A_blk * B  (A_blk: rows_local x N, B: N x N, C_blk: rows_local x N)
static void matmul_local_omp(const double *Ablk, const double *B, double *Cblk, int rows_local, int N) {
    #pragma omp parallel for
    for (int i = 0; i < rows_local; ++i) {
        for (int j = 0; j < N; ++j) {
            double s = 0.0;
            for (int k = 0; k < N; ++k) {
                s += Ablk[(size_t)i*N + k] * B[(size_t)k*N + j];
            }
            Cblk[(size_t)i*N + j] = s;
        }
    }
}

int main(int argc, char **argv) {
    // Request a thread level that allows only main thread to call MPI
    int provided = 0;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
    if (provided < MPI_THREAD_FUNNELED) {
        fprintf(stderr, "MPI does not provide required thread level\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    int N = (argc > 1) ? atoi(argv[1]) : 8192;
    int T = (argc > 2) ? atoi(argv[2]) : omp_get_max_threads();
    double tol = (argc > 3) ? atof(argv[3]) : 1e-12;
    omp_set_num_threads(T);
    if (rank == 0) {
        printf("Hybrid MPI+OpenMP scatter/gather: N=%d, ranks=%d, threads/rank=%d\n", N, P, T);
    }

    // Root allocates full A,B,C for init/checksum (C only for gathering)
    double *Aroot = NULL, *B = NULL, *Croot = NULL;
    if (rank == 0) {
        Aroot = xmalloc((size_t)N * N);
        B     = xmalloc((size_t)N * N);
        Croot = xmalloc((size_t)N * N);
        init_mats(Aroot, B, N);
    } else {
        B = xmalloc((size_t)N * N);   // everyone needs full B
    }

    // Broadcast B to all ranks
    MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Compute uneven row partition (supports any N, P)
    int *rows = xmalloc((size_t)P);
    int *disp_rows = xmalloc((size_t)P);
    int base = N / P, rem = N % P;
    for (int p = 0; p < P; ++p) rows[p] = base + (p < rem ? 1 : 0);
    disp_rows[0] = 0;
    for (int p = 1; p < P; ++p) disp_rows[p] = disp_rows[p-1] + rows[p-1];

    // Build counts/displs in elements for Scatterv/Gatherv (row blocks)
    int *sendcounts = xmalloc((size_t)P);
    int *displsA    = xmalloc((size_t)P);
    int *recvcounts = xmalloc((size_t)P);
    int *displsC    = xmalloc((size_t)P);
    for (int p = 0; p < P; ++p) {
        sendcounts[p] = rows[p] * N;
        displsA[p]    = disp_rows[p] * N;
        recvcounts[p] = rows[p] * N;
        displsC[p]    = disp_rows[p] * N;
    }

    // Local buffers
    int rows_local = rows[rank];
    double *Ablk = xmalloc((size_t)rows_local * N);
    double *Cblk = xmalloc((size_t)rows_local * N);
    zero_mat(Cblk, (size_t)rows_local * N);

    // Scatter rows of A
    MPI_Scatterv(
        Aroot, sendcounts, displsA, MPI_DOUBLE,
        Ablk,  rows_local * N, MPI_DOUBLE,
        0, MPI_COMM_WORLD
    );

    // Local parallel multiply
    matmul_local_omp(Ablk, B, Cblk, rows_local, N);

    // Gather rows of C back to root
    MPI_Gatherv(
        Cblk, rows_local * N, MPI_DOUBLE,
        Croot, recvcounts, displsC, MPI_DOUBLE,
        0, MPI_COMM_WORLD
    );

    // Small checksum (and optional correctness check vs recomputed baseline on root)
    if (rank == 0) {
        double chk = 0.0;
        for (long long i = 0; i < (long long)N*N; ++i) chk += Croot[i];
        printf("Checksum(C): %.6f\n", chk);

        // Optional quick self-check (baseline re-multiply for tiny N)
        if (N <= 1024) {
            double *Cref = xmalloc((size_t)N*N);
            zero_mat(Cref, (size_t)N*N);
            // Simple serial baseline
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    double s = 0.0;
                    for (int k = 0; k < N; ++k) s += Aroot[(size_t)i*N+k]*B[(size_t)k*N+j];
                    Cref[(size_t)i*N+j] = s;
                }
            }
            double maxabs = 0.0, num=0.0, den=0.0;
            for (long long i = 0; i < (long long)N*N; ++i) {
                double d = fabs(Cref[i] - Croot[i]);
                if (d > maxabs) maxabs = d;
                num += d*d; den += Cref[i]*Cref[i];
            }
            double rel = (den>0.0)? sqrt(num/den) : sqrt(num);
            printf("Validation: max|diff|=%.3e, relL2=%.3e (tol=%.1e)\n", maxabs, rel, tol);
            free(Cref);
        }
    }

    // Cleanup
    free(Ablk); free(Cblk); free(B);
    if (rank == 0) { free(Aroot); free(Croot); }
    free(rows); free(disp_rows); free(sendcounts); free(displsA); free(recvcounts); free(displsC);

    MPI_Finalize();
    return 0;
}
