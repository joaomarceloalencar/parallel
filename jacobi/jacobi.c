/* Initial versions, no optimization. */
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
    sscanf(argv[2], "%lf", &epsilon);
    
    double **grid = allocate2dMatrix(n+2,n+2);
    double **new = allocate2dMatrix(n+2,n+2);
    stableValues2dMatrix(grid, n+2, n+2);

    while(TRUE) {
        if (ECHO) printf("Calculando nova etapa.\n");
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) {
                new[i][j] = (grid[i-1][j] + grid[i+1][j] + grid[i][j-1] + grid[i][j+1]) / 4;
            }

        iters++;

        if (ECHO) printf("Verificando a diferença.\n");
        double maxdiff = 0.0;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                maxdiff = fmax(maxdiff, fabs(new[i][j]-grid[i][j]));

        if (ECHO) printf("Iteração %d, Diferença = %.5f.\n", iters, maxdiff);
        if (maxdiff < epsilon)
            break;

        if (ECHO) printf("Atualizando o plano.\n");
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                grid[i][j] = new[i][j];
        
        if (ECHO) printf("\n");   
    }

    printf("Iterações: %d\n", iters);
    free2dMatrix(grid, n+1);
    free2dMatrix(new, n+1);
    return 0;
}