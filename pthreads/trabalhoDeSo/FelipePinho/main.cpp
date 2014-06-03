#include <iostream>
#include <cstdlib>
#include <string>
#include <pthread.h>
#include "Lista.cpp"
#include "fileManager.cpp"
#include "utilidades.cpp"

using namespace std;

int main() {

	system("clear");

	showL("Abrindo arquivos", YELLOW);
	Lista l = newLista();
	abrirArquivos(l, "binarios/arquivo");
	showL("Arquivos abertos!", GREEN);
    LINE;


	//Variavel que auxilia na porcentagem
	setTotalQuick(l.tam);

	//criando primeiro processo
    showL("Ordenando lista com " << l.tam << " elementos...", YELLOW);
	Processo p = {l.primeiro, l.primeiro->ant};
    addProcesso(p);


	//iniciando reação em cadeia
	executarProcessos();
    showL("Ordenação finalizada! ", GREEN);
	LINE;


	//mostrarLista(l);
	if(isOrdenado(l)) {
        showL("Lista ordenada! ☺ ", BLUE);
	}
	else {
        showL("ERRO! ☹ ", RED);
	}
	LINE;
	
	showL("Gravando saida...", YELLOW);
    gravarLista(l, "binarios/output.bin");
    showL("Saida gravada em output.bin!", GREEN);
    LINE;
	
	
	return 0;
}

