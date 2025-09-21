/* 
 * Performs sum-reduction of two vectors and stores the result in a scalar 
 * 
 * Demo of OpenMP reduction clause
 */

#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include <omp.h>

#define REPS 100000

double t0;

double mysecond() {
  struct timeval tp;
  struct timezone tzp;
  int i;
  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

int main(int argc, char *argv[]) {
  
  if (argc < 3) {
    fprintf(stderr, "Usage: ./sum_reduce N T\n");
    fprintf(stderr, "    N = size of vector\n");
    fprintf(stderr, "    T = number of threads\n");
    exit(0);
  }

  int N = atoi(argv[1]);          // size of vectors 
  int threads = atoi(argv[2]);    // number of OpenMP threads

  double*a, *b, *c;
  a = (double*) malloc(sizeof(double) * N);
  b = (double*) malloc(sizeof(double) * N);
  c = (double*) malloc(sizeof(double) * N);
  
  int i, j, k;
  for (i = 0; i < N; i++) {
    a[i] = 1.0; 
    b[i] = 2.0; 
  }

  omp_set_num_threads(N);

  double sum = 0;
  t0 = mysecond();
  for (j = 0; j < REPS; j++) {
    sum = 0;
#pragma omp simd reduction (sum:+)
    for (i = 0; i < N; i++)
	sum +=  a[i] + b[i];
  }
  t0 = (mysecond() - t0) * 1.e3;

  fprintf(stdout, "result = %1.3e\n", sum);
  fprintf(stdout, "parallel loop = %3.2f ms\n", t0);

  return 0;

}
