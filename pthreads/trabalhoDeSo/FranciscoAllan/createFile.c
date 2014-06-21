#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 
Recebe o nome do arquivo e o tamanho em megabytes. 
Cria um arquivo com o tamanho indicado, preenchido com inteiros aleatórios. 
*/

#define INTSIZE 4

void createFile(char *filename, int size);
char *files[] = {"files/arquivo0","files/arquivo1","files/arquivo2","files/arquivo3",
	"files/arquivo4","files/arquivo5","files/arquivo6",
	"files/arquivo7","files/arquivo8","files/arquivo9"};

int main( int argc, char *argv[] ) {
   int i,size = atoi(argv[1]);
   for(i = 0; i < 10; i++){
	   createFile(files[i],size);
   }
   return 0;
}

void createFile(char *filename, int size){
	FILE *fp = fopen(filename, "w+");
	int numberOfInts = size * 1024 * 1024 / INTSIZE;
    int i;
	srand(time(0));
	for ( i = 0; i < numberOfInts; i++ ) {
		int number = rand() % 10000;
		fwrite( &number, sizeof(number), 1, fp); 
	}   
	fclose(fp);
}

