#include <stdio.h>
#include <stdlib.h>
#include "Listas.h"
#include "InsertionSort.h"
#include "auxilio.h"
#include <time.h>


int main(){
	Lista* l = preenche();
 	Lista* lis = threads(l);
	ordenar(lis);
	//imprimir(lis);
	l=entrelacar(lis);
	//imprimir_l(l);
	gera_arquivo(l);
	return 0;
}

