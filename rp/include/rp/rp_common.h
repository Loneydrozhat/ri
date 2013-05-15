#ifndef _RP_COMMON_H_

#define _RP_COMMON_H_

#include <util/vector.h>
#include <util/hash.h>

ptr_hash_t read_relevants(const char*);
ptr_vector_t read_documents(const char*);
float* read_precision(const char*);

#endif
