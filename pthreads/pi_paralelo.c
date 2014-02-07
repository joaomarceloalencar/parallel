#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define RAIO 5
#define N 1000000000 
#define N_THREADS 4 

void *generatePoints() {
   long i;
   int seed;
   
   long *my_countCircle = (long *) malloc(sizeof(int));
   *my_countCircle = 0;
   for (i = 0; i < N / N_THREADS; i++) {
        double x = 0 + ((double) rand_r(&seed) / RAND_MAX) * (RAIO - 0);
        double y = 0 + ((double) rand_r(&seed) / RAND_MAX) * (RAIO - 0);

        double dist = (x - RAIO) * (x - RAIO) + (y - RAIO) * (y - RAIO);
        dist = sqrt(dist);
        if (dist <= RAIO) {
            (*my_countCircle)++;
        }
    }
    printf("%ld\n", i);
    pthread_exit((void *) my_countCircle);
}

int main(int argc, char *argv[]) {
    int i;
    long countCircle = 0;

    pthread_t threads[N_THREADS];

    for (i = 0; i < N_THREADS; i++) {
        pthread_create(&threads[i], NULL, generatePoints, NULL);
    }  

    for (i = 0; i < N_THREADS; i++) {
        long *p;
        pthread_join(threads[i], (void **) &p);
        countCircle += *p;
    }

    printf("%.5f\n", ((double) 4 * countCircle) / N);
    return 0;
}
