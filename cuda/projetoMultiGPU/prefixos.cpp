#include "prefixos.h"


unsigned long long int calculaNPrefixos(const int nivelPrefixo, const int nVertice) {
  unsigned long long int x = nVertice - 1;
  int i;
  for (i = 1; i < nivelPrefixo-1; ++i) {
    x *= nVertice - i-1;
  }
  return x;
}



void fillFixedPaths(short* preFixo, int nivelPrefixo) {
  char flag[50];
  int vertice[50]; 
  int cont = 0;
  int i, nivel; 


  for (i = 0; i < N; ++i) {
    flag[i] = 0;
    vertice[i] = -1;
  }

  vertice[0] = 0; 
  flag[0] = 1;
  nivel = 1;
  while (nivel >= 1){

    if (vertice[nivel] != -1) {
      flag[vertice[nivel]] = 0;
    }

    do {
      vertice[nivel]++;
    } while (vertice[nivel] < N && flag[vertice[nivel]]); 

    if (vertice[nivel] < N) { 
      flag[vertice[nivel]] = 1;
      nivel++;
      if (nivel == nivelPrefixo) {
        for (i = 0; i < nivelPrefixo; ++i) {
          preFixo[cont * nivelPrefixo + i] = vertice[i];
        }
        cont++;
        nivel--;
      }
    } else {
      vertice[nivel] = -1;
      nivel--;
    }
  }
}

