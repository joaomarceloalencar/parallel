/*
 * Hypercube.h
 *
 *  Created on: 24/06/2014
 *      Author: jmhal
 */
#ifndef HYPERCUBE_H_
#define HYPERCUBE_H_

struct _Node {
	char *id;
	int d;
};
typedef struct _Node Node;

typedef struct Edge {
	Node *start;
	Node *end;
} Edge;

typedef struct NodeSet{
	Node *set;
	int size;
} NodeSet;

typedef struct EdgeSet{
	Node *set;
	int size;
} EdgeSet;

typedef struct R {
	NodeSet *set;
	int i;
	int j;
} R;

Node *createNode(char *str, int d);
int areNodesEquals(Node *node1, Node *node2);
int doesNodeExists(Node *node, int lastpos);
Node **createEquivalenceArray(int d);
Node *shiftNodeLeft(Node *node, int d);
void printEquivalenceClasses(int d);


int n(Node t);
int m(Node t);




#endif /* HYPERCUBE_H_ */
