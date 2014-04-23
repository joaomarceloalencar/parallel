#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define RAIO 5
#define N 1000000000

double fRand(double fMin, double fMax) {
    double f = (double) rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main(int argc, char *argv[]) {
    long i;
    long countCircle = 0;

    srand(time(0));
  
    for (i = 0; i < N; i++) {
        double x = fRand(0, 2 * RAIO);
        double y = fRand(0, 2 * RAIO);

        double dist = (x - RAIO) * (x - RAIO) + (y - RAIO) * (y - RAIO);
        dist = sqrt(dist);
        if (dist <= RAIO){
            countCircle++;
        }
    }

    printf("%ld %.5f\n", i,  ((double) 4 * countCircle) / N);
}
