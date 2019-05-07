#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

double f(double x) {
   double return_val = 0.0;
   return_val = cos(x * log(1.0/x));
   return return_val;
}

double xValue(double a, double h, int rank, int n, int p){
   if (rank == 0) {
      return a ;
   }
   int it = n / p - 1;
   for (int i = 1; i < rank; i++)
      it += n/p;
   return a + it * h;
}

int main(int argc, char *argv[]) {
   // Valor da integral
   double integralGlobal, integralLocal; 
   // Limites do intervalo
   double a, b;
   // Número de trapézios
   int n;
   // Base do trapézio
   double h;
   double x;
   int i;

   // Recupera o valor dos parâmetros.
   a = atof(argv[1]);
   b = atof(argv[2]);
   n = atoi(argv[3]);

   // Variáveis do MPI
   int numeroDeProcessos, rank;

   // Inicializa o ambiente MPI.
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &numeroDeProcessos);

   // h é o mesmo para todos os processos.
   h = (b - a) / n;

   // Só preciso inicializar integralGlobal no rank 0.
   if (rank == 0) {
      integralGlobal = (f(a) + f(b))/2.0;
   } 
   integralLocal = 0.0;
  

   // O valor de x muda de acordo com o rank do processo.
   // x = a + rank * (n / numeroDeProcessos - 1) * h;

   // Cada processo calcula n / numeroDeProcessos trapézios.
   // Considere n divisível por numeroDeProcessos.
   if (rank == 0) { 
      i = 1;
   }
   else {
      i = 0;
   }
   x = xValue(a, h, rank, n, numeroDeProcessos);
   for (; i < n / numeroDeProcessos; i++) {
      x += h;
      // printf("rank:%d x:%.5f\n", rank, x);
      integralLocal += f(x);
   }
   
   double integralTemp = 0.0;
   MPI_Reduce(&integralLocal, &integralTemp, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

   if (rank == 0) {
      integralGlobal += integralTemp;
      integralGlobal *= h;
      printf("%d trapézios, estimativa de %.2f a %.2f = %.5f\n", n, a, b, integralGlobal);
   }

   MPI_Finalize();
   return 0;
}
