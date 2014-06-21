#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <stdio.h>
#include <cstdlib>

#define para(i, a, b) for(int i = a; i < b; i++)

using namespace std;

pthread_mutex_t count_mutex;

int getNucleos() {
	int x;
	system ("grep \"model name\" /proc/cpuinfo | wc -l >> saida.txt");
	FILE *arquivos_file = fopen("saida.txt", "r");
	fscanf(arquivos_file, "%d", &x);
	fclose(arquivos_file);
	system ("rm saida.txt");
	return x;
}

#endif
