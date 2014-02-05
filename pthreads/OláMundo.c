/******************************************************************************
* FILE: hello.c
* DESCRIPTION:
*   A "hello world" Pthreads program.  Demonstrates thread creation and
*   termination.
* AUTHOR: Blaise Barney
* LAST REVISED: 08/09/11
* TRANSLATION: João Marcelo
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	4

void *PrintHello(void *threadid) {
   long tid;
   tid = (long) threadid;
   printf("Olá Mundo! Sou eu, thread #%ld!\n", tid);
   pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;

   for (t = 0; t < NUM_THREADS; t++){
       printf("Na main: criando thread %ld\n", t);
       rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
       if (rc) {
           printf("ERROR; return code from pthread_create() is %d\n", rc);
           exit(-1);
       }
   }

   /* Última coisa que a main() deve fazer */
   pthread_exit(NULL);
}
