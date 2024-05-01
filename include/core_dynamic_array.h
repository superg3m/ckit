/*===========================================================
 * File: core_dynamic_array.h
 * Date: April 23, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#pragma once
#include "./core_types.h"
#include "./core_assert.h"


//************************* Begin Functions *************************
void* _dynamic_array_create(u64 capacity, u32 type_size_in_bytes);
void _dynamic_array_push(void** dynamic_array, void* element);
void* _dynamic_array_pop(void** dynamic_array);
void _dynamic_array_free(void** dynamic_array);
u64 dynamic_array_size(void* dynamic_array);
//************************** End Functions **************************

//++++++++++++++++++++++++++ Begin Macros +++++++++++++++++++++++++++
#define dynamic_array_create(type) (type*)_dynamic_array_create(0, sizeof(type))
#define dynamic_array_reserve(capacity, type) (type*)_dynamic_array_create(capacity, sizeof(type))

#define _dynamic_array_validate_push_type(dynamic_array, element, func, line, file)              \
{                                                                                                \
    Boolean expression = sizeof(decltype(element)) == sizeof(decltype(dynamic_array[0]));        \
	if (isCharArray(element)) {                                                                  \
		expression = sizeof(decltype((char*)element)) == sizeof(decltype(dynamic_array[0]));     \
		assert(expression, "Type of element being inserted into dynamic array doesn't match");   \
	} else {                                                                                     \
		assert(expression, "Type of element being inserted into dynamic array doesn't match");   \
	}																							 \
}                                                                                                \

// TODO(Jovanni): I want to clean this up a bit
// Date: April 23, 2024
#define dynamic_array_push(dynamic_array, element)                                           \
{                                                                                            \
	_dynamic_array_validate_push_type(dynamic_array, element, __func__, __LINE__, __FILE__); \
    decltype(element) temp_element = element;					                             \
    _dynamic_array_push((void**)(&dynamic_array), &temp_element);                            \
}                                                                                            \

#define dynamic_array_pop(dynamic_array, type) *((type*)_dynamic_array_pop((void**)(&dynamic_array)));

#define dynamic_array_free(dynamic_array) _dynamic_array_free((void**)(&dynamic_array));
//+++++++++++++++++++++++++++ End Macros ++++++++++++++++++++++++++++


