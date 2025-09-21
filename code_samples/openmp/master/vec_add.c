/* 
 * Performs element-wise addition of two vectors and stores the result in a third vector
 * 
 * Demo of OpenMP simd clause
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
  
  int num_threads;
  if (argc < 3) {
    fprintf(stderr, "Usage: ./sum_reduce N T\n");
    fprintf(stderr, "    N = size of vectors\n");
    fprintf(stderr, "    T = number of threads\n");
    exit(0);
  }

  int N = atoi(argv[1]);  // size of vectors 
  int threads = atoi(argv[2]);  // number of OpenMP threads

  double*a, *b, *c;
  a = (double*) malloc(sizeof(double) * N);
  b = (double*) malloc(sizeof(double) * N);
  c = (double*) malloc(sizeof(double) * N);
  
  int i, j, k;
  for (i = 0; i < N; i++) {
    a[i] = i; 
    b[i] = i + 3; 
  }

  omp_set_num_threads(threads);

  double sum = 0;
  t0 = mysecond();
  for (j = 0; j < REPS; j++) {
#pragma omp parallel simd 
    for (i = 0; i < N; i++)
	c[i] =  a[i] + b[i];
  }
  t0 = (mysecond() - t0) * 1.e3;

  fprintf(stdout, "result = %1.3e\n", c[N - 1]);
  fprintf(stdout, "parallel loop = %3.2f ms\n", t0);

  return 0;

}
