#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

// Número total de interações
#define N 1000000
#define RAIO 10

// Função que executa o trabalho. Está aqui por organização. 
long generatePoints(long my_n) {
   long i;
   int seed;
   
   long my_countCircle = 0;
   for (i = 0; i < my_n ; i++) {
        double x = 0 + ((double) rand_r(&seed) / RAND_MAX) * (RAIO - 0);
        double y = 0 + ((double) rand_r(&seed) / RAND_MAX) * (RAIO - 0);

        double dist = (x - RAIO) * (x - RAIO) + (y - RAIO) * (y - RAIO);
        dist = sqrt(dist);
        if (dist <= RAIO) {
            my_countCircle++;
        }
    }
    
    return my_countCircle;  
}



int main(int argc, char *argv[]) {
    int rank, size;
    long my_n;
    long circle = 0;    

    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
   
    // Quanto cada processo executará.
    my_n = N / size;
 
    // Distribuindo valor de my_n.
    MPI_Bcast(&my_n, 1, MPI_LONG, 0, MPI_COMM_WORLD);       
  
    // Calcula a sua parte.
    circle = generatePoints(my_n);

    // Acumula com as dos outros.
    long temp;
    MPI_Reduce(&circle, &temp, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    circle = temp;
            
    if (rank == 0)  printf("%.5f\n", ((double) 4 * circle) / N);              

    MPI_Finalize();
    return 0;
}
