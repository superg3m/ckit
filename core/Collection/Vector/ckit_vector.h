#pragma once

#include "../../ckit_types.h"
#include "../../Assert/ckit_assert.h"
#include "../../Memory/ckit_memory.h"
//========================== Begin Types ==========================
typedef struct CKIT_VectorHeader {
	u32 count;
	u32 capacity;
	char* magic;
} CKIT_VectorHeader;

//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    void* ckit_vector_grow(void* vector, size_t element_size, Boolean force_grow);
    void* MACRO_ckit_vector_reserve(size_t element_size, u32 capacity);
	void* MACRO_ckit_vector_free(void* vector);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

// Date: July 13, 2024
// NOTE(Jovanni): This doesn't have very good checking if the vector is NULLPTR
// Attemptying to get the length/cap/pop of a NULLPTR will result in a uncontrolled crash
//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_vector_base(vector) ((CKIT_VectorHeader*)(((u8*)vector) - sizeof(CKIT_VectorHeader)))
#define ckit_vector_count(vector) (*ckit_vector_base(vector)).count
#define ckit_vector_capacity(vector) (*ckit_vector_base(vector)).capacity
#define ckit_vector_push(vector, element) vector = ckit_vector_grow(vector, sizeof(element), FALSE); vector[ckit_vector_base(vector)->count++] = element
#define ckit_vector_reserve(capactiy, type) (type*)MACRO_ckit_vector_reserve(sizeof(type), capactiy)
#define ckit_vector_pop(vector) vector[--ckit_vector_base(vector)->count]
#define ckit_vector_remove_at(vector, index) ckit_vector_base(vector)->count--; ckit_memory_delete_index(vector, ckit_vector_capacity(vector), index)
#define ckit_vector_insert_at(vector, element, index) ckit_vector_base(vector)->count++; ckit_memory_insert_index(vector, ckit_vector_capacity(vector), element, index)
#define ckit_vector_free(vector) vector = MACRO_ckit_vector_free(vector)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++