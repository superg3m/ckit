#pragma once

#include "ckg_types.h"

//========================== Begin Types ==========================
typedef struct CKG_VectorHeader {
	u32 count;
	u32 capacity;
} CKG_VectorHeader;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    void* ckg_vector_grow(void* vector, size_t element_size);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************


//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
	#define VECTOR_DEFAULT_CAPACITY 1

	#define ckg_vector_header_base(vector) ((CKG_VectorHeader*)(((u8*)vector) - sizeof(CKG_VectorHeader)))
	#define ckg_vector_count(vector) (*ckg_vector_header_base(vector)).count
	#define ckg_vector_capacity(vector) (*ckg_vector_header_base(vector)).capacity
	#define ckg_vector_push(vector, element) vector = ckg_vector_grow(vector, sizeof(element)); vector[ckg_vector_header_base(vector)->count++] = element
	#define ckg_vector_free(vector) ckg_free(ckg_vector_header_base(vector))
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++



