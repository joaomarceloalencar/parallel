/*
 * Hypercube.c
 *
 *  Created on: 24/06/2014
 *      Author: jmhal
 */
#include "Hypercube.h"
#include <stdlib.h>
#include <stdio.h>
#include <tgmath.h>

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
			/* Create the first node.*/
			Node *newNode = createNode(NULL, d);
			int k;
			for (k = d - 1; k >= d - i + (j - 1); k--)
				newNode->id[k] = '1';
			newNode->id[d - i - (j - 1)] = '1';

			/* Just a small memory waste. */
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



