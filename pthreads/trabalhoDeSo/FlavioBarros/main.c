#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct node{
	int v;
	struct node* prox;
	struct node* ant;
}NO;

typedef struct lista{
	int n;
	NO* primeiro;
	NO* ultimo;
}Lista;

typedef struct parametros{
	int numArquivo;
	Lista *l;
	sem_t *semaforo;
}Parametros;

typedef struct parametros_ord{
	NO *base,*limite;
}Parametros_ord;

typedef struct patametros_merge{
	NO *base1, *limite1, *base2, *limite2;
}Parametros_merge;

#define NUMERO_DE_THREADS 10

Lista* criarLista();
NO* criarNo(int x);
void inserir(Lista* l, int x);
void troca(NO* n1, NO* n2);
void inserirNo(Lista* l, NO* no);
void imprime_lista(Lista* l);

void *ler_arquivo(void *ptr);
void ordena_lista(Lista* l);
void leitura_arquivos(Lista* l);
void fusao_partes(NO* noBases[], int num_processadores, Lista* l);
void *merge(void *ptr);
void escreve_arquivo(Lista* l);
void *lixeiro(void *ptr);

Parametros_ord criaParametrosOrd(NO* n1, NO* n2);
Parametros criar_parametros(int numArq, Lista* list, sem_t* sem);
Parametros_merge criaParametrosMerge(NO* n1, NO* n2, NO* n3, NO* n4);

NO* particionar(NO* no1, NO* no2);
void quickSort(NO* no1, NO* no2);

int qtd_numeros_lidos = 0;

int main(){
	
    Lista *l = criarLista();
	
    puts("Lendo arquivos...");
		
    leitura_arquivos(l);

    printf("Feito.\n");
	
    puts("Ordenando lista...");
	
    ordena_lista(l);
    
    printf("Feito.\n");

    puts("Escrevendo no arquivo...");

    escreve_arquivo(l);

    puts("Feito");
    
    printf("Números na lista: %d\n",l->n);

	return 0;
}

// Funções da lista encadeada

Lista* criarLista(){
	Lista *l = (Lista*) malloc(sizeof(Lista));
	l->n=0;
	l->primeiro = NULL;
	l->ultimo = NULL;
	return l;
}

NO* criarNo(int x){
	NO* NoAux = (NO*) malloc(sizeof(NO));
	NoAux->v = x;
	NoAux->prox = NULL;
	NoAux->ant = NULL;
	return NoAux;
}

void inserir(Lista* l, int x){

	NO* NoAux = criarNo(x);
	if(l->n == 0){
		l->primeiro = NoAux;
		l->ultimo = NoAux;
	}else{		
		l->ultimo->prox = NoAux;
		NoAux->ant = l->ultimo;
		l->ultimo = NoAux;
	}
	l->n++;
}

void trocaValores(NO* n1, NO* n2){
	int temp = n1->v;
	n1->v = n2->v;
	n2->v = temp;
}

void imprime_lista(Lista* l){
	NO* noAux = l->primeiro;
	while(noAux != NULL){
		printf("%d\n",noAux->v);
		noAux = noAux->prox;
	}
	printf("\n");
}

// Funções do Quick Sort

void quickSort(NO* no1, NO* no2){
	NO* pivo;
	if(no1 != no2 && no1->ant != no2){
		pivo = particionar(no1, no2);
		if(pivo->ant != NULL) quickSort(no1, pivo->ant);
		if(pivo->prox != NULL)quickSort(pivo->prox, no2);
	}
}

NO* particionar(NO* no1, NO* no2){
	
	NO* pivo = no1;
	no1 = no1->prox;
	
	while(no1 != no2){
		if(no1->v > pivo->v){
			trocaValores(no1,no2);
			no2 = no2->ant;
		}else{
			no1 = no1->prox;
		}
	}
	
	if(no1->v < pivo->v){
		trocaValores(no1, pivo);
		return no1;
	}else{
		if(no1 != pivo){
			trocaValores(no1->ant, pivo);
			return no1->ant;
		}
	}
	return pivo;
}

//Funções de criação de parametros passados as threads

Parametros criar_parametros(int numArq, Lista* list, sem_t* sem){
	Parametros param;
	param.numArquivo = numArq;
	param.l = list;
	param.semaforo = sem;
	return param;
}

Parametros_ord criaParametrosOrd(NO* n1, NO* n2){
	Parametros_ord p_ord;
	p_ord.base = n1;
	p_ord.limite = n2;
	return p_ord;
}

Parametros_merge criaParametrosMerge(NO* n1, NO* n2, NO* n3, NO* n4){
	Parametros_merge p_merge;
	p_merge.base1 = n1;
	p_merge.limite1 = n2;
	p_merge.base2 = n3;
	p_merge.limite2 = n4;
	return p_merge;
}

//Funções de leitura dos arquivos

void leitura_arquivos(Lista* l){
	
	sem_t semaforo;
	sem_init(&semaforo, 0, 1);
	
	
	pthread_t vet_threads[NUMERO_DE_THREADS];
	Parametros vet_parametros[NUMERO_DE_THREADS];
	
	int i;
	for(i=0; i<NUMERO_DE_THREADS;i++){
		vet_parametros[i] = criar_parametros(i,l,&semaforo);
		pthread_create( &vet_threads[i], NULL, ler_arquivo,(void *) &vet_parametros[i]);
	}
	
	for(i=0; i<NUMERO_DE_THREADS;i++){
		pthread_join(vet_threads[i],NULL);
	}
	
}

void *ler_arquivo(void* ptr){
	
	Parametros* p = (Parametros*) ptr; 
	Lista *l = p->l;
	sem_t *semaforo = p->semaforo;

	char nomeArquivo[] = "arquivoX.bin";
	nomeArquivo[7] = p->numArquivo+48;

	FILE *fp = fopen(nomeArquivo, "r");
	
	int n1,n2;
	
	while (/*!feof(fp)*/fread(&n1, sizeof(n1), 1, fp) != NULL && fread(&n2, sizeof(n2), 1, fp) != NULL) {
		
		sem_wait(semaforo); //inicio região critica
		inserir(l,n1);
		inserir(l,n2);
		sem_post(semaforo); //fim da região critica
   	}
	
   	fclose(fp);
   	
   	return (void*) NULL;
	
}

// Funções responsáveis pelo gerenciamento da ordenação

void *ordenacao_threads(void* ptr){
	Parametros_ord *p = (Parametros_ord*)ptr;
	quickSort(p->base, p->limite);
	return NULL;
}

void fusao_partes(NO* noBases[], int num_processadores, Lista* l){
	
	pthread_t vet_threads[num_processadores];
	Parametros_merge vet_parametros[num_processadores];
	
	int cont = 1,i;
	
	while(num_processadores >= 1){
		if(num_processadores > 1) num_processadores /= 2;
		
		for(i=0; i < num_processadores; i++){
			if(i==0) vet_parametros[i] = criaParametrosMerge(noBases[0], noBases[1*cont], noBases[1*cont]->prox, noBases[2*cont]);
			else vet_parametros[i] = criaParametrosMerge(noBases[i*(cont+cont)]->prox, noBases[i*(cont+cont)+cont], noBases[i*(cont+cont)+cont]->prox, noBases[i*(cont+cont)+(2*cont)]);
			pthread_create( &vet_threads[i], NULL, merge,(void *) &vet_parametros[i]); 
		}
		
		for(i=0; i<num_processadores;i++){
			pthread_join(vet_threads[i],NULL);
		}
		
		cont *= 2;
		if(num_processadores == 1) break;
	}
}

void *merge(void *ptr){
	
	Lista *l = criarLista();
	
	Parametros_merge *p = (Parametros_merge*) ptr;
	
	NO* aux, *p1, *p2, *u1, *u2;
	aux = p1 = p->base1;
	u1 = p->limite1;
	p2 = p->base2;
	u2 = p->limite2;
	
	while(p1 != u1 && p2 != u2){
		if(p1->v <= p2->v){
			inserir(l, p1->v);
			p1 = p1->prox;
		}else{
			inserir(l, p2->v);
			p2 = p2->prox;
		}
	}

	while(p1 != u1){
		inserir(l, p1->v);
		p1 = p1->prox;
	}
	
	while(p2 != u2){
		inserir(l, p2->v);
		p2 = p2->prox;
	}
	
	if(u1->v <= u2->v){
		inserir(l, u1->v);
		inserir(l, u2->v);
	}else{
		inserir(l, u2->v);
		inserir(l, u1->v);
	}
	
	NO* aux2 = l->primeiro;
	
	while(aux2 != NULL){
		aux->v = aux2->v;
		aux = aux->prox;
		aux2 = aux2->prox;
	}
	
	return NULL;
}

void ordena_lista(Lista* l){
	
	int num_processadores = (int) sysconf(_SC_NPROCESSORS_ONLN);
	int qtd_no_threads = l->n/num_processadores;
	
	NO* noBases[num_processadores+1];
	int proxEntrada = 1;
	
	NO* aux = l->primeiro;
	int contador = 0;
	
	while(aux->prox != NULL){
		aux = aux->prox;
		contador++;
		if(contador % qtd_no_threads == 0){
			noBases[proxEntrada] = aux;
			proxEntrada++;
		}
	}
	
	noBases[0] = l->primeiro;
	noBases[num_processadores] = l->ultimo;
	
	pthread_t vet_threads[num_processadores];
	Parametros_ord vet_parametros[num_processadores];
	
	int i,j=0;
	for(i=0; i<num_processadores;i++){
		if(i==0) vet_parametros[i] = criaParametrosOrd(noBases[i],noBases[i+1]);
		else vet_parametros[i] = criaParametrosOrd(noBases[i]->prox,noBases[i+1]);
		pthread_create( &vet_threads[i], NULL, ordenacao_threads,(void *) &vet_parametros[i]);
		j+=2;
	}
	
	for(i=0; i<num_processadores;i++){
		pthread_join(vet_threads[i],NULL);
	}
	
	fusao_partes(noBases, num_processadores, l);
}

// Função de escrever no arquivo

void escreve_arquivo(Lista* l){
	
	FILE *fp = fopen("output.bin", "w+");
	
	int numero;
	
	NO* aux = l->primeiro;
	
	while(aux != NULL) {
      	numero = aux->v;
      	fwrite( &numero, sizeof(numero), 1, fp);
		aux = aux->prox;
   	}
   	fclose(fp);
}
