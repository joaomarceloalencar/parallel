#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utilidades.cpp"

#define TAM_INT 4

int main(int argc, char *argv[]) {
	
	char *nomeArquivo = argv[1];
	FILE *arq = fopen(nomeArquivo, "r");
	
	int num = 0;
	int number, number2;
	fread(&number2, sizeof(number2), 1, arq);
	
	while( !feof(arq) ) {
		fread(&number, sizeof(number), 1, arq);
		num++;
		if(number < number2) {
			showL("Erro! Lista não ordenada!", RED);
			return 0;
		}
	}
	
	show("Lista ordenada! ", GREEN);
	show(num, GREEN);
	showL(" elementos ", GREEN);
	
	fclose(arq);
	
	return 0;
}
