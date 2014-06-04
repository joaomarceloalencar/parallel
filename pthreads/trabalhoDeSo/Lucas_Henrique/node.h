#ifndef NODE_H
#define NODE_H

class Node
{
public:
    int info;
    Node *next;

public:
    Node(int info, Node *ptr = 0)
    {
        this->info = info;
        next = ptr;
    }
};

#endif // NODE_H
