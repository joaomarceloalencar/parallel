#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TAM_VET 10

int main(int argc, char *argv[]) {
    int i, my_rank, size, root = 0;
    int *inbuf, outbuf[TAM_VET];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (my_rank == root) {
        inbuf = (int *) malloc(size * TAM_VET * sizeof(int));
        for (i = 0; i < TAM_VET * size; i++)
            inbuf[i] = i;
    }

    MPI_Scatter(inbuf, TAM_VET, MPI_INT, outbuf, TAM_VET, MPI_INT, root, MPI_COMM_WORLD);

    printf("Processo %d recebeu: ", my_rank);
    for (i = 0; i < TAM_VET; i++) {
        printf(" %d", outbuf[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}