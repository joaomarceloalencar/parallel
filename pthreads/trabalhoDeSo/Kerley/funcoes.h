#ifndef FUNCOES_H
#define FUNCOES_H
#include "estruturas.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int turn=0;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;


//cria um arquivo de saida chamdado output.bin com todos os numeros lidos dos 10 arquivos ordenados
void saida(lista* l){
	char arquivo[11] = {'o','u','t','p','u','t','.','b','i','n', '\0'};
	char *filename = arquivo;
	FILE *fp = fopen(filename, "w+");	   
	no* no1 = l->primeiro;
	while(no1!=NULL) {
		int number = no1->x;
		fwrite( &number, sizeof(number), 1, fp); 
		no1=no1->prox;
	}
	fclose(fp);
	printf("Arquivo %s criado!\n",arquivo);
}


//faz a troca entre os valores de um no
void swap(no* i, no* j){
	int temp = j->x;
	j->x = i->x;
	i->x = temp;
}


//faz o particionamento de uma lista e tem como retorno o seu pivo
no* PartEnc(no* p, no* q){
	no* pivo = q;
	no* i = p;
	no* j = q->ant;
	
	while (j!=i && j->prox!=i){
		if(j->x < pivo->x){
			swap(j,i);
			i = i->prox;
		}else
			j = j->ant;
	}
	if(j->x > pivo->x){
		swap(j,pivo);
		return j;
	}else{
		swap(j->prox, pivo);
			return j->prox;
	}
}

//ordena a lista utilizando a funcao part para poder dividir lista e
//atraves da recursividade vai ordenando do lado esquerdo e direito
void QuickSortEnc(no* p ,no* q){
	if(p!=q && p->ant!=q){
		no* pivo = PartEnc(p,q);
		if(pivo->ant!=NULL)
			QuickSortEnc(p,pivo->ant);
		if(pivo->prox!=NULL)
			QuickSortEnc(pivo->prox,q);
	}
}

//recebe um ponteiro para uma lista e chama a funcao quicksort para ordenar essa lista.
//Essa funcao é um ponteiro para void pois ela é utilizada por thread
void* ordena(void * l1){
	lista *l = ((lista*)(l1));
	QuickSortEnc(l->primeiro,l->ultimo);
	return NULL;
}

//cria uma lista no espaco de memoria e retorna um ponteiro para esse espaço
lista* criarlista(){
	lista* temp  = (lista*)malloc(sizeof(lista));
	temp->primeiro = NULL;
	temp->n = 0;
	return temp;
}

//recebe um ponteiro para uma lista e imprime o valor de cada no
void imprimirEnc(lista* l){
	if(l->primeiro==NULL){
		printf("Lista vazia");
		return;
	}else{
		no *proximo_no= l->primeiro;
		while(proximo_no!=NULL){
			printf("[%d]\n ", proximo_no->x);
			proximo_no=proximo_no->prox;
		}
	}
}

//recebe um valor inteiro, cria um no no espaco de memoria, adiciona esse valor ao no e retorna um ponteiro para esse no
no* NovoNo(int x){
	no *temp = (no*)malloc(sizeof(no));
	temp->x=x;
	temp->prox=NULL;
	temp->ant=NULL;
	return temp;	
}

//recebe um ponteiro para uma lista e um valor inteiro, cria um novo no atraves da funcao NovoNo e adiciona na ṕrimeira posicao da lista
void inserirValor(lista* l, int x){
	no *temp = NovoNo(x);
	if(l->n==0){
		l->primeiro = temp;
		l->ultimo = temp;
	}else{
		temp->prox = l->primeiro;
		temp->ant = l->primeiro->ant;
		l->primeiro->ant = temp;
		l->primeiro = temp;
	}
	l->n++;
}

//recebe um ponteiro para uma lista e para um no e adiciona esse no na ultima posicao da lista
void inserirNo(lista* l, no* temp){
	if(l->n==0){
		l->primeiro = temp;
		l->ultimo = temp;
	}else{
		l->ultimo->prox = temp;
		temp->ant = l->ultimo;
		l->ultimo = temp;
	}
	l->n++;
}

//funcao que retorna a quantidade de processadores da maquina
int nProc(){
return (int) sysconf(_SC_NPROCESSORS_ONLN);
}

//funcao que recebe um ponteiro para um no e utiliza para desaloca o espaco de memoria de um no
void liberaNo(no* morre){
	free(morre);
}

//funcao que faz o entrelacamento entre duas listas e retorna o ponteiro para uma nova lista
lista* entrelacamento(lista* l1, lista* l2){
	no* no1 = l1->primeiro;
	no* no2 = l2->primeiro;
	lista* novaLista = criarlista();
	while(no1!=NULL && no2!=NULL){
		if(no1->x < no2->x){
			inserirNo(novaLista,no1);
			no1=no1->prox;
		}else{
			inserirNo(novaLista,no2);
			no2=no2->prox;
		}
	}
	while(no1!=NULL){
		inserirNo(novaLista,no1);
		no1=no1->prox;		
	}
	while(no2!=NULL){
		inserirNo(novaLista,no2);
		no2=no2->prox;		
	}
	return novaLista;
}

//funcao que recebe uma estrutura parametros que dentro contem o nome do arquivo e uma lista,
//ela ira abir o arquivo e ler o numero e inserir na lista
void* LerArquivo(void* param1){
	parametros *param = ((parametros*)(param1));
	FILE *fp = fopen(param->nome_arquivo, "r");
	while ( !feof(fp) ) {
      int number;
      fread(&number, sizeof(number), 1, fp); 
      pthread_mutex_lock(&mymutex); 
      inserirValor(param->list[turn],number);
      turn= (turn+1) % nProc();
      pthread_mutex_unlock(&mymutex); 
	}
	fclose(fp);
	return NULL;
}

//funcao que recebe uma lista e cria 10 threads para ler 10 arquivos, uma thread para cada e chama a funcao LerArquivo
void lerVariosArquivos(lista* lista1[]){
	int nThreads = 10;
	parametros *param = malloc(sizeof(parametros));
	int k;
	for(k=0;k<nProc();k++){
		param->list[k] = lista1[k];
	}
	pthread_t thread[nThreads];
	int a=48;
	int j;
	for(j=0;j<nThreads;j++){
		char c[]= {'a','r','q','u','i','v','o',(char)a,'.','b','i','n'};
		int x;
		for(x=0;x<12;x++){
			param->nome_arquivo[x] = c[x];
		}
		pthread_create( &thread[j], NULL, LerArquivo, (void*)param);
		a++;
	}
	for(j=0;j<nThreads;j++){
		pthread_join( thread[j], NULL);
	}
}

//funcao que recebe um vetor de ponteiros para lista e cria uma quantidade de threads aparti do numero de processadores da maquina
//e chama a funcao ordena para cada thread passando uma lista
void ordenarThreads(lista * l[]){
	pthread_t thread[nProc()];
	int i;
	for(i=0;i<nProc();i++){
		pthread_create( &thread[i], NULL, ordena, (void*)l[i]);
	}
	for(i=0;i<nProc();i++){
		pthread_join( thread[i], NULL);
	}
}










#endif

