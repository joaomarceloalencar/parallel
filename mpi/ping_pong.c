#include <stdio.h>
#include "mpi.h"

int main (int argc, char *argv[]) {
   int rank, size;
   int value = 0;
   MPI_Status status;

   MPI_Init (&argc, &argv);	
   MPI_Comm_rank (MPI_COMM_WORLD, &rank);	
   MPI_Comm_size (MPI_COMM_WORLD, &size);	

   if (rank == 0) {
       value++;
       MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
       printf("Processo %d: %d.\n", rank, value);		
   } else if (rank == 1) {
       MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
       value++;
       printf("Processo %d: %d.\n", rank, value);		
   }
   
   MPI_Finalize();
   return 0;
}
