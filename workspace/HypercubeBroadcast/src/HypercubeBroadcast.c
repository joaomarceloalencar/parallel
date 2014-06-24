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

 
int main(int argc, char *argv[]) {

	createEquivalenceArray(4);
	printEquivalenceClasses(4);
	return 0;
}

