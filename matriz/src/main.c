#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "matriz.h"

int main(int argc, char *argv[]) {
   // Controle dos laços
   int i, j,  k;

   // Nome do arquivo com a matriz A.	
   char *nomeArquivoMatrizA = argv[1];

   // Nome do arquivo com o matriz B.
   char *nomeArquivoMatrizB = argv[2];

   // Nome do arquivo com o resultado.
   char *nomeArquivoMatrizResultado = argv[3];

   FILE *arquivoMatrizA = fopen(nomeArquivoMatrizA, "r");
   FILE *arquivoMatrizB = fopen(nomeArquivoMatrizB, "r");
   if ((arquivoMatrizA == NULL) || (arquivoMatrizB == NULL)) {
      printf("Erro ao abrir os arquivos.\n");
      return -1;
   }

   Matriz matrizA = lerMatriz(arquivoMatrizA);
   // imprimirMatriz(matrizA);

   Matriz matrizB = lerMatriz(arquivoMatrizB);
   // imprimirMatriz(matrizA);

   if (matrizA.m != matrizB.n) {
      printf("Dimensões incompatíveis.\n");
      return -1;
   }
   
   // Realiza a multiplicação.
   Matriz resultado;
   resultado.n = matrizA.n;
   resultado.m = matrizB.m;
   resultado.data = (double *) malloc(resultado.m * resultado.n * sizeof(double));
   for (i = 0; i < resultado.n * resultado.m; i++) resultado.data[i] = 0.0;

#pragma omp parallel for shared(matrizA, matrizB, resultado) private(i, j, k)
   for (i = 0; i < matrizA.n; i++) {
      // printf("Thread %d, iteração %d.\n", omp_get_thread_num(), i);
      for (j = 0; j < matrizB.m; j++)
         for (k = 0; k < matrizB.n; k++)
            resultado.data[i * matrizB.m + j] += matrizA.data[i * matrizA.m + k] * matrizB.data[k * matrizB.m + j];	      
   }
   // Guarda o resultado no arquivo.
   FILE *arquivoMatrizResultado = fopen(nomeArquivoMatrizResultado, "w");
   fprintf(arquivoMatrizResultado, "%d\n", resultado.n);
   fprintf(arquivoMatrizResultado, "%d\n", resultado.m);
   for (i = 0; i < resultado.n; i++) {
      for(j = 0; j < resultado.m - 1; j++)
         fprintf(arquivoMatrizResultado, "%0.4f:", resultado.data[i * resultado.m + j]);
      fprintf(arquivoMatrizResultado, "%0.4f\n", resultado.data[i * resultado.m + j]);
   }
 
   free(matrizA.data);
   free(matrizB.data);
   free(resultado.data);
   fclose(arquivoMatrizA);
   fclose(arquivoMatrizB);
   fclose(arquivoMatrizResultado);

   return 0;
}
