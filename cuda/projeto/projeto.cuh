#ifndef __PROJETO_CUH__
#define __PROJETO_CUH__

#define mat(i,j) mat_h[i*N+j]
#define mat_h(i,j) mat_h[i*N+j]
#define mat_d(i,j) mat_d[i*N_l+j]
#define mat_block(i,j) mat_block[i*N_l+j]
#define proximo(x) x+1
#define anterior(x) x-1
#define MAX 8192
#define INFINITO 999999
#define ZERO 0
#define ONE 1

#define _VAZIO_      -1
#define _VISITADO_    1
#define _NAO_VISITADO_ 0

extern int qtd;
extern int custo;
extern int N;
extern int melhor;
extern int upper_bound;

extern int mat_h[MAX];

#define CUDA_CHECK_RETURN(value) {                      \
  cudaError_t _m_cudaStat = value;                    \
  if (_m_cudaStat != cudaSuccess) {                   \
    fprintf(stderr, "Error %s at line %d in file %s\n",         \
        cudaGetErrorString(_m_cudaStat), __LINE__, __FILE__);   \
    exit(1);                              \
  } }

#define HANDLE_NULL( a ) {if (a == NULL) { \
    printf( "Host memory failed in %s at line %d\n", \
        __FILE__, __LINE__ ); \
        exit( EXIT_FAILURE );}}


void checkCUDAError(const char *msg);

int fillFixedPaths(short* preFixo, int nivelPrefixo, int inicio, int fim);

// int callCompleteEnumStreams(const int nivelPreFixos, const int inicio, const int fim);

int callCompleteEnumStreams(int nivelPreFixos,  int nPreFixos,int inicio, int fim, short *path_h, int *qtd_sol);

unsigned long long int calculaNPrefixos(const int nivelPrefixo, const int nVertice);
unsigned long long int calculaNPrefixos(const int nivelPrefixo, const int nVertice, const int intervalo,const int rank);
// int fillFixedPaths(int nivelPrefixo,int inicio,int fim);
#endif