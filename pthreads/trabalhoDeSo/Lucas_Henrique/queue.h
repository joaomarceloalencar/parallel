#ifndef QUEUE_H
#define QUEUE_H

#include "node.h"

class Queue
{
public:
    Queue();
    void enqueue(int info);
    int dequeue();
    bool empty();
private:
    Node *first;
    Node *last;
    int len;
};

#endif // QUEUE_H
