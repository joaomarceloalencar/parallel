#include <stdio.h>
#include <omp.h>

long fib(int n) {
    return (n < 2? 1 : fib(n-1) + fib(n-2));
}

int main(int argc, char *argv[]) {
    int max;
    sscanf(argv[1],"%d", &max);

    #pragma omp parallel
        #pragma omp single
        for (int n = 1; n <= max; n++)
            #pragma omp task
            printf("%d: %d %ld\n", omp_get_thread_num(), n, fib(n));
    return 0;
}
