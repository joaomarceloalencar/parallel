#include "auxilio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "InsertionSort.h"
#include <unistd.h>

#define num_arq 10

int nucleos=0;
int indice = 0;
pthread_t thread_arq[num_arq];
pthread_mutex_t mutex;

void* carrega(void* arg){
	printf("\nThread Criada.\n");
	ALA* ala=(ALA*) arg;
	FILE *fp = fopen(ala->a, "r");
	
 
   while ( !feof(fp) ) {
      int number = 0;
      fread(&number, sizeof(number), 1, fp);
      pthread_mutex_lock(&mutex);
      
      inserirLista(ala->l,number);
      
      pthread_mutex_unlock(&mutex);
   }
	return arg;
}

Lista* preenche(){
	
	Lista* l = criarLista();
	char arquivos[14]="files/arq";
	char ext[]=".bin";
	for(int i=0;i<num_arq;i++){
		arquivos[9]=(char) i+ '0';
		strcpy(&arquivos[10],ext);
		ALA* ala = (ALA*)(malloc(sizeof(ALA)));
		ala->l=l;
		ala->a=arquivos;
		pthread_create(&(thread_arq[i]) , NULL , carrega , (void *) ala);	
	}
	for(int i=0 ; i<num_arq ;i++){
		pthread_join(thread_arq[i],NULL);
	}
return l;	
}

Lista* threads(Lista* l){
	
	nucleos = (int)sysconf(_SC_NPROCESSORS_ONLN);
	//printf("\n\nquantidade de nucleos : %d\n\n", nucleos);
	
	Lista* lis = (Lista * ) malloc(nucleos*sizeof(Lista));
	int t = l->tamanho/nucleos;
	
	int i;
	No* aux = l->primeiro;
	int count=0;
	lis[0].primeiro = l->primeiro;
	
	for(i=1;i<l->tamanho;i++){
		if(i==t*(count+1)){
			lis[count].ultimo = aux;
			count++;
			lis[count].primeiro = aux->prox;
		}
		if(aux!=NULL)
			aux = aux->prox;
	}
	
	lis[nucleos-1].ultimo = l->ultimo;
	
	for(i=0;i<nucleos-1;i++){
		lis[i].ultimo->prox = NULL;
	} 
	
	return lis;
}

void* thread_ordenacao(void * arg){
		Lista * lis=(Lista *)arg;
		insertion(lis);

		return arg;
}

void ordenar(Lista* lis){
	printf("Ordenando paralelamente...");
	pthread_t thread_nucleos[nucleos];
	int i;
	for(i=0;i<nucleos;i++){
		pthread_create(&(thread_nucleos[i]) , NULL , thread_ordenacao , (void *) &lis[i]);
	}
	for(i=0;i<nucleos;i++){
		pthread_join(thread_nucleos[i],NULL);
	}
}

Lista* entrelacar(Lista* lis){
	printf("Unindo as listas...");
	Lista* l = criarLista();
	int i;
	int count=642;
	for(i=0;i<nucleos;i++){
			if(l->primeiro == NULL){
				l->primeiro = lis[i].primeiro;
				l->ultimo = lis[i].ultimo;
			}else{
				No* aux;
				No* a;
				for(No* n=lis[i].primeiro;n!=NULL;n=a){
						a=n->prox;
						for(aux=l->primeiro;aux!=NULL && aux->prox!=NULL;aux=aux->prox){
								if(n->valor==aux->valor){
									a = n->prox;
									aux->prox->ant=n;
									n->prox = aux->prox;
									aux->prox=n;
									n->ant = aux;
									count++;
									break;
								}else{
									if(n->valor<aux->valor && n->valor>aux->ant->valor){
											aux->ant->prox = n;
											n->ant = aux->ant;
											n->prox=aux;
											aux->ant=n;
											break;
									}else{
										if(n->valor>aux->valor && n->valor<aux->prox->valor || n->valor>aux->valor && aux->prox==NULL){
												aux->prox->ant=n;
												n->prox = aux->prox;
												n->ant=aux;
												aux->prox=n;
												break;
										}
									}
								}
						}
				}
			}
	}
	printf("\n\n%d \n\n",count);
	return l;
}

void imprimir(Lista* lis){//imprime o conteudo das diferentes listas
	int i;
	No* n;
	for(i=0;i<nucleos; i++){
		for(n=lis[i].primeiro;n!=NULL && n->prox!=NULL;n=n->prox){
				printf("%d ",n->valor);
			}
		printf("\n\n\n");
	}
}

void imprimir_l(Lista* l){//imprime o conteudo de uma lista
	No* n;
	for(n=l->primeiro;n!=NULL && n->prox!=NULL;n=n->prox){
				printf("%d ",n->valor);
			}
}

void gera_arquivo(Lista* l){
	printf("Gerando arquivo de saida...");
	FILE *fp = fopen("files/saida.bin","w+");
	if(!fp){
			printf("Erro na abertura do arquivo!");
		exit(1);
	}else{
		No* aux;
		for ( aux=l->primeiro; aux!=NULL; aux=aux->prox) {
				fwrite( &aux->valor, sizeof(aux->valor), 1, fp); 
			}
	}
	
	fclose(fp);
}
