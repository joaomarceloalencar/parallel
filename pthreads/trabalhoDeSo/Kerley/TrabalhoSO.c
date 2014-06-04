#include "funcoes.h"

int main(){
	printf("Lendo os arquivos ...\n");
	lista* l[nProc()];
	int k;
	for(k=0;k<nProc();k++)
		l[k] = criarlista();
	lerVariosArquivos(l);
	
	printf("Arquivos Lidos \nOrdenando Numeros ...\n");
	ordenarThreads(l);
	
	printf("Entrelaçando...\n");
	lista* listaOrdenada;
	listaOrdenada = entrelacamento(l[0],l[1]);
	for(k=2;k<nProc();k++)
		listaOrdenada = entrelacamento(listaOrdenada,l[k]);
	
	saida(listaOrdenada);//criando um arquivo de saida
	//imprimirEnc(listaOrdenada);
	printf("%d de numeros ordenados\n", listaOrdenada->n);
	return 0;
}
