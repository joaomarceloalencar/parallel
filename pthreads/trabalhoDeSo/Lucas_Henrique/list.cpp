#include "list.h"

List::List()
{
    node = new Node(0, NULL);
    len = 0;
}

List::~List()
{
    if(len > 0)
        clean();
    else
    {
        delete node;
    }
}

void List::add(int info)
{
    Node *newNode = new Node(info);

//    Node *p;
//    for(p = node; p->next != NULL
//        && p->next->info < info; p = p->next);

    newNode->next = node->next;
    node->next = newNode;

//    newNode->next = p->next;
//    p->next = newNode;

    len++;
}

void List::deleteNode(int info)
{

}

void List::imprimir()
{
    Node *p;
    for(p = node->next; p != NULL; p = p->next)
        std::cout << p->info << " ";
    std::cout << std::endl;
}

void List::save(string local)
{
    fstream stream(local.c_str(), ios_base::out);
    Node *p;
    for(p = node->next; p != NULL; p = p->next)
        stream << p->info << " ";
    stream.close();
}

void List::saveBinary(string local)
{
    fstream stream(local.c_str(), ios_base::out);
    Node *p;
    for(p = node->next; p != NULL; p = p->next)
        stream.write((char*)&p->info,sizeof(p->info));
    stream.close();
}

void List::radixsort()
{
    register int i, j, factor;
    const int radix = 10;
    const int digits = 10;
    Queue queue[radix];
    Node *p;
    for(i = 0, factor = 1; i < digits; factor *= radix, i++)
    {
        for(p = node->next; p != NULL; p = p->next)
            queue[(p->info / factor) % radix].enqueue(p->info);
        for(j = 0, p = node->next; j < radix; j++)
            while(!queue[j].empty())
            {
                p->info = queue[j].dequeue();
                p = p->next;
            }
    }
}

void List::tradixsort()
{
    int np = (int)sysconf(_SC_NPROCESSORS_ONLN);
    if(np == 1)
    	radixsort();
    else
    {
	    int t = len/np;
	    Node *n[np+1];
	    Thread *radixs[np];
	    Node *p = node->next;
	    n[0] = p;
	    for(int i=1; i<np; i++)
	    {
	        for(int j=0;j<t;j++)
	            p = p->next;
	        n[i] = p;
	        radixs[i-1] = new TRadix(n[i-1], p);
	    }
	    n[np] = NULL;
	    radixs[np-1] = new TRadix(n[np-1], NULL);
	    for(int i=0; i<np; i++)
	    {
	        radixs[i]->start();
	    }
	    for(int i=0; i<np; i++)
	        radixs[i]->wait();

	    register int j = np;
	    while( j != 0 )
	    {
	        j /= 2;
	        TMerge *merge[j];
	        for(int i = 0, k = 0; k < j; i+=2, k++)
	        {
	            merge[k] = new TMerge(n[i], n[i+1], n[i+1], n[i+2]);
	            merge[k]->start();
	        }
	        for(int i = 0; i < j; i++)
	            merge[i]->wait();
	        for(int i = 0; i <= j; i++)
	            n[i] = n[2*i];
	    }
	}

}

void List::clean()
{
    Node *p = node->next;
    while(p != NULL)
    {
        Node *q = p;
        p = p->next;
        delete q;
    }
    node->next = NULL;
    len = 0;
}

int List::getLen()
{
    return len;
}

TRadix::TRadix(Node *p, Node *q)
{
    this->p = p;
    this->q = q;
}

void TRadix::run()
{
    register int i, j, factor;
    const int radix = 10;
    const int digits = 10;
    Queue queue[radix];
    Node *ptr;
    for(i = 0, factor = 1; i < digits; factor *= radix, i++)
    {
        for(ptr = p; ptr != q; ptr = ptr->next)
            queue[(ptr->info / factor) % radix].enqueue(ptr->info);
        for(j = 0, ptr = p; j < radix; j++)
            while(!queue[j].empty())
            {
                ptr->info = queue[j].dequeue();
                ptr = ptr->next;
            }
    }

}


TMerge::TMerge(Node *p1, Node *q1, Node *p2, Node *q2)
{
    this->p1 = p1;
    this->q1 = q1;
    this->p2 = p2;
    this->q2 = q2;
}

void TMerge::run()
{
    Node *m1 = p1, *m2 = p2;
    Queue queue;
    while(m1 != q1 && m2 != q2)
    {
        if(m1->info<=m2->info)
        {
            queue.enqueue(m1->info);
            m1 = m1->next;
        }
        else
        {
            queue.enqueue(m2->info);
            m2 = m2->next;
        }
    }
    if(m1!=q1)
        for(;m1 != q1; m1 = m1->next)
            queue.enqueue(m1->info);
    else
        for(;m2 != q2; m2 = m2->next)
            queue.enqueue(m2->info);
    for(m1 = p1; m1 != q2; m1 = m1->next)
        m1->info = queue.dequeue();
}
