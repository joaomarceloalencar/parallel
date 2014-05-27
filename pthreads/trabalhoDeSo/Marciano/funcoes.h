#ifndef FUNCOES_H
#define FUNCOES_H

#include "lista.h"

//variavel que armazenara a quantidade de nucleos em minha CPU
int quant_nucleos;

/*
 * essa funcao recebe um INTEIRO e me retorna um caractere referente
 * ao inteiro, por exemplo: 1 retorna '1' e 7 reotorna '7'
 */
char atoi_reverse(int n);

/*
 * essa duas funcoes retorna a quantidade de nnucleos que existe em sua
 * CPU, obs: nao implementei ainda a funcao para o Windows
 */
void retorna_nucleos_unix();
void retorna_nucleos_dos();

// troca valores de duas variaveis
void swap(int *a, int *b);

/*
 * essa funcao recebe uma Lista e preenche-a com os valores contidos nos
 * arquivos usando THREADS
 */
void carrega_lista(Lista *L);

/*
 * essa e a THREAD da funcao anterior, ela recebe um OBJETO e dentro
 * dele temos uma lista, um nome de um arquivo, um tipo(1 ou 2) e um
 * MUTEX, ela pega o arquivo referente ao nome e carrega na lista, inde-
 * pendentemente das outras
 */
void * carrega_lista_threads(void * obj);

/*
 * essa funcao e especial, ela recebe uma lista grande, quebra ela em
 * pedacos menores e retorna esses pedacos em forma de uma lista de
 * listas
 */
Lista * particiona_lista(Lista *L);

/*
 * essa funcao recebe essas listas de listas e ordena elas
 */
void ordena_lista(Lista *Vetor);

/*
 * essa funcao recebe uma lista de listas e remove o menor elementos delas
 * e em seguida retorna esse valor
 */
int menor_das_listas(Lista l[], FILE * arq);

/*
 * como agora eu tenho varias listas de listas ordenadas, eu posso entre-
 * lacar todas como acontece com o merge_sort
 */
void entrelaca_listas(Lista * Vetor);

/*
 * essa daqui faz apenas pegar uma lista e joga ela num arquivo output.bin
 */
void cria_saida(Lista * L);

/*
 * esta funcao verifica se este arquivo output.bin esta realmente com
 * seus valores ordenados
 */
int esta_ordenado();

#endif
