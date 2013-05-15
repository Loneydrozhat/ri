#include <stdlib.h>
#include <stdio.h>

#include <rp/rp_common.h>

char *prgname = NULL;

int main (int argc, char **argv) {
	uint32 i, j, size;
	float *joined_precision = NULL;
	
	prgname = argv[0];
	if (argc  == 1) {
		fprintf(stderr, "%s:\n", prgname);
		fprintf(stderr, "Usage: <rp_file>+\n");
		exit(1);
	}

	joined_precision = (float*)calloc(11, sizeof(float));
	if (joined_precision == NULL) error(E_NOMEM);

	size = argc -1;
	for (i=1; i<argc; i++) {
			float *precision = read_precision(argv[i]);
			for (j=0; j<11; j++) {
				joined_precision[j] += precision[j]/size;
			}
			free(precision);
	}
	
	for (i=0; i<11; i++) {
		printf("%d %.4f\n", i*10, joined_precision[i]);
	}
	
	free(joined_precision);
	
	return 0;
}
