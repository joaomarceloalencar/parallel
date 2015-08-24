#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "matrix.h"

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

int main (int argc, char *argv[])
{
   if (argc != 8) 
   {
      printf("usage: generateMatrices filenameA rowsA columnsA filenameB rowsB columnsB filenameC\n");
      return -1;
   } 

   int rowsA = atoi(argv[2]);
   int columnsA = atoi(argv[3]);

   int rowsB = atoi(argv[5]);
   int columnsB = atoi(argv[6]);              

   if (columnsA != rowsB) 
   {
      printf("Matrices dimensions don't match!\n");
      return -2;
   }
   
   struct timespec start, end;
   double elapsed;

   double *A = readMatrixFile(argv[1], rowsA, columnsA);
   double *B = readMatrixFile(argv[4], rowsB, columnsB);
   
   clock_gettime(CLOCK_MONOTONIC, &start);
   double *C = matMultSerial(A, rowsA, columnsA, B, rowsB, columnsB);
   clock_gettime(CLOCK_MONOTONIC, &end);
   elapsed = ( end.tv_sec - start.tv_sec ) + ( end.tv_nsec - start.tv_nsec ) / 1000000000L;
   printf("%.3f\n", elapsed);

   createMatrixFile(argv[7], C, rowsA, columnsB);

   return 0;
}
