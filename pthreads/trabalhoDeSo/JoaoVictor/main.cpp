#include "arquivos.cpp"
#include "Lista.cpp"
#include <iostream>
#include <pthread.h>

using namespace std;

int main() {
	
	int numerodeNucleos;
	numerodeNucleos = getNucleos();
	
	Lista l[numerodeNucleos];
	
	para(i, 0, numerodeNucleos)
		l[i] = newLista();
	
	cout << "Arquivos sendo abertos............" << endl;
	openarquivos(l, "binarios/arquivo");
	
	cout << "Lista sendo ordenada............" << endl;
	ordenarListas(l);
	
	cout << "Merge listas............" << endl;
	Lista novaLista = newLista();
	mergeThread(l, numerodeNucleos, novaLista);
	
	
	cout << "Calma, Gravando Listas : " << novaLista.tamanho << endl;
	gravarLista(novaLista, "binarios/output.bin");
	
	//exibirLista(novaLista);

	return 0;
}
