#include "queue.h"

Queue::Queue()
{
    len = 0;
}

void Queue::enqueue(int info)
{
    if(len == 0)
    {
        first = new Node(info);
        last = first;
    }
    else
    {
        last->next = new Node(info);
        last = last->next;
    }
    len++;
}

int Queue::dequeue()
{
    Node *n = first;
    first = n->next;

    int i = n->info;
    delete n;

    len--;

    return i;
}

bool Queue::empty()
{
    return len == 0;
}
