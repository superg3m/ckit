#include "../include/core_types.h"

struct DynamicArray {
	u32 size;
	u32 capacity;
	size_t element_size;
	void* data;
};

/**
 * @brief dynamic_array_create() allocates memory you must free the memory with dynamic_array_destroy()
 * 
 * @param capacity 
 * @param element_size 
 * @return DynamicArray* 
 */
DynamicArray _dynamic_array_create(u32 capacity, size_t element_size);
#define dynamic_array_create(type) _dynamic_array_create(0, sizeof(type))
#define dynamic_array_reserve(capacity, type) _dynamic_array_create(capacity, sizeof(type))

/**
 * @brief After using this method you can just reuse the Dynamic array by pushing something on to it
 * however be aware that if you free a dynamic array and then push something onto it. The dynamic array will go through a resize
 * 
 * @param dynamic_array 
 */
void dynamic_array_free(DynamicArray* dynamic_array);

void _dynamic_array_push(DynamicArray* dynamic_array, void* element);
#define dynamic_array_push(dynamic_array, element) _dynamic_array_push(dynamic_array, ((void*)element))


