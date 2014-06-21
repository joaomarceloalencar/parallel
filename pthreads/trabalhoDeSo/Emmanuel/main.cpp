#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "createArqBin.h"

//inicialização das threads de leitura e mutex;
pthread_t threadsLeitura [10];
pthread_mutex_t threadLock;

//criação da estrutura NÓ 
typedef struct _no_{
	_no_* prox;
	_no_* ant;
	int valor;
}No;
//criação da estrutura lista
typedef struct List{
	No* prim;
	No* ulti;
	int tam;
}List;

//criação da variavel global lista
List* lista;

//inicialização de um NÓ
No* new_no(int valor){
	No* no = (No*)malloc(sizeof(No));
	no->prox = NULL;
	no->ant = NULL;
	no->valor = valor;
	return no;
}

//iniciaçização de uma lista
List* new_list(){
	List *list = (List *) malloc(sizeof(List));
	list->prim = NULL;
	list->ulti = NULL;
	list->tam = 0;
	return list;
}

//metodo para adicionar um NÓ na lista
void add(List* list, int valor){
	No* no = new_no(valor);
	if(list->tam==0){
		list->prim=no;
		list->ulti=no;
	}
	else {
		list->ulti->prox=no;
		no->ant=list->ulti;
		list->ulti=no;
	} 
	list->tam++;
}

//criação da estrutura com os argumentos a serem passados para
//a thread
typedef struct _args{
	No* init;
	No* end;
}_args_;

//metodo para imprimir a lista 
void show_list(List* list){
	No* pont=list->prim;
	printf("\n[");
	while(pont!=NULL){
		printf(" %d ",pont->valor);
		pont=pont->prox;	
	}
	printf("]\n");
}

//metodo usado pelo algoritmo de ordenação para troca de NÓS
void swap (No* a, No* b){
	int c = a->valor;
	a->valor = b->valor;
	b->valor = c;
}

//metodo usado pelo algoritmo de ordenação metodo para particionar a lista
No* particionar(No* init, No* end){
	No* i = init;
	No* j = new_no(0);
	j->prox = i;
	while(i != end){
		if((i->valor) < (end->valor)){
			j = j->prox;
			swap(i,j);
		}
		i = i->prox;
	}
	swap(j->prox,i);
	
	return j->prox;
}

//metodo para chamar o metodo 'particionar' em recursividade
void quickList(No* init, No* end){
	if (init==end) return;

	if(init != end){
		
		No* r = particionar(init,end);
		if(init != r)
			quickList(init, r->ant);
		if(end!=r)
			quickList(r->prox,end);
	}
}

//chama o metodo 'particionar com recursiva'
// e tbm é o metodo para ser passado para as threads  
void *quicksort(void* args){
	_args_* pont = ((_args_*)args);
	No* init = pont->init;
	No* end = pont->end;
	
	quickList(init, end);
}
 
//metodo para retornar o numero de CPUs do computador
int numberCPUs(){
	return (int) sysconf(_SC_NPROCESSORS_ONLN);
}

//metodo para "dividir a lista", criar as threads de ordenação
// e semi-ordenar a lista
void divideLista(List* l){
	int tamLista = 1, numberCPU=numberCPUs(), tamParticao, numberThread=0;
	pthread_t threadOrdenacao[numberCPU];
		
	No* auxInicio = l->prim;
	No* auxFim = l->prim;
		
	tamParticao = l->tam/numberCPU;
		
	while(auxFim->prox != NULL && (numberThread < (numberCPU-1))){
		
		_args_ indices;
		auxFim = auxFim->prox;
		tamLista++;
		
		if(tamLista==tamParticao){
			indices.init = auxInicio;
			indices.end = auxFim;
			pthread_create(&threadOrdenacao[numberThread], NULL, quicksort, &indices);
			auxInicio = auxFim->prox;
			auxFim = auxFim->prox;
			tamLista = 1;
			numberThread++;
		}
	}
		_args_ indices2;
		indices2.init = auxInicio;
		indices2.end = l->ulti;
		pthread_create(&threadOrdenacao[numberThread], NULL, quicksort, &indices2);
		
		for(int i =0; i<numberCPU; i++){
			pthread_join(threadOrdenacao[i], NULL);
		}
}
	
//metodo para ler os arquivos binarios	
void *readArq(void* arg){
    FILE *args = (FILE *) arg;
    int numberOfInts = 0; 

    do {
      int number = 0;
      fread(&number, sizeof(number), 1, args);
      if(feof(args))break;
      pthread_mutex_lock(&threadLock);
      add(lista, number); 
      pthread_mutex_unlock(&threadLock);
      numberOfInts++;
   } while( !feof(args) );
   
   printf("Total de elementos lidos: %d \n", numberOfInts);
   
   fclose(args);
}

//metodo para escrever os elementos ordenados em um arquivo '.bin'	
void writeBin (List* l) {
	int number;
	char arq[50];
	
	No* i; 	
	
	  sprintf(arq,"output.bin");
	  FILE *fp = fopen(arq, "w+");
	  
	  for (i=l->prim; i!=NULL; i=i->prox){
		number = i->valor;
		fwrite( &number, sizeof(number), 1, fp); 
	  }
	fclose(fp);
}

//função principal
int main(){
	//inicializa o mutex lock
	pthread_mutex_init(&threadLock, NULL);
	
	//inicializa a lista
	lista = new_list();
	
	//cria os arquivo com os binario
	//parametros: numero de arquivos, tamanho do arquivo em 'MB'
	//createBin(10, 1);
	
	char filename[50];
	
	//inicializa as dez threads de leitura
	for(int i=0; i<10; i++){ 
		sprintf(filename,"arquivo%d.bin",i+1);
		FILE* f = fopen (filename, "rb");
		pthread_create(&threadsLeitura[i],NULL,readArq,f);
	}
	//inicializa as 10 threads_join
	for(int j=0; j<10; j++){
		pthread_join(threadsLeitura[j], NULL);
	}
	
	printf("'Dividindo' a lista e semi-ordenando...\n");
	
	//chama o metodo de "dividir a lista" e semi-ordenar
	divideLista(lista);
	
	//referencias para o metodo de ordenação
	No* ini = lista->prim;
	No* fim = lista->ulti;
	
	printf("Ordenando lista...\n");
	
	//chamada do metodo de ordenação
	quickList(ini, fim);
	
	printf("Escrevendo no arquivo '.bim'\n");
	
	//chamado do metodo para escrever a lista ordenada
	writeBin(lista);
	
	printf("terminado");
	
	//destruir as possiveis threads que ainda estejam executando
	pthread_mutex_destroy(&threadLock);		
	return 0;
}
