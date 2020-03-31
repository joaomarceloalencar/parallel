#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
   int  numtasks, rank, len, rc; 

   // Inicializar o Ambiente.  
   MPI_Init(&argc,&argv);

   // Recuperar o número de Processos.
   MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

   if (numtasks != 2) {
       printf("O exemplo requer dois processos.\n");
       MPI_Finalize();
       return 0;
   }

   // Recuperar o identificador do processo.
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   
   printf("Iniciando comunicação.\n");
   if (rank == 0) {
       MPI_Status status;
       int buffer = 42;

       // Primeira Mensagem
       printf("Rank %d: enviando primeira mensagem.\n", rank);
       MPI_Send(&buffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

       // Segunda Mensagem
       printf("Rank %d: recebendo segunda mensagem.\n", rank);
       MPI_Recv(&buffer, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);

       printf("Rank %d: Buffer %d.\n", rank, buffer);

   } else if (rank == 1) {
       MPI_Status status;
       int buffer;

       // Primeira Mensagem
       printf("Rank %d: recebendo primeira mensagem.\n", rank);
       MPI_Recv(&buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
       buffer = buffer + 1;

       // Segunda Mensagem 
       printf("Rank %d: enviando segunda mensagem.\n", rank);
       MPI_Send(&buffer, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
   }


   // Finalizar o ambiente. 
   MPI_Finalize();
   
   return 0;
}
