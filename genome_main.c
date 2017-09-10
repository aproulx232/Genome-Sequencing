#include "genome.h"

int main(int argc, char ** argv)
{
	char * load_file = argv[1];
	int * seq = NULL;
	int seqSize = -1;
	seq = Longest_conserved_gene_sequence(load_file, &seqSize);
	free(seq);
	printf("Length: %d\n", seqSize);
	return(EXIT_SUCCESS);
}
