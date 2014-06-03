#ifndef STRUTURES_H
#define STRUTURES_H

#include <string>

struct Item{
	int value;
	Item* next;
	Item* prev;
};

struct List{
	int n = 0;
	Item* first;
	Item* last;
};


struct Values
{	
	int core;//Quantidade de nucleos.
	char fileName[13] = {'a','r','q','u','i','v','o','X','.','b','i','n','\0'};
	pthread_mutex_t* mutex;
	List* vectList[20];
	List* listMerge[1];
};


#endif