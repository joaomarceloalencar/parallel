#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define ECHO 0

double **mtxMul(double **c, double **a, double **b, int n) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++)
    	for (int j = 0; j < n; j++) {
		c[i][j] = 0.0;
		for (int k = 0; k < n; k++)
			c[i][j] = c[i][j] + a[i][k] * b[k][j];
	}
    return c;
}

int main(int argc, char *argv[]) {
    int size = atoi(argv[1]);

    double **a = malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++)
	    a[i] = malloc(size * sizeof(double));

    double **b = malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++)
	    b[i] = malloc(size * sizeof(double));

    double **c = malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++)
	    c[i] = malloc(size * sizeof(double));

    for (int i = 0; i < size; i++) {
	    for (int j = 0; j < size; j++) {
        	a[i][j] = b[i][j] = 1.0;
	        c[i][j] = 0;
	    }
    }

    mtxMul(c, a, b, size);

    if (ECHO) {
    	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
        		printf("c[%d][%d]=%0.2f ", i, j, c[i][j]);
		}	
		printf("\n");
    	}
    }
    printf("\n");

    for (int i = 0; i < size; i++) {
    	free(a[i]);
	    free(b[i]);
	    free(c[i]);
    }

    free(a);
    free(b);
    free(c);

    return 0;
}
