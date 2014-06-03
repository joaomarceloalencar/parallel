#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

#define TAM_ARQUIVO 1024
#define QTD_ARQUIVOS 10
#define TAM_INT 4
#define MAX_RAND 1000000

void deletarArquivos(string end) {
	int i = 0;
	while(true) {
		stringstream s, s2;
		s << end << i << ".bin";
		string end2 = s.str();
		FILE *arq = fopen(end2.c_str(), "r");
		if(arq == NULL) return;
		
		s2 << "rm " << end2;
		system(s2.str().c_str());
		
		i++;
	}
}

int main( int argc, char *argv[] ) {
	
	string nomeArquivo = "binarios/arquivo";
	int tam = TAM_ARQUIVO;
	int qtd = QTD_ARQUIVOS;
	
	cout << "Quantos arquivos vc quer? " << endl;
	cin >> qtd;
	cout << "Informe o tamanho do(s) arquivo(s) em KiloBytes: " << endl;
	cin >> tam;
	
	deletarArquivos("binarios/arquivo");
	
	for(int j = 0; j < qtd; j++) {
		stringstream s;
		s << nomeArquivo << j << ".bin";
		
		printf("Criado arquivo %s\n", s.str().c_str());
		FILE *arq = fopen(s.str().c_str(), "w+");
		int numeroInt = tam * 1024 / TAM_INT;
		
		srand(1);
		int i;
		for(i = 0; i < numeroInt; i++) {
			int numero = rand();
			fwrite(&numero, sizeof(numero), 1, arq);
		}
		
		fclose(arq);
	}
	
	return 0;
}
