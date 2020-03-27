#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int max;
    sscanf(argv[1], "%d", &max);
    #pragma omp parallel for schedule(runtime)
    for (int i = 1; i <= max; i++) {
        printf("%2d @ %d\n", i, omp_get_thread_num());
        sleep(i<4?i+1:1);
    }
    return 0;    
}