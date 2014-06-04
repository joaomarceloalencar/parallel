#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* 
Recebe o nome do arquivo. 
Imprime os inteiros um por um na tela.  
*/

#define INTSIZE 4

int main(int argc, char *argv[]) {

   //char *filename = argv[1];
   char arquivo[] = "arquivo1.bin";
   FILE *fp = fopen(arquivo, "r");
  
   int numberOfInts = 0;

   while ( !feof(fp) ) {
      int number = 0;
      fread(&number, sizeof(number), 1, fp); 
      printf("%d\n", number);
      numberOfInts++;
   }
   
   printf("Total: %d \n", numberOfInts);
   fclose(fp);

   return 0;
}

