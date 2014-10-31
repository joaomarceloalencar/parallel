#ifndef __PREFIXOS_H__
#define __PREFIXOS_H__

#define mat(i,j) mat_h[i*N+j]
#define mat_h(i,j) mat_h[i*N+j]

extern int qtd;
extern int custo;
extern int N;
extern int melhor;
extern int upper_bound;

extern int mat_h[MAX];

void fillFixedPaths(short* preFixo, int nivelPrefixo);
unsigned long long int calculaNPrefixos(const int nivelPrefixo, const int nVertice);
#endif