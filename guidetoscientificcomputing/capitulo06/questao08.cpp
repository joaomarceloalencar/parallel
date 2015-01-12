#include <iostream>
#include <cmath>
#include <cassert>

void printMatrix(double **A, int row, int col);
void matExclude(double **A, double **new_A, int N, int i, int j);
double det(double **A, int N);

int main(int argc, char *argv[])
{
   double **A;
   int rows = 2;
   int cols = 2; 
   A = new double* [rows];   
   for (int i = 0; i < rows; i++)
   {
       A[i] = new double [cols];
       for (int j = 0;  j < cols; j++)
       {
           A[i][j] = i * j;
       }
   }   

   double **new_A;
   new_A = new double* [rows - 1];   
   for (int i = 0; i < rows - 1; i++)
   {
       new_A[i] = new double [cols - 1];
   }   

   printMatrix(A, rows, cols);
   matExclude(A, new_A, 2, 1, 1);
   printMatrix(new_A, rows - 1, cols - 1);

   for (int i = 0; i < rows; i++)
   {
       delete [] A[i];
       if ( i < rows - 1)
          delete [] new_A[i]; 
   }

   delete [] A;
   delete [] new_A;
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
   /*
   if (N == 2) 
   {
      return A[0][0] * A[1][1] - A[0][1] * A[1][0]; 
   }
   */
   if (N == 1)
   {
      return A[0][0];
   }   

   double _det = 0;
   double ***_A;
   _A = new double** [N];
   for (int i = 0; i < N; i++)
   {
      _A[i] = new double* [N - 1];
      for (int j = 0;  j < N; j++)
      {
         _A[i][j] = new double [N - 1]; 
      }
   }

   for (int i = 0; i < N; i++)
   {
      matExclude(A, _A[i], N, 0, i);
      _det += A[0][i] * std::pow(-1, i) * det(_A[i], N - 1);
   }


   for (int i = 0; i < N; i++)
   {
      for (int j = 0; j < N; j++) 
      {
         delete [] _A[i][j];
      }
      delete [] _A[i];
   }
   delete [] _A;
   return _det;
}

