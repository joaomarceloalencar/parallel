#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int i, valor, rank, root = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    valor = rank;

    if (rank == root) {
        printf("Informe um valor a ser distribuído.\n");
        scanf("%d", &valor);
    }

    MPI_Bcast(&valor, 1, MPI_INT, root, MPI_COMM_WORLD);
    printf("Processo %d valor %d.\n", rank, valor);

    MPI_Finalize();

    return 0;
}