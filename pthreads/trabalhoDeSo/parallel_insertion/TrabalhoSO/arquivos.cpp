#ifndef FILE_MANAGER_CPP
#define FILE_MANAGER_CPP

#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <pthread.h>
#include "Lista.cpp"

extern pthread_mutex_t count_mutex;

typedef struct {
	Lista *l;
	string arquivo;
	//aqui
}ListaArquivos;

//aqui
void* loadList(void *listaArq) {
		
	ListaArquivos *la = (ListaArquivos*) listaArq;
	string arquivo = (string)la->arquivo;
	Lista *l = (Lista*)la->l;
	//aqui
	FILE *arquivos_file;
	arquivos_file = fopen(arquivo.c_str(), "r");
	
	if(arquivos_file == NULL)
		return listaArq;
		
	while(true) {
		int num;
		fread(&num, sizeof(num), 1, arquivos_file);
		
			pthread_mutex_lock(&count_mutex);
		inserir(*l, num);
			pthread_mutex_unlock(&count_mutex);
			
		if(feof(arquivos_file)) {
				pthread_mutex_lock(&count_mutex);
			removerFim(*l);
				pthread_mutex_unlock(&count_mutex);
			break;
		}
		
	}
	
	fclose(arquivos_file);
	
	return NULL;
}

void openarquivos(Lista l[], string arquivo) {
	int i = 0;
	int numNucleos = getNucleos();
	
	pthread_t vetThread[20];
	
	while(true) {
		stringstream s;
		s.clear();
		
		s << arquivo << i << ".bin";
		
		string arquivotemp = s.str();
		
		FILE* x = fopen(arquivotemp.c_str(), "r");
		
		if(x == NULL) {
			break;
		}
		
		fclose(x);
		
		ListaArquivos *la = new ListaArquivos();
		la->arquivo = arquivotemp;
		la->l = &l[i % numNucleos];
		
		pthread_create(&vetThread[i], NULL, loadList, (void*)la);
		
		i++;
	}

	para(j, 0, i) {
		pthread_join(vetThread[j], NULL);
	}
}

void gravarLista(Lista l, string arquivo) {
	FILE *arquivos_file = fopen(arquivo.c_str(), "w+");
	
	if(arquivos_file == NULL)
		cout << "erro na gravação!: " << arquivo << endl;
	
	int tam = l.tamanho;
	No *no = l.primeiro;
	
	for(int i = 0; i < tam; i++) {
		int num = no->valor;
		no = no->prox;
		fwrite(&num, sizeof(num), 1, arquivos_file);
	}
	
	fclose(arquivos_file);
}

#endif
