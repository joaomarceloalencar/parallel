/*
 ============================================================================
 Name        : RingAllToAll.c
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


int main(int argc, char *argv[]) {
	int	my_rank;												/* rank of process */
	int	num_procs;												/* number of processes */
	int	tag = 0;												/* tag for messages */
	MPI_Status	status ;										/* return status for receive */

	/* start up MPI */
	MPI_Init(&argc, &argv);

	/* find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

	/* find out number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs); 

	/* find out predecessor and successor */
	int	succ = (my_rank + 1) % num_procs;						/* rank of sender */
	int	pred = (my_rank - 1 + num_procs) % num_procs;			/* rank of receiver */

	/* create the data for each processor */
	int blocksize = 10;
	int *my_data = (int *) malloc(num_procs * blocksize * sizeof(int));
	int i, j;
	for (i = 0; i < num_procs * blocksize ; i++)
		my_data[i] = my_rank;

	/* create the send & receive buffer and initialize it. */
	int *buffer = (int *) malloc((num_procs - 1) * blocksize * sizeof(int));
	for (i = 0; i < (num_procs - 1) * blocksize; i++)
		buffer[i] = my_data[i];

	for (i = 0; i < num_procs - 1; i++) {
		printf("Process%d Iteration: %d\n", my_rank, i);

		int numberOfSlots = num_procs - 1 - i;

		/* Sending data. */
		int store_send = my_rank - i;
		if (store_send < 0)
			store_send += num_procs;

		MPI_Send(buffer, numberOfSlots * blocksize, MPI_INT, succ, tag, MPI_COMM_WORLD);

//		printf("Process%d store_send %d\n", my_rank, store_send);
//		j = my_rank + 1;
//		while (j != store_send) {
//			printf("Process%d sending %d,%d to %d\n", my_rank, store_send, j, succ);
//			j = (j + 1) % num_procs;
//		}

		/* Receiving data. */
		int store_recv = my_rank - i - 1;
		if (store_recv < 0)
			store_recv += num_procs;

		MPI_Recv(buffer, numberOfSlots * blocksize, MPI_INT, pred, tag, MPI_COMM_WORLD, &status);

//		printf("Process%d store_recv %d\n", my_rank, store_recv);
//		j = my_rank;
//		while (j != store_recv) {
//			printf("Process%d receiving %d,%d from %d\n", my_rank, store_recv,
//					j, pred);
//			j = (j + 1) % num_procs;
//		}

		/* Store the data. */
		for (int i = 0; i < blocksize; i++)
			my_data[store_recv * blocksize + i] = buffer[i];

		/* Shift the buffer. */
		for (int i = 0; i < numberOfSlots - 1; i++)
			for (int j = 0; j < blocksize; j++)
				buffer[i * blocksize + j] = buffer[(i+1) * blocksize + j];
	}

	for (i = 0; i < num_procs; i++){
		printf("Process%d data from process%d\n", my_rank, i);
		for (j = 0; j < blocksize; j++)
			printf("Process%d %d\n", my_rank, my_data[i * blocksize + j]);
	}

	/* shut down MPI */
	MPI_Finalize(); 

	free(my_data);
	free(buffer);
	return 0;
}
