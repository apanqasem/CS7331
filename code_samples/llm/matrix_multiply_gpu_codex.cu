#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

#include <cuda_runtime.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

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
// Simple tiled matrix multiply kernel using shared memory for better throughput.
__global__ void matMulKernel(const double *A, const double *B, double *C, int n) {
    extern __shared__ double shared[];
    double *tileA = shared;
    double *tileB = shared + blockDim.x * blockDim.y;

    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    double value = 0.0;
    for (int m = 0; m < (n + blockDim.x - 1) / blockDim.x; ++m) {
        int tiledCol = m * blockDim.x + threadIdx.x;
        int tiledRow = m * blockDim.y + threadIdx.y;

        tileA[threadIdx.y * blockDim.x + threadIdx.x] =
            (row < n && tiledCol < n) ? A[row * n + tiledCol] : 0.0;
        tileB[threadIdx.y * blockDim.x + threadIdx.x] =
            (col < n && tiledRow < n) ? B[tiledRow * n + col] : 0.0;

        __syncthreads();

        for (int k = 0; k < blockDim.x; ++k) {
            value += tileA[threadIdx.y * blockDim.x + k] *
                     tileB[k * blockDim.x + threadIdx.x];
        }

        __syncthreads();
    }

    if (row < n && col < n) {
        C[row * n + col] = value;
    }
}

void cpuMatMul(const std::vector<double> &A, const std::vector<double> &B,
               std::vector<double> &C, int n) {
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            double acc = 0.0;
            for (int k = 0; k < n; ++k) {
                acc += A[row * n + k] * B[k * n + col];
            }
            C[row * n + col] = acc;
        }
    }
}

int main(int argc, char **argv) {

  if (argc < 2) {
    printf("usage: \n");
    printf("       ./matmult N THREAD_BLOCK_SIZE\n");
    exit(0);
  }
  unsigned n = atoi(argv[1]);
  unsigned BLOCK_SIZE = atoi(argv[2]); 

  size_t elems = static_cast<size_t>(n) * static_cast<size_t>(n);
  size_t bytes = elems * sizeof(double);
  
  std::vector<double> hA(elems), hB(elems), hC(elems), hRef(elems);
  
  // initialize
  for (int i = 0; i < n * n; i++) {
    hA[i] = sin(i); 
    hB[i] = cos(i);
    hC[i] = 0.0f;
  }

  double *dA = nullptr, *dB = nullptr, *dC = nullptr;
  CUDA_CHECK(cudaMalloc(&dA, bytes));
  CUDA_CHECK(cudaMalloc(&dB, bytes));
  CUDA_CHECK(cudaMalloc(&dC, bytes));
  
  
  const int TILE = BLOCK_SIZE;
  dim3 threads(TILE, TILE);
  dim3 blocks((n + TILE - 1) / TILE, (n + TILE - 1) / TILE);
  size_t sharedMem = 2 * TILE * TILE * sizeof(double);
  
  t0 = mysecond();
  CUDA_CHECK(cudaMemcpy(dA, hA.data(), bytes, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemcpy(dB, hB.data(), bytes, cudaMemcpyHostToDevice));
  matMulKernel<<<blocks, threads, sharedMem>>>(dA, dB, dC, n);

  CUDA_CHECK(cudaGetLastError());
  CUDA_CHECK(cudaDeviceSynchronize());
  
  CUDA_CHECK(cudaMemcpy(hC.data(), dC, bytes, cudaMemcpyDeviceToHost));
  t0 = (mysecond() - t0) * 1.e3;
  
  
  // Checksum 
  printf("Checksum: %3.4f\n", hC[0]);
  printf("Execution Time = %3.4f ms\n", t0);


  CUDA_CHECK(cudaFree(dA));
  CUDA_CHECK(cudaFree(dB));
  CUDA_CHECK(cudaFree(dC));
  
  return EXIT_SUCCESS;
}
