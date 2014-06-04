#include <iostream>
#include <sys/time.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "list.h"
#include "thread.h"
#include <semaphore.h>
#include "tleitura.h"
#include "unistd.h"

#define SIZE 10
#define OUTPUT "arq/out.bin"

sem_t mutex;

using namespace std;

void ordenar();
void read();
void write();
int menu();

int main()
{
    int res = 0;

    while(res != 4)
    {
        res = menu();
        switch (res) {
        case 1:
            write();
            break;
        case 2:
            read();
            break;
        case 3:
            ordenar();
            break;
        default:
            break;
        }
    }



    return 0;
}

void ordenar()
{
    timeval inicio, fim;
    int tml;

    sem_init(&mutex,0,1);

    string names[SIZE];

    for(int i=0; i<SIZE; i++)
        cin >> names[i];
    for(int i=0; i<SIZE; i++)
        names[i] = "arq/" + names[i];

    cout << "iniciando leitura ..." << endl;

    Thread *threads[SIZE];
    for(int i=0;i<SIZE;i++)
    {
        threads[i] = new TLeitura(names[i], &mutex);
        threads[i]->start();
    }

    for(int i=0;i<SIZE;i++)
        threads[i]->wait();

    cout << "iniciar ordenação ..." << endl;

    gettimeofday(&inicio, NULL);
    TLeitura::list.tradixsort();
    gettimeofday(&fim, NULL);

    tml = (int) (1000*(fim.tv_sec - inicio.tv_sec) + (fim.tv_usec - inicio.tv_usec)/1000);
    cout << "tempo de ordenação: " << tml/1000.0 << endl;
    cout << "nome do arquivo de saída: " << OUTPUT << endl;

    TLeitura::list.save("arq/a.txt");
    TLeitura::list.saveBinary(OUTPUT);

    TLeitura::list.clean();

}

void read()
{
    string filename;
    cout << "Nome do arquivo: ";
    cin >> filename;
    filename = "arq/" + filename;

    fstream file(filename.c_str(), ios_base::in);

    int j = 0;
    while(!file.eof())
    {
        int num;
        file.read((char*)&num, sizeof(num));

        cout << num << endl;
        j++;
    }

    cout << j << endl;

    file.close();
}

void write()
{
    srand(time(NULL));

    string filename;
    cout << "Nome do arquivo: ";
    cin >> filename;
    filename = "arq/" + filename;

    int qtd;
    cout << "Quantidade de inteiros: ";
    cin >> qtd;

    fstream file(filename.c_str(), ios_base::out);

    for(int i=0; i<qtd; i++)
    {
        int num = rand()%1000000;
        file.write((char*)&num, sizeof(num));
    }

    file.close();
}

int menu()
{
    int res;
    cout << "1 - Deseja criar arquivo?" << endl;
    cout << "2 - Deseja imprimir arquivo" << endl;
    cout << "3 - Deseja unir 10 arquivos e ordena-los" << endl;
    cout << "4 - Sair" << endl;
    cin >> res;

    return res;
}
