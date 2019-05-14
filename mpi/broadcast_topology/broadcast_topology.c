#include <stdio.h>
#include "mpi.h"
#include <string.h>
#include <stdlib.h>

int printArrayForRank(int rank, int *array,  int arraySize) {
   int i;
   char *output = (char *) malloc((arraySize + 100) * 4 * sizeof(char));
   sprintf(output, "Rank %d: ", rank);
   for (int i = 0; i < arraySize; i++)
      sprintf(output, "%s %d", output, array[i]);
   sprintf(output,"%s\n", output);
   printf("%s", output);
   fflush(stdout);
   free(output);
   return i;
}

int MPI_Ring_broadcast(int *array, int arraySize, MPI_Comm comm) {
   int size, rank;
   MPI_Comm_size(comm, &size);
   MPI_Comm_rank(comm, &rank);

   MPI_Request request[2];
   MPI_Status status[2];

   if (rank == 0) {
     int right = 1;
      int left = size - 1;
      MPI_Isend(array, arraySize, MPI_INT, right, 0, comm, &request[0]);
      MPI_Isend(array, arraySize, MPI_INT, left, 1, comm, &request[1]);

      MPI_Wait(&request[0], &status[0]);
      MPI_Wait(&request[1], &status[1]);

   } else {
      int middle = size / 2;
      if (rank < middle) {
         int right = rank + 1;
         int left = rank - 1;

         MPI_Irecv(array, arraySize, MPI_INT, left, 0, comm, &request[0]);
         MPI_Wait(&request[0], &status[0]);

         if (rank != middle - 1) { 
            MPI_Isend(array, arraySize, MPI_INT, right, 0, comm, &request[1]);
            MPI_Wait(&request[1], &status[1]);
         }
      } else {
         int right = (rank + 1) % size;
         int left = rank - 1;

         MPI_Irecv(array, arraySize, MPI_INT, right, 1, comm, &request[0]);
         MPI_Wait(&request[0], &status[0]);

         if (rank != middle) {
            MPI_Isend(array, arraySize, MPI_INT, left, 1, comm, &request[1]);
            MPI_Wait(&request[1], &status[1]);
         }
      }
   }
 
   return 0;
}

int MPI_Mesh_broadcast(int *array, int arraySize, MPI_Comm comm) {
   int size, rank;

   MPI_Comm_size(comm, &size);
   MPI_Comm_rank(comm, &rank);
   MPI_Request request[3];
   MPI_Status status[3];

   if (size != 16)  {
      return -1;
   }
   
   int ndims = 2;
   int dims[2] = {4, 4};
   int periods[2] = {0, 0};
   int reorder = 0; 
   MPI_Comm mesh_comm;

   MPI_Cart_create(comm, ndims, dims, periods, reorder, &mesh_comm);
   int rank_source, rank_dest, coords[2];
   MPI_Cart_coords(mesh_comm, rank, ndims, coords); 

   MPI_Cart_shift(mesh_comm, 0, 1, &rank_source, &rank_dest);
   int up = rank_source;
   int down = rank_dest;
   // printf("Rank %d Dir %d Coords (%d, %d) Source %d  Dest %d\n", rank, 0, coords[0], coords[1], rank_source, rank_dest);

   MPI_Cart_shift(mesh_comm, 1, 1, &rank_source, &rank_dest);
   int left = rank_source; 
   int right = rank_dest;
   // printf("Rank %d Dir %d Coords (%d, %d) Source %d  Dest %d\n", rank, 1, coords[0], coords[1], rank_source, rank_dest);

   if ((coords[0] == 0 ) && (coords[1] == 0)) {
      MPI_Isend(array, arraySize, MPI_INT, right, 1, comm, &request[0]);
      MPI_Isend(array, arraySize, MPI_INT, down, 0, comm, &request[1]);

      MPI_Wait(&request[0], &status[0]);
      MPI_Wait(&request[1], &status[1]);
   } else if (coords[1] == 0) {
      MPI_Irecv(array, arraySize, MPI_INT, up, 0, comm, &request[0]);
      MPI_Wait(&request[0], &status[0]);
      
      if (right > 0) {
         MPI_Isend(array, arraySize, MPI_INT, right, 1, comm, &request[1]);
      }

      if (down > 0) {
         MPI_Isend(array, arraySize, MPI_INT, down, 0, comm, &request[2]);
      }

      if (right > 0) 
         MPI_Wait(&request[1], &status[1]);
      if (down > 0)
         MPI_Wait(&request[2], &status[2]);

   } else {
      MPI_Irecv(array, arraySize, MPI_INT, left, 1, comm, &request[0]);
      MPI_Wait(&request[0], &status[0]);

      if (right > 0) {
         MPI_Isend(array, arraySize, MPI_INT, right, 1, comm, &request[1]);
         MPI_Wait(&request[1], &status[1]);
      }
   }

   return 0;
}


int main(int argc, char *argv[]) {
   int size, rank;
   int *array, arraySize;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
   arraySize = atoi(argv[1]);
   array = (int *) malloc(arraySize * sizeof(int));
 
   // Inicializa o array com 0.
   if (rank == 0) {
     for (int i = 0; i < arraySize; i++)
         array[i] = 0;
   }

   // Imprime o array inicial de todos os processos.
   printArrayForRank(rank, array, arraySize);

   // Faz broadcast usando uma topologia anel.
   MPI_Ring_broadcast(array, arraySize, MPI_COMM_WORLD);

   // Barreira de sincronização.
   MPI_Barrier(MPI_COMM_WORLD);

   // Imprime o array após o broadcast na topologia anel.
   printArrayForRank(rank, array, arraySize);
  
   // Barreira de sincronização.
   MPI_Barrier(MPI_COMM_WORLD);
   
   // Reconfigura o array com todos os elementos iguais a 1000.
   if (rank == 0) {
      for (int i = 0; i < arraySize; i++)
         array[i] =  1000;
   }

   // Faz broadcast usando uma topologia mesh.
   MPI_Mesh_broadcast(array, arraySize, MPI_COMM_WORLD);

   // Barreira de sincronização.
   MPI_Barrier(MPI_COMM_WORLD);

   // Imprime o array após o broadcast na topologia mesh.
   printArrayForRank(rank, array, arraySize);
     
   free(array);
   MPI_Finalize();
   return 0;
}
