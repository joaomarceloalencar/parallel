#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <stdlib.h>
#include <sys/sysinfo.h>
#include <omp.h>

/* 
  Multiplies matrix A (rowsA x columnsA) and matrix B (rowsB x columnsB) 
  the result is matrix C (rowsA x columnsB). This is the serial version.
*/
double *matMultSerial(double *A, long rowsA, long columnsA, double *B, long rowsB, long columnsB)
{
   if (columnsA != rowsB) 
      return NULL;

   double *C = malloc(rowsA * columnsB * sizeof(double));
   
   long i,j,k;
   for (i = 0; i < rowsA; i++)
      for (j = 0; j < columnsB; j++) {
         C[i * columnsB + j] = 0;
         for (k = 0; k < columnsA; k++)
            C[i * columnsB + j] += A[i * columnsA + k] * B[k * columnsB + j];
      }      

   return C;
}

/* 
  Same as above, but with OpenMP. I keep the two methods for performance comparison.
*/
double *matMultOpenMP(double *A, long rowsA, long columnsA, double *B, long rowsB, long columnsB)
{
   if (columnsA != rowsB) 
      return NULL;

   double *C = malloc(rowsA * columnsB * sizeof(double));
   
   long i,j,k;
   // Each thread is responsible for a block of rows from A.
   #pragma omp parallel for schedule(static) private(j,k)
   for (i = 0; i < rowsA; i++)
      for (j = 0; j < columnsB; j++) {
         C[i * columnsB + j] = 0;
         for (k = 0; k < columnsA; k++)
            C[i * columnsB + j] += A[i * columnsA + k] * B[k * columnsB + j];
      }      

   return C;
}

/*
   Same as above, but with Pthreads.
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
   long rowsB = my_data->rowsB;
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

#endif