#include <pthread.h>

typedef struct Node {
   int val;
   struct Node *next;
   pthread_mutex_t mutex;
} Node;

Node *create_list(int value);
int insert_node(Node *head, int value);
Node *insert_in_place_node(Node *head, int value);
void print_list(Node *head);
