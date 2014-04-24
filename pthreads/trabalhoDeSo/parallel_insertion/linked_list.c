#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

Node *create_list(int value) {
   Node *head = (Node *) malloc(sizeof(Node));
   head->val = value;
   head->next = NULL;
   return head;
}

int insert_node(Node *head, int value) {
   Node *ptr = head;
   while (ptr->next != NULL)
       ptr = ptr->next;
   Node *new = (Node *) malloc(sizeof(Node));
   if (new <= 0)
       return -1;
   ptr->next = new;
   new->val = value;
   new->next = NULL;
   return 0;
}

Node *insert_in_place_node(Node *head, int value) {
   Node *ptr = head;
   Node *prev = NULL;

   // Encontra a posição onde colocar o novo valor.
   while ((ptr != NULL) && (ptr->val < value)) {
       prev = ptr;
       ptr = ptr->next;
   }

   // Tenta alocar o novo nó.
   Node *new = (Node *) malloc(sizeof(Node));
   if (new <= 0)
      return NULL;

   // Caso o novo nó seja a nova cabeça.
   if (prev == NULL) { 
      new->val = value;
      new->next = ptr;
      return new;
   }

   // Caso o novo nó não seja a cabeça.
   new->val = value;
   prev->next = new;
   new->next = ptr;
   return head;
}

void print_list(Node *head) {
   Node *ptr = head;
   while (ptr != NULL) {
      printf("%d->", ptr->val);
      ptr = ptr->next;
   }
   printf("\n");
}
