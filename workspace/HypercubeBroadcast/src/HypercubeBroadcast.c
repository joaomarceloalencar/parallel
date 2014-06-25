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


 
int main(int argc, char *argv[]) {
	int d = 4;
	int q = (int) ceil((pow(2,d) - 1) / d);

    Node **equivalenceClasses = createEquivalenceArray(d);
	NodeSet **E = createESet(equivalenceClasses, d);
	EdgeSet **A = createASet(equivalenceClasses, E, d);

	int i;
	for (i = 0; i < q + 1; i++ ){
		printf("A[%d], size: %d\n", i, A[i]->size);
		int j;
		for (j = 0; j < A[i]->size; j++) {
			Edge e = A[i]->set[j];
			printf("start: %s end: %s \n", e.start->id, e.end->id);
		}
	}

	return 0;
}

