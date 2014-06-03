#ifndef CREATEFILE_H
#define CREATEFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
/* 
Recebe o nome do arquivo e o tamanho em megabytes. 
Cria um arquivo com o tamanho indicado, preenchido com inteiros aleatórios. 
*/

#define INTSIZE 4

//Função usada para tranformar um inteiro em um char respectivo.
char IntToChar(int number)
{
    char a = (char)(number+48);
    return a;
}

void createFile(char fileName[], int numberOfFiles) {
   
   srand(time(0));
   for(int j = 0; j < numberOfFiles; j++){
      fileName[7] = IntToChar(j);

      FILE *fp = fopen(fileName, "w+");

      int numberOfInts = (10*1024*1024)/4;
      
      for (int i = 0; i < numberOfInts; i++ ) {
         int number = rand() % 1000000;
         fwrite( &number, sizeof(number), 1, fp); 
      }
      
      fclose(fp);
   }
}

//Função usada para escrever a lista ordenada em um arquivo..
void createFileOrdenado(char fileName[], List* listMerge[]) {
    Item* aux;
    FILE *fp = fopen(fileName, "w+");
   for(aux = listMerge[0]->first; aux != NULL; aux = aux->next){
      fwrite( &aux->value, sizeof(aux->value), 1, fp); 
   }
   fclose(fp);
   printf("%d numeros ordenados com SUCESSO!\n",listMerge[0]->n);
}

#endif