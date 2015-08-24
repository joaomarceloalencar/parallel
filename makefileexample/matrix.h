#ifndef MATRIX_H
#define MATRIX_H

/*
   Creates a file and fill it with random doubles
*/
extern void createRandomMatrixFile(char filename[100], long rows, long columns);

/*
   Creates a file for a matrix from memory
*/
extern void createMatrixFile(char filename[100], double *C, long rows, long columns);

/*
   Reads a lot of doubles from a file and store it into a array.
   The same array can be used for matrix with different dimensions.
*/
extern double *readMatrixFile(char filename[100], long rows, long columns);

/*
   Very naive print of matrix, just for debugging.
*/
extern void printMatrix(double *A, long rows, long columns);
#endif
