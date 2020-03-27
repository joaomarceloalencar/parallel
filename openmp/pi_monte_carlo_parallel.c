/* Este exemplo usa a diretiva parallel e somente depois, um for interno */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double rnd (unsigned int * seed) {
    *seed = (1140671485 * (* seed) + 12820163) % (1 << 24);
    return ((double)(*seed)) / (1 << 24);
}

int main (int argc , char * argv[]) {
    int num_shots; 
    int num_hits = 0;
    sscanf (argv[1], "%d", & num_shots);
    unsigned int seeds[omp_get_max_threads()];
    
    #pragma omp parallel {
        unsigned int seed = omp_get_thread_num();
        #pragma omp for reduction(+:num_hits)
        for (int shot = 0; shot < num_shots; shot++) {
                double x = rnd (&seed);
                double y = rnd (&seed);
                if (x * x + y * y <= 1) num_hits = num_hits + 1;
        }
    }

    double pi = 4.0 * (double) num_hits / (double)num_shots;
    printf ("%20.18lf\n", pi);
    return 0;
}