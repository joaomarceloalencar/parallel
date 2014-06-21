#include <stdio.h>
#include <stdlib.h>
#include "Listas.h" 

No* criarNo(int valor){
	No* novoNo = (No*)(malloc(sizeof(No)));
	novoNo->valor = valor;
	novoNo->prox = NULL;
	novoNo->ant = NULL;
	return novoNo;
}

Lista* criarLista(){
	Lista* novaLista = (Lista*)(malloc(sizeof(Lista)));
	novaLista->primeiro = NULL;
	novaLista->ultimo = NULL;
	novaLista->tamanho = 0;
	return novaLista;
}

void inserirLista(Lista* l, int valor){
	No* novoNo = criarNo(valor);
	if(l->tamanho==0){
		l->primeiro = novoNo;
		l->ultimo = novoNo;	
	}else{
			novoNo->prox = l->primeiro;
			l->primeiro->ant = novoNo;
			l->primeiro = novoNo;
	}
	l->tamanho++;
}


void mostrar(Lista* l){ //utilizada para teste, saber quantidade de elementos da lista
	int ala=0;//quantidade de elementos dos 10 arquivos;
	No* n=l->primeiro;
	int i;
	for(i=0;i<l->tamanho;i++){
		ala++;
		//printf("[%d]",n->valor);
		n=n->prox;
	}
	printf("\nAla=%d\n",ala);
}
