#include <stdio.h>
#include <stdlib.h>
#include<sys/time.h>

#include <cuda_runtime.h>

double t0;
double mysecond() {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

#define CUDA_CHECK(call) {   \
   cudaError_t err = call; \
   if (err != cudaSuccess) { \
      fprintf(stderr, "CUDA error in %s:%d: %s\\n", __FILE__, __LINE__, cudaGetErrorString(err)); \
      exit(EXIT_FAILURE); \
   } \
 }


__global__ void matMul(double *A, double *B, double *C, int N) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  int j = blockIdx.y * blockDim.y + threadIdx.y;
  if (i < N && j < N) {
    double sum = 0;
    for (int k = 0; k < N; ++k) {
      sum += A[i * N + k] * B[k * N + j];
    }
    C[i * N + j] = sum;
  }
}

int main(int argc, char**argv) {

  if (argc < 2) {
    printf("usage: \n");
    printf("       ./matmult N THREAD_BLOCK_SIZE\n");
    exit(0);
  }

  unsigned N = atoi(argv[1]);
  unsigned BLOCK_SIZE = atoi(argv[2]); 

  // Declare and allocate memory for the input and output matrices on the host side
  double *A, *B, *C;
  cudaMallocHost((void **)&A, N * N * sizeof(double));
  cudaMallocHost((void **)&B, N * N * sizeof(double));
  cudaMallocHost((void **)&C, N * N * sizeof(double));

  // Initialize the input matrices on the host side
  for (int i = 0; i < N * N; i++) {
    A[i] = sin(i); 
    B[i] = cos(i);
    C[i] = 0.0f;
  }


  // Define the grid and block dimensions for the kernel
  dim3 threadsPerBlock(16, 16);
  dim3 numBlocks(N / threadsPerBlock.x, N / threadsPerBlock.y);

  t0 = mysecond();
  // Launch the kernel
  matMul<<<numBlocks, threadsPerBlock>>>(A, B, C, N);

  // Synchronize the device and print the output matrix on the host side
  cudaDeviceSynchronize();
  t0 = (mysecond() - t0) * 1.e3;

  // Checksum 
  printf("Checksum: %3.4f\n", C[0]);
  printf("Execution Time = %3.4f ms\n", t0);

  // Deallocate memory on the host side
  cudaFreeHost(A);
  cudaFreeHost(B);
  cudaFreeHost(C);

  return 0;
}
