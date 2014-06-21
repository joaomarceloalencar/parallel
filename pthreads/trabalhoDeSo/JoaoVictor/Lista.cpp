#ifndef LISTA_H
#define LISTA_H

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
#include "funcoes.cpp"

#define para(i, a, b) for(int i = a; i < b; i++)

using namespace std;

int contador = 0;

typedef struct No_no{
	int valor;
	int indice;
	struct No_no *prox;
	struct No_no *ant;
}No;

typedef struct List{
	int tamanho;
	No *primeiro;
}Lista;

typedef struct Quick{
	No *primeiro;
	No *ultimo;
}ListaQuick;

typedef struct Lista_Merge{
	Lista *listas;
	int numNucleos;
	Lista *l2;
}ListaMerge;

Lista newLista() {
	Lista l = {0, NULL};
	return l;
}

void exibirLista(Lista l) {
	if(l.tamanho == 0)
		return;
		
	No *no = l.primeiro;
	int i = 0;
	
	cout << "[" ;
	do {
		if(i == l.tamanho-1)
			cout << no->valor << "]\n";
		else
			cout << no->valor << ", ";
			
		no = no->prox;
	} while(++i < l.tamanho);
	
}

void inserir(Lista &l, int valor) {
	if(l.tamanho == 0) {
		No *no = new No();
		no->prox = no;
		no->ant = no;
		no->valor = valor;
		no->indice = 0;
		
		l.primeiro = no;
		l.tamanho++;
	}
	else {
		No *no = new No;
		No *ant = l.primeiro->ant;
		No *prox = l.primeiro;
		ant -> prox = no;
		prox -> ant = no;
		no->ant = ant;
		no -> prox = prox;
		no -> valor = valor;
		no -> indice = no->ant->indice+1;
		l.tamanho++;
	}
}

int removerFim(Lista &l) {
	No *no = l.primeiro -> ant;
	int elem = no->valor;
	No *fim = no -> ant;
	No *ini = l.primeiro;
	fim->prox = ini;
	ini->ant = fim;
	delete no;
	l.tamanho--;
	
	return elem;
}

int removerIni(Lista &l) {
	int elem = l.primeiro->valor;
	
	No *ultimo = l.primeiro->ant;
	No *primeiro = l.primeiro->prox;
	ultimo->prox = primeiro;
	primeiro->ant = ultimo;
	l.primeiro = primeiro;
	
	l.tamanho--;
	return elem;
	
}

void swap(No *a, No *b) {
	int aux = a->valor;
	a->valor = b->valor;
	b->valor = aux;
}

No* part(No *ini, No *fim) {
	contador++;
	No *pivo = ini;
	No *iPivo = ini->prox;
	No *no = ini;
	
	while(no != fim) {
		no = no->prox;
		if(no->valor < pivo->valor) {
			swap(iPivo, no);
			iPivo = iPivo->prox;
		}
	}
	
	iPivo = iPivo->ant;
	
	swap(iPivo, pivo);
	
	return iPivo == fim? iPivo->ant: iPivo;
	
}
   
void quickSort(No *ini, No *fim) {
	
	if(ini != fim) {
		No *meio = part(ini, fim);
		quickSort(ini, meio);
		quickSort(meio->prox, fim);
	}
}

void* OrdenarEmThreads(void * args) {
	
	No *ini = ((ListaQuick*) args)->primeiro;
	No *fim = ((ListaQuick*) args)->ultimo;
	
	quickSort(ini, fim);
	
	return NULL;
}

void ordenarListas(Lista *l) {
	pthread_t id_thread[getNucleos()];
	int numNucleos = getNucleos();
	
	para(i, 0, numNucleos) {
		ListaQuick *lq = new ListaQuick;
		lq->primeiro = l[i].primeiro;
		lq->ultimo = l[i].primeiro->ant;
		pthread_create(&id_thread[i], NULL, OrdenarEmThreads, (void*)lq);
		
	}

	para(i, 0, numNucleos)
		pthread_join(id_thread[i], NULL);
	
}


void* mergeListas(void* args) {
	
	Lista *listas = ((ListaMerge*) args)->listas;
	int tam = ((ListaMerge*) args)->numNucleos;
	Lista *novaL = ((ListaMerge*) args)->l2;
	
	*novaL = newLista();
	
	No* nonv = new No;
	nonv -> prox = novaL->primeiro;
	Lista *menor;
	
	while(true) {
		
		int i = 0;
		while(i < tam && listas[i].tamanho == 0)
			i++;
		
		if(i == tam) break;
		
		menor = &listas[i];
		
		for(int i = 0; i < tam; i++)
			if(listas[i].tamanho > 0 && listas[i].primeiro->valor < menor->primeiro->valor)
				menor = &listas[i];
		
		pthread_mutex_lock(&count_mutex);
		int m = removerIni(*menor);
		inserir(*novaL, m);
		pthread_mutex_unlock(&count_mutex);
	}
	
	return NULL;
}

void mergeThread(Lista listas[], int tam, Lista &novaL) {
	pthread_t t;
	ListaMerge *lm = new ListaMerge;
	
	lm->listas = listas;
	lm->numNucleos = tam;
	lm->l2 = &novaL;
	
	pthread_create(&t, NULL, mergeListas, lm);
	

	
	pthread_join(t, NULL);
}


#endif
