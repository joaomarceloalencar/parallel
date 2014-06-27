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

    /* Create an array containing all the operations at step 1,2, ... q for each root. */
    Node **equivalenceClasses = createEquivalenceArray(d);
    NodeSet **E = createESet(equivalenceClasses, d);
    EdgeSet **_A = createASet(equivalenceClasses, E, d);

    EdgeSet ***A = (EdgeSet ***) malloc(nproc * sizeof(EdgeSet **));
    int root;
    for (root = 0; root < nproc; root++) {
    	A[root] = createASetForNewRoot(_A, intToBinary(root,d), d);
    	int step;
    	for (step = 0; step < q + 1; step++) {
    		int edge;
    		for (edge = 0; edge < A[root][step]->size; edge++) {
    			char *start = A[root][step]->set[edge].start->id;
    			char *end = A[root][step]->set[edge].end->id;
    			 // printf("Process%d root%d Step%d start:%s end:%s\n", rank, root, step, start, end);

    			char *binaryRank = intToBinary(rank, d);
    			/* If this process is a start node, then sends the message. */
				if (!strcmp(binaryRank, start)) {
					printf("Process%d root%d Step%d start: %s end: %s\n", rank, root, step, start, end);
				}
				/* Else if is a end node, receives the message. */
				else if (!strcmp(binaryRank, end)) {
					printf("Process%d root%d Step%d start: %s end: %s\n", rank, root, step, start, end);
				}
    		}
    	}
    }


//    int i;
//    for (i = 0; i < q + 1; i++){
//    	int j;
//    	for (j = 0; j < A_total[i]->size; j++){
//    		char *start = A_total[i]->set[j].start->id;
//    		char *end = A_total[i]->set[j].end->id;
//    		printf("Process%d step%d start:%s end:%s\n", rank, i, start, end);
//    	}
//    }

//    /* For each superSet A_total[i] in my spanning tree, look for edges with my rank. */
//    int i;
//    for (i = 0; i < q + 1; i++) {
//    	/* Let's verify each edge in newA[i]. */
//    	int j;
//    	for (j = 0; j < A_total[i]->size; j++) {
//    		Edge e = A_total[i]->set[j];
//    		// int rootRank = j / A[i]->size;
//    		printf("Process%d Step%d j%d start:%s end:%s\n", rank, i, j, e.start->id, e.end->id);
//    		/* If this process is a start node, then sends the message. */
//    		if (!strcmp(binaryRank, e.start->id)) {
//    			// printf("Process%d root%d Step%d start: %s end: %s\n", rank, rootRank, i, binaryRank, e.end->id);
//    		}
//    		/* Else if is a end node, receives the message. */
//    		else if (!strcmp(binaryRank, e.end->id)) {
//    			// printf("Process%d root%d Step%d start: %s end: %s\n", rank, rootRank, i, e.start->id, binaryRank);
//    		}
//    	}
//    }

	/*
	int i;


	for (i = 0; i < q + 1; i++ ){
		printf("A[%d], size: %d\n", i, A[i]->size);
		int j;
		for (j = 0; j < A[i]->size; j++) {
			Edge e = A[i]->set[j];
			printf("start: %s end: %s \n", e.start->id, e.end->id);
		}
	}


    int i;
	char root[] = "101";
	EdgeSet **newA = createASetForNewRoot(A, root, d);

	printf("\n New root: %s\n", root);
	for (i = 0; i < q + 1; i++) {
		printf("newA[%d], size: %d\n", i, newA[i]->size);
		int j;
		for (j = 0; j < newA[i]->size; j++) {
			Edge e = newA[i]->set[j];
			printf("start: %s end: %s \n", e.start->id, e.end->id);
		}
	}
	*/

    for (m = 0; m < messageSize * nproc; m++) {
    	printf("Process%d fullArray[%d]:%d\n", rank, m, fullArray[m]);
    }

    MPI_Finalize();
    return 0;
}

