#ifndef FILE_MANAGER_CPP
#define FILE_MANAGER_CPP

#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <pthread.h>
#include "Lista.cpp"


 pthread_mutex_t count_mutex_file;


//**************************
//Estruturas
//**************************
typedef struct {
	Lista *l;
	string arquivo;
	int numThreads;
	int id;
}ListaArq;


//**************************
//Variaveis globais
//**************************
int tamFiles = 0;
int porcentFiles = 0;

bool* threadsModificadas;
//**************************


//função que retorna a quantidade de inteiros em um arquivo binario
int tamFile(FILE *file) {
	fseek(file, 0, SEEK_END);
	int tam = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	return tam/4;
}


//função chamada em cada uma das 10 threads criadas na leitura dos arquivos
void* arquivo2Lista(void *listaArq) {
		
	ListaArq *la 	=	(ListaArq*) listaArq;
	Lista *l 		=	(Lista*)la->l;
	string arquivo 	= 	(string)la->arquivo;
	int numThreads 	= 	(int)la->numThreads;
	int id 			=	(int)la->id;
	
	FILE *file = fopen(arquivo.c_str(), "r");
	
	if(file == NULL)
		return NULL;

    int tamF 			= tamFile(file);	//ponto de parada da leitura do arquivo
	bool inserirNoMeio 	= false;			//define quando o mutex deixará de controlar a inserção
	No* noInserir 		= NULL;				//referencia para inserção na lista

    while(tamF-- > 0) {
		int num = 0;

		fread(&num, sizeof(num), 1, file);		//lendo inteiro
		
		pthread_mutex_lock(&count_mutex_file);	//gerencia a concorrencia na inserção
		add(*l, num);							//adicionando na lista
		porcentFiles++;							//variával auxiliar na visualização da porcentagem
		
		if(noInserir == NULL && l->tam >= numThreads*2) {	//se a lista tiver um tamanho suficiente
			No* no 	= l->primeiro;							//criamos uma referencia de algum no da lista
			int i 	= id*2;

			while(i-- > 0)
				no = no->prox;

			noInserir 				= no;
			threadsModificadas[id] 	= true;
		}
		
		inserirNoMeio = true;			//se todas as threads jah refenciaram seu 'noInserir'
		para(i, 0, numThreads)			//paramos esse primeiro laço
			inserirNoMeio &= threadsModificadas[i];
			
		pthread_mutex_unlock(&count_mutex_file);
		
		if(inserirNoMeio) break;
	}
	
	while(tamF-- > 0) {
		int num;
		fread(&num, sizeof(num), 1, file);
	
		addAt(num, noInserir);	//dessa vez o add sem mutex pois cada thread insere em um no diferente
		
		pthread_mutex_lock(&count_mutex_file);	//o mutex e usado apenas no incremento do tamanho
		porcentFiles++;
		l->tam++;
		pthread_mutex_unlock(&count_mutex_file);
		
	}
	
	fclose(file);
	
	return NULL;
}

int numeroDeArquivos(string arquivo) {
	int i = 0;
	while(true) {
		stringstream s;
		s << arquivo << i << ".bin";
		string arquivo2 = s.str();

		FILE* x = fopen(arquivo2.c_str(), "r");
		if(x == NULL)
			break;

		fclose(x);
		
		i++;
	}
	return i;
}

void abrirArquivos(Lista &l, string arquivo) {
	int i = 0;

	int numThreads 		= numeroDeArquivos(arquivo);
	threadsModificadas 	= new bool[numThreads];

	para(i, 0, numThreads)
		threadsModificadas[i] = false;
	
	pthread_t vetThread[numThreads];
	
	while(true) {
		stringstream s;
		s << arquivo << i << ".bin";
		string arquivo2 = s.str();
		
		FILE* x = fopen(arquivo2.c_str(), "r");
		if(x == NULL)
			break;
		
		tamFiles += tamFile(x);
		fclose(x);
		
		ListaArq *la 	= new ListaArq();
		la->arquivo 	= arquivo2;
		la->l 			= &l;
		la->numThreads 	= numThreads;
		la->id 			= i;
		
		pthread_create(&vetThread[i], NULL, arquivo2Lista, (void*)la);
		
		i++;
	}
	
	pthread_t tpf;		//criando thread que vai mostrar o progresso da leitura do qrquivo
	MosPor* args 	= new MosPor;
	args->tam 		= tamFiles;
	args->porcent 	= &porcentFiles;
	args->nome 		= "Carregando Lista";

	pthread_create(&tpf, NULL, mostrarProgresso, (void*) args);
	
	show(i, RED);
	showL(" Threads criadas!", YELLOW);
	LINE;
	
	para(j, 0, i)
		pthread_join(vetThread[j], NULL);

	pthread_join(tpf, NULL);
	
}

void gravarLista(Lista l, string arquivo) {
	FILE *arq = fopen(arquivo.c_str(), "w+");
	if(arq == NULL)
		cout << "erro na gravação!: " << arquivo << endl;
	
	int tam = l.tam;
	No *no 	= l.primeiro;
	
	for(int i = 0; i < tam; i++) {
		int num = no->valor;
		no 		= no->prox;
		
		fwrite(&num, sizeof(num), 1, arq);
	}
	
	fclose(arq);
}

#endif
