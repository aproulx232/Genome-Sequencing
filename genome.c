#include "genome.h"

static GNode *** loadFile(char * filename, int * numRow, int * numCol);
static void destroyMatrix(GNode *** matrix, int row, int col);
static GNode * createGNode(int value, int pos);
static GNode * compare(GNode *** matrix, int row, int col);
static void addList(GNode * curr, GNode * add);
static int * buildSeq(GNode * head, int * size_of_seq);
static int findLongest(GNode * node);
static void freeNtoN(GNode * start, GNode * end);



//free a linked list from a start node to a end or NULL
static void freeNtoN(GNode * start, GNode * end)
{
	GNode * curr = NULL;
	while(start != end)
	{
		curr = start;
		start = start -> list;
		free(curr);
	}
}

//finds the longest seq that a single value can connect with
//sets the link value to >0 if that path has been traversed already;
static int findLongest(GNode * node)
{
	GNode * maxNode = NULL;
	GNode * curr = NULL;
	int max = -1;
	int test = -1;
	curr = node -> list;
	if(curr == NULL)
	{
		return 1;
	}
	if(curr -> value > 0)
	{
		return curr -> value;
	}
	while(curr != NULL)
	{
		test = findLongest(curr -> next);
		test++;
		if( test >= max )
		{
			max = test;
			maxNode = curr;
		}
		curr = curr -> list;
	}
	freeNtoN(node -> list, maxNode);
	freeNtoN(maxNode -> list, NULL);
	node -> list = maxNode;
	node -> tail = maxNode;
	node -> list -> value = max;
	return max;
}


//runs through all the nodes of the first row of the matrix. 
//finding the node that has the longest link
static int * buildSeq(GNode * head, int * size_of_seq)
{
	int * seq = NULL;
	int test = 0;
	int max = 0;
	int index = 0;
	GNode * maxNode = NULL;
	GNode * curr = NULL;
	GNode * print = NULL;
	//find first with a list and list value = 0
	//have int max and GNode max and keep updating them
	curr = head;
	while(curr != NULL)
	{
		test = findLongest(curr);
		if(test >= max)
		{
			maxNode = curr;
			max = test;
		}
		curr = curr -> next;
	}
	print = maxNode;
	curr = NULL;
	if(print -> list == NULL)
	{
		(*size_of_seq) = 1;
	}
	else
	{
		(*size_of_seq) = (print -> list -> value);
	}
	seq = malloc(sizeof(int) * (*size_of_seq));
	if(seq == NULL)
	{
		printf("\nbuild seq error: malloc error m\n"); 
		return NULL;
	}

	while(print != NULL)
	{
		seq[index] = print -> value;
		index++;
		if(print -> list == NULL)
		{
			print = NULL;
		}
		else{
				print = print -> list -> next;
		}
	}
	return seq;
}


//adds a pointer to a node that when that node is able to point to the add node in a sub seq
static void addList(GNode * curr, GNode * add)
{
	GNode * dummy = createGNode(-1,-2);
	dummy -> next = add; 
	if(curr -> list == NULL)
	{
		curr -> list = dummy;
		curr -> tail = dummy;
	}
	else
	{	
		curr -> tail -> list  = dummy;
		curr -> tail = dummy;
	}
}

// makes a 2 node linked list. goes through every node and if the two nodes can form
// a 2 node seq, the first node points to the second
static GNode * compare(GNode *** matrix, int row, int col)
{
	int i = 0;
	GNode * test = NULL;
	GNode * curr = NULL;
	GNode * dcurr = NULL;
	GNode * dtest = NULL;
	GNode * head = NULL;
	//can implement a head so dont have to search TODO
	for(i =0; i < col; i++)
	{
		if(matrix[0][i]-> pos == 0)
		{
			curr = matrix[0][i];
			head = matrix[0][i];
			i = col;
		}
	}
	for(i = 0; i < col-1; i ++)
	{
		dcurr = curr -> down;
		test = curr -> next;
		dtest = test -> down;
		while(test != NULL)
		{
			//printf("j: %d\n",j);
			//compre to next
			while(dtest != NULL)
			{
				if(dtest -> pos < dcurr -> pos)
				{
					//printf("no seq dstest pos: %d dcurr: %d\n", dtest -> pos, dcurr -> pos);
					break;
				}
				dtest = dtest -> down;
				dcurr = dcurr -> down;
			}			
			if( dtest == NULL && dcurr == NULL)
			{
				//add test to curr list
				addList(curr, test);
			}
			test = test -> next;
			if(test != NULL)
			{
				dtest = test -> down;
				dcurr = curr -> down;
			}
		}
		//printList(curr); 
		curr = curr -> next;
	}
	return head;
}

//reads in the file into a matrix of GNodes.
//Stores the index of each value. uses the value as a index in the matrix.
//this allows the positions to be compared very quickly
static GNode *** loadFile(char * filename, int * numRow, int * numCol)
{
	int numN = 0;
	int numM = 0;
	GNode *** matrix = NULL;
	int readNum = -1;
	int row = 0;
	int col = 0;
	int temp = 0;
	GNode * curr = NULL;
	GNode * prev = NULL;
	FILE * fp = NULL;
	fp = fopen(filename, "r");
	if(fp == NULL)
	{
		printf("\nload from file: fopen error\n"); 
		return NULL;
	}
	readNum = fread(&numN, sizeof(int), 1, fp);
	*numCol = numN;
	if(readNum != 1)
	{
		printf("\nload from file: fread error\n"); 
		return NULL;
	}
	readNum = fread(&numM, sizeof(int), 1, fp);
	*numRow = numM;
	if(readNum != 1)
	{
		printf("\nload from file: fread error\n"); 
		return NULL;
	}
	matrix = malloc(sizeof(GNode*) * numM);
	if(matrix == NULL)
	{
		printf("\nload from file: malloc error m\n"); 
		return NULL;
	}
	for(; row < numM; row++)//m
	{
		matrix[row] =  malloc(sizeof(GNode) * numN);
		if(matrix[row]== NULL)
		{
			printf("\nload from file: malloc error m[i]\n"); 
			return NULL;
		}
	}
	for(row = 0; row < numM; row++)//m
	{
		for(col = 0; col < numN; col++)//n
		{
			//when creating matrix also link next nodes 
			readNum = fread(&temp , sizeof(int), 1, fp);
			//create node with temp value and col value
			curr = createGNode(temp, col);
			if(col == 0)
			{
				prev = curr;
				if(row == 0)
				{
				//	head -> next = curr;
				}
			}
			else
			{
				prev -> next = curr;
				prev = curr;
			}
			(matrix[row][temp - 1]) = curr;
			if(row != 0)
			{
				//link down for row above with curr row
				matrix[row -1][temp -1] -> down = curr;				
			}
			if(readNum != 1)
			{
				printf("\nload from file: fread error\n"); 
				return NULL;
			}
		}
	}
	fclose(fp);
	return matrix;
}

//initializes the struct GNode *
static GNode * createGNode(int amount, int posn)
{
	GNode * curr = malloc(sizeof(GNode));
	curr -> next = NULL;
	curr -> down = NULL;
	curr -> list = NULL;
	curr -> tail = NULL;
	curr -> pos = posn;
	curr -> value = amount;
	return curr;
}

int * Longest_conserved_gene_sequence(char * filename, int * size_of_seq)
{	
	GNode *** matrix = NULL;
	int numRow = 0;
	int numCol = 0;
	int * sequence = NULL;
	GNode * head = NULL;
	matrix = loadFile(filename, &numRow, &numCol);
	if(matrix == NULL)
	{
		size_of_seq = 0;
		return NULL;
	}
	head = compare(matrix, numRow, numCol);
	sequence = buildSeq(head, size_of_seq);
	destroyMatrix(matrix, numRow, numCol);
	return sequence;
}

//frees the matrix
static void destroyMatrix(GNode *** matrix, int row, int col)
{
	int i = 0;
	int j = 0;
	if( matrix == NULL)
	{
		return;
	}
	for(i = 0; i < row; i++)
	{
		for(j = 0; j < col; j++)
		{
			if(matrix[i][j] -> list != NULL)
			{
				free(matrix[i][j] -> list); 
			}
			free(matrix[i][j]);
		}
		free(matrix[i]);
	}
	free(matrix);
}


