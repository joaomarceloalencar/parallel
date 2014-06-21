#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 4

int number = 0;
pthread_mutex_t mutex;

void *add(void *count) {
    int *c = (int *) count; 
    int i, rc;
    for (i = 0; i < *c; i++) {
	rc = pthread_mutex_trylock(&mutex);
        if (rc != 0) {
            printf("Fazendo outra coisa.\n");
        }
        number++;  
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char *argv[]){
    pthread_t threads[NUM_THREADS];
    int i, rc;    
    int n = 100;

    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_create(&threads[i], NULL, add, (void *) &n);
        if (rc) {
            exit(-1);
        }
    }

    for (i = 0; i< NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d\n", number);   
    return 0;
}

