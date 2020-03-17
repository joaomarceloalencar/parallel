#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double rnd (unsigned int * seed) {
    *seed = (1140671485 * (* seed) + 12820163) % (1 << 24);
    return ((double)(*seed)) / (1 << 24);
}

int main (int argc , char * argv[]) {
    int num_shots; 
    sscanf ( argv[1], "%d", & num_shots);
    unsigned int seeds[ omp_get_max_threads()];
    
    for (int thread = 0; thread < omp_get_max_threads(); thread++)
        seeds[thread] = thread;
    int num_hits = 0;
    #pragma omp parallel for reduction(+:num_hits)
        for (int shot = 0; shot < num_shots; shot++) {
                int thread = omp_get_thread_num ();
                double x = rnd (&seeds[thread]);
                double y = rnd (&seeds[thread]);
                if (x * x + y * y <= 1) num_hits = num_hits + 1;
        }
    double pi = 4.0 * (double) num_hits / (double)num_shots;
    printf ("%20.18lf\n", pi);
    return 0;
}