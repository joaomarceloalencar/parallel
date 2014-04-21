#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* 
Recebe o nome do arquivo. 
Descobre quantos inteiros tem no arquivo e imprime um por um na tela.  
*/

#define INTSIZE 4

int main(int argc, char *argv[]) {
   char *filename = argv[1];
   FILE *fp = fopen(filename, "r");
  
   int numberOfInts = 0;

   while ( !feof(fp) ) {
      int number = rand();
      fread(&number, sizeof(number), 1, fp); 
      printf("%d\n", number);
      numberOfInts++;
   }
   
   printf("Total: %d \n", numberOfInts);
   fclose(fp);

   return 0;
}

