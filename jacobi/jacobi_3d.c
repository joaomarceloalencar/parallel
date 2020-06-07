/*
    Three dimentional array to avoid copy of the plane.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jacobi.h"

#define ECHO 0

int main(int argc, char *argv[]){
    int n;
    int iters = 0;
    double epsilon = 0.0;

    sscanf(argv[1], "%d", &n);
    sscanf(argv[2], "%d", &iters);
    int old = 0;
    int new = 1;
    
    double ***grid = malloc(2 * sizeof(double **));
    grid[old] = allocate2dMatrix(n+2,n+2);
    grid[new] = allocate2dMatrix(n+2,n+2);
    stableValues2dMatrix(grid[old], n+2, n+2);

    for (int k = 0; k < iters; k++) {
        if (ECHO) printf("Calculando nova etapa.\n");
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) 
                grid[new][i][j] = (grid[old][i-1][j] + grid[old][i+1][j] + grid[old][i][j-1] + grid[old][i][j+1]) * 0.25;

        if (ECHO) {
            printf("Valores dos planos.\n");
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= n; j++)
                {
                    printf("grid[old][%d][%d]: %.5f\n", i, j, grid[old][i][j]);
                    printf("grid[new][%d][%d]: %.5f\n", i, j, grid[new][i][j]);
                }

            printf("Verificando a diferença.\n");
            double maxdiff = 0.0;
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= n; j++)
                    maxdiff = fmax(maxdiff, fabs(grid[new][i][j] - grid[old][i][j]));
            printf("Iteração %d, Diferença = %.5f.\n", k, maxdiff);
        }

        old = 1 - old;
        new = 1 - new;    
        
        if (ECHO) printf("\n");   
    }

    
    double maxdiff = 0.0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) {
            //printf("%.5f %.5f\n", new[i][j], grid[i][j]);
            maxdiff = fmax(maxdiff, fabs(grid[new][i][j]-grid[old][i][j]));
        }
        
    printf("Diferença: %.5f\n", maxdiff);
    
    free2dMatrix(grid[old], n+1);
    free2dMatrix(grid[new], n+1);

    free(grid);
    return 0;
}