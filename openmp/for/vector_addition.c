#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

double* vectAdd(double *c, double *a, double *b, int n) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++)
        c[i] = a[i] + b[i];
    return c;
}

int main(int argc, char *argv[]) {
    int size = atoi(argv[1]);

    double *a = malloc(size * sizeof(double));
    double *b = malloc(size * sizeof(double));
    double *c = malloc(size * sizeof(double));

    #pragma omp parallel for 
    for (int i = 0; i < size; i++) {
        a[i] = b[i] = 1.0;
        c[i] = 0;
    }

    vectAdd(c, a, b, size);

    for (int i = 0; i < size; i++) {
        if ((i+1) % 10 == 0)
            printf("\nc[%3d]=%0.2f ", i, c[i]);
        else 
            printf("c[%3d]=%0.2f ", i, c[i]);        
    }
    printf("\n");

    free(a);
    free(b);
    free(c);
    return 0;
}
