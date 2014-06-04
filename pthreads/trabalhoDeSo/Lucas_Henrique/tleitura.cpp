#include "tleitura.h"

List TLeitura::list;

TLeitura::TLeitura(string file, sem_t *s)
{
    fileName = file;
    semaphore = s;
}

void TLeitura::run()
{
    fstream file( fileName.c_str(), ios_base::in );

    while(file)
    {
        int num;
        file.read((char *)&num, sizeof(num));

        sem_wait(semaphore);
        list.add(num);
        sem_post(semaphore);
    }

    file.close();
}
