#include <cstdio>
#include <string>
#include <sstream>
#include <pthread.h>
#include <unistd.h>
#include "MergeSort.h"
#include "estruturas.h"
#include "Lista.h"

#define NUMFILE 10
#define Nucleos num_nucleo
using namespace std;

void fazerThreadsAddLista(string ,int,Lista*);
void* colocaFileLista(void*);
void ordena(Lista* );
void* MergeSorteThreads(void*);

void criarSaida(Lista* );

pthread_mutex_t count_mutex;

int main(int argc, char *argv[]){
	
	//Crio lista
	Lista* L = CriarLista();
	
	string filename = "arquivo";
	int quant_File = NUMFILE;
		

	fazerThreadsAddLista(filename,quant_File,L);
	ordena(L);
	criarSaida(L);
	
	//ListarValores(L);

	return 0;
}

void ordena(Lista* L){
	int num_nucleo=(int)sysconf(_SC_NPROCESSORS_ONLN);
	//quantidade nos
	int med = L->n/Nucleos;
	int rest = L->n%Nucleos;
	
	pthread_t vetThreads[Nucleos];
	
	ArgMerge vetArgMerge[Nucleos];

	NO* No=L->raiz;
	int i=0;
	while(i<Nucleos){
		vetArgMerge[i].tam=med;
		if(rest>0){
			vetArgMerge[i].tam++;
			rest--;
		}
		vetArgMerge[i].No=No;
		No=NoDeInicio(No,vetArgMerge[i].tam);
		pthread_create(&vetThreads[i], NULL, MergeSorteThreads, (void*)&vetArgMerge[i]);
		i++;
	}
	
	for(i = 0; i < Nucleos; i++){
		pthread_join(vetThreads[i],NULL);
	}
	
	int nuc = Nucleos;
	while(nuc>1){
		if(nuc%2==1){
			vetArgMerge[0].No=Merge(vetArgMerge[0].No,vetArgMerge[nuc-1].No,vetArgMerge[0].tam,vetArgMerge[nuc-1].tam);
			vetArgMerge[0].tam+=vetArgMerge[nuc-1].tam;
		}
		int perc=nuc/2;
		for(int i=0;i<perc;i++){
			vetArgMerge[i].No=Merge(vetArgMerge[i].No,vetArgMerge[i+perc].No,vetArgMerge[i].tam,vetArgMerge[i+perc].tam);
			vetArgMerge[i].tam+=vetArgMerge[i+perc].tam;
		}
		nuc=nuc/2;
	}
	L->raiz=vetArgMerge[0].No;
}

void* MergeSorteThreads(void* args){
	
	ArgMerge *am = (ArgMerge*) args;
	am->No=MergeSorte(am->No,am->tam);
}

void fazerThreadsAddLista(string filename,int quant_files,Lista* L){
	pthread_t vetThreads[quant_files];
	
	ArgFuncoes vetArgumentos[quant_files];
	int i;

	for(i = 0; i < quant_files; i++) {
		std::stringstream s;
		s << filename << i << ".bin";
		vetArgumentos[i].filename = s.str();
		vetArgumentos[i].list = L;
		vetArgumentos[i].ref = false;
		pthread_create(&vetThreads[i], NULL, colocaFileLista, (void*)&vetArgumentos[i]);
	}
	
	for(i = 0; i < quant_files; i++){
		pthread_join(vetThreads[i], NULL);
	}
	
}

void* colocaFileLista(void* args){
	ArgFuncoes *af = (ArgFuncoes*) args;
		
	Lista *L = af->list;
	string f = af->filename;
	
	FILE *fp = fopen(f.c_str(), "r");
	
	//if(fp == NULL) printf("%s : criando thread\n", f.c_str());
	
	int number = 0;
	while (1) {
		if(L->n>=NUMFILE) break;
		if(af->ref==true) continue;
		
		fread(&number, sizeof(number), 1, fp);
		if(feof(fp)) break; 
		
		pthread_mutex_lock(&count_mutex);
		af->No=Inserir(L,number);
		af->ref=true;
		pthread_mutex_unlock(&count_mutex);
	}
	
	int i=0;
	while (1) {
		fread(&number, sizeof(number), 1, fp);
		if(feof(fp)) break; 
		InserirPosicaoNO(number,af->No);
		i++;
	}
	
	pthread_mutex_lock(&count_mutex);
		L->n=L->n+i;
	pthread_mutex_unlock(&count_mutex);

	fclose(fp);
}

void criarSaida(Lista* L){
	FILE *fp = fopen("output.bin", "w+");
	
	srand(time(0));
	NO* N=L->raiz;
	while( N!= NULL) {
		fwrite( &N->valor, sizeof(N->valor), 1, fp);
		N=N->prox;
	}
   
   fclose(fp);
}
