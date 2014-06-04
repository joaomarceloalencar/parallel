#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

//estrutura no que tem um ponteiro para o proximo no, para o anterior e um valor inteiro
typedef struct  No{
	int x;
	struct No *prox;
	struct No *ant;
}no;

//estrutura lista que tem a quantidade de numero na lista, um ponteiro para o primeiro no e o ultimo
typedef struct Lista{
	int n;
	no *primeiro;
	no *ultimo;
}lista;

//estrutura parametros que tem uma vetor  de ponteiros para lista e o nome do arquivo
typedef struct Parametros{
		lista* list[32];
		char nome_arquivo[30];	
}parametros;

#endif
