#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    struct node *next;
    int val;
} node;

int insert_node(struct node **head, int val) {
    struct node *elem;
    elem = (struct node *)malloc(sizeof(struct node));
    if (!elem)
        return 0;
    elem->val = val;
    elem->next = *head;
    *head = elem;
    return 1;
}

int get_lval(struct node *head, int l) {
    while(head && l) {
        head = head->next;
        l--;
    }
    if (head != NULL)
        return head->val;
    else
        return -1;
}

void swap(struct node *head, int i, int j) {
    struct node *tmp = head;
    int tmpival;
    int tmpjval;
    int ti = i;
    while(tmp && i) {
        i--;
        tmp = tmp->next;
    }
    tmpival = tmp->val;
    tmp = head;
    while(tmp && j) {
        j--;
        tmp = tmp->next;
    }
    tmpjval = tmp->val;
    tmp->val = tmpival;
    tmp = head;
    i = ti;
    while(tmp && i) {
        i--;
        tmp = tmp->next;
    }
    tmp->val = tmpjval;
}


struct node *Quick_Sort_List(struct node *head, int l, int r) {
    int i, j;
    int jval;
    int pivot;
    i = l + 1;
    if (l + 1 < r) {
        pivot = get_lval(head, l);
        printf("Pivot = %d\n", pivot);
        for (j = l + 1; j <= r; j++) {
            jval = get_lval(head, j);
            if (jval < pivot && jval != -1) {
                swap(head, i, j);
                i++;
            }
        }
        swap(head, i - 1, l);
        Quick_Sort_List(head, l, i);
        Quick_Sort_List(head, i, r);
    }
    return head;
}


struct node *Sort_linkedlist(struct node *head) {
    struct node *tmp = head;
    // Using Quick sort.
    int n = 0;

    while (tmp) {
        n++;
        tmp = tmp->next;
    }
    printf("n = %d\n", n);
    head = Quick_Sort_List(head, 0, n);
    return head;
}

void print_list(struct node *head) {
    while(head) {
        printf("%d->", head->val);
        head = head->next;
    }
    printf("\n");
}


void write_list(struct node *head, char *filename) {
    FILE *fp = fopen(filename, "w+");
    while (head) {
        int val = head->val;
        fwrite(&val, sizeof(val), 1, fp);
        head = head->next;
    }
    fclose(fp);
}


int main(int argc, char *argv[]){
    struct node *head = NULL;
    struct node *shead = NULL;

    /* Abrir cada um dos arquivos e colocar os valores na lista. */
    int i;
    for (i = 0; i < 10; i++) {
        FILE *fp;
          
        char filename[100];
        snprintf(filename, 100, "%s%d%s", "arquivos/arquivo", i, ".bin");
 
        fp = fopen(filename,"r");
        while (!feof(fp)) {
            int number;
            fread(&number, sizeof(number), 1, fp);
            insert_node(&head, number);
        }
        fclose(fp);
    }
    
    shead = Sort_linkedlist(head);
    write_list(shead, "output.bin");
    
    return 0;
}
