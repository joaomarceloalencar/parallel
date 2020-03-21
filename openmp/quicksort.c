#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void par_qsort(char **data, int lo, int hi, int (*compare)(const char *, const char *)) {
    if (lo > hi) return;
    int l = lo;
    int h = hi;
    char *p = data[(hi + lo) / 2];
    while (l <= h) {
        while (compare(data[l], p) < 0) l++;
        while (compare(data[h], p) > 0) h--;
        if (l <= h) {
            char *tmp = data[l];
            data[l] = data[h];
            data[h] = tmp;
            l++;
            h--;
        }
    }

    #pragma omp task final (h - lo < 1000)
        par_qsort(data, lo, h, compare);
    
    #pragma omp task final (hi - l < 1000)
        par_qsort(data, l, hi, compare);
}

int main() {

    #pragma omp parallel
        #pragma omp single
            par_qsort(strings, 0, num_strings - 1, compare);
    return 0;
}
