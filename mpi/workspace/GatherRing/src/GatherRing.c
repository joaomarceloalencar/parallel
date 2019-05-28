/*
 ============================================================================
 Name        : GatherRing.c
 Author      : João Marcelo Uchôa de Alencar
 Version     :
 Copyright   : Your copyright notice
 Description : Calculate Pi in MPI
 ============================================================================
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gather_ring(float x[], int blocksize, float y[]){
	int i, p, my_rank, succ, pred;
	int send_offset, recv_offset;
	MPI_Status status;

	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	for (i = 0; i < blocksize; i++)
		y[i + my_rank * blocksize] = x[i];

	succ = (my_rank + 1) % p;
	pred = (my_rank - 1 + p) % p;

	for (i = 0; i < p - 1; i++) {
		send_offset = ((my_rank - i + p) % p) * blocksize;
		recv_offset = ((my_rank -i - 1 +p) %p) * blocksize;

		MPI_Send(y + send_offset, blocksize, MPI_FLOAT, succ, 0, MPI_COMM_WORLD);
		MPI_Recv(y + recv_offset, blocksize, MPI_FLOAT, pred, 0, MPI_COMM_WORLD, &status);
	}
}


void gather_ring_sync(float x[], int blocksize, float y[]){
	int i, p, my_rank, succ, pred;
	int send_offset, recv_offset;
	MPI_Status status;

	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	for (i = 0; i < blocksize; i++)
		y[i + my_rank * blocksize] = x[i];

	succ = (my_rank + 1) % p;
	pred = (my_rank - 1 + p) % p;

	for (i = 0; i < p - 1; i++) {
		send_offset = ((my_rank - i + p) % p) * blocksize;
		recv_offset = ((my_rank -i - 1 +p) %p) * blocksize;

		MPI_Request send_request;
		MPI_Issend(y + send_offset, blocksize, MPI_FLOAT, succ, 0, MPI_COMM_WORLD, &send_request);
		MPI_Recv(y + recv_offset, blocksize, MPI_FLOAT, pred, 0, MPI_COMM_WORLD, &status);
		MPI_Wait(&send_request, &status);
	}
}

void gather_ring_buff(float x[], int blocksize, float y[]){
	int i, p, my_rank, succ, pred;
	int send_offset, recv_offset;
	MPI_Status status;

	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	for (i = 0; i < blocksize; i++)
		y[i + my_rank * blocksize] = x[i];

	succ = (my_rank + 1) % p;
	pred = (my_rank - 1 + p) % p;

	void *buffer = malloc(p * blocksize * sizeof(float));
	MPI_Buffer_attach(buffer, p * blocksize * sizeof(float));

	for (i = 0; i < p - 1; i++) {
		send_offset = ((my_rank - i + p) % p) * blocksize;
		recv_offset = ((my_rank -i - 1 +p) %p) * blocksize;

		MPI_Bsend(y + send_offset, blocksize, MPI_FLOAT, succ, 0, MPI_COMM_WORLD);
		MPI_Recv(y + recv_offset, blocksize, MPI_FLOAT, pred, 0, MPI_COMM_WORLD, &status);
	}

	int buffersize;
	MPI_Buffer_detach(buffer,&buffersize);
}

int main(int argc, char *argv[]) {
	int			my_rank;		/* rank of process */
	int			num_procs;		/* number of processes */
	int 		blocksize = 100;

	/* start up MPI */
	MPI_Init(&argc, &argv);

	/* find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

	/* find out number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	float *x = (float *) malloc(blocksize * sizeof(float));
	for (int i = 0; i < blocksize; i ++)
		x[i] = my_rank;
	float *y = (float *) malloc(num_procs * blocksize * sizeof(float));

    double startComm = MPI_Wtime();
	gather_ring(x, blocksize, y);
    double endComm = MPI_Wtime();
    printf("Process%d Standard Time: %.4f\n\n", my_rank, endComm - startComm);

    startComm = MPI_Wtime();
   	gather_ring_buff(x, blocksize, y);
   	endComm = MPI_Wtime();
   	printf("Process%d Buffered Time: %.4f\n\n", my_rank, endComm - startComm);

    startComm = MPI_Wtime();
	gather_ring_sync(x, blocksize, y);
	endComm = MPI_Wtime();
	printf("Process%d Synchronous Time: %.4f\n\n", my_rank, endComm - startComm);



	/* shut down MPI */
	MPI_Finalize(); 

	return 0;
}
