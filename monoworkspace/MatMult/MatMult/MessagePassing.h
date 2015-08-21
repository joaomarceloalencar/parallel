#ifndef MESSAGEPASSING_H
#define MESSAGEPASSING_H

#include "mpi.h"


double *matMultMPI(int argc, char *argv[], double *A, long rowsA, long columnsA, double *B, long rowsB, long columnsB)
{
   /* Start the MPI Envinronment */
   int rc = MPI_Init(&argc, &argv);
   if (rc != MPI_SUCCESS) {
      printf("Error starting MPI program. Terminating.\n");
      MPI_Abort(MPI_COMM_WORLD, rc);
   }

   int rank, numtasks;
   MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   /* Transfer the Matrices */
   /** Broadcast the Matrix B **/
   MPI_Bcast(B, rowsB * columnsB, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   
   /** Transfer the lines from Matrix A **/
   
   /* Do the calculation */
   
   /* Get back the results */

}

#endif
