#include <stdio.h>
#include <stdlib.h>
//add struct TODO

typedef struct _graph_Node{
	int value;//value of the node
	int pos;//position in the org array
	struct _graph_Node * next;//pointers to the next int the the original secuence
	struct _graph_Node * down;//pointer to the same value in the series below in the matrix
	struct _graph_Node * list;//list of nodes that can come after curr node in sunsequence
	struct _graph_Node * tail;//tail of the list of nodes
}	GNode;
int * Longest_conserved_gene_sequence(char * filename, int * size_of_seq);



















































