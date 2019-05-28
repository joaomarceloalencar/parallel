/*
 * Hypercube.c
 *
 *  Created on: 24/06/2014
 *      Author: jmhal
 */
#include "Hypercube.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

Node **equivalenceClasses = NULL;

Node *createNode(char *str, int d) {
	Node *n = (Node *) malloc(sizeof(Node));
	n->d = d;
	n->id = (char *) malloc(d * sizeof(char));

	int i;
	for (i = 0; i < d; i++)
		if (str == NULL)
			n->id[i] = '0';
		else
		    n->id[i] = str[i];

	return n;
}

void deleteNode(Node *node, int d){
	free(node->id);
	free(node);
}

int areNodesEquals(Node *node1, Node *node2){
	if (node1->d == node2->d){
		int i;
		for (i = 0; i < node1->d; i++)
			if (node1->id[i] != node2->id[i])
				return 0;
		return 1;
	} else
		return 0;
}

int doesNodeExists(Node *node, int lastpos){
	int i;
	for (i = 0; i < lastpos; i++) {
		if (areNodesEquals(node, equivalenceClasses[i]))
			return 1;
	}
	return 0;
}

void printEquivalenceClasses(int d) {
	int n = (int) pow(2, d);
	int i;
	for (i = 0; i < n ; i++)
		printf("%s ", equivalenceClasses[i]->id);
}

Node *shiftNodeLeft(Node *node, int d){
	Node *newNode = createNode(NULL, d);
	int i;
	for (i = 0; i < d ; i++){
		newNode->id[i] = node->id[(i+1) % d];
	}
	return newNode;
}

Node **createEquivalenceArray(int d){
	int n = (int) pow(2, d);

	if (equivalenceClasses == NULL)
		equivalenceClasses = (Node **) malloc(n * sizeof(Node *));

	/* First class, all zeroes. */
	Node *R00 = createNode(NULL, d);
	equivalenceClasses[0] = R00;

	int i;
	int lastpos = 1;
	for (i = 1; i <= d ; i++) {
		int j;
		for (j = 1; j <= i; j++) {
			/* If all the equivalence classes are full, quit.*/
			if (lastpos == n - 1 ) break;
			if (i + j > d) break;

		    Node *newNode = createNode(NULL, d);
			int k;
			for (k = 0; k <= d - i - j; k++) newNode->id[k] = '0';
			newNode->id[k] = '1';
			for (k = d - i - j + 2; k <= d - i; k++) newNode->id[k] = '0';
			for (k = d - i + 1; k < d; k++) newNode->id[k] = '1';

			/* The bit at position m(t) from the right is 1. */
			int nt = lastpos;
			int mt = 1 + ((nt - 1) % d);
			while (newNode->id[d-mt] != '1') {
				Node *oldNode = newNode;
				newNode = shiftNodeLeft(newNode, d);
				deleteNode(oldNode, d);
			}

			/* Two extra restrictions for the case where j = 1.
			 * If m(t) > 1, the bit at position m(t) - 1 from right is a 0.
			 * If m(t) = 1, the bit at the leftmost position is a 0. */
			while(!(mt > 1 && newNode->id[d - mt + 1] == '0') && !(mt == 1 && newNode->id[0] == '0')) {
				Node *oldNode = newNode;
				newNode = shiftNodeLeft(newNode, d);
				deleteNode(oldNode, d);
			}

			/* Just a small memory waste.
			 * Creates all the other nodes in the class.*/
			while (!doesNodeExists(newNode, lastpos)) {
				equivalenceClasses[lastpos] = newNode;
				lastpos++;
				newNode = shiftNodeLeft(newNode, d);
			}
		}
	}

	/* Last class, all ones. */
	Node *RDD = createNode(NULL, d);
	for (i = 0; i < d; i++)
		RDD->id[i] = '1';
	equivalenceClasses[n-1] = RDD;


	return equivalenceClasses;
}


int n(char *id, int d) {
	int n = (int) pow(2, d);
	int i;
	for (i = 0; i < n; i++) {
		/* Different dimensions. */
		if (equivalenceClasses[i]->d != d)
			return -1;

		int value = 0;
		int j;
		for (j = 0; j < d; j++)
			if (equivalenceClasses[i]->id[j] == id[j])
				value++;

		if (value == d)
			return i;
		else
			continue;
	}
	return -1;
}

int m(char *id, int d) {
	int a = n(id,d);
	return 1 + (a -1)%d;
}

NodeSet **createESet(Node **equivalenceClasses, int d){
	int q = (int) ceil((pow(2,d) - 1) / d);
	int size = (int) pow(2, d);
	NodeSet **E = (NodeSet **) malloc((q+1)*sizeof(NodeSet *));

	/* First set. */
	E[0] = (NodeSet *) malloc(sizeof(NodeSet));
	E[0]->size = 1;
	E[0]->set = (Node *) malloc(sizeof(Node));
	E[0]->set[0] = *createNode(NULL,d);

	/* Other Sets. */
	int i;
	for (i = 1; i < q + 1; i++){
		E[i] = (NodeSet *) malloc(sizeof(NodeSet));
		E[i]->size = 0;

		int j;
		for (j = 0; j < size; j++) {
			int nt = n(equivalenceClasses[j]->id,d);
			if (i != q) {
				if ((nt >= (i - 1) * d + 1) && (nt <= i * d))
					E[i]->size++;
			} else if (i == q) {
				if ((nt >= (q - 1) * d + 1) && (nt <= pow(2, d) - 1))
					E[i]->size++;
			}

		}
		E[i]->set = (Node *) malloc(E[i]->size * sizeof(Node));
		E[i]->size = 0;
		for (j = 0; j < size; j++) {
			int nt = n(equivalenceClasses[j]->id, d);
			if (i != q) {
				if ((nt >= (i - 1) * d + 1) && (nt <= i * d)) {
					E[i]->set[E[i]->size] = *equivalenceClasses[j];
					E[i]->size++;
				}
			} else if (i == q) {
				if ((nt >= (q - 1) * d + 1) && (nt <= pow(2, d) - 1)) {
					E[i]->set[E[i]->size] = *equivalenceClasses[j];
					E[i]->size++;
				}
			}
		}
	}

	return E;
}

EdgeSet **createASet(Node **equivalenceClasses, NodeSet **E, int d) {
	int q = (int) ceil((pow(2,d) - 1) / d);
	int i;
	// int size = (int) pow(2, d);
	EdgeSet **A = (EdgeSet **) malloc((q+1)*sizeof(EdgeSet *));

	/* First set. It's empty, just for control. */
	A[0] = (EdgeSet *) malloc(sizeof(EdgeSet));
	A[0]->size = 0;
	A[0]->set = NULL;

	/* The real A sets that matter. */
	// int i;
	for (i = 1; i < q + 1; i++){
		/* Allocate the set.*/
		A[i] = (EdgeSet *) malloc(sizeof(EdgeSet));
		A[i]->size = 0;
		A[i]->set = NULL;

		int j;
		/* For each t belonging to E[i]. */
		for (j = 0; j < E[i]->size; j++) {
			/* Define _t as the same t with an invertion of the m(t) from the right. */
			Node *t = createNode(E[i]->set[j].id,d);
			Node *temp = createNode(E[i]->set[j].id, d);

			int mt = m(t->id, d);
			t->id[d - mt] = '0';

			Node *_t = createNode(t->id,d);

			free(t);
			t = temp;

			/* Find if _t belongs to the union of E[0]...E[i-1].*/
			int status = 0;
			int k;
			for (k = 0; k < i; k++) {
				int l;
				/* Searching for _t in the specific E[k].*/
				for (l = 0; l < E[k]->size; l++) {
					if (areNodesEquals(_t, &E[k]->set[l])) {
						status = 1;
						break;
					}
				}
				if (status == 1)
					break;
			}
			/* If _t does not belong to the union, let's try another t.*/
			if (status == 0)
				continue;
			else if (status == 1){
				/* Update the A[i] set.*/
				EdgeSet *temp = A[i];
				A[i] = (EdgeSet *) malloc(sizeof(EdgeSet));
				A[i]->size = temp->size + 1;
				A[i]->set = (Edge *) malloc((A[i]->size) * sizeof(Edge));

				Edge *newEdge = (Edge *) malloc(sizeof(Edge));;
				newEdge->start = createNode(_t->id, d);
				newEdge->end = createNode(t->id, d);

				int p;
				for (p = 0; p < temp->size; p++)
					A[i]->set[p] = temp->set[p];
				A[i]->set[p] = *newEdge;

				free(temp);
				free(newEdge);
				free(_t);
			}
		}
	}
	return A;
}

EdgeSet **createASetForNewRoot(EdgeSet **A, char root[], int d) {
	int q = (int) ceil((pow(2,d) - 1) / d);
	EdgeSet **newRootA = (EdgeSet **) malloc((q+1)*sizeof(EdgeSet *));

	/* For each edge set A[i]. */
	int i;
	for (i = 0; i < q + 1; i++ ) {
		/* A new newRootA[i] with the same size as A[i].*/
		newRootA[i] = (EdgeSet *) malloc(sizeof(EdgeSet));
		newRootA[i]->size = A[i]->size;
		newRootA[i]->set = (Edge *) malloc((A[i]->size) * sizeof(Edge));

		/* For each edge in the set A[i]. */
		int j;
		for (j = 0; j < A[i]->size; j++) {
			Node *start = A[i]->set[j].start;
			Node *end = A[i]->set[j].end;

			Node *newStart = createNode(NULL, d);
			Node *newEnd = createNode(NULL, d);

			/* Do the xor. */
			int k;
			for (k = 0; k < d; k++){
				if ((root[k] == '1' && start->id[k] == '1') || (root[k] == '0' && start->id[k] == '0'))
					newStart->id[k] = '0';
				else
					newStart->id[k] = '1';

				if ((root[k] == '1' && end->id[k] == '1') || (root[k] == '0' && end->id[k] == '0'))
					newEnd->id[k] = '0';
				else
					newEnd->id[k] = '1';
			}

			newRootA[i]->set[j].start = newStart;
			newRootA[i]->set[j].end = newEnd;
		}
	}

	return newRootA;
}

int binaryToInt(char id[], int d) {
	int result = 0;

	int i;
	for (i = 0; i < d; i++){
		int pos = 0;
		if (id[i] == '1')
			pos = 1;
		else if (id[i] == '0')
			pos = 0;

		result += pos * (int) pow(2, d - i - 1);
	}

	return result;
}

char *intToBinary(int id, int d) {
	if (id > pow(2,d))
		return NULL;
	char *result = (char *) malloc(d * sizeof(char));

	int i;
	for (i = d - 1; i >= 0; i--){
		if (id % 2 == 0)
			result[i] = '0';
		else if (id % 2 == 1)
			result[i] = '1';
		id = id / 2;
	}
	return result;
}

EdgeSet **createASuperSet(EdgeSet **A, int nproc, int d) {
	int q = (int) ceil((pow(2,d) - 1) / d);
	int i;
	EdgeSet **A_total = (EdgeSet **) malloc((q + 1) * sizeof(EdgeSet *));
	for (i = 0; i < q + 1; i++) {
		A_total[i] = (EdgeSet *) malloc(sizeof(EdgeSet));
		A_total[i]->size = nproc * A[i]->size;
		A_total[i]->set = (Edge *) malloc((nproc * A[i]->size) * sizeof(Edge));
		int pos = 0;

		/* For each process, create a spanning tree */
		int j;
		for (j = 0; j < nproc; j++) {
			char *root = intToBinary(j, d);
			EdgeSet **newA = createASetForNewRoot(A, root, d);
			/* Iterate through each spanning tree in order to create the superset A_total[i]. */
			int k;
			for (k = 0; k < newA[i]->size; k++) {
				A_total[i]->set[pos] = newA[i]->set[k];
				pos++;
			}
		}
	}
	return A_total;
}


