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
int N = 15;
int melhor = INFINITO;
int upper_bound;

//int mat_h[]={999999,36,515,317,96,491,288,108,725,104,
//421,999999,718,392,331,329,502,303,731,229,
//78,435,999999,807,604,247,478,455,615,943,
//619,216,572,999999,883,247,488,576,297,618,
//163,180,497,523,999999,704,640,790,256,516,
//636,701,6,241,531,999999,206,643,968,799,
//224,144,824,90,697,280,999999,914,945,64,
//655,660,868,493,3,199,930,999999,9,258,
//628,486,909,656,709,804,968,861,999999,561,
//623,858,885,768,111,205,669,740,146,999999};

/*
int mat_h[]={999999,82,902,283,294,74,472,904,519,889,905,649,8,348,
600,999999,264,912,985,42,948,98,788,709,834,379,281,607,
347,901,999999,36,60,383,248,916,196,123,830,179,114,68,
835,104,314,999999,673,662,191,123,939,86,511,578,665,764,
431,168,842,68,999999,135,238,762,612,542,623,268,265,481,
40,699,356,923,222,999999,30,717,693,927,391,376,489,176,
812,91,169,144,721,966,999999,856,960,714,744,618,395,425,
79,343,814,785,913,913,537,999999,946,784,549,184,6,18,
214,710,292,337,800,796,484,48,999999,397,286,873,149,854,
686,144,507,21,384,485,716,648,456,999999,374,250,483,731,
911,150,394,25,639,412,694,632,979,186,999999,717,331,507,
159,829,550,462,679,514,997,45,276,306,775,999999,263,855,
699,951,22,830,187,874,759,619,302,80,375,61,999999,26,
74,699,421,924,498,506,386,29,156,210,840,225,910,999999};  
*/

 int mat_h[]={999999,95,382,198,245,556,428,341,822,399,72,58,184,610,248,
 739,999999,724,944,199,263,898,669,99,362,674,164,62,290,267,
 233,48,999999,375,240,113,764,502,537,394,314,366,171,526,506,
 60,665,22,999999,241,910,50,921,563,796,731,412,628,455,168,
 463,756,247,211,999999,120,733,335,221,581,291,43,246,62,469,
 902,305,741,540,62,999999,10,741,328,918,796,982,256,261,514,
 377,812,355,795,469,752,999999,817,483,264,809,554,455,357,361,
 402,878,760,154,614,348,790,999999,55,395,830,266,244,66,241,
 268,833,718,998,757,251,978,905,999999,107,723,966,160,469,188,
 889,549,602,416,110,163,677,571,77,999999,347,513,246,397,549,
 504,402,300,54,222,155,967,692,679,283,999999,763,974,339,648,
 879,19,269,49,844,234,772,2,978,919,874,999999,659,925,474,
 786,881,157,149,553,686,887,295,582,924,559,393,999999,936,654,
 327,134,481,611,16,653,595,984,508,164,888,506,824,999999,925,
 139,35,225,820,309,357,511,306,709,187,1,680,385,474,999999};

//int mat_h[]={999999, 65, 949, 257, 38, 23, 859, 92, 140, 619, 880, 388, 789, 
//971, 999999, 421, 621, 513, 970, 214, 803, 534, 238, 852, 305, 76, 
//810, 839, 999999, 116, 75, 857, 443, 128, 612, 801, 670, 767, 445, 
//912, 247, 932, 999999, 480, 247, 941, 925, 961, 269, 438, 305, 28, 
//448, 46, 368, 820, 999999, 823, 213, 840, 23, 143, 423, 104, 746, 
//16, 339, 200, 305, 253, 999999, 580, 901, 625, 131, 832, 823, 282, 
//184, 12, 353, 333, 725, 566, 999999, 443, 829, 139, 572, 192, 699, 
//573, 461, 913, 231, 604, 817, 355, 999999, 375, 89, 68, 706, 801, 
//725, 426, 388, 248, 918, 89, 984,672,999999,563,410,914,370, 
//334, 736, 424, 926, 660, 118,884,942,224,999999,899,802,312, 
//480, 607, 148, 689, 938, 960,997,494,149,176,999999,42,120, 
//977, 146, 223, 72, 89, 666,338,732,563,993,30,999999,424, 
//204, 558, 134, 979, 925, 470, 913, 706, 632, 65, 995, 17, 999999 };

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
  melhor_solucao_local = callCompleteEnumStreams(nivel,nPrefixos,inicio,fim,prefixos,&qtd_solucoes,rank);

  

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


