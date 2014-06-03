#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>

#define RED				    cout << "\033[1;31m";
#define GREEN   			cout << "\033[1;32m";
#define BLUE    			cout << "\033[1;36m";
#define BLUE_UNDERLINE    	cout << "\033[4;36m";
#define YELLOW  			cout << "\033[1;33m";
#define PURPLE  			cout << "\033[1;35m";
#define WHITE   			cout << "\033[1;37m";
#define RESET  				cout << "\033[0m";
#define CLEAR  				cout << "\033[2J";

#define SLEEP_P 100

#define para(i, a, b) for(int i = a; i < b; i++)
#define showL(m, color) color cout << m << endl; RESET;
#define show(m, color) color cout << m; RESET;
#define LINE cout << endl;

using namespace std;

typedef struct {
	int tam;
	int* porcent;
	string nome;
}MosPor;

unsigned int caractere = 0;
unsigned int caractere2 = 0;
int caractere3 = 0;
pthread_mutex_t count_mutex;

void printPorcent(int total, int porcent, string texto) {
	++caractere %= 8*3;
	++caractere2 %= 4*3;
	++caractere3 %= 40;
	double x = (double)porcent/total*40;
	printf("\033[F");
	
	show(texto + " ", BLUE);

	GREEN;
    
    if(total == porcent) cout << "";
    else if(caractere/3 == 0) cout << "";				
    else if(caractere/3 == 1) cout << "";
    else if(caractere/3 == 2) cout << "";
    else if(caractere/3 == 3) cout << "";
    else if(caractere/3 == 4) cout << "";
    else if(caractere/3 == 5) cout << "";
    else if(caractere/3 == 6) cout << "";
    else if(caractere/3 == 7) cout << "";
			

	cout << " [";
	
	GREEN;
	for(int i = 0; i < 40; i++) {
        if(i < (int)x) {
			if(caractere3 >= i-1 && caractere3 <= i+1 && total > porcent) {
				WHITE;
				cout << "■";
				GREEN; 
			}
			else 
				cout << "■"; 
		}
		else if(i == (int)x)
			cout << " ▶";
		else 
			cout << " ";
	}
	
	cout << " ] " << (int)(x*100/40) << "% ";
	
	if(total == porcent) cout << " ";
	else if(caractere2/3 == 0) cout << "◐ ";				
    else if(caractere2/3 == 1) cout << "◒ ";
    else if(caractere2/3 == 2) cout << "◑ ";
    else if(caractere2/3 == 3) cout << "◓ ";
    
    cout << endl;
    
    RESET;
}

void sleep(int sl) {
	clock_t t = clock();
	while(clock() - t < 1000*sl);
}

int getNucleos() {
	int x;
	system ("grep \"model name\" /proc/cpuinfo | wc -l >> saida.txt");
	FILE *arq = fopen("saida.txt", "r");
	fscanf(arq, "%d", &x);
	fclose(arq);
	system ("rm saida.txt");
	return x;
}

void* mostrarProgresso(void* args) {
	int tam = ((MosPor*)args)->tam;
	int* porcent = ((MosPor*)args)->porcent;
	string nome = ((MosPor*)args)->nome;
	
	while(true) {
		//cout << tam << " ; " << *porcent << endl << endl;
		pthread_mutex_lock(&count_mutex);
		int x = *porcent;
		pthread_mutex_unlock(&count_mutex);
		
		printPorcent(tam, x, nome);
        sleep(SLEEP_P);
		if(x >= tam) break;
	}
	
	return NULL;
}

#endif
