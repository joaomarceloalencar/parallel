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

void print_list(Node *head) {
   Node *ptr = head;
   while (ptr != NULL) {
      printf("%d->", ptr->val);
      ptr = ptr->next;
   }
   printf("\n");
}
