#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

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
   Creates a file for a matrix from memory
*/
void createMatrixFile(char filename[100], double *C, long rows, long columns)
{
   FILE *fp = fopen(filename, "w+");
   long i;
 
   for (i = 0; i < rows * columns; i++) {
      fwrite(&C[i], sizeof(double), 1, fp); 
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

#endif
