#include <stdio.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char *argv[]) {
    int done = 0, n, myid, numprocs, i;
    double PI25DT = 3.141592653589793238462643;
    double pi, h, sum, x, start, finish;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    
    while (!done) {
        if (myid == 0) {
            printf("Número de Intervalos: (0 sai) ");
            fflush(stdout);
            scanf("%d", &n);
            start = MPI_Wtime();
        }

        // Executa em todos os processos
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (n == 0)
            done = 1;
        
        h = 1.0 / (double) n;
        sum = 0.0;

        for (i = myid + 1; i <= n; i += numprocs) {
            x = h * ((double)i - 0.5);
            sum += 4.0 * h * sqrt(1.0 - x * x);
        }

        MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        
        if (myid == 0) {
            finish = MPI_Wtime();
            printf("Pi é aprox. %.16f, Erro é %.16f\n", pi, fabs(pi - PI25DT));
            printf("Tempo gasto é %f\n", finish - start);
        }
    }
    MPI_Finalize();
    return 0;
}