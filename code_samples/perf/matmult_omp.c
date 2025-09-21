#include <stdio.h>
#include <stdlib.h>
#include<sys/time.h>

#include <omp.h>

double t0;
double mysecond() {
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

static double* alloc_matrix(int N) {
  double *M = (double*)malloc((size_t)N * N * sizeof(double));
  if (!M) { fprintf(stderr, "Allocation failed\n"); exit(1); }
  return M;
}

static void init_matrices(double *A, double *B, int N) {
#pragma omp parallel for
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      A[i*N + j] = (double)((i + j) % 100) / 10.0;
      B[i*N + j] = (double)((i - j + 100) % 100) / 10.0;
    }
  }
}

static void zero_matrix(double *C, int N) {
#pragma omp parallel for
  for (int i = 0; i < N*N; ++i) C[i] = 0.0;
}

static void matmul_openmp(const double *A, const double *B, double *C, int N) {
  // Parallelize the i,j loops. Each (i,j) computes a private sum.
#pragma omp parallel for
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      double sum = 0.0;
      for (int k = 0; k < N; ++k) {
	sum += A[i*N + k] * B[k*N + j];
      }
      C[i*N + j] = sum;
    }
  }
}

int main(int argc, char **argv) {

  if (argc < 4) {
    printf("usage: \n");
    printf("       ./matmult N REPS THREADS\n");
    exit(0);
  }

  long long N = atoi(argv[1]);
  unsigned REPS = atoi(argv[2]);
  unsigned THREADS = atoi(argv[3]);


  omp_set_num_threads(THREADS);
  
  double *A = alloc_matrix(N);
  double *B = alloc_matrix(N);
  double *C = alloc_matrix(N);
  
  init_matrices(A, B, N);
  zero_matrix(C, N);
  
  t0 = mysecond();
  for (int i = 0; i < REPS; i++)
    matmul_openmp(A, B, C, N);
  t0 = (mysecond() - t0) * 1.e3;
  
  // crude validation 
  double chk = 0.0;
  for (int i = 0; i < N * N; ++i)
    chk += C[i];
  printf("Checksum: %.6f\n", chk);

  printf("Execution Time = %3.2f ms\n", t0);
  
  free(A);
  free(B);
  free(C);
  return 0;
}
