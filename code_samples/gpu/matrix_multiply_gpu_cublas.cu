#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include<sys/time.h>
#include <cublas_v2.h>


double t0;
double mysecond() {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

// Macro for checking CUDA errors
#define CUDA_CHECK(call) { \
    cudaError_t err = call; \
    if (err != cudaSuccess) { \
        fprintf(stderr, "CUDA error in %s:%d: %s\n", __FILE__, __LINE__, cudaGetErrorString(err)); \
        exit(EXIT_FAILURE); \
    } \
}

// Macro for checking cuBLAS errors
#define CUBLAS_CHECK(call) { \
    cublasStatus_t status = call; \
    if (status != CUBLAS_STATUS_SUCCESS) { \
        fprintf(stderr, "cuBLAS error in %s:%d\n", __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    } \
}

int main(int argc, char**argv) {

  if (argc < 1) {
    printf("usage: \n");
    printf("       ./matmult N THREAD_BLOCK_SIZE\n");
    exit(0);
  }

  unsigned N = atoi(argv[1]);

  
  cublasHandle_t handle;
  float *h_A, *h_B, *h_C; // Host matrices
  float *d_A, *d_B, *d_C; // Device matrices
  
  int size = N * N * sizeof(float);

  // 1. Initialize cuBLAS
  CUBLAS_CHECK(cublasCreate(&handle));
  
  // 2. Allocate memory on the Host (CPU)
  h_A = (float*)malloc(size);
  h_B = (float*)malloc(size);
  h_C = (float*)malloc(size);
  
  
  // initialize
  for (int i = 0; i < N * N; i++) {
    h_A[i] = sin(i); 
    h_B[i] = cos(i);
    h_C[i] = 0.0f;
  }

  // 3. Allocate memory on the Device (GPU)
  CUDA_CHECK(cudaMalloc((void**)&d_A, size));
  CUDA_CHECK(cudaMalloc((void**)&d_B, size));
  CUDA_CHECK(cudaMalloc((void**)&d_C, size));
  
  // 4. Copy input matrices A and B from Host to Device
  CUDA_CHECK(cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice));
  
  // 5. Call the cuBLAS function (cublasSgemm)
  // C = alpha * A * B + beta * C
  float alpha = 1.0f;
  float beta = 0.0f;
  
  // The matrices A, B, C are assumed to be Column-Major by cuBLAS.
  // Since we store them in C/C++ Row-Major order, we pass CUBLAS_OP_T (transpose)
  // for both A and B in the call, effectively calculating B_T * A_T, which is (A*B)_T
  // and storing it in C_T (d_C). This works out correctly for square matrices.
  
  // For non-square matrices or different layouts, careful management of
  // leading dimensions (lda, ldb, ldc) and transpose operations is needed.
  
  t0 = mysecond();
  CUBLAS_CHECK(cublasSgemm(
        handle,            // cuBLAS handle
        CUBLAS_OP_T,       // Transpose A (treats C row-major A as column-major A_T)
        CUBLAS_OP_T,       // Transpose B (treats C row-major B as column-major B_T)
        N,                 // m: number of rows of op(A) and C (N)
        N,                 // n: number of columns of op(B) and C (N)
        N,                 // k: number of columns of op(A) and rows of op(B) (N)
        &alpha,            // Pointer to alpha
        d_A,               // Pointer to A data on GPU
        N,                 // lda: Leading dimension of A (N)
        d_B,               // Pointer to B data on GPU
        N,                 // ldb: Leading dimension of B (N)
        &beta,             // Pointer to beta
        d_C,               // Pointer to C data on GPU
        N                  // ldc: Leading dimension of C (N)
    ));

    // 6. Copy the result matrix C from Device to Host
    CUDA_CHECK(cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost));
    t0 = (mysecond() - t0) * 1.e3;

    // Checksum 
    printf("Checksum: %3.4f\n", h_C[0]);
    printf("Execution Time = %3.4f ms\n", t0);
    
    CUBLAS_CHECK(cublasDestroy(handle));
    CUDA_CHECK(cudaFree(d_A));
    CUDA_CHECK(cudaFree(d_B));
    CUDA_CHECK(cudaFree(d_C));
    free(h_A);
    free(h_B);
    free(h_C);

     return 0;
}