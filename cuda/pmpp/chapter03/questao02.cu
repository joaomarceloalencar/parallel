#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

__global__
void vecMatMultKernel(float *A, float *B, float *C, int n)
{
   int i = threadIdx.x + blockDim.x * blockIdx.x;
   A[i] = 0;
   int j;
   for (j = 0; j < n; j++)  
   {
      if (i * n + j < n * n)
         A[i] += B[i * n + j] * C[j];
         // A[i] += B[i * n + j];
   } 
}


__host__ 
void vecMatMult(float *h_A, float *h_B, float* h_C, int n)
{
   int size = n * sizeof(float);
   float *d_A, *d_B, *d_C;

   
   cudaMalloc((void **) &d_B, n * size);
   cudaMemcpy(d_B, h_B, n * size, cudaMemcpyHostToDevice); 
   cudaMalloc((void **) &d_C, size);
   cudaMemcpy(d_C, h_C, size, cudaMemcpyHostToDevice); 

   cudaMalloc((void **) &d_A, size);

   vecMatMultKernel<<<ceil(n / 256.0), 256>>>(d_A, d_B, d_C, n);

   cudaMemcpy(h_A, d_A, size, cudaMemcpyDeviceToHost);
   cudaFree(d_A);
   cudaFree(d_B);
   cudaFree(d_C);
}

int main(int argc, char *argv[])
{
   int n = atoi(argv[1]);
   printf("Dimension: %d\n", n);

   float *h_B = (float *) malloc(n * n * sizeof(float));
   float *h_C = (float *) malloc(n * sizeof(float));
   float *h_A = (float *) malloc(n * sizeof(float));

   int i;
   for (i = 0; i < n * n; i++)
   {
      h_B[i] = 1;
      if (i < n)
         h_C[i] = 1;
   }

   vecMatMult(h_A, h_B, h_C, n);  
 
  
   for (i = 0; i < n; i++) 
   {
      printf("%.2f ", h_A[i]);
      if (!((i+1) % 10))
         printf("\n");
   }

   printf("\n");

   free(h_A);
   free(h_B);
   free(h_C);
   
   return 0;
}
