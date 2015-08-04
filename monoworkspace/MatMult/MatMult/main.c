#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "SharedMemory.h"

/*
   Creates a file and fill it with random doubles
*/
void createRandomMatrixFile(char filename[100], long rows, long columns)
{
   FILE *fp = fopen(filename, "w+");
   long i;
   
   srand(time(0));
   for (i = 0; i < rows * columns; i++) {
      // double number = rand() % 100;
      double number = 1.0;
      fwrite(&number, sizeof(number), 1, fp); 
   }
   
   fclose(fp);
   return;
}

/*
   Reads a lot of doubles from a file and store it into a array.
   The same array can be used for matrix with different dimensions.
*/
double *readMatrixFile(char filename[100], long rows, long columns)
{
   FILE *fp = fopen(filename, "r");
  
   double *matrix = malloc(rows * columns * sizeof(double));
   long index = 0;
   
   while ( !feof(fp) ) {
      double number = 0;
      fread(&number, sizeof(number), 1, fp); 
      matrix[index++] = number;
   }
  
   fclose(fp);
   return matrix;
}

/*
   Very naive print of matrix, just for debugging.
*/
void printMatrix(double *A, long rows, long columns)
{
   long i, j;
   for (i = 0; i < rows; i++) 
   {
      printf("\n");
      for (j = 0; j < columns; j++)
         printf("%.2f ", A[i * columns + j]);
   }
   printf("\n");
   
   return;
}

/* 
   The main program has two options:
   filename row col: create a random matrix with dimensions row x col and save it to filename
   filename1 row1 col1 filename2 row2 col2: multiply matrix from filename1 and matrix from filename2
*/

#define SIZE 1000
int main (int argc, char *argv[])
{
   struct timespec start, end;
   double elapsed;

   clock_gettime(CLOCK_MONOTONIC, &start);
   createRandomMatrixFile("A.mtrx", SIZE, SIZE);
   double *A = readMatrixFile("A.mtrx", SIZE, SIZE);
   
   createRandomMatrixFile("B.mtrx", SIZE, SIZE); 
   double *B = readMatrixFile("B.mtrx", SIZE, SIZE);
   clock_gettime(CLOCK_MONOTONIC, &end);
 
   elapsed = ( end.tv_sec - start.tv_sec ) + ( end.tv_nsec - start.tv_nsec ) / 1000000000L;
   printf( "Create Files: %.3f\n", elapsed);
   
   clock_gettime(CLOCK_MONOTONIC, &start);
   double *C = matMultSerial(A, SIZE, SIZE, B, SIZE, SIZE);
   clock_gettime(CLOCK_MONOTONIC, &end);
   elapsed = ( end.tv_sec - start.tv_sec ) + ( end.tv_nsec - start.tv_nsec ) / 1000000000L;
   printf( "Multiply Matrices Serial: %.3f\n", elapsed);

   free(C);
   clock_gettime(CLOCK_MONOTONIC, &start);
   C = matMultOpenMP(A, SIZE, SIZE, B, SIZE, SIZE);
   clock_gettime(CLOCK_MONOTONIC, &end);
   elapsed = ( end.tv_sec - start.tv_sec ) + ( end.tv_nsec - start.tv_nsec ) / 1000000000L;
   printf( "Multiply Matrices OpenMP: %.3f\n", elapsed);
         
   free(A);
   free(B);
   free(C);
   return 0;
}

