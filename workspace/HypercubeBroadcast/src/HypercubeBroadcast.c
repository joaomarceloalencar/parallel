/*
 ============================================================================
 Name        : HypercubeBroadcast.c
 Author      : João Marcelo Uchôa de Alencar
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 ============================================================================
 */
#include <stdio.h>
#include "Hypercube.h"
#include "mpi.h" 
#include <tgmath.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int d = atoi(argv[1]);
	int q = (int) ceil((pow(2,d) - 1) / d);
	int rank, nproc;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    /* Creating the message. The message from each processor consists of an array of
     * char with several copies of the process rank. The final array after the broadcast
     * will be sequential junks of the processes ranks repeated by the message size.
     */
    int messageSize = atoi(argv[2]);
    char *message = (char *) malloc(messageSize * sizeof(char));
    int m;
    for (m = 0; m < messageSize; m++)
    	message[m] = (char) rank;

    char *fullArray = (char *) malloc(messageSize * nproc * sizeof(char));
    for (m = rank * messageSize; m < rank * messageSize + messageSize; m++)
    	fullArray[m] = (char) rank;

    /* Create the equivalence classes.*/
    Node **equivalenceClasses = createEquivalenceArray(d);
    NodeSet **E = createESet(equivalenceClasses, d);
    EdgeSet **A = createASet(equivalenceClasses, E, d);
    EdgeSet **A_total = createASuperSet(A, nproc, d);
    char *binaryRank = intToBinary(rank, d);

    /* For each superSet A_total[i] in my spanning tree, look for edges with my rank. */
    int step;
    for (step = 0; step < q + 1; step++) {
    	/* Create an array of requisitions requests. A process will wait for all communication
    	 * in a step to be over before starting a new step.
    	 */
    	MPI_Request *requests = (MPI_Request *) malloc(A_total[step]->size * sizeof(MPI_Request));
    	int numberOfRequests = 0;

    	/* Let's verify each edge in A_total[i]. */
    	int edge;
    	for (edge = 0; edge < A_total[step]->size; edge++) {
    		Edge e = A_total[step]->set[edge];
    		int root = edge / A[step]->size;
    		/* If this process is a start node, then sends the message. */
    		if (!strcmp(binaryRank, e.start->id)) {
    			// printf("process%d root%d step%d start: %s end: %s\n", rank, root, step, e.start->id, e.end->id);
    			/* The message starts at position fullArray + root * messageSize and goes on by messageSize bytes. */
    			int rankDest = binaryToInt(e.end->id, d);
    			MPI_Isend(fullArray + root * messageSize, messageSize, MPI_CHAR, rankDest, 0, MPI_COMM_WORLD, &requests[numberOfRequests]);
    			numberOfRequests++;
    		}
    		/* Else if is a end node, receives the message. */
    		else if (!strcmp(binaryRank, e.end->id)) {
    			// printf("process%d root%d step%d start: %s end: %s\n", rank, root, step, e.start->id, e.end->id);
    			/* The message is stored at position fullArray + root * messageSize and goes on by messageSize bytes. */
    			int rankSouce = binaryToInt(e.start->id, d);
    			MPI_Irecv(fullArray + root * messageSize, messageSize, MPI_CHAR, rankSouce, 0, MPI_COMM_WORLD, &requests[numberOfRequests]);
    			numberOfRequests++;
    		}
    	}

    	int req = 0;
    	for (req = 0; req < numberOfRequests; req++) {
    		MPI_Status status;
    		MPI_Wait(&requests[req], &status);
    	}
    }

    for (m = 0; m < messageSize * nproc; m++) {
    	printf("Process%d fullArray[%d]:%d\n", rank, m, fullArray[m]);
    }

    MPI_Finalize();
    return 0;
}

