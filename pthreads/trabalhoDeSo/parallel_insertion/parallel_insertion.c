#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "linked_list.h"

#define N 1000
#define N_THREADS 10

/*
   Vou criar o primeiro nó da lista contendo 0. Como todos os elementos gerados são maiores ou iguais a zero,
   podemos retirá-lo depois.
*/
Node *head;

void *insert_items(void *id) {
    int thread_id = (int) id;

    // Carregar inteiros do arquivo
    FILE *fp;
    char filename[100];
    snprintf(filename, 100, "%s%d%s", "arquivos/arquivo", thread_id, ".bin");
    fp = fopen(filename,"r");

    while (!feof(fp)) {
        int number;
        fread(&number, sizeof(number), 1, fp);

        // Esse laço irá controlar a situação em que mais de um thread tenta adicionar um novo nó na mesma posição.
        int flag = 1;
        while (flag) {
            // Encontra a posição onde colocar o novo valor.
            Node *ptr = head;
            Node *prev = NULL;
            while ((ptr != NULL) && (ptr->val < number)) {
                prev = ptr;
                ptr = ptr->next;
            }

            // Não adicionar valores iguais à lista para evitar desperdício de memória.
            if ((ptr != NULL) && (ptr->val == number)) {
                flag = 0;
                continue;
            }

            // Adquire exclusividade do nó anterior ao novo.
            pthread_mutex_lock(&prev->mutex);

            // Verifica se não houve tentativa simultânea de inserção após a posição indicada por 'prev'
            if ((prev->next != NULL) && (prev->next->val < number)) {
                flag = 1;
                pthread_mutex_unlock(&prev->mutex);
                continue;
            } else {

                Node *new = (Node *) malloc(sizeof(Node));
                if (new <= 0)
                {
                    printf("Unable to allocate new node...");
                    return NULL;
                }
                new->val = number;
                prev->next = new;
                new->next = ptr;
                pthread_mutex_init(&new->mutex, NULL);
                flag = 0;
                pthread_mutex_unlock(&prev->mutex);
                continue;
            }
        }
    }
    return NULL;
}


int main(int arc, char *argv[]) {
    /* Primeiro elemento tem 0.*/
    head = create_list(-1);
    pthread_mutex_init(&head->mutex, NULL);

    int i;
    pthread_t threads[N_THREADS];

    printf("Criando os threads.\n");
    for (i = 0; i < N_THREADS; i++)
        pthread_create(&threads[i], NULL, insert_items, (void *) i);

    for (i = 0; i < N_THREADS; i++)
        pthread_join(threads[i], NULL);

    // print_list(head);
    printf("Threads finalizaram. Escrevendo no arquivo.");
    char *filename = "output.bin";
    FILE *fp = fopen(filename, "w+");
    Node *ptr = head;
    while (ptr != NULL) {
       fwrite( &ptr->val, sizeof(int), 1, fp);
       ptr = ptr->next;
    }
    fclose(fp);

    printf("Finalizando.");
    return 0;
}
