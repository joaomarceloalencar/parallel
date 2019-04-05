#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
   int rank, size, vetorTamanho;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   vetorTamanho = atoi(argv[1]);

   if (rank == 0) {

      // Cria os vetores
      srand(time(0));
      float *vetorA = (float *) malloc(vetorTamanho * sizeof(float));
      float *vetorB = (float *) malloc(vetorTamanho * sizeof(float));
      float *vetorC = (float *) malloc(vetorTamanho * sizeof(float));
      float valorA = (rand() % 100) / 3.0;
      float valorB = (rand() % 100) / 3.0;
      printf("rank %d: vou preencher vetorA com %.2f e vetor B com  %.2f.\n", rank, valorA, valorB);
      for (int i = 0; i < vetorTamanho; i++) {
         vetorA[i] = valorA;
         vetorB[i] = valorB;
         vetorC[i] = 0.0;
      }
      
      // Envia metade dos vetores para o rank 1
      int trabalhoLocal = vetorTamanho / 2;
      MPI_Send(vetorA + trabalhoLocal, trabalhoLocal, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
      MPI_Send(vetorB + trabalhoLocal, trabalhoLocal, MPI_FLOAT, 1, 1, MPI_COMM_WORLD);

      // Calcula sua parte
      for (int i = 0; i < trabalhoLocal; i++)
         vetorC[i] = vetorA[i] + vetorB[i];

      // Recebe a parte do rank 1
      MPI_Status status;
      MPI_Recv(vetorC + trabalhoLocal, trabalhoLocal, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, &status);

      // Imprime o resultado da soma
      printf("Vetor Resultado: \n");
      for (int i = 0; i < vetorTamanho; i++)
         printf("%.2f ", vetorC[i]);
      printf("\n");

      // Limpa a memória
      free(vetorA);
      free(vetorB);
      free(vetorC);
   
   } else {
      // Aloca espaço para os vetores
      int trabalhoLocal = vetorTamanho / 2;
      float *vetorALocal = (float *) malloc(trabalhoLocal * sizeof(float));
      float *vetorBLocal = (float *) malloc(trabalhoLocal * sizeof(float));
      float *vetorCLocal = (float *) malloc(trabalhoLocal * sizeof(float));

      // Recebe os vetores
      MPI_Status status;
      MPI_Recv(vetorALocal, trabalhoLocal, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(vetorBLocal, trabalhoLocal, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);
      
      // Faz seu trabalho
      for (int i = 0; i < trabalhoLocal; i++)
         vetorCLocal[i] = vetorALocal[i] + vetorBLocal[i];

      // Envia para rank 0 
      MPI_Send(vetorCLocal, trabalhoLocal, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
      
      // Limpa a memória
      free(vetorALocal);
      free(vetorBLocal);
      free(vetorCLocal);
   }

   MPI_Finalize();
   return 0;
}

