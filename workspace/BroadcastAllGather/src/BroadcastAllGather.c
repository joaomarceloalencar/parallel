/*
 * BroadcastAllGather.c
 *
 *  Created on: 28/06/2014
 *      Author: jmhal
 */
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[]) {
    int messageSize = atoi(argv[1]);
    messageSize = messageSize * 1024;

	int rank, nproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	char *message = (char *) malloc(messageSize * sizeof(char));
	int m;
	for (m = 0; m < messageSize; m++)
		message[m] = (char) rank;

	char *fullArray = (char *) malloc(messageSize * nproc * sizeof(char));
	for (m = rank * messageSize; m < rank * messageSize + messageSize; m++)
		fullArray[m] = (char) rank;

	double startComm = MPI_Wtime();

	MPI_Allgather(message, messageSize, MPI_CHAR, fullArray, messageSize, MPI_CHAR, MPI_COMM_WORLD);

	double endComm = MPI_Wtime();

	printf("%.4f\n", endComm - startComm);

    MPI_Finalize();
    return 0;
}

