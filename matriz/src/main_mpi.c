#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <mpi.h>
#include "matriz.h"

int main(int argc, char *argv[]) {
   // Variáveis do ambiente MPI
   int nprocs, rank;

   // Inicializa o ambiente MPI
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   // Quantidade de threads OpenMP
   int nthreads = atoi(argv[4]);

   // Controle dos laços
   int i, j,  k;

   // Dimensões das Matrizes
   int dimensoes[4];

   // Quantidade de linhas da Matriz A que cada processo irá tratar.
   int trabalhoLocal;

   // Matrizes
   Matriz matrizA, matrizB;
   
   // Somente o rank zero lerá as matrizes.
   if (rank == 0) {
      // Nome do arquivo com a matriz A.	
      char *nomeArquivoMatrizA = argv[1];

      // Nome do arquivo com o matriz B.
      char *nomeArquivoMatrizB = argv[2];

      FILE *arquivoMatrizA = fopen(nomeArquivoMatrizA, "r");
      FILE *arquivoMatrizB = fopen(nomeArquivoMatrizB, "r");
      if ((arquivoMatrizA == NULL) || (arquivoMatrizB == NULL)) {
         printf("Erro ao abrir os arquivos.\n");
         return -1;
      }

      matrizA = lerMatriz(arquivoMatrizA);
      matrizB = lerMatriz(arquivoMatrizB);

      if (matrizA.m != matrizB.n) {
         printf("Dimensões incompatíveis.\n");
         return -1;
      }

      trabalhoLocal = matrizA.n / nprocs;

      // Broadcast de Dimensões das Matrizes
      dimensoes[0] = matrizA.n;
      dimensoes[1] = matrizA.m;
      dimensoes[2] = matrizB.n;
      dimensoes[3] = matrizB.m;
      MPI_Bcast(dimensoes, 4, MPI_INT, 0, MPI_COMM_WORLD);

      // Scatter de A 
      MPI_Scatter(matrizA.data, trabalhoLocal * matrizA.m, MPI_DOUBLE, MPI_IN_PLACE, trabalhoLocal * matrizA.m, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
      
      // Broadcast de B
      MPI_Bcast(matrizB.data, matrizB.n * matrizB.m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      
      // Não são mais necessários.
      fclose(arquivoMatrizA);
      fclose(arquivoMatrizB);
   } else {
      // Broadcast de Dimensões das Matrizes
      MPI_Bcast(dimensoes, 4, MPI_INT, 0, MPI_COMM_WORLD);
      matrizA.n = dimensoes[0] / nprocs;
      matrizA.m = dimensoes[1];
      matrizB.n = dimensoes[2];
      matrizB.m = dimensoes[3];
      trabalhoLocal = matrizA.n;
      
      // Aloca suas versões de A e B
      matrizA.data = (double *) malloc(matrizA.n * matrizA.m * sizeof(double));
      matrizB.data = (double *) malloc(matrizB.n * matrizB.m * sizeof(double));

      // Recebe partição de A
      MPI_Scatter(MPI_IN_PLACE, trabalhoLocal * matrizA.m, MPI_DOUBLE, matrizA.data, trabalhoLocal * matrizA.m, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
      
      // Recebe B
      MPI_Bcast(matrizB.data, matrizB.n * matrizB.m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   }
   
   // Realiza a multiplicação.
   // Vamos cuidar para iniciar um resultado local.
   Matriz resultadoLocal;
   resultadoLocal.n = trabalhoLocal;
   resultadoLocal.m = matrizB.m;
   resultadoLocal.data = (double *) malloc(resultadoLocal.n * resultadoLocal.m * sizeof(double));

   omp_set_num_threads(nthreads);
#pragma omp parallel shared(matrizA, matrizB, resultadoLocal) private(i, j, k) 
   {  
#pragma omp for nowait
      for (i = 0; i < resultadoLocal.n * resultadoLocal.m; i++) resultadoLocal.data[i] = 0.0;

#pragma omp for nowait
      for (i = 0; i < trabalhoLocal; i++) {
         for (j = 0; j < matrizB.m; j++)
            for (k = 0; k < matrizB.n; k++)
               resultadoLocal.data[i * matrizB.m + j] += matrizA.data[i * matrizA.m + k] * matrizB.data[k * matrizB.m + j];	      
      }
   }
#pragma omp barrier

   // Teremos que fazer um Gather em um resultado mais geral.
   Matriz resultado;
   if (rank == 0) {
      resultado.n = matrizA.n;
      resultado.m = matrizB.m;
      resultado.data = (double *) malloc(resultado.n * resultado.m * (sizeof(double)));
      MPI_Gather(resultadoLocal.data, resultadoLocal.n * resultadoLocal.m, MPI_DOUBLE, resultado.data, resultadoLocal.n * resultadoLocal.m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      
      // Nome do arquivo com o resultado.
      char *nomeArquivoMatrizResultado = argv[3];
      // Guarda o resultado no arquivo.
      FILE *arquivoMatrizResultado = fopen(nomeArquivoMatrizResultado, "w");
      fprintf(arquivoMatrizResultado, "%d\n", resultado.n);
      fprintf(arquivoMatrizResultado, "%d\n", resultado.m);

      for (i = 0; i < resultado.n; i++) { 
         for (j = 0; j < resultado.m - 1; j++)
            fprintf(arquivoMatrizResultado, "%0.4f:", resultado.data[i * resultado.m + j]);
         fprintf(arquivoMatrizResultado, "%0.4f\n", resultado.data[i * resultado.m + j]);
      }

      free(resultado.data);
      fclose(arquivoMatrizResultado);
   } else {
      MPI_Gather(resultadoLocal.data, resultadoLocal.n * resultadoLocal.m, MPI_DOUBLE, resultado.data, resultadoLocal.n * resultadoLocal.m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   }


   MPI_Barrier(MPI_COMM_WORLD);
   free(matrizA.data);
   free(matrizB.data);
   free(resultadoLocal.data);
   MPI_Finalize();
   return 0;
}
