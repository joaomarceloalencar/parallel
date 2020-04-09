#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define TAM 5

int main(int argc, char *argv[]) {
    int my_rank, size, i, root = 0;

    float inbuf[TAM];
    float outbuf[TAM];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (i = 0; i < TAM; i++)
        inbuf[i] = (float) (my_rank * TAM + i);

    MPI_Reduce(inbuf, outbuf, TAM, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);

    if (my_rank == 0) {
        for (i = 0; i < TAM; i++)
            printf("outbuf[%d] = %3.1f\n", i, outbuf[i]);
    }
    MPI_Finalize();
    return 0;
}