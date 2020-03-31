#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NUMBER_OF_TESTS 10  // para podermos calcular a média.

int main(int argc, char *argv[]) {
    double *buf;
    int rank, numprocs;
    int n;
    double t1, t2;
    int j, k, nloop;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    
    if (numprocs != 2) {
        printf("O número de processos deve ser 2!\n");
        return (0);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        printf("\t\tn\ttempo [s]\tTaxa [Mb/seg]\n");
    }

    for (n = 1; n < 100000000; n *= 2) { // o tamanho da mensagem vai dobrando.
        nloop = 1000000 / n;
        if (nloop < 1)
            nloop = 1; // um único laço para mensagens longas. 

        buf = (double *)malloc(n * sizeof(double));

        if (!buf) {
            printf("Problema ao alocar um buffer de tamanho %d\n", n);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        for (k = 0; k < NUMBER_OF_TESTS; k++) {
            if (rank == 0)  {
                t1 = MPI_Wtime();
                for (j = 0; j < nloop; j++) { // envia a mensagem nloop vezes
                    MPI_Send(buf, n, MPI_DOUBLE, 1, k, MPI_COMM_WORLD);
                }
                t2 = (MPI_Wtime() - t1) / nloop;
            }
            else if (rank == 1) {
                for (j = 0; j < nloop; j++) { // recebe a mensagem nloop vezes
                    MPI_Recv(buf, n, MPI_DOUBLE, 0, k, MPI_COMM_WORLD, &status);
                }
            }
        }
        if (rank == 0) { // calcular largura de banda
            double bandwidth;
            bandwidth = n * sizeof(double) * 1.0e-6 * 8 / t2; // Mb/s
            printf("\t%10d\t %10.8f\t%8.2f\n", n, t2, bandwidth);
        }
        free(buf);
    }
    MPI_Finalize();
    return 0;
}