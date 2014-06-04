#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define INTSIZE 4

int main( ) {
	int i, num=48;
	int size;
	printf("Qual o tamanho(MB) que voce deseja criar o arquivo?\n");
	scanf("%d",&size);
	for(i=0;i<10;i++){
	   char  arquivo[] = {'a','r','q','u','i','v','o',(char)num,'.','b','i','n'};
	   char *filename = arquivo;
	   
	   FILE *fp = fopen(filename, "w+");
	   int numberOfInts =(int) size * 1024 * 1024 / INTSIZE;
	   int j;
	   srand(time(0));
	   for ( j = 0; j < numberOfInts-1; j++ ) {
		  int number = rand() % 10000000;
		  fwrite( &number, sizeof(number), 1, fp); 
	   }
	   
	   fclose(fp);
	   printf("Arquivo %s criado!\n",arquivo);
	   num++;
	   
	}
   return 0;
}

