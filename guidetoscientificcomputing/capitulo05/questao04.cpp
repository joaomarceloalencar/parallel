#include <iostream>

int main( int argc, char **argv ) 
{
   double **A;
   double **B;
   double **C;

   for ( int k = 0 ; k < 1000000000; k++)
   {
      A = new double* [2];
      A[0] = new double[2];
      A[1] = new double[2];

      B = new double* [2];
      B[0] = new double[2];
      B[1] = new double[2];
      
      C = new double* [2];
      C[0] = new double[2];
      C[1] = new double[2];

      C[0][0] = A[0][0] + B[0][0];
      C[0][1] = A[0][1] + B[0][1];
      C[1][0] = A[1][0] + B[1][0];
      C[1][1] = A[1][1] + B[1][1];

      delete[] A[0];
      delete[] A[1];
      delete[] A;
     
      delete[] B[0];
      delete[] B[1];
      delete[] B;

      delete[] C[0];
      delete[] C[1];
      delete[] C;
   }
   return 0;
}
