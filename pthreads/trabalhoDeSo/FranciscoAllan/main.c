#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
/**/

/*Criação de estruturas*/
typedef struct node{
	int value;
	struct node *next;
	struct node *prev;
}Node;

typedef struct list{
	int size;
	Node *first;
	Node *last;
}List;

/*Cabeçalhos*/
void quickSortThread(List *l[], int n);
void insertListThread(List *l);
Node* newNodeEl(int element);
Node* newNode(int element, Node *n, Node *p);
List* newList();
void insertLastNode(Node *n, List *l);
void insertLast(int element, List *l);
void printList(Node *n);
void insertListP(List *l);
Node* partQuick(Node *p, Node *q);
void quickSort(Node *p, Node *q);
void quickSortList(List *l);
void divideList(int n, List *new[], List *origin);
List* joinList(List *l1, List *l2);
List* joinListAll(List *l[]);
void createFile(Node *n);

/*Variáveis Globais*/
int NUM_THREADS = 10;
int NUM_CORE = 0;
int IND = 0;
List *LISTA;
pthread_mutex_t MUTEX;

char *files[] = {"files/arquivo0.bin","files/arquivo1.bin","files/arquivo2.bin","files/arquivo3.bin",
	"files/arquivo4.bin","files/arquivo5.bin","files/arquivo6.bin",
	"files/arquivo7.bin","files/arquivo8.bin","files/arquivo9.bin"};
/*Main*/
int main(){
	LISTA = newList();
	NUM_CORE = (int)sysconf(_SC_NPROCESSORS_ONLN);//Numero de nucleos
	int i;
	printf("\n\tPressione ENTER para iniciar....\n\t");
	getchar();
	printf("\n\tInserindo....\n");
	insertListThread(LISTA);
	printf("\tSize %d\n", LISTA->size);
	List *L[NUM_CORE];
	for(i = 0; i<NUM_CORE;i++){
		L[i] = newList();
	}	
	printf("\n\tDividindo....\n\t");
	divideList(NUM_CORE,L,LISTA);
	printf("\n\tOrdenando...\n\t");	
	quickSortThread(L,NUM_CORE);	
	printf("\n\tUnindo...\n\t");
	LISTA = joinListAll(L);
	printf("\n\tImprimir...\n\t");
	printf("Size %d\n\t", LISTA->size);
	printf("\n\tSalvando arquivo...\n\t");
	createFile(LISTA->first);
	getchar();
	printList(LISTA->first);
	
	return 0;
}
//Usando threads para realizar o quickSort
void quickSortThread(List *l[], int n){
	pthread_t thread[n];
	int i;
	for(i = 0; i< n;i++){
		pthread_create(&thread[i], NULL, (void*)quickSortList,(void*)l[i]);
	}					
	for(i = 0; i< n;i++){
		pthread_join(thread[i],NULL);
	}
}
//Usando threads para realizar a inserção na lista
void insertListThread(List *l){
	pthread_t thread[NUM_THREADS];
	int i;
	for(i = 0; i<NUM_THREADS;i++){
		pthread_create(&thread[i], NULL, (void*)insertListP,(void*)l);
	}					
	for(i = 0; i<NUM_THREADS;i++){
		pthread_join(thread[i],NULL);
	}	
}

//Inserindo na lista usando arquivos
void insertListP(List *l){
	char *filename = files[IND];
	IND++;
	FILE *fp = fopen(filename, "r");
	int numberOfInts = 0;
	int number;
	while ( fread(&number, sizeof(number), 1, fp) != NULL ) {
		pthread_mutex_lock(&MUTEX);//travando área crítica
		insertLast(number,l);//área crítica
		numberOfInts++;
		pthread_mutex_unlock(&MUTEX);//liberando área crítica		
   }
   printf("\t%d\n",numberOfInts);      
   fclose(fp);
}
//Cria um nó usando um valor de elemento
Node* newNodeEl(int element){
	Node *aux;
	aux = (Node*)malloc (sizeof(Node));
	aux->next = NULL;
	aux->prev = NULL;
	aux->value = element;
	return aux;
}
//Cria um nó usando um valor de elemento, o nó anterior e o próximo
Node* newNode(int element, Node *n, Node *p){
	Node *aux;
	aux = newNodeEl(element);
	aux->next = n;
	aux->prev = p;
	return aux;	
}
//Cria uma nova lista
List* newList(){
	List *aux;
	aux = (List*) malloc(sizeof(List));
	aux->size = 0;
	aux->first = aux->last = NULL;
	return aux;
}
//Inseri um nó no final da lista usando um nó
void insertLastNode(Node *n, List *l){
	n->next = n->prev = NULL;
	if(l->last != NULL){
		n->prev = l->last;
		l->last = n;
		l->last->prev->next = l->last;
	}
	else l->first = l->last = n;
	l->size++;
} 
//Inseri um elemenro no final da lista
void insertLast(int element, List *l){
	if(l->last != NULL){
		l->last = newNode(element, NULL, l->last);
		l->last->prev->next = l->last;
	}
	else l->first = l->last = newNodeEl(element);
	l->size++;
}
//Imprimi a lista
void printList(Node *n){
	int i = 0;
	if( n == NULL){
		printf("LISTA VAZIA");
	}else{
		while (n->next != NULL){
			printf("%d \n\t", n->value);
			
			n = n->next;
			i++;		
	}
	printf("%d ", n->value);
	}	
}
//Troca os valores dos nós
void swap(Node *n1, Node *n2){
	int n;
	n = n1->value;
	n1->value = n2->value;
	n2->value = n;
}
//Realiza o particionamento para o quickSort
Node* partQuick(Node *p, Node *q){
	Node *pivot = p;
	p = p->next;
	while( p != q){
		if(p->value > pivot->value){
			swap(p,q);
			q = q->prev;
		}
		else{
			p = p->next;
		} 
	}	
	if(p->value > pivot->value){
		swap(p->prev, pivot);
		return p->prev;
	}else if(p->value < pivot->value){
		swap(p,pivot);
		return p;
	}
	return pivot;	
}
//Ordenação por quickSort usando o nó primeiro e ultimo
void quickSort(Node *p, Node *q){
	Node *pivot;
	if(p != q && p->prev != q){
		pivot = partQuick(p,q);
		if(pivot->prev != NULL) quickSort(p,pivot->prev);
		if(pivot->next != NULL) quickSort(pivot->next, q);
	}	
}
//Ordenação por quickSort usand lista
void quickSortList(List *l){
	quickSort(l->first, l->last);
}
//Dividi a lista em n novas listas
void divideList(int n, List *new[], List *origin){
	int i,j;
	int k = origin -> size/n;
	
	for(i = 0; i < n-1; i++){
		new[i]->first = origin ->first;
		for(j = 0; j < k; j++){
			origin->first = origin->first->next;
			origin->size--;
			new[i]->size++;
		}
		new[i]->last = origin->first;
		origin->first = origin->first->next;
		origin->first->prev = NULL;
		new[i]->last->next = NULL;
		origin->size--;
		new[i]->size++;		
	}
	new[n-1]->first = origin->first;
	new[n-1]->last = origin->last;
	new[n-1]->size = origin->size;
	origin->first = origin->last = NULL;
	origin->size = 0;
}
//Junta de duas em duas listas em uma nova lista;
List* joinList(List *l1, List *l2){
	Node *f1,*f2;
	f1 = l1->first;
	f2 = l2->first;
		
	List *tmp = newList();
	Node *aux;
	while(f1->next != NULL && f2->next != NULL){
		if(f1->value <= f2->value){
			aux = f1->next;
			insertLastNode(f1,tmp);
			f1 = aux;
			l1->size--;
		}else{
			aux = f2->next;
			insertLastNode(f2,tmp);
			f2 = aux;
			l2->size--;	
		}
	}
	if(f1->value <= f2->value){
		aux = f1->next;
		insertLastNode(f1,tmp);
		f1 = aux;
		l1->size--;
	}else{
		aux = f2->next;
		insertLastNode(f2,tmp);
		f2 = aux;
		l2->size--;
	}
	if(f1 == NULL){
		tmp->last->next = f2;
		tmp->last->next->prev = tmp->last->next;
		tmp->last = l2->last;
		tmp->size += l2->size;
	}else{
		tmp->last->next = f1;
		tmp->last->next->prev = tmp->last->next;
		tmp->last = l1->last;
		tmp->size += l1->size;
	}
	return tmp;
}

List* joinListV(List *l[]){
	List *result = joinList(l[0],l[1]);
	return result;
}
//Troca as listas
void swapList(List *l, List *m){
	l->first = m->first;
	l->last = m->last;
	l->size = m->size;
	
	m->first = NULL;
	m->last = NULL;
	m->size = 0;
}
//Junta quantas listas estiverem em um vetor de listas
List* joinListAll(List *l[]){
	int n = NUM_CORE/2;
	int k = NUM_CORE;
	List *result;
	List *aux[k];
	List *tmp[n];
	int i,j=0;
	
	for(i = 0; i < k;i++){
		aux[i] = newList();
		swapList(aux[i],l[i]);
	}
	for(i = 0; i < n; i++){
		tmp[i] = newList();
	}
	
	while(n > 1){
		i = j = 0;
		while(i < n){
			tmp[j] = joinList(aux[i*2],aux[(i*2)+1]);
			j++;
			i+=1;
		}
		for(i = 0; i < n;i++){
			aux[i] = tmp[i];
		}
		n = n/2;
				
	}
	result = joinList(aux[0],aux[1]);
	return result;
	
}
//Cria o arquivo de saida
void createFile(Node *n){
	char *filename = "files/output";
	FILE *fp = fopen(filename, "w+");
	if( n == NULL){
		printf("LISTA VAZIA");
	}else{
		while (n->next != NULL){
			fwrite( &n->value, sizeof(n->value), 1, fp);
			n = n->next;
		}
	fwrite( &n->value, sizeof(n->value), 1, fp);
	}
	  
	fclose(fp);
}









