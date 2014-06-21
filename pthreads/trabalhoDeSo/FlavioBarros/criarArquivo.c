#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_INT 4

int main(int argc, char *argv[])
{	
	int qtd_arquivos = atoi(argv[1]); //pega a quantidade de arquivos a serem criados
	int tamanho_arquivos = atoi(argv[2]); //pega o tamanho dos arquivos a serem criados
	int numberOfInts = tamanho_arquivos * 1024 * 1024 / TAMANHO_INT;
	char nomeArquivo[] = "arquivox.bin";
	int i;
	int j;
	int number;
	int teste;
	for (i = 0; i < qtd_arquivos; i++)
	{	
		nomeArquivo[7] = i+48;//transformando int em char
		FILE *fp = fopen(nomeArquivo, "w+");
   		
   		srand(i*3*i*time(0));
   		for ( j = 0; j < numberOfInts; j++ ) {
      		number = rand() % 100000000;//de 0 a 100.000.000
      		fwrite( &number, sizeof(number), 1, fp); 

   		}
   		fclose(fp);
	}

   

	return 0;
}
