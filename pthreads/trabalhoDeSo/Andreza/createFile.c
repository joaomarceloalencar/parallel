#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* 
Recebe o nome do arquivo e o tamanho em megabytes. 
Cria um arquivo com o tamanho indicado, preenchido com inteiros aleatórios. 
*/

#define INTSIZE 4

void cria_arquivos(char * filename, int size){
//   int size = atoi(argv[2]);

   FILE *fp = fopen(filename, "w+");
   int numberOfInts = size * 1024/ INTSIZE;
   
   int i;
   srand(time(NULL));
   for ( i = 0; i < numberOfInts; i++ ) {
      int number = rand() % 100;
      fwrite( &number, sizeof(number), 1, fp); 
   }
   fclose(fp);
}

int main() {
	char arquivos[14]="files/arq";
	char ext[]=".bin";
	int k;
	printf("Digite a quantidade de KB do seu arquivo: ");
	scanf("%d",&k);
	for(int i=0;i<10;i++){
		arquivos[9]=(char) i+'0';
		strcpy(&arquivos[10],ext);
		cria_arquivos(arquivos,k);
	}
	return 0;
}
