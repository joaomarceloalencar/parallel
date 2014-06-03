#ifndef LISTA_CPP
#define LISTA_CPP

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
#include "utilidades.cpp"

#define SLEEP_ORDEM 1000

using namespace std;

struct No_{
    int valor;
	struct No_ *prox;
	struct No_ *ant;
};

typedef struct No_ No;

typedef struct {
	int tam;
	No *primeiro;
}Lista;

typedef struct {
	No *ini;
	No *fim;
}Processo;

struct NoProcesso_ {
	Processo *p;
	struct NoProcesso_ *prox;
	struct NoProcesso_ *ant;
};
typedef struct NoProcesso_ NoProcesso;

typedef struct {
	int tam;
	NoProcesso *primeiro;
	NoProcesso *ultimo;
}FilaProcessos;

FilaProcessos newFila();

/*
 * *****************
 * VARIAVEIS GLOBAIS
 * ***************** */

	pthread_mutex_t mutex_fila;
	pthread_mutex_t mutex_lista;	

	FilaProcessos fp = newFila();

	bool* threadsAtivas;
	int numNucleos = 0;

	int totalQuick = 0;
	int porcentQuick = 0;
//*****************************

void setTotalQuick(int tam) {
	totalQuick = tam;
}

bool isOrdenado(Lista l) {
	int i = 0;
	No *no = l.primeiro;
	while(++i != l.tam) {
		if(no->valor > no->prox->valor)
			return false;
		no = no->prox;
	}
	return true;
}

int getTam(Lista l) {
	int i = 0;
	No* no = l.primeiro;
	while(no->prox != l.primeiro) {
		i++;
		no = no->prox;
	}
	return i+1;
}

Lista newLista() {
	Lista l = {0, NULL};
	return l;
}

FilaProcessos newFila() {
	FilaProcessos f = {0, NULL, NULL};
	return f;
}

void mostrarLista(Lista l) {
	if(l.tam == 0)
		return;
		
	No *no = l.primeiro;
	int i = 0;
	
	cout << "[" ;
	do {
		if(i == l.tam-1)
			cout << no->valor << "]\n" << endl;
		else
			cout << i << " = " << no->valor << " " << endl;
			
		no = no->prox;
	} while(++i != l.tam);
}

void mostrarListaProcesso() {
	if(fp.tam == 0)
		return;
		
	NoProcesso *no = fp.primeiro;
	int i = 0;
	
	cout << "[" ;
	do {
		if(i == fp.tam-1)
			cout << "p]\n";
		else
			cout << "p" << ", ";
			
		no = no->prox;
	} while(++i != fp.tam);
}

void add(Lista &l, int valor) {
	if(l.tam == 0) {
		No *no 		= new No();
		no->prox 	= no;
		no->ant 	= no;
        no->valor 	= valor;
		
		l.primeiro 	= no;
		l.tam++;
	}
	else {
		No *no 		= new No;
		No *ant 	= l.primeiro->ant;
		No *prox	= l.primeiro;
		ant->prox 	= no;
		prox->ant 	= no;
		no->ant 	= ant;
		no->prox 	= prox;
        no->valor 	= valor;
		l.tam++;
	}
}

void addAt(int valor, No* no) {
	
	No* ant 	= no;
	No* prox 	= no->prox;
	No* novo 	= new No;
	novo->valor = valor;
	
	ant->prox 	= novo;
	prox->ant 	= novo;
	novo->prox 	= prox;
	novo->ant 	= ant;
}

void addProcesso(Processo &p) {
	NoProcesso *no = new NoProcesso;
	no->p = &p;

	if(fp.tam == 0) {
		fp.primeiro	 = no;
		no->prox	 = no;
		no->ant		 = no;
	} else {
		no->prox 	= fp.primeiro;
		no->ant 	= fp.primeiro->ant;
		fp.primeiro->ant->prox	= no;
		fp.primeiro->ant 		= no;
	}
	fp.tam++;
}


int removerFim(Lista &l) {
	No *no 		= l.primeiro -> ant;
	int elem 	= no->valor;
	No *fim 	= no -> ant;
	No *ini 	= l.primeiro;
	fim->prox 	= ini;
	ini->ant 	= fim;

	delete no;
	l.tam--;
	
	return elem;
}

int removerIni(Lista &l) {
	int elem 	= l.primeiro->valor;
	No *no 		= l.primeiro;
	l.primeiro 	= l.primeiro->prox;
	l.primeiro->prox->ant = l.primeiro;

	delete no;
	l.tam--;

	return elem;
	
}

Processo* removeProcesso() {
	Processo* fim 	= fp.primeiro->ant->p;
    NoProcesso *no 	= fp.primeiro->ant;
	
	NoProcesso *ant 	= fp.primeiro->ant->ant;
	NoProcesso *prox 	= fp.primeiro;
	
	ant->prox = prox;
	prox->ant = ant;
	
	fp.tam--;
    delete no;
	return fim;
}

void swap(No *a, No *b) {
    int aux 	= a->valor;
    a->valor 	= b->valor;
    b->valor 	= aux;
}

No* part(No *ini, No *fim) {
	No *pivo 	= ini;
	No *iPivo 	= ini->prox;
	No *no 		= ini;
	
	while(no!= fim) {
		no = no->prox;
        if(no->valor < pivo->valor) {
			swap(iPivo, no);
			iPivo = iPivo->prox;
		}
	}
	
	iPivo = iPivo->ant;
	
	swap(iPivo, pivo);
	
	return iPivo;
	
}
   
void quickSort(No *ini, No *fim, Lista l) {
	
	if(ini != fim) {
		No *meio = part(ini, fim);
		quickSort(ini, meio, l);
		quickSort(meio->prox, fim, l);
	}
}


void quickSortFila(Processo &proces) {

	No *ini = proces.ini;
	No *fim = proces.fim;

		pthread_mutex_lock(&mutex_fila);
	porcentQuick++;
		pthread_mutex_unlock(&mutex_fila);
	
	No *meio = part(ini, fim);
	
	
	No *meioAux = meio;
	while(meioAux->prox != fim && meioAux != fim && meioAux->valor == meioAux->prox->valor) {
			pthread_mutex_lock(&mutex_fila);
		porcentQuick++;
			pthread_mutex_unlock(&mutex_fila);

		meioAux = meioAux->prox;
	}

	if(meioAux != fim) {
		Processo *p2 	= new Processo;
		p2->ini 		= meioAux->prox;
		p2->fim 		= fim;

            pthread_mutex_lock(&mutex_fila);
		addProcesso(*p2);
            pthread_mutex_unlock(&mutex_fila);
	}

	No *meioAux2 = meio;

	if(ini != meioAux2) {
		Processo *p = new Processo;
		p->ini 		= ini;
		p->fim 		= meioAux2->ant;

            pthread_mutex_lock(&mutex_fila);
		addProcesso(*p);
            pthread_mutex_unlock(&mutex_fila);
	} 
	
	

}

bool isThreadsAtivas() {
	for(int i = 0; i < numNucleos; i++)
		if(threadsAtivas[i])
			return true;
	return false;
}

void* executarProcesso(void *args) {
	int id = *((int*) args);

	while(isThreadsAtivas()) {	
		Processo *p = NULL;
		
		pthread_mutex_lock(&mutex_fila);
		if(fp.tam == 0) {
			threadsAtivas[id] = false;
			pthread_mutex_unlock(&mutex_fila);
			continue;
		}
		else {		
			threadsAtivas[id] = true;
			p = removeProcesso();
			pthread_mutex_unlock(&mutex_fila);
		}
		
		quickSortFila(*p);
		
		
	}
	
	return args;
}

void executarProcessos() {
	numNucleos= getNucleos();
	
	show(numNucleos, RED);
	showL(" Threads criadas para a ordenação", YELLOW);
	LINE;
	
	pthread_t t[numNucleos];
	threadsAtivas = new bool[numNucleos+1000];
	int ids[numNucleos];
	
	pthread_t tp;
	MosPor ms = {totalQuick, &porcentQuick, "Quick Sort"};

	pthread_create(&tp, NULL, mostrarProgresso, (void*)&ms);
	
	para(i, 0, numNucleos) {
		threadsAtivas[i] = true;
		ids[i] = i;
		pthread_create(&t[i], NULL, &executarProcesso, &ids[i]);
	}

	para(i, 0, numNucleos)
		pthread_join(t[i], NULL);
	
	pthread_join(tp, NULL);

}

#endif
