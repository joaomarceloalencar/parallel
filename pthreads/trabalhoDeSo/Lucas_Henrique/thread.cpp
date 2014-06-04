#include "thread.h"
#include <iostream>

Thread::Thread()
{

}

Thread::~Thread()
{
}

void Thread::start()
{
    if(pthread_create(&threadID, NULL, entryPointer, this) != 0)
        std::cout << "Erro na criação da thread" << std::endl;
}

void Thread::abort()
{
    if(pthread_cancel(threadID) != 0)
        std::cout << "Erro no cancelamento da thread" << std::endl;
}

void Thread::wait()
{
    if(pthread_join(threadID, NULL) != 0)
        std::cout << "Erro ao esperar o encerramento da thread" << std::endl;
}

void *Thread::entryPointer(void *pthis)
{
    Thread *ptr = static_cast<Thread *>(pthis);
    ptr->run();
}
