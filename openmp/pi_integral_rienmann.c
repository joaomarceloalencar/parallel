#include <stdio.h>
#include <omp.h>
#include <math.h>

int main(int argc, char *argv[]) {
    int intervals;
    sscanf(argv[1], "%d", &intervals);

    double integral = 0.0;
    double dx = 1.0 / intervals;

    #pragma omp parallel for reduction (+:integral)
    for (int i = 0; i <= intervals; i++) {
        double x = i * dx;
        double fx = sqrt(1.0 - x * x);
        integral = integral + fx * dx;
    }

    /* Versão não paralelizável 
    double x = 0.0;
    #pragma omp parallel for reduction (+:integral)
    for (int i = 0; i <= intervals; i++) {
        double x = i * dx;
        double fx = sqrt(1.0 - x * x);
        integral = integral + fx * dx;
    }
    */
       
    double pi = 4 * integral;   
    printf("%20.18lf\n", pi);
    return 0;
}