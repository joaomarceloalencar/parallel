#ifndef GPU_H
#define GPU_H

#include <stdio.h>
#include <cuda.h>

/* 
  Multiplies matrix A (rowsA x columnsA) and matrix B (rowsB x columnsB) 
  the result is matrix C (rowsA x columnsB). This is the CUDA version.
*/

__global__ void matMult(double *A, long rowsA, long columnsA, double *B, long rowsB, long columnsB, double *C) 
{
   // Calculate the row index of the C element
   int Row = blockIdx.y * blockDim.y + threadIdx.y;
   
   // Calculate the column index of the C element
   int Col = blockIdx.x * blockDim.x + threadIdx.x;

   if ((Row < rows A) && (Col < columnsB)) {
      double C_element = 0.0;
      
      for (int k = 0; k < columnsA; k++)
         C_element += A[Row * columnsA + k] * B[k * columnsB + Col];
   }
}

double *matMultCUDA(double *A, long rowsA, long columnsA, double *B, long rowsB, long columnsB)
{
   double *d_A, *d_B, *d_C;
   double *C = malloc(rowsA * columnsB * sizeof(double));

   cudaMalloc((void **) &d_A, rowsA * columnsA * sizeof(double));
   cudaMemcpy(d_A, A, rowsA * columnsA * sizeof(double), cudaMemcpyHostToDevice); 

   cudaMalloc((void **) &d_B, rowsB * columnsB * sizeof(double));
   cudaMemcpy(d_B, B, rowsB * columnsB * sizeof(double), cudaMemcpyHostToDevice); 
   
   cudaMalloc((void **) &d_C, rowsA * columnsB * sizeof(double));

   matMult<<<ceil(n / 256.0), 256>>>(d_A, d_B, d_C, n);

   cudaMemcpy(C, d_C, rowsA * columnsB * sizeof(double), cudaMemcpyDeviceToHost);

   cudaFree(d_A);
   cudaFree(d_B);
   cudaFree(d_C);
   return C;
}


#endif
