#ifndef TLEITURA_H
#define TLEITURA_H

#include "thread.h"
#include "list.h"

class TLeitura : public Thread
{
public:
    TLeitura(string file, sem_t *s);
private:
    void run();
public:
    static List list;
private:
    string fileName;
    sem_t *semaphore;
};

#endif // TLEITURA_H
