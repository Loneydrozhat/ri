#ifndef _RP_H_

#define _RP_H_

#include <util/vector.h>
#include <util/hash.h>

float rp_mean_precision(ptr_vector_t, ptr_hash_t, uint32);
float rp_R_precision(ptr_vector_t, ptr_hash_t, uint32);
float* rp_precision_11(ptr_vector_t, ptr_hash_t, uint32);
float* rp_interpolated_precision(ptr_vector_t, ptr_hash_t, uint32);

#endif
