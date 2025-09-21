/* 
 * Scales the values in a matrix and stores it in a second matrix 
 * 
 * Demo of OpenMP of scalability and Amdahl's Law
 */
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

#include <omp.h>

#define REPS 100

double t0;
double mysecond() {
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

int main(int argc, char *argv[]) {

  float **a, **b;

  if (argc < 3) {
    fprintf(stderr, "Usage: ./sum_reduce N T\n");
    fprintf(stderr, "    N = size of vector\n");
    fprintf(stderr, "    T = number of threads\n");
    exit(0);
  }

  int N = atoi(argv[1]);          // size of vectors 
  int threads = atoi(argv[2]);    // number of OpenMP threads
  
  omp_set_num_threads(threads);

  a = (float **) malloc(sizeof(float *) * N);
  b = (float **) malloc(sizeof(float *) * N);
  
  int i, j, k;
  for (i = 0; i < N; i++) {
    a[i] = (float *) malloc(sizeof(float) * N);
    b[i] = (float *) malloc(sizeof(float) * N);
  }

  for (j = 0; j < N; j++)
    for (i = 0; i < N; i++)
      b[i][j] = i + j;

  t0 = mysecond();
#pragma omp parallel for 
  for (int k = 0; k < REPS; k++) {
    for (int j = 0; j < N; j++) 
      for (int i = 0; i < N; i++)
	a[i][j] = b[i][j] * 17;
  }
  t0 = (mysecond() - t0) * 1.e3;

  /* print an arbirtrary value from the result array */
  printf("result = %3.2f\n", a[N - 1]);
  printf("parallel loop = %3.2f ms\n", t0);

  return 0;

}
