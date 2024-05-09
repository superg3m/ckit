/*===========================================================
 * File: core_vector.h
 * Date: April 23, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

/*
	Header Based
*/

#pragma once
#include "./core_types.h"
#include "./core_assert.h"


//************************* Begin Functions *************************
void* _vector_create(u64 capacity, u32 type_size_in_bytes);
void _vector_push(void** vector, const void* element);
void* _vector_pop(void** vector);
void _vector_free(void** vector);
u64 vector_size(void* vector);
//************************** End Functions **************************

//++++++++++++++++++++++++++ Begin Macros +++++++++++++++++++++++++++
#define vector_create(type) (type*)_vector_create(0, sizeof(type))
#define vector_reserve(capacity, type) (type*)_vector_create(capacity, sizeof(type))

#define _vector_validate_push_type(vector, element)                                                 \
{                                                                                                   \
    Boolean expression = sizeof(decltype(element)) == sizeof(decltype(vector[0]));                  \
	assert_in_macro(expression, "Type of element being inserted into dynamic array doesn't match"); \
}                                                                                                   \

// TODO(Jovanni): I want to clean this up a bit
// Date: April 23, 2024
#define vector_push(vector, element)                           \
{                                                              \
	_vector_validate_push_type(vector, element);               \
    decltype(element) temp_element = element;				   \
    _vector_push(MUTABLE_VOID_POINTER(vector), &temp_element); \
}                                                              \

#define vector_pop(vector, type) *((type*)_vector_pop((void**)(&vector)));

#define vector_free(vector) _vector_free((void**)(&vector));
//+++++++++++++++++++++++++++ End Macros ++++++++++++++++++++++++++++
