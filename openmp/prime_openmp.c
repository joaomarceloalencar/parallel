#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void primo_numero_varredura(int, int, int);
int primo_numero(int);

int main ( int argc, char *argv[] ) {
   int n_fator;
   int n_superior;
   int n_inferior;

   printf("\n");
   printf ("Processadores = %d\n", omp_get_num_procs());
   printf ("Threads       = %d\n", omp_get_max_threads());

   n_inferior = 1; 
   n_superior = 131072;
   n_fator = 2;

   primo_numero_varredura(n_inferior, n_superior, n_fator);

   n_inferior = 5;
   n_superior = 500000;
   n_fator = 10;

   primo_numero_varredura(n_inferior, n_superior, n_fator);
  
   // Finaliza.
   printf ("Fim.\n");

   return 0;
}

void primo_numero_varredura(int n_inferior, int n_superior, int n_fator) {
   int n;
   int primos;
   double wtime;

   n = n_inferior;

   while (n <= n_superior)  {
      wtime = omp_get_wtime();
      primos = primo_numero(n);
      wtime = omp_get_wtime() - wtime;
      printf ("  %8d  %8d  %14f\n", n, primos, wtime);
      n = n * n_fator;
   }
   return;
}

int primo_numero(int n) {
   int i;
   int j;
   int primo;
   int total = 0;

   #pragma omp parallel shared (n) private (i, j, primo)
   #pragma omp for reduction (+:total)
   for (i = 2; i <= n; i++) {
      primo = 1;

      for (j = 2; j < i; j++) {
         if ( i % j == 0 ) {
            primo = 0;
            break;
         }
      }
      total = total + primo;
   }

   return total;
}


