#pragma once
/*===========================================================
 * File: ckit_vector.h
 * Date: April 23, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

/*
	Header Based
*/

#include "./ckit_types.h"
#include "./ckit_assert.h"

typedef struct VectorHeader VectorHeader;

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	void* MACRO_vector_create(u32 size, u64 capacity, u32 type_size_in_bytes);
	void* MACRO_vector_push(void* vector, const void* element);
	void* MACRO_vector_push_ptr(void* vector, const void* element);
	void* MACRO_vector_pop(void* vector);
	void* MACRO_vector_free(void* vector);
	u64 vector_size(void* vector);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//++++++++++++++++++++++++++ Begin Macros +++++++++++++++++++++++++++
#define vector_create(type) (type*)MACRO_vector_create(0, 0, sizeof(type))

#define vector_reserve(capacity, type) (type*)MACRO_vector_create(0, capacity, sizeof(type))

#ifdef __cplusplus
	#define _vector_validate_push_type(vector, element)                                               \
	{                                                                                                 \
		Boolean expression = sizeof(decltype(element)) == sizeof(decltype(vector[0]));                  \
		assert_in_macro(expression, "Type of element being inserted into dynamic array doesn't match"); \
	}

	#define vector_push(vector, element)                                                  \
	{                                                                                     \
		_vector_validate_push_type(vector, element);                                      \
		decltype(element) temp_element = element;                                         \
		vector = (decltype(vector))MACRO_vector_push(vector, (const void*)&temp_element); \
	}
	#define vector_pop(vector, type) *((type*)MACRO_vector_pop(vector))
	#define vector_free(vector) vector = (decltype(vector))MACRO_vector_free(vector)
#else 
	// Date: May 11, 2024
	// NOTE(Jovanni): Its painful but no literals allowed in c (MSVC)
	#define vector_push(vector, element)                           		  \
	{                                                              		  \
		vector = MACRO_vector_push(vector, (const void*)&element); \
	}

	#define vector_pop(vector, type) *((type*)MACRO_vector_pop(vector))
	#define vector_free(vector) vector = MACRO_vector_free(vector)
#endif




//+++++++++++++++++++++++++++ End Macros ++++++++++++++++++++++++++++