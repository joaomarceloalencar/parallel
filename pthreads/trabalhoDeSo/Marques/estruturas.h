#ifndef ESTRUTURA
#define ESTRUTURA

#include <string>
typedef struct no{
	int valor;
	struct no *prox;
}NO;
 
typedef struct lista{
	NO *raiz;
	int n;
}Lista;

typedef struct argFuncoes{
	std::string filename;
	Lista* list;
	NO* No;
	bool ref;
}ArgFuncoes;

typedef struct argMerge{
	NO* No;
	int tam;
}ArgMerge;
#endif
