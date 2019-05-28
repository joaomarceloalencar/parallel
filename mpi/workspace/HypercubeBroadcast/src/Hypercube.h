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
	Edge *set;
	int size;
} EdgeSet;

Node *createNode(char *str, int d);
void deleteNode(Node *node, int d);
int areNodesEquals(Node *node1, Node *node2);
int doesNodeExists(Node *node, int lastpos);
Node **createEquivalenceArray(int d);
Node *shiftNodeLeft(Node *node, int d);
void printEquivalenceClasses(int d);
NodeSet **createESet(Node **equivalenceClasses, int d);
EdgeSet **createASet(Node **equivalenceClasses, NodeSet **E, int d);
EdgeSet **createASetForNewRoot(EdgeSet **A, char root[], int d);
EdgeSet **createASuperSet(EdgeSet **A, int nproc, int d);

int n(char *id, int d);
int m(char *id, int d);

int binaryToInt(char id[], int d);
char *intToBinary(int id, int d);


#endif /* HYPERCUBE_H_ */
