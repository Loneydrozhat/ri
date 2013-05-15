#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <util/vector.h>
#include <util/hash.h>
#include <util/int.h>

#include <rp/rp_common.h>

ptr_hash_t read_relevants(const char *file_name) {
	char *line;
	uint32 dummy = 100;
	ptr_hash_t relevants = hash_create(100, 0.75, int_compare, (_HASH_CODE*)int_hash_code, NULL, NULL, NULL, NULL);
	FILE *file = fopen(file_name, "r");

	if (file == NULL) error(E_FREAD, file_name);
	
	while ((line = read_line(file)) != NULL && !feof(file)) {
		hash_put(relevants, (void*)atoi(line), (void*)dummy);
		free(line);
	}
	
	if (line != NULL) {
		hash_put(relevants, (void*)atoi(line), (void*)dummy);
		free(line);
	}
	
	fclose(file);

	return relevants;
}

ptr_vector_t read_documents(const char *file_name) {
	char *line;
	ptr_vector_t documents = vector_create(100, int_compare, (_HASH_CODE*)int_hash_code, NULL, NULL);
	FILE *file = fopen(file_name, "r");

	if (file == NULL) error(E_FREAD, file_name);

	while ((line = read_line(file)) != NULL && !feof(file)) {
		vector_add(documents, (void*)atoi(line));
		free(line);
	}
	
	if (line != NULL) {
		vector_add(documents, (void*)atoi(line));
		free(line);
	}
	
	fclose(file);

	return documents;
}

float* read_precision(const char *file_name) {
	FILE *file = fopen(file_name, "r");
	float *precision = NULL;
	char *line, *original_line;
	uint32 index = 0;
	
	precision = (float*)calloc(11, sizeof(float));
	if (precision == NULL) error(E_NOMEM);
	
	while ((original_line = line = read_line(file)) != NULL && !feof(file) && index < 11) {
		// discarting the recall value
		strsep(&(line), " ");
		
		// reading the precision value
		precision[index++] = atof(strsep(&(line), " "));
		
		free(original_line);
	}
	
	if (original_line != NULL) {
		if (index < 11) {
			// discarting the recall value
			strsep(&(line), " ");
		
			// reading the precision value
			precision[index++] = atof(strsep(&(line), " "));
		}
		free(original_line);
	} 
		
	fclose(file);

	return precision;
}
