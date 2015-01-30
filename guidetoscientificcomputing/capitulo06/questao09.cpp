#include <iostream>
#include <cmath>
#include <cassert>

#define ORDER 3 

void printMatrix(double **A, int row, int col);
void matExclude(double **A, double **new_A, int N, int i, int j);
double det(double **A, int N);
double **allocateMatrix(int rows, int cols);
void freeMatrix(double **A, int rows);
void solveLinearSquareThreeSingularMatrix(double **A, double *b, double *r);

int main(int argc, char *argv[])
{
   double **A = allocateMatrix(3, 3);  

   A[0][0] = 4; A[0][1] = 5; A[0][2] = -2;
   A[1][0] = 7; A[1][1] = -1; A[1][2] = 2;
   A[2][0] = 3; A[2][1] = 1; A[2][2] = 4;

   double *b = new double[3];
   b[0] = -14; b[1] = 42; b[2] = 28;
   double *r = new double[3];

   printMatrix(A, 3, 3);
  
   solveLinearSquareThreeSingularMatrix(A, b, r);

   std::cout << r[0] << " " << r[1] << " " << r[2] << "\n" ;

   freeMatrix(A, 3);

   delete [] b;
   delete [] r;
   return 0;
}

void printMatrix(double **A, int row, int col) 
{
   for (int i = 0; i < row; i++)
   {
      for (int j = 0; j < col; j++)
      {
         std::cout<< A[i][j] << " ";
      }
      std::cout << "\n";
   }

   return;
}

void matExclude(double **A, double **new_A, int N, int i, int j)
{
   assert(i < N && j < N); 
  
   int new_i = 0;
   int new_j = 0;
   for (int k = 0; k < N; k++)
   {
      if (k == i)
         continue;
      for (int l = 0; l < N; l++)
      {
         if (l == j) 
            continue; 
         new_A[new_i][new_j] = A[k][l];
         new_j++;
      }
      new_i++;
      new_j = 0;
   }   

   return;
}

double det(double **A, int N)
{
   if (N == 3) 
   {
      return A[0][0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1]) - A[0][1] * (A[1][0] * A[2][2] - A[1][2] * A[2][0]) + A[0][2] * (A[1][0] * A[2][1] - A[1][1] * A[2][0]);  
   }

   double _det = 0;
   double ***_A;
   _A = new double** [N];
   for (int i = 0; i < N; i++)
   {
      _A[i] = new double* [N - 1];
      for (int j = 0;  j < N - 1; j++)
      {
         _A[i][j] = new double [N - 1]; 
      }
   }

   for (int i = 0; i < N; i++)
   {
      matExclude(A, _A[i], N, 0, i);
      if (A[0][i] == 0)
      {
         return 0;
      }
      _det += A[0][i] * std::pow(-1, i) * det(_A[i], N - 1);
   }


   for (int i = 0; i < N; i++)
   {
      for (int j = 0; j < N - 1; j++) 
      {
         delete [] _A[i][j];
      }
      delete [] _A[i];
   }
   delete [] _A;
   return _det;
}

double **allocateMatrix(int rows, int cols)
{
   double **A = new double* [rows];   
   for (int i = 0; i < rows; i++)
   {
       A[i] = new double [cols];
       for (int j = 0;  j < cols; j++)
       {
           A[i][j] = i * j;
       }
   }
   return A;
}

void freeMatrix(double **A, int rows)
{
   for (int i = 0; i < rows; i++)
   {
       delete [] A[i];
   }
   delete [] A; 
   return;
}

void solveLinearSquareThreeSingularMatrix(double **A, double *b, double *r)
{
   // Copy matrix
   double **cA = allocateMatrix(3, 3);

   // Dx
   for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
         cA[i][j] = A[i][j];
   double Dx = 0;
   for (int i = 0; i < 3; i++) 
   {
      cA[i][0] = b[i];
   }
   Dx = det(cA, 3);

   // Dy
   for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
         cA[i][j] = A[i][j];
   double Dy = 0;
   for (int i = 0; i < 3; i++) 
   {
      cA[i][1] = b[i];
   }
   Dy = det(cA, 3);

   // Dz
   for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
         cA[i][j] = A[i][j];
   double Dz = 0;
   for (int i = 0; i < 3; i++) 
   {
      cA[i][2] = b[i];
   }
   Dz = det(cA, 3);

   // Result Vector
   double D = det(A, 3);
   std::cout << D << " " << Dx << " " << Dy << " " << Dz << "\n";
   assert(Dx != 0 && Dy != 0 && Dz != 0);
   r[0] = Dx / D;
   r[1] = Dy / D;
   r[2] = Dz / D;
 
   // Free the memory
   freeMatrix(cA, 3);

   return;
}
