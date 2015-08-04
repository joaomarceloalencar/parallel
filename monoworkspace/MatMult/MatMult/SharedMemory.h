#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <stdlib.h>
#include <omp.h>

/* 
  Multiplies matrix A (rowsA x columnsA) and matrix B (rowsB x columnsB) 
  the result is matrix C (rowsA x columnsB). This is the serial version.
*/
double *matMultSerial(double *A, long rowsA, long columnsA, double *B, long rowsB, long columnsB)
{
   if (columnsA != rowsB) 
      return NULL;

   double *C = malloc(rowsA * columnsB * sizeof(double));
   
   long i,j,k;
   for (i = 0; i < rowsA; i++)
      for (j = 0; j < columnsB; j++) {
         C[i * columnsB + j] = 0;
         for (k = 0; k < columnsA; k++)
            C[i * columnsB + j] += A[i * columnsA + k] * B[k * columnsB + j];
      }      

   return C;
}

/* 
  Same as above, but with OpenMP. I keep the two methods for performance comparison.
*/
double *matMultOpenMP(double *A, long rowsA, long columnsA, double *B, long rowsB, long columnsB)
{
   if (columnsA != rowsB) 
      return NULL;

   double *C = malloc(rowsA * columnsB * sizeof(double));
   
   long i,j,k;
   #pragma omp parallel for schedule(static) private(j,k)
   for (i = 0; i < rowsA; i++)
      for (j = 0; j < columnsB; j++) {
         C[i * columnsB + j] = 0;
         for (k = 0; k < columnsA; k++)
            C[i * columnsB + j] += A[i * columnsA + k] * B[k * columnsB + j];
      }      

   return C;
}



#endif