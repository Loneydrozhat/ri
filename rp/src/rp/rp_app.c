#include <stdlib.h>
#include <stdio.h>

#include <rp/rp.h>
#include <rp/rp_common.h>

#define RP 1
#define R 2
#define MEAN 3
#define MEAN_INTER 4

char *prgname = NULL;

int main (int argc, char **argv) {
	uint32 i;
	uint32 num_answers, type;

	float *precision = NULL;
	float mean = 0.0;
	
	ptr_vector_t documents = NULL;
	ptr_hash_t relevants = NULL;
	
	prgname = argv[0];

	if (argc != 5) {
		fprintf(stderr, "%s:\n", prgname);
		fprintf(stderr, "Usage: <documents_file> <relevants_file> <num_answers> <type>\n");
		fprintf(stderr, "type: RecallxPrecision = 1, R_precision = 2, Mean_precision = 3, Mean_inter_precision = 4\n");

		exit(1);
	}
	
	documents = read_documents(argv[1]);
	relevants = read_relevants(argv[2]);
	if (atoi(argv[3]) > 0) {
		num_answers = atoi(argv[3]);
	} else {
		num_answers = documents->size;
	}
	type = atoi(argv[4]);

	switch(type) {
		case MEAN_INTER:
			precision = rp_interpolated_precision(documents, relevants, num_answers);
	//		precision = rp_precision_11(documents, relevants, num_answers);
			for (i=0; i<11; i++) {
				mean += precision[i];
			}
			mean /= 11;
			printf("%.4f\n", mean);
			break;

		case MEAN:
			mean = rp_mean_precision(documents, relevants, num_answers);
			printf("%.4f\n", mean);
			break;
		case R:
			printf("%.4f\n", rp_R_precision(documents, relevants, num_answers));
			break;
		default:
		case RP:
			precision = rp_interpolated_precision(documents, relevants, num_answers);
		//	precision = rp_precision_11(documents, relevants, num_answers);
			for (i=0; i<11; i++) {
				printf("%d %.4f\n", i*10, precision[i]);
			}
			free(precision);
			break;
	}	
	vector_destroy(documents);
	hash_destroy(relevants);
	
	return 0;
}
