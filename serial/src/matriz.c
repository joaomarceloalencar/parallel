#include "matriz.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Função auxiliar na leitura dos vetores/matrizes.
void adicionaFinalString(char *str, char c, int limite) {
   int tamanho = strlen(str);
   
   if (tamanho > limite)
      return;
  
   str[tamanho] = c;
   str[tamanho + 1] = '\0';

   return;
}

Matriz lerMatriz(FILE *arquivoMatriz){
   // Recupera as dimensões da Matriz
   char linhasString[10];
   char colunasString[10];
   fscanf(arquivoMatriz, "%[^\n]%*c", linhasString);
   int linhas = atoi(linhasString);
   fscanf(arquivoMatriz, "%[^\n]%*c", colunasString);
   int colunas = atoi(colunasString);
   printf("Lendo matriz com %d linhas e %d colunas\n", linhas, colunas);
   if (linhas == 0 || colunas == 0) {
      printf("Dimensões de matriz inválidas.\n");
      Matriz m;
      m.n = m.m = -1;
      m.data = NULL;
      return m;
   }

   // Aloca a matriz
   Matriz m;
   m.n = linhas;
   m.m = colunas;
   m.data = (float *) malloc(linhas * colunas * sizeof(float));
   
   // Fazer a leitura da Matriz
   int posicao = 0;
   char ch = fgetc(arquivoMatriz);
   char str[100] = "";
   while (ch != EOF && posicao < linhas * colunas) {
      if (ch == '\n' || ch == ':') {
         m.data[posicao] = atof(str);
         posicao++;
         strcpy(str, "");
      } else {
         adicionaFinalString(str, ch, 100);
      }
      ch = fgetc(arquivoMatriz);
   }
   
   return m;
}

// Libera a memória alocada para a matriz.
void liberarMatriz(Matriz m) {
   free(m.data);
}
