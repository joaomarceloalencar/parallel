#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 
Recebe o nome do arquivo e o tamanho em megabytes. 
Cria um arquivo com o tamanho indicado, preenchido com inteiros aleatórios. 
*/

#define INTSIZE 4

int main( int argc, char *argv[] ) {
   
   char *filename = argv[1];
   int size = atoi(argv[2]);

   FILE *fp = fopen(filename, "w+");
   int numberOfInts = size * 1024 * 1024 / INTSIZE;
   
   int i;
   srand(time(0));
   for ( i = 0; i < numberOfInts; i++ ) {
      int number = rand();
      fwrite( &number, sizeof(number), 1, fp); 
   }
   
   fclose(fp);

   return 0;
}

