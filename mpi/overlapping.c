#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

double other_work(int numproc) {
    int i;
    double a;
    for (i = 0; i < 100000000 / numproc; i++) {
        a = sin(sqrt(i)); // só para variar a quantidade de cálculo
    }
    return a;
}

int main(int argc, char *argv[]) { // Obrigatório mais de um processo
    int p, i, myid, tag = 1, proc, ierr;
    double start_p, run_time, start_c, comm_t, start_w, work_t, work_r;
    double *buff = NULL;

    MPI_Request request;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    start_p = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

#define master 0
#define MSGSIZE 100000000 // pode variar o tamanho da mensagem
    buff = (double *) malloc(MSGSIZE * sizeof(double)); //aloca buffer

    if (myid == master)  {
        for (i = 0; i < MSGSIZE; i++) { // inicializa o buffer
            buff[i] = 1;
        }
        start_c = MPI_Wtime();
        for (proc = 1; proc < p; proc++) {
#if 0
            MPI_Irecv(buff, MSGSIZE, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &request);
#endif
#if 1
            MPI_Recv(buff , MSGSIZE, MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status);
#endif
        }

        comm_t = MPI_Wtime() - start_c;
        start_w = MPI_Wtime();

        work_r = other_work(p); // cálculo local

        work_t = MPI_Wtime() - start_w;
        // MPI_Wait(&request, &status); // bloqueia até Irecv concluir
    }
    else { // processos escravos
        start_c = MPI_Wtime();
#if 0
        MPI_Isend(buff, MSGSIZE, MPI_DOUBLE, master, tag, MPI_COMM_WORLD, &request);
#endif 
#if 1
        MPI_Send(buff, MSGSIZE,  MPI_DOUBLE, master, tag, MPI_COMM_WORLD);
#endif
        comm_t = MPI_Wtime() - start_c;
        start_w = MPI_Wtime();

        work_r = other_work(p); // cálculo local

        work_t = MPI_Wtime() - start_w;
        // MPI_Wait(&request, &status); // bloqueia até Isend concluir
    }
    run_time = MPI_Wtime() - start_p;

    printf("Rank \t Comm[s] \t Calc[s] \t Total[s] \t Work_result \n");
    printf(" %d\t %e\t %e\t %e\t %e\t\n", myid, comm_t, work_t, run_time, work_r);
    
    fflush(stdout); // força limpeza da saída

    free(buff);
    MPI_Finalize();
}