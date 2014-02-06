#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define BUFF_SIZE 10 
#define N 100

/* Buffer de inteiros positivos. Se a posição está vazia, o valor é -1.*/
int buffer[BUFF_SIZE];
int lastpos = -1;
pthread_mutex_t mutex;
pthread_cond_t full;
pthread_cond_t empty;

void print_buffer(){
    int i;
    for (i = 0; i < BUFF_SIZE; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");
}

void *produce(){
    int i;
    for (i = 0; i < N; i++){
        pthread_mutex_lock(&mutex);
        while (lastpos == BUFF_SIZE - 1)
            pthread_cond_wait(&full, &mutex);
       
        int value = rand() % 100;
        buffer[lastpos + 1] = value;
        lastpos++;

        printf("Produtor: %d\n", value);
        print_buffer();
      
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }

}

void *consume(){
    int i;
    for (i = 0; i < N; i++){
        pthread_mutex_lock(&mutex);
        while (lastpos == -1)
            pthread_cond_wait(&empty, &mutex);

        int value = buffer[lastpos];
        buffer[lastpos] = -1;
        lastpos--;

        printf("Consumidor: %d\n", value);
        print_buffer();
 
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
    }

}

int main(int argc, char *argv[]){
    pthread_t consumer, producer;
    
    srand(time(0));
 
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&empty, NULL);
  
    int i;
    for (i = 0; i < BUFF_SIZE; i++)
        buffer[i] = -1;

    pthread_create(&producer, NULL, produce, NULL);
    pthread_create(&consumer, NULL, consume, NULL);
   
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
   
    return 0;
}
