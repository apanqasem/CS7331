/*
 * Hello world in OpenMP
 */

#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main(int argc, char* argv[]) {

  if (argc < 2) {
    fprintf(stderr, "Usage: ./hello T\n");
    fprintf(stderr, "    T = number of threads\n");
    exit(0);
  }

  int threads = atoi(argv[1]);    // number of OpenMP threads

  omp_set_num_threads(threads);
  
#pragma omp parallel   
  {
    printf("Hello World from thread %d!\n", omp_get_thread_num());  
    printf("Goodbye World from thread %d!\n", omp_get_thread_num());  
  }    
  return 0;
}
