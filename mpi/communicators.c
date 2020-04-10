#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv) {
    int numprocs, org_rank, new_size, new_rank;
    MPI_Comm new_comm;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &org_rank);

    MPI_Comm_split(MPI_COMM_WORLD, org_rank % 2, 0, &new_comm);
    // MPI_Comm_split(MPI_COMM_WORLD , org_rank >=2,org_rank <=3,& new_comm);
    MPI_Comm_size(new_comm, &new_size);
    MPI_Comm_rank(new_comm, &new_rank);
    printf("'MPI_COMM_WORLD' processo rank/quantidade %d/%d tem rank/quantidade %d/%d em 'new_comm '\n", org_rank, numprocs, new_rank, new_size);

    int sum_ranks; // calcula a soma dos ranks nos dois comunicadores novos.
    MPI_Reduce(&new_rank, &sum_ranks, 1, MPI_INT, MPI_SUM, 0, new_comm);
    if (new_rank == 0) {
        printf("Soma de ranks em 'new_com ': %d\n", sum_ranks);
    }

    MPI_Comm_free(&new_comm);
    MPI_Finalize();
    return 0;
}