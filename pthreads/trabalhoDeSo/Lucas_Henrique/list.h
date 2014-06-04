#ifndef List_H
#define List_H

#include "node.h"
#include <iostream>
#include <fstream>
#include "queue.h"
#include "thread.h"
#include <unistd.h>

using namespace std;

class List
{
public:
    List();
    ~List();
    void add(int info);
    void deleteNode(int info);
    void imprimir();
    void save(string local);
    void saveBinary(string local);
    void radixsort();
    void tradixsort();
    void clean();
    int getLen();
private:
    int len;
    Node *node;
};

class TRadix : public Thread
{
public:
    TRadix(Node *p, Node *q);
private:
    Queue queue;
    Node *p;
    Node *q;
    int len;
private:
    void run();
};

class TMerge : public Thread
{
public:
    TMerge(Node *p1, Node *q1, Node *p2, Node *q2);
private:
    Node *p1, *q1, *p2, *q2;
private:
    void run();
};

#endif // List_H
