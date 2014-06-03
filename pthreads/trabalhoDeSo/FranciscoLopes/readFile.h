#ifndef READFILE_H
#define READFILE_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* 
Recebe o nome do arquivo. 
E coloca no vetor de listas.
*/

#define INTSIZE 4


void *readFile(void* values) {

   Values* v = ((Values*)values);

   FILE *fp = fopen(v->fileName, "r");

   int numberOfInts = 0;

   int turn = 0;

   while ( !feof(fp) ) {
      int number;
      fread(&number, sizeof(number), 1, fp); 

      pthread_mutex_lock(v->mutex);//Tranca o mutex
      //Inicio da região crítica...
      addItem(v->vectList[turn], number);//Adiciona o numero lido na list | A variável turn é usada para dividir a quantidade de numeros entre as listas.
      turn = (turn+1)%v->core;
      //Fim da região crítica...
      pthread_mutex_unlock(v->mutex);//Destranca o mutex
      numberOfInts++;
      
   }
   
   printf("Total: %d \n", numberOfInts);
   fclose(fp);

   
}



#endif