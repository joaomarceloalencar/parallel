#ifndef LISTAS_H
#define LISTAS_H

typedef struct no{
	int valor;
	struct no* prox;
	struct no* ant;
}No;

typedef struct lista{
	No* primeiro;
	No* ultimo;
	int tamanho;
}Lista;

No* criarNo(int valor);
Lista* criarLista();
void inserirLista(Lista* l, int valor);
void mostrar(Lista* l);

#endif
