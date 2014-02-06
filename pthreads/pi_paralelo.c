#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define RAIO 5
#define N 100000000
#define N_THREADS 2 


double fRand(double fMin, double fMax) {
    double f = (double) rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void *generatePoints() {
   int i;
   int *my_countCircle = (int *) malloc(sizeof(int));
   *my_countCircle = 0;
   for (i = 0; i < N / N_THREADS; i++) {
        double x = fRand(0, 2 * RAIO);
        double y = fRand(0, 2 * RAIO);

        double dist = (x - RAIO) * (x - RAIO) + (y - RAIO) * (y - RAIO);
        dist = sqrt(dist);
        if (dist <= RAIO) {
            (*my_countCircle)++;
        }
    }
    printf("%d\n", i);
    pthread_exit((void *) my_countCircle);
}

int main(int argc, char *argv[]) {
    int i;
    int countCircle = 0;

    srand(time(0));
    pthread_t threads[N_THREADS];

    for (i = 0; i < N_THREADS; i++) {
        pthread_create(&threads[i], NULL, generatePoints, NULL);
    }  

    for (i = 0; i < N_THREADS; i++) {
        int *p;
        pthread_join(threads[i], (void **) &p);
        countCircle += *p;
    }

    printf("%.5f\n", ((double) 4 * countCircle) / N);
    return 0;
}
