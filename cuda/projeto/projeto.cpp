#include <stdio.h>
#include <mpi.h>

#include <string.h>
#include <stdlib.h>

#include "projeto.cuh"

#define mat(i,j) mat_h[i*N+j]
#define mat_h(i,j) mat_h[i*N+j]

#define _MASTER_ 0


int qtd = 0;
int custo = 0;
int N = 10;
int melhor = INFINITO;
int upper_bound;

int mat_h[]={999999,36,515,317,96,491,288,108,725,104,
421,999999,718,392,331,329,502,303,731,229,
78,435,999999,807,604,247,478,455,615,943,
619,216,572,999999,883,247,488,576,297,618,
163,180,497,523,999999,704,640,790,256,516,
636,701,6,241,531,999999,206,643,968,799,
224,144,824,90,697,280,999999,914,945,64,
655,660,868,493,3,199,930,999999,9,258,
628,486,909,656,709,804,968,861,999999,561,
623,858,885,768,111,205,669,740,146,999999};



// void read() {
//   int i;
//   scanf("%d", &N);
//   for (i = 0; i < (N * N); i++) {
//     scanf("%d", &mat_h[i]);
//   }

// }





int main(int argc, char *argv[]) {

  int melhor_solucao_local;

  unsigned long long nPrefixos;

  int numprocs, rank, namelen;
  int melhor_solucao_global = 0;
  int solucaoRemota;
  int acumulador_de_solucoes = 0;
  int qtd_sol_remota;
  int nivel = 5;
  int qtd_solucoes;


  int intervalo;

  int inicio;
  int fim;

  short *prefixos;

  printf("\nPrefixos comuns: %llu", calculaNPrefixos(nivel, N));

  char processor_name[MPI_MAX_PROCESSOR_NAME];

  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(processor_name, &namelen);



  if((N%numprocs)!=0){

      if(rank == (numprocs-1)){
        intervalo = N/numprocs + (N%numprocs);
      }
      else
        intervalo = N/numprocs;
  }
  else
    intervalo = N/numprocs;



  printf("\nSou o processo %d, faixa e' %d, meu intervalo e' de %d a %d\n",rank, intervalo,rank*(N/numprocs), rank*(N/numprocs)+intervalo-1 );


  inicio = rank*(N/numprocs);

  fim = rank*(N/numprocs)+intervalo-1;

  nPrefixos = calculaNPrefixos(nivel,N,intervalo,rank);

  prefixos = (short*)malloc(sizeof(short)*nPrefixos*nivel);

 // callCompleteEnumStreams(5);
 // callCompleteEnumStreams(2,inicio,fim);
  
  fillFixedPaths(prefixos,nivel,inicio,fim);
  printf("Quantidade de prefixos: %llu \n", nPrefixos );
  // for(int cont = 0; cont<nPrefixos;++cont){
  //   for (int i = 0; i < nivel; ++i) {
  //         printf("%d - ", prefixos[cont * nivel + i]);
  //   }
  //   printf("\n");
  // }


/*
  *chamando kernel
*/
  melhor_solucao_local = callCompleteEnumStreams(nivel,nPrefixos,inicio,fim,prefixos,&qtd_solucoes);

  

  /*****************************
    * Sincronizacao de UB
  ******************************/

  if(rank == _MASTER_){
   melhor_solucao_global = melhor_solucao_local;
    for (int proc=0; proc<numprocs; proc++) if (proc!=rank) {
          MPI_Recv(&solucaoRemota, 1, MPI_INT, proc, 1, MPI_COMM_WORLD, &status);
          printf("\nRecebendo %d de %d.\n", solucaoRemota,proc);
          if(melhor_solucao_global>solucaoRemota)
            melhor_solucao_global = solucaoRemota;
    }
  }
  else
     MPI_Send(&melhor_solucao_local, 1, MPI_INT, _MASTER_, 1, MPI_COMM_WORLD); 


  if(rank == _MASTER_)
    printf("\nA melhor solucao global foi: %d.\n", melhor_solucao_global);



 /*****************************
    * Sincronizacao de qtd de solucoes
  ******************************/

  if(rank == _MASTER_){
   acumulador_de_solucoes = qtd_solucoes;
    for (int proc=0; proc<numprocs; proc++) if (proc!=rank) {
          MPI_Recv(&qtd_sol_remota, 1, MPI_INT, proc, 1, MPI_COMM_WORLD, &status);
          printf("\nRecebendo %d solucoes de %d.\n", qtd_sol_remota,proc);
          acumulador_de_solucoes+=qtd_sol_remota;
    }
  }
  else
     MPI_Send(&qtd_solucoes, 1, MPI_INT, _MASTER_, 1, MPI_COMM_WORLD); 


  if(rank == _MASTER_)
    printf("\nA quantidade de solucoes do sistema foi: %d.\n", acumulador_de_solucoes);


  MPI_Finalize();
}


