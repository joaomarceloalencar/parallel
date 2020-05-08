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
    long my_circle = 0;
    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
   
    // Quanto cada processo executará.
    my_n = N / size;
   
    if (rank == 0) {
        // O processo zero distribui o valor de my_n e acumula a contribuição de cada um em circle.
        int i;
        int circle = 0;
 
        // Distribuindo valor de my_n.
        for (i = 1; i < size; i++) {
            MPI_Send(&my_n, 1, MPI_LONG, i, 0, MPI_COMM_WORLD);
        }
        
        // Calcula a sua parte.
        circle = generatePoints(my_n);

        // Acumula com as dos outros.
        for (i = 1; i < size; i++) {
            long temp;
            MPI_Recv(&temp, 1, MPI_LONG, i, 0, MPI_COMM_WORLD, &status);
            circle += temp;
        }
        
        printf("%.5f\n", ((double) 4 * circle) / N);              
 
    } else {
        // Recebe de zero o valor de my_n, calcula os pontos e retorna para o processo 0.

        // Recebe de zero o valor de my_n
        MPI_Recv(&my_n, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD, &status);
        
	    // Calcula os pontos
        my_circle = generatePoints(my_n);

        // Envia para 0 o valor calculado.
        MPI_Send(&my_circle, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
