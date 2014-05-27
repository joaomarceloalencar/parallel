#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "lista.h"
#include "funcoes.h"

#define size_int 4
#define size_file 1024

//verifico o Sistema Operacional Atual
#ifdef __unix__
#define SO_ 1
#elif defined(_WIN32) || defined(WIN32)
#define SO_ 2
#endif

int main(){
	srand(time(NULL));
	if(SO_ == 1){
		retorna_nucleos_unix(); 
	}else{
		retorna_nucleos_dos();
	}

	//eu crio aqui uma lista
	Lista * L=Cria_Lista();

	//preencho essa lista com a funcao carrega_lista()
	carrega_lista(L);

	//crio agora uma lista de lista com a funcao particiona_lista()
	Lista * Vetor=particiona_lista(L);

	//agora eu ordeno essa lista de listas
	ordena_lista(Vetor);

	//agora eu pego essas listas de listas todas ordenadas e entrelaco
	//diretamente no output.bin
	cria_saida(Vetor);

	//para concluir falta so verificar se a lista esta ordenada realmente
	int es_ordenada=esta_ordenado();
	if(es_ordenada == 0)
		printf("Lista Totalmente Ordenada!!! :D \n");
	else
		printf("Lista Não-Ordenada Completamente!!! %d Valores Desordenados :( \n",es_ordenada);
	return 0;
}
