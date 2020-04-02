// CSc 522
// MPI matrix multiplication example
// The dimension of the matrices must be divisible by the number of processes

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>	

#define TAG 13

int main(int argc, char *argv[]) {
  double **A, **B, **C, *tmp;
  double startTime, endTime;
  int numElements, offset, stripSize, myrank, numnodes, N, i, j, k;
  
  MPI_Init(&argc, &argv);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &numnodes);
  
  N = atoi(argv[1]);
  
  // allocate A, B, and C --- note that you want these to be
  // contiguously allocated.  Workers need less memory allocated.
  
  if (myrank == 0) {
    // The rank 0 will have the full matrix
    tmp = (double *) malloc (sizeof(double ) * N * N);
    A = (double **) malloc (sizeof(double *) * N);
    for (i = 0; i < N; i++)
      A[i] = &tmp[i * N];
  }
  else {
    // The other ranks will have a set of lines of the Matrix A.
    tmp = (double *) malloc (sizeof(double ) * N * N / numnodes);
    A = (double **) malloc (sizeof(double *) * N / numnodes);
    for (i = 0; i < N / numnodes; i++)
      A[i] = &tmp[i * N];
  }
  
  // Everyone will have a full copy of B. 
  // To compute a linha of C, the process needs all the columns of B.
  tmp = (double *) malloc (sizeof(double ) * N * N);
  B = (double **) malloc (sizeof(double *) * N);
  for (i = 0; i < N; i++)
    B[i] = &tmp[i * N];
  
  
  if (myrank == 0) {
    tmp = (double *) malloc (sizeof(double ) * N * N);
    C = (double **) malloc (sizeof(double *) * N);
    for (i = 0; i < N; i++)
      C[i] = &tmp[i * N];
  }
  else {
    tmp = (double *) malloc (sizeof(double ) * N * N / numnodes);
    C = (double **) malloc (sizeof(double *) * N / numnodes);
    for (i = 0; i < N / numnodes; i++)
      C[i] = &tmp[i * N];
  }

  if (myrank == 0) {
    // initialize A and B
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++) {
        A[i][j] = 1.0;
        B[i][j] = 1.0;
      }
    }
  }
  
  // start timer
  if (myrank == 0) {
    startTime = MPI_Wtime();
  }
  
  stripSize = N/numnodes;

  // Spread the A Matrix
  MPI_Scatter(A[0], stripSize * N, MPI_DOUBLE, A[0], stripSize*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  
  // everyone gets B
  MPI_Bcast(B[0], N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Let each process initialize C to zero 
  for (i=0; i<stripSize; i++) {
    for (j=0; j<N; j++) {
      C[i][j] = 0.0;
    }
  }

  /* do the work
  for (i=0; i<stripSize; i++) {
    for (j=0; j<N; j++) {
      for (k=0; k<N; k++) {
	C[i][j] += A[i][k] * B[k][j];
      }
    }
  }*/
  int dim1 = stripSize;
  int dim2 = N;
  int dim3 = N;
  double alpha = 1;
  double beta = 0;
  cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, dim1, dim3, dim2, alpha, *A, dim1, *B, dim2, beta, *C, dim1);

  // Gather the results
  MPI_Gather(C[0], stripSize * N, MPI_DOUBLE, C[0], stripSize * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // stop timer
  if (myrank == 0) {
    endTime = MPI_Wtime();
    printf("Time is %f\n", endTime-startTime);
  }
  
  // print out matrix here, if I'm the master
  if (myrank == 0 && N <= 20) {
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++) {
        printf("%f ", C[i][j]);
      }
      printf("\n");
    }
  }
  
  MPI_Finalize();
  return 0;
}


