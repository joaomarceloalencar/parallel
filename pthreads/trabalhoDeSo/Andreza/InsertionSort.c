#include "InsertionSort.h"
#include <stdio.h>

void insertion(Lista* l){
	
	for(No* n=l->primeiro;n != NULL; n=n->prox){
		No* aux;
		int v = n->valor;
		for(aux=n->ant; aux!=NULL && aux->prox!=NULL && aux->valor > v ; aux=aux->ant){
			aux->prox->valor=aux->valor;
			aux->valor = v;
		}
	}

}
