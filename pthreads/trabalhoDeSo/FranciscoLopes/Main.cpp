#include <pthread.h>
#include <string>
#include <stdlib.h>
#include "strutures.h"
#include "createFile.h"
#include "List.h"
#include <unistd.h>
#include "readFile.h"

/*Especificação da máquina:
 *-Core i5 
 *-4 Núcleos
 *6gb de RAM
 */

//Constante usada para informar a quantidade de threads que serão criadas para a leitura dos arquivos
#define THREADS_READ 10
#define THREADS_SORT getCore()

//Mutex global para todas as thread...
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void readFilesThreads(pthread_t threads[], List vectList[], int qtd_thread);
void sortFilesThreads(pthread_t threads[], List vectList[], int qtd_thread);
void mergeListsThread(pthread_t threadMerge[], List vectList[]);

void threadsJoins(pthread_t threads[], int qtd_thread);


//Função que pega a quantidade de núcleos.
int getCore();

//Função usada para copiar os endereços de memória do vetor de listas para o vetor de listas auxliar.
void copyVector(List* v1[], List v2[], int cores);

int main(){

	Values values;
	
	List vectList[getCore()];///Lista de vetor com tamanho dos nucleos do pc. Usada para dividir o processo de ordenação

	pthread_t threadsRead[THREADS_READ];//Threads usadas na leitura dos arquivos.
	pthread_t threadsSort[THREADS_SORT];//Threads usadas na ordenação dos arquivos.
	pthread_t threadMerge[1];//Thread usada para fazer o merge das listas.

	int opcao = 0;
	 while(opcao != 5){
	 	cout << "-----Selecione uma opção-----"<<endl;
	 	cout << "1 - Criar arquivos"<<endl;
	 	cout << "2 - Ler arquivos"<<endl;
	 	cout << "3 - Ordenar"<<endl;
	 	cout << "4 - Sair"<<endl;
	 	cin >> opcao;

	 	if(opcao == 1){
	 		createFile(values.fileName,10);
	 		cout<<"10 arquivos criados..."<<endl;
	 	}else if(opcao == 2){
	 		cout<<"Criando as threads de leitura..."<<endl;
	 		readFilesThreads(threadsRead,vectList,THREADS_READ);
	 		threadsJoins(threadsRead,THREADS_READ);
	 	}else if(opcao == 3){
			cout<<"Ordenando arquivos..."<<endl;
	 		sortFilesThreads(threadsSort,vectList,THREADS_SORT);
	 		threadsJoins(threadsSort,THREADS_SORT);
	 		mergeListsThread(threadMerge, vectList);
	 		threadsJoins(threadMerge,1);
	 	}else
	 		break;
		
	 }


	pthread_exit(NULL);		
	return 0;
}


void copyVector(List* v1[], List v2[], int cores){
	for (int i = 0; i < cores; i++){
		v1[i] = &v2[i];
	}
}


void readFilesThreads(pthread_t threads[], List vectList[] ,int qtd_thread){
	Values* v = new Values();
	v->mutex = &mutex;
	v->core = getCore();
	copyVector(v->vectList,vectList,getCore());
	
	v->core = getCore();
	v->vectList[0] = &vectList[0];
	
	for (int i = 0; i < qtd_thread; i++){
		v->fileName[7] = IntToChar(i);
		pthread_create(&threads[i],NULL,readFile,(void*)v);
	}
}


void sortFilesThreads(pthread_t threads[], List vectList[], int qtd_thread){
	for (int i = 0; i < qtd_thread; i++){
		pthread_create(&threads[i],NULL,QuickSortThread,(void*)&vectList[i]);
	}
}


void mergeListsThread(pthread_t threadMerge[], List vectList[]){
	Values* v = new Values();
	v->core = getCore();
	copyVector(v->vectList,vectList,getCore());
	pthread_create(&threadMerge[0],NULL,mergeThread,(void*)v);
}


void threadsJoins(pthread_t threads[], int qtd_thread){
	for(int i = 0; i < qtd_thread; i++)
		pthread_join(threads[i],NULL);
}	

int getCore(){
	int n = (int)sysconf(_SC_NPROCESSORS_ONLN);
	return n;

}

