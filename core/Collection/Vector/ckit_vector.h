#pragma once

#include "../../ckit_types.h"
#include "../../Assert/ckit_assert.h"
//========================== Begin Types ==========================
typedef struct CKIT_VectorHeader {
	u32 length;
	u32 capacity;
} CKIT_VectorHeader;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    void* ckit_vector_grow(void* vector, size_t element_size);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
	#define VECTOR_DEFAULT_CAPACITY 1

	#define ckit_vector_header_base(vector) ((CKIT_VectorHeader*)(((u8*)vector) - sizeof(CKIT_VectorHeader)))
	#define ckit_vector_length(vector) *(ckg_vector_header_base(vector)).length
	#define ckit_vector_capacity(vector) *(ckg_vector_header_base(vector)).capacity
	#define ckit_vector_push(vector, element) vector = ckg_vector_grow(vector, sizeof(element)); vector[ckit_vector_header_base(vector)->length++] = element
	#define ckit_vector_pop(vector) vector[ckit_vector_header_base(vector)->length--]
	#define ckit_vector_free(vector) ckit_assert(vector); ckit_free(ckit_vector_header_base(vector))
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++



