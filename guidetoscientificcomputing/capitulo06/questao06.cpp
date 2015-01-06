#include <iostream>
#include <cassert>


void Multiply(double **A, int rowA, int colA,
               double **B, int rowB, int colB,
	       double **C, int rowC, int colC);

void Multiply(double *v, int size, 
              double **A, int rowA, int colA,
              double *new_v, int new_size);

void Multiply(double **A, int rowA, int colA,
              double *v, int size,
              double *new_v, int new_size);

void Multiply(double scalar, 
              double **A, int rowA, int colA,
              double **B, int rowB, int colB);

void Multiply(double **A, int rowA, int colA,
              double scalar,
              double **B, int rowB, int colB);



int main(int argc, char *argv[])
{
   int size = 3;

   double **A;
   A = new double*[size];
   for (int i = 0 ; i < size; i++)
   {
      A[i] = new double[size];   
   }
   for (int i = 0; i < size; i++)
      for (int j = 0; j < size; j++)
         A[i][j] = 1.0;

   double **B;
   B = new double*[size];
   for (int i = 0 ; i < size; i++)
   {
      B[i] = new double[size];   
   }
   for (int i = 0; i < size; i++)
      for (int j = 0; j < size; j++)
         B[i][j] = 2.0;

   double **C;
   C = new double*[size];
   for (int i = 0 ; i < size; i++)
   {
      C[i] = new double[size];   
   }
   
   Multiply(A, size, size, B, size, size, C, size, size);

   std::cout << C[0][0] << " " << C[0][1] << '\n';
   std::cout << C[1][0] << " " << C[1][1] << '\n';

   for (int i = 0; i < size; i++)
   {
      delete [] A[i];
      delete [] B[i];
      delete [] C[i];
   }
   delete [] A;
   delete [] B;
   delete [] C;
   
   return 0;
}


void Multiply (double **A, int rowA, int colA,
               double **B, int rowB, int colB,
	       double **C, int rowC, int colC)
{
   assert((colA == rowB) && (rowC == rowA) && (colC == colB));

   for (int i = 0; i < rowA; i++)
   {
      for (int j = 0; j < colB; j++)
      {
         C[i][j] = 0.0;
         for (int k = 0; k < colA; k++)
         {
            C[i][j] += A[i][k]*B[k][j];
         }
      }
   }
   return;
}

void Multiply(double *v, int size, 
              double **A, int rowA, int colA,
              double *new_v, int new_size)
{
   assert((colA == size) && (new_size == rowA));
   for (int i = 0; i < rowA; i++)
   {
      new_v[i] = 0.0;
      for (int j = 0; j < colA; j++) 
      {
         new_v[i] += A[i][j] * v[j];;      
      }
   }

   return;
}

void Multiply(double **A, int rowA, int colA,
              double *v, int size,
              double *new_v, int new_size)
{
   Multiply(v, size, A, rowA, colA, new_v, new_size);   
   return;
}

void Multiply(double scalar, 
              double **A, int rowA, int colA,
              double **B, int rowB, int colB)
{
   assert((rowA == rowB) & (colA == colB));
   for (int i = 0; i  < rowA; i++)
      for (int j = 0; j < colA; j++)
         B[i][j] = A[i][j] * scalar;    
   return;
}

void Multiply(double **A, int rowA, int colA,
              double scalar,
              double **B, int rowB, int colB)
{
   Multiply(scalar, A, rowA, colA, B, rowB, colB);
   return;
}




