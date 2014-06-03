#ifndef LIST_H
#define LIST_H
#include <iostream>

using namespace std;

//É usado para adicionar os itens na lista onde ocorrerá o merge do vetor de listas.
void addItemMerge(List* l, Item* nItem){
	if(l->n == 0){
		l->first = nItem;
		l->last = nItem;
		l->n++;
	}else{
		l->last->next = nItem;
		nItem->prev = l->last;
		l->last = nItem;
		l->n++;
	}
}

//Função Usada na hora de ler os arquivos...
void addItem(List* l, int value){
	Item* nItem = new Item();
	nItem->value = value;
	if(l->n == 0){
		l->first = nItem;
		l->last = nItem;
		l->n++;
	}else{
		l->last->next = nItem;
		nItem->prev = l->last;
		l->last = nItem;
		l->n++;
	}
}

void swapItens(Item* i1, Item* i2){
	int aux = i1->value;
	i1->value = i2->value;
	i2->value = aux;
}

Item* part(Item* p, Item* q){
	Item* i = p;
	Item* j = q->prev;

	while(j != i && j->next != i){
		if(j->value < q->value){
			swapItens(j,i);
			i = i->next;
		}else{
			j = j->prev;
		}
	}
	if(j->value > q->value){
		swapItens(j,q);
		return j;
	}else{
		swapItens(j->next,q);
		return j->next;
	}
}


void QuickSort(Item* p, Item* q){
	if( p!= q && p->prev != q){
		Item* pivo = part(p,q);
		if(q->prev != NULL)
			QuickSort(p,pivo->prev);
		if(pivo->next != NULL)
			QuickSort(pivo->next,q);
	}
}

void *QuickSortThread(void* lista){
	List* l = (List*)((void*)lista);
	Item* p = l->first;
	Item* q = l->last;
	QuickSort(p,q);
}


List* merge(List* l1, List* l2){
	List* laux = new List();
	Item* aux1 = l1->first;
	Item* auxRemove;
	Item* aux2 = l2->first;
	while(aux1 != NULL && aux2 != NULL){
		if(aux1->value < aux2->value){
			addItemMerge(laux, aux1);
			aux1 = aux1->next;
		}else{
			addItemMerge(laux, aux2);
			aux2 = aux2->next;
		}
	}
	if(aux1 != NULL){
		while(aux1 != NULL){
			addItemMerge(laux, aux1);
			aux1 = aux1->next;
		}
	}
	if(aux2 != NULL){
		while(aux2 != NULL){
			addItemMerge(laux, aux2);
			aux2 = aux2->next;
		}
	}
	return laux;
}

void showList(List* l){
	Item* item = l->first;
	if(l->n != 0){
		while(item->next != NULL){
			cout<<"["<< item->value <<"]"<<endl;
			item = item->next;
		}
	}
	cout << "----- Quantidade de elementos na lista "<<l->n<<endl;
}


void *mergeThread(void* values){
	Values* v = ((Values*)values);
	if(v->core > 1){//se a quantidade de nucleos for maior que 1 ele executa o merge , se não ele só escreve a lista.
		v->listMerge[0] = merge(v->vectList[0],v->vectList[1]);
		for(int i = 2; i < v->core; i++){
			v->listMerge[0] = merge(v->vectList[i],v->listMerge[0]);

		}
		createFileOrdenado("output.bin",v->listMerge);
		showList(v->listMerge[0]);
	}else{
		createFileOrdenado("output.bin",v->vectList);
		showList(v->vectList[0]);
	}
	
	
}
 



#endif