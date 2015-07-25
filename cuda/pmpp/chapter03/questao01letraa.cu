#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

__global__ 
void vecAddKernel(float *A, float *B, float *C, int n)
{
   int i = threadIdx.x + blockDim.x * blockIdx.x;
   if (i < n)
      C[i] = A[i] + B[i];
}

__global__
void vecAddKernel(float *A, float *B, float *C, int dimension)
{
   int id = threadIdx.x + blockDim.x * blockIdx.x;
   int i;
   for (i = id; i < id + dimension; i++)
      if (i < dimension * dimension)
         C[i] = A[i] + B[i];

}


__host__ 
void vecAdd(float *h_A, float *h_B, float* h_C, int n)
{
   int size = n * sizeof(float);
   float *d_A, *d_B, *d_C;

   cudaMalloc((void **) &d_A, size);
   cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice); 
   cudaMalloc((void **) &d_B, size);
   cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice); 

   cudaMalloc((void **) &d_C, size);

   vecAddKernel<<<ceil(n / 256.0), 256>>>(d_A, d_B, d_C, n);

   cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
   cudaFree(d_A);
   cudaFree(d_B);
   cudaFree(d_C);
}

int main(int argc, char *argv[])
{
   int n = atoi(argv[1]);
   printf("Size: %d\n", n);

   float *h_A = (float *) malloc(n * sizeof(float));
   float *h_B = (float *) malloc(n * sizeof(float));
   float *h_C = (float *) malloc(n * sizeof(float));

   int i;
   for (i = 0; i < n; i++)
   {
      h_A[i] = 1;
      h_B[i] = 1;
   }

   vecAdd(h_A, h_B, h_C, n);  
 
   printf("%.2f\n", h_C[0]);

   free(h_A);
   free(h_B);
   free(h_C);
   
   return 0;
}
