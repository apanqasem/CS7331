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


__global__ void matrixMul(const double* A, const double* B, double* C, int n) {
  
    // Calculate global row and column index for the current thread
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < n && col < n) {
        double sum = 0.0f;
        // Perform the dot product of the row from A and column from B
        for (int k = 0; k < n; k++) {
            sum += A[row * n + k] * B[k * n + col];
        }
        C[row * n + col] = sum;
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

  double *h_A, *h_B, *h_C; 
  double *d_A, *d_B, *d_C; 
  
  int size = N * N * sizeof(double); 


  // host allocation 
  h_A = (double*) malloc(size);
  h_B = (double*) malloc(size);
  h_C = (double*) malloc(size);
  
  // initialize
  for (int i = 0; i < N * N; i++) {
    h_A[i] = sin(i); 
    h_B[i] = cos(i);
    h_C[i] = 0.0f;
  }

  // device allocation  
  CUDA_CHECK(cudaMalloc((void**)&d_A, size));
  CUDA_CHECK(cudaMalloc((void**)&d_B, size));
  CUDA_CHECK(cudaMalloc((void**)&d_C, size));
    
  /*
   * 
   *  GPU offload: copy host-to-device; launch kerne; copy device-to-host 
   * 
   */
  
  dim3 threadsPerBlock(BLOCK_SIZE, BLOCK_SIZE);
  dim3 blocksPerGrid((N + BLOCK_SIZE - 1) / BLOCK_SIZE, (N + BLOCK_SIZE - 1) / BLOCK_SIZE);


  t0 = mysecond();
  // copy host to device 
  CUDA_CHECK(cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice));

  matrixMul<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);

  CUDA_CHECK(cudaGetLastError());
  CUDA_CHECK(cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost));
  t0 = (mysecond() - t0) * 1.e3;

  
  // Checksum 
  printf("Checksum: %3.4f\n", h_C[0]);
  printf("Execution Time = %3.4f ms\n", t0);

  CUDA_CHECK(cudaFree(d_A));
  CUDA_CHECK(cudaFree(d_B));
  CUDA_CHECK(cudaFree(d_C));
  
  free(h_A);
  free(h_B);
  free(h_C);
  
  return 0;
}