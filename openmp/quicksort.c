#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int compareInt(int a, int b) {
    if (a == b)
        return 0;
    else if (a < b)
        return -1;
    return 1;    
}

void printData(int *data, int size){
    for (int i = 0; i < size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
    return;
}


void par_qsort(int *data, int lo, int hi, int (*compare)(int, int)) {
    if (lo > hi) return;
    int l = lo;
    int h = hi;
    int p = data[(hi + lo) / 2];
    // printf("Pivo: data[%d] = %d\n", (hi + lo) / 2, p);
    while (l <= h) {
        while (compare(data[l], p) < 0) l++;
        while (compare(data[h], p) > 0) h--;
        if (l <= h) {
            int tmp = data[l];
            data[l] = data[h];
            data[h] = tmp;
            l++;
            h--;
        }
    }
    //printData(data, hi);
    #pragma omp task final (h - lo < 1000)
        par_qsort(data, lo, h, compare);
    
    #pragma omp task final (hi - l < 1000)
        par_qsort(data, l, hi, compare);
}

int main(int argc, char *argv[]) {
    int size;
    int max;
    sscanf(argv[1],"%d", &size);
    sscanf(argv[2],"%d", &max);

    int *data = (int *) malloc(size * sizeof(int));
    srand(time(0));
    for (int i = 0; i < size; i++) 
        data[i] = rand() % max;
    
    //printData(data, size);
    #pragma omp parallel
        #pragma omp single
            par_qsort(data, 0, size - 1, &compareInt);
    
    //printf("\n\n");
    //printData(data, size);

    free(data);
    return 0;
}

