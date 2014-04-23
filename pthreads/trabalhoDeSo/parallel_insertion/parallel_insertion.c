#include <stdio.h>
#include "linked_list.h"

int main(int arc, char *argv[]) {
   Node *list = create_list(10);
   
   insert_node(list, 11);
   insert_node(list, 12);
   insert_node(list, 13);
  
   print_list(list); 


   return 0;
}
