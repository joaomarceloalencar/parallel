#include <stdio.h>
#include <stdlib.h>
#include "Lista.h"

#define true 1
#define false 0

NO* NoDeInicio(NO* No,int tam){
	int cont=1;
	
	NO* aux=NULL;
	while(cont<=tam){
		aux=No;
		No=No->prox;
		cont++;
	}
	
	if(cont>1){
		aux->prox=NULL;
	}
	
	return No; 
}

Lista* CriarLista(){
	Lista* L = (Lista*)malloc(sizeof(Lista));
	L->raiz=NULL;
	L->n=0;
	return L;
}


NO* NovoNo(int x){
	NO *N=(NO*) malloc(sizeof(NO));
	N->valor=x;
	N->prox=NULL;
	return N;
}

void GuardarNO(Lista* L,NO* N){
	L->raiz=N;
	L->n=1;
}

void InserirPosicaoNO(int x,NO* referencia){
	NO *N = NovoNo(x);
	N->prox=referencia->prox;
	referencia->prox=N;
}

NO* Inserir(Lista *L,int x){
	NO *N = NovoNo(x);
	N->prox = L->raiz;
	L->raiz = N;
	L->n++;
	return N;
}

void InserirNoInicio(Lista *L,int x){
	NO *N = NovoNo(x);
	N->prox = L->raiz;
	L->raiz = N;
	L->n++;
}

void InserirNo(NO** No,NO* NoL,NO* ultimoNO){
	
	if(*No==NULL){ 
		*No=NoL;
	}
	else{
		ultimoNO->prox=NoL;
	}
}

void InserirNoFinal(Lista* L,int x){
	
	if(L->n==0){ 
		InserirNoInicio(L,x);
	}
	else{
		NO *n=L->raiz;
	
		while(n->prox!=NULL){
			n=n->prox;
		}
	
		n->prox=NovoNo(x);
		L->n++;
	}
}

int BuscarValor(Lista* L, int x){
	NO* aux = L->raiz;
	
	while(aux != NULL){
		if(aux->valor==x){
			return true;
		}
		
		aux = aux->prox;
	}
	
	return false;
}

void DevolveNo(NO* No){
	free(No);
}

int RemoverValor(Lista* L, int x){
	
	if(L->n == 0) return false;
	
	NO* aux = L->raiz;
	NO* No;
	
	if(aux->valor==x){
		L->raiz = aux->prox;
		No=aux;
	}else{
		while(aux->prox != NULL){
			if(aux->prox->valor == x)
				break;
				
			aux=aux->prox;
		}
		if(aux->prox == NULL) return false;
		
		No = aux->prox;
		aux->prox = No->prox;
	}
	
	DevolveNo(No);
	L->n--;
	
	return true;
}

void DestroiLista(Lista* L){
	NO* No;
	NO *aux = L->raiz;
	
	while(aux!=NULL){
		No = aux;
		aux =aux->prox;
		DevolveNo(No);
	}
	
	free(L);
}

void ListarValores(Lista* L){
	NO* aux = L->raiz;
	
	while(aux != NULL){
		printf("%d\n",aux->valor);
		aux = aux->prox;
	}
	printf("\n");
} 
