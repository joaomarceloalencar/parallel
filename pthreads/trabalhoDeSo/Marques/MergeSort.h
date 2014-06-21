#include <stdio.h>
#include <stdlib.h>
#include "Lista.h"
#include "estruturas.h"

NO* Merge(NO* NoL,NO* NoLi,int tam,int seg){
	
	int n=tam+seg;
	int i=0;

	seg=tam;
	tam=0;
	
	NO* ultimoNO=NULL;
	NO* auxNO=ultimoNO;
	while(tam < n){
		if(i<seg && (NoLi==NULL || NoL->valor<=NoLi->valor)){
			InserirNo(&auxNO,NoL,ultimoNO);
			ultimoNO=NoL;
			NoL=NoL->prox;
			tam++;
			i++;
		}else{
			InserirNo(&auxNO,NoLi,ultimoNO);
			ultimoNO=NoLi;
			NoLi=NoLi->prox;
			tam++;
		}
	}
	
	return auxNO;
}

NO* MergeSorte(NO* No,int tam){
	if(tam>1){
		int aux=tam;
		tam=aux/2;
		int seg = tam+(aux%2);
		
		NO* SegNO=NoDeInicio(No,tam);
		
		No = MergeSorte(No,tam);
		SegNO = MergeSorte(SegNO,seg);
		No=Merge(No,SegNO,tam,seg);
	}
	return No;
}

