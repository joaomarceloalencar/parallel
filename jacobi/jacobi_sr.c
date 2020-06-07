/* 
    Replace division by multiplication (strength reduction).
    The while loop is replaced by the for loop, keeping track of the iterations.
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
    
    double **grid = allocate2dMatrix(n+2,n+2);
    double **new = allocate2dMatrix(n+2,n+2);
    stableValues2dMatrix(grid, n+2, n+2);

    for (int k = 0; k < iters; k++) {
        if (ECHO) printf("Calculando nova etapa.\n");
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) {
                new[i][j] = (grid[i-1][j] + grid[i+1][j] + grid[i][j-1] + grid[i][j+1]) * 0.25;
                if (ECHO) printf("(%d, %d) %0.2f = (%.2f + %.2f + %.2f + %.2f) * 0.25\n", i, j, new[i][j], grid[i-1][j], grid[i+1][j], grid[i][j-1], grid[i][j+1]);
            }
            
        if (ECHO) {
            printf("Valores dos planos.\n");
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= n; j++) {
                    printf("grid[old][%d][%d]: %.5f\n", i, j, grid[i][j]);
                    printf("grid[new][%d][%d]: %.5f\n", i, j, new[i][j]);
                }

            printf("Verificando a diferença.\n");
            double maxdiff = 0.0;
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= n; j++)
                    maxdiff = fmax(maxdiff, fabs(new[i][j]-grid[i][j]));
            printf("Iteração %d, Diferença = %.5f.\n", k, maxdiff);      
        }  

        if (k == iters - 1) 
            break;   

        if (ECHO) printf("Atualizando o plano.\n");
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                grid[i][j] = new[i][j];
        
        if (ECHO) printf("\n");   
    }

    double maxdiff = 0.0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) {
            //printf("%.5f %.5f\n", new[i][j], grid[i][j]);
            maxdiff = fmax(maxdiff, fabs(new[i][j]-grid[i][j]));
        }
        
    printf("Diferença: %.5f\n", maxdiff);

    free2dMatrix(grid, n+1);
    free2dMatrix(new, n+1);
    return 0;
}