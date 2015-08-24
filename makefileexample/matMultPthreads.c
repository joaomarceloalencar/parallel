#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include "matrix.h"

/*
   Multiplies matrix A (rowsA x columnsA) and matrix B (rowsB x columnsB) 
  the result is matrix C (rowsA x columnsB). This is the Pthreads version.
*/

typedef struct thread_data {
  int thread_id;
  int nthreads;
  double *A;
  long rowsA;
  long columnsA;
  double *B;
  long rowsB;
  long columnsB;
  double *C;
} thread_data;

void *matMultThread(void *data)
{
   thread_data *my_data = (thread_data *) data;
   
   int my_id = my_data->thread_id;
   int nprocs = my_data->nthreads;
      
   double *A = my_data->A;
   long rowsA = my_data->rowsA;
   long columnsA = my_data->columnsA;
   double *B = my_data->B;
   long columnsB = my_data->columnsB;   
   double *C = my_data->C;
   
   int i,j,k;
   for (i = my_id * (rowsA / nprocs); i < (my_id + 1) * (rowsA / nprocs); i++)
      for (j = 0; j < columnsB; j++) {
         C[i * columnsB + j] = 0;
         for (k = 0; k < columnsA; k++)
            C[i * columnsB + j] += A[i * columnsA + k] * B[k * columnsB + j];
      }   
   
   pthread_exit(NULL);
   return NULL;
}

double *matMultPthreads(double *A, long rowsA, long columnsA, double *B, long rowsB, long columnsB)
{
   int nprocs = get_nprocs();
   pthread_t *threads;
   thread_data *data;
   threads = malloc(nprocs * sizeof(pthread_t));
   data = malloc(nprocs * sizeof(thread_data));
   
   double *C = malloc(rowsA * columnsB * sizeof(double));
   int i;
   for (i = 0; i < nprocs; i++) 
   {
      data[i].thread_id = i;
      data[i].nthreads = nprocs;
      data[i].A = A;
      data[i].rowsA = rowsA;
      data[i].columnsA = columnsA;
      data[i].B = B;
      data[i].rowsB = rowsB;
      data[i].columnsB = columnsB;
      data[i].C = C;
      
      pthread_create(&threads[i], NULL, matMultThread, (void *) &data[i]);
   }
   
   void *status;
   for (i = 0; i < nprocs; i++) 
      pthread_join(threads[i], &status);
   
   free(threads);   
   free(data);
   return C;
}

int main (int argc, char *argv[])
{
   if (argc != 8) 
   {
      printf("usage: generateMatrices filenameA rowsA columnsA filenameB rowsB columnsB filenameC\n");
      return -1;
   } 

   int rowsA = atoi(argv[2]);
   int columnsA = atoi(argv[3]);

   int rowsB = atoi(argv[5]);
   int columnsB = atoi(argv[6]);              

   if (columnsA != rowsB) 
   {
      printf("Matrices dimensions don't match!\n");
      return -2;
   }
   
   struct timespec start, end;
   double elapsed;

   double *A = readMatrixFile(argv[1], rowsA, columnsA);
   double *B = readMatrixFile(argv[4], rowsB, columnsB);
   
   clock_gettime(CLOCK_MONOTONIC, &start);
   double *C = matMultPthreads(A, rowsA, columnsA, B, rowsB, columnsB);
   clock_gettime(CLOCK_MONOTONIC, &end);
   elapsed = ( end.tv_sec - start.tv_sec ) + ( end.tv_nsec - start.tv_nsec ) / 1000000000L;
   printf("%.3f\n", elapsed);

   createMatrixFile(argv[7], C, rowsA, columnsB);

   return 0;
}
