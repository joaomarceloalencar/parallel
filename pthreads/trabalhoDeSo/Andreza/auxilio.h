#ifndef AUXILIO_H
#define AUXILIO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Listas.h"
#include <string.h>

typedef struct armazena_lista_arquivo{
	Lista* l;
	char* a;
}ALA;

void* carrega(void* arg);
Lista* preenche();
Lista* threads(Lista* l);
void* thread_ordenacao(void * arg);
void ordenar(Lista* lis);
Lista* entrelacar(Lista* lis);
void imprimir(Lista* lis);
void imprimir_l(Lista* l);
void gera_arquivo(Lista* l);

#endif
