#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TAM_VET 10

int main(int argc, char *argv[])
{
    int i, my_rank, size, root = 0;
    int *outbuf, inbuf[TAM_VET];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (my_rank == root) {
        outbuf = (int *)malloc(size * TAM_VET * sizeof(int));
    }
    for (i = 0; i < TAM_VET; i++)
        inbuf[i] = my_rank;

    MPI_Gather(inbuf, TAM_VET, MPI_INT, outbuf, TAM_VET, MPI_INT, root, MPI_COMM_WORLD);

    if (my_rank == root) {
        printf("Processo %d recebeu: ", my_rank);
        for (i = 0; i < TAM_VET * size; i++)  {
            printf(" %d", outbuf[i]);
        }
        printf("\n");
        free(outbuf);
    }

    MPI_Finalize();
    return 0;
}