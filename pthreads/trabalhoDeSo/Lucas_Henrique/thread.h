#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include "semaphore.h"

using namespace std;

class Thread
{

public:
    Thread();
    ~Thread();
    void start();
    void abort();
    void wait();

protected:
    virtual void run() = 0;

private:
    static void *entryPointer(void *pthis);

public:
    pthread_t threadID;

};

#endif // THREAD_H
