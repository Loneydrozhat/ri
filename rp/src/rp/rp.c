#include <stdlib.h>
#include <math.h>

#include <rp/rp.h>

float rp_mean_precision(ptr_vector_t documents, ptr_hash_t relevants, uint32 num_answers) {
	uint32 i;
	uint32 total_relevants = 0;
	uint32 size = MIN(documents->size,num_answers);
	float precision = 0.0;
		
	// calculating the mean precision 
	for (i=0; i<size; i++) {
		uint32 doc_id = (uint32)documents->elements[i];
		if (hash_get(relevants, (void*)doc_id) != NULL) {
			total_relevants++;
			precision += ((float)total_relevants/(i+1))*100;
		}
	}

	return precision/total_relevants;
}

float rp_R_precision(ptr_vector_t documents, ptr_hash_t relevants, uint32 R) {
	uint32 i;
	uint32 total_relevants = 0;
	
	if (R > documents->size) {
		R = documents->size;
//		error(E_OPTARG, "rp_R_precision: invalid R argument");
	}

	// calculating the R-precision
	for (i=0; i<R; i++) {
		uint32 doc_id = (uint32)documents->elements[i];
		if (hash_get(relevants, (void*)doc_id) != NULL) {
			total_relevants++;
		}
	}

	return ((float)total_relevants/R)*100;
}

float* rp_precision_11(ptr_vector_t documents, ptr_hash_t relevants, uint32 num_answers) {
	uint32 i;
	uint32 total_relevants = 0;
	uint32 recall = 0, curr_recall = 0;
	uint32 size = MIN(documents->size,num_answers);
	float *precision = NULL;
	
	precision = (float*)malloc(11*sizeof(float));
	if (precision == NULL) error(E_NOMEM);
	
	// calculating the precision and recall values
	for (i=0; i<size; i++) {
		uint32 doc_id = (uint32)documents->elements[i];
		if (hash_get(relevants, (void*)doc_id) != NULL) {
			total_relevants++;
			curr_recall = rint(((float)total_relevants/relevants->size)*10);
			while (recall <= curr_recall) {
				precision[recall++] = ((float)total_relevants/(i+1))*100;
			}
		}
	}
	
	while (recall <=10) {
		precision[recall++] = 0.0;
	}

	return precision;
}

float* rp_interpolated_precision(ptr_vector_t documents, ptr_hash_t relevants, uint32 num_answers) {
	int32 i, index = 0;
	uint32 total_relevants = 0;
	uint32 size = MIN(documents->size,num_answers);
	float *recall = NULL;
	float *precision = NULL;
	float *inter_precision = NULL;
	
	recall = (float*)malloc(documents->size*sizeof(float));
	if (recall == NULL) error(E_NOMEM);
	
	precision = (float*)malloc(documents->size*sizeof(float));
	if (precision == NULL) error(E_NOMEM);
	
	// calculating the precision and recall values
	for (i=0; i<size; i++) {
		uint32 doc_id = (uint32)documents->elements[i];
		if (hash_get(relevants, (void*)doc_id) != NULL) {
			total_relevants++;
			recall[total_relevants-1] = ((float)total_relevants/relevants->size)*100;
			precision[total_relevants-1] = ((float)total_relevants/(i+1))*100;
		}
	}

	inter_precision = (float*)calloc(11, sizeof(float));
	if (inter_precision == NULL) error(E_NOMEM);
	
	// finding the 11-point interpoleted precision
	index = total_relevants - 1;
	if (recall[index] >= 90) {
		inter_precision[10] = precision[index];
	} else {
		inter_precision[10] = 0;
	}
	
	for (i=9; i>=0; i--) {
		uint32 curr_recall = i*10;
		float inter_prec = 0.0;
		
		while (curr_recall <= recall[index] && index >= 0) {
			if (inter_prec < precision[index]) {
				inter_prec = precision[index];
			}
			index--;
		}
		
		if (inter_prec < inter_precision[i+1]) {
			inter_prec = inter_precision[i+1];
		}

		inter_precision[i] = inter_prec;
	}
	
	free(recall);
	free(precision);
	
	return inter_precision;
}

