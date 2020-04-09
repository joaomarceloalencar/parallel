#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "matriz.h"

int main(int argc, char *argv[]) {
   // Variáveis do ambiente MPI	
   int  numeroDeProcessos, rank; 
   MPI_Status status;

   // Inicializa o ambiente MPI
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numeroDeProcessos);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);

   // Variáveis das Matrizes e quantidade de trabalho a ser feita
   // por cada processo.
   Matriz matriz, vetor;
   int linhasLocais = 0;
 
   // Nomes dos arquivos. Usados apenas no rank == 0
   FILE *arquivoMatriz, *arquivoVetor;
   char *nomeArquivoResultado;

   // Guardar as dimensões do vetor e da matriz
   int dimensoesMatriz[2];
   int dimensoesVetor[2];

   // Leitura e distribuição da matriz e vetor.
   if (rank == 0) {
      // Nome do arquivo com a matriz.	
      char *nomeArquivoMatriz = argv[1];

      // Nome do arquivo com o vetor.
      char *nomeArquivoVetor = argv[2];

      // Nome do arquivo com o resultado.
      nomeArquivoResultado = argv[3];

      arquivoMatriz = fopen(nomeArquivoMatriz, "r");
      arquivoVetor = fopen(nomeArquivoVetor, "r");
      if ((arquivoMatriz == NULL) || (arquivoVetor == NULL)) {
         printf("Erro ao abrir os arquivos.\n");
         return 1;
      }

      matriz = lerMatriz(arquivoMatriz);
      dimensoesMatriz[0] = matriz.n;
      dimensoesMatriz[1] = matriz.m;
      vetor = lerMatriz(arquivoVetor);
      dimensoesVetor[0] = vetor.n;
      dimensoesVetor[1] = vetor.m;

   }

   // Broadcast do Vetor
   MPI_Bcast(dimensoesVetor, 2, MPI_INT, 0, MPI_COMM_WORLD);
   if (rank != 0) {
      vetor.n = dimensoesVetor[0];
      vetor.m = dimensoesVetor[1];
      vetor.data = (float *) malloc(vetor.n * vetor.m * sizeof(float));
   }
   MPI_Bcast(vetor.data, vetor.n * vetor.m, MPI_FLOAT, 0, MPI_COMM_WORLD);

   // Scatter da Matriz
   MPI_Bcast(dimensoesMatriz, 2, MPI_INT, 0, MPI_COMM_WORLD);
   linhasLocais = dimensoesMatriz[0] / numeroDeProcessos;
   if (rank != 0) {
      matriz.n = linhasLocais;
      matriz.m = dimensoesMatriz[1];
      matriz.data = (float *) malloc(matriz.n * matriz.m * sizeof(float)):
   }
   MPI_Scatter(matriz.data, linhasLocais * matriz.m, MPI_FLOAT, matriz.data, linhasLocais * matriz.m, MPI_FLOAT, 0, MPI_COMM_WORLD);

   // Todos os ranks - Realiza o trabalho local.
   Matriz resultado;
   resultado.n = matriz.n;
   resultado.m = vetor.m;
   resultado.data = (float *) malloc(resultado.n * resultado.m * sizeof(float));
   
   for (int i = 0; i < resultado.n; i++) 
      for (int j = 0; j < resultado.m; j++)	   
         resultado.data[i * resultado.m + j] = 0.0;

   for (int i = 0; i < linhasLocais; i++)
      for (int j = 0; j < matriz.m; j++)
         resultado.data[i] += matriz.data[i * matriz.m + j] * vetor.data[j];	      


   // Gather do Resultado
   MPI_Gather(resultado.data, linhasLocais * resultado.m, MPI_FLOAT, resultado.data, linhasLocais * resultado.m, MPI_FLOAT, MPI_COMM_WORLD);

   // Consolidação dos Resultados
   if (rank == 0) {
      // Rank == 0 - Guarda o resultado no arquivo.
      FILE *arquivoResultado = fopen(nomeArquivoResultado, "w");
      fprintf(arquivoResultado, "%d\n", resultado.n);
      fprintf(arquivoResultado, "%d\n", resultado.m);
      int i, j;
      for (i = 0; i < resultado.n; i++) {
         for(j = 0; j < resultado.m - 1; j++)
            fprintf(arquivoResultado, "%0.2f:", resultado.data[i * resultado.m + j]);
         fprintf(arquivoResultado, "%0.2f\n", resultado.data[i * resultado.m + j]);
      }
      fclose(arquivoMatriz);
      fclose(arquivoVetor);
      fclose(arquivoResultado);
   } 
 
   free(matriz.data);
   free(vetor.data);
   free(resultado.data);
   MPI_Finalize();
   return 0;
}
