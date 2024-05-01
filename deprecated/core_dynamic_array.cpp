#include "deprecated/core_dynamic_array.h"
// Actually thinkj about this maybe don't expose this to the user?
#include "../include/core_memory.h"


#define DYNAMIC_ARRAY_DEFAULT_CAPACITY 1

DynamicArray _dynamic_array_create(u32 capacity, size_t element_size) {
	DynamicArray dynamic_array;
	memory_zero(sizeof(dynamic_array), &dynamic_array);
    dynamic_array.size = 0;
    dynamic_array.capacity = (capacity == 0) ? DYNAMIC_ARRAY_DEFAULT_CAPACITY : capacity;
    dynamic_array.element_size = element_size;
	size_t dynamic_array_data_size = dynamic_array.element_size * dynamic_array.capacity;
	dynamic_array.data = memory_allocate(dynamic_array_data_size, MEMORY_TAG_DYNAMIC_ARRAY);
	memory_zero(dynamic_array_data_size, dynamic_array.data);
    return dynamic_array;
}

void dynamic_array_free(DynamicArray* dynamic_array) {
	size_t dynamic_array_data_size = dynamic_array->element_size * dynamic_array->capacity;
	memory_free(dynamic_array_data_size, (void**)&dynamic_array->data, MEMORY_TAG_DYNAMIC_ARRAY);
	memory_free(sizeof(dynamic_array), (void**)&dynamic_array, MEMORY_TAG_DYNAMIC_ARRAY);
	dynamic_array->size = 1;
	dynamic_array-> capacity = DYNAMIC_ARRAY_DEFAULT_CAPACITY;
}

u32 _resize(DynamicArray* dynamic_array) {
	size_t old_dynamic_array_data_size = dynamic_array->element_size * dynamic_array->capacity;
    dynamic_array->capacity *= 2;
	size_t new_dynamic_array_data_size = dynamic_array->element_size * dynamic_array->capacity;
	void* newBuffer = memory_allocate(new_dynamic_array_data_size, MEMORY_TAG_DYNAMIC_ARRAY);
	memory_copy(old_dynamic_array_data_size, dynamic_array->data, new_dynamic_array_data_size, newBuffer);
	memory_free(old_dynamic_array_data_size, &dynamic_array->data, MEMORY_TAG_DYNAMIC_ARRAY);
	dynamic_array->data = newBuffer;
}

void _dynamic_array_push(DynamicArray* dynamic_array, void* element) {
    if (dynamic_array->size >= dynamic_array->capacity) {
        _resize(dynamic_array);
    }
	
    u8* dest_ptr = ((u8*)dynamic_array->data) + (dynamic_array->size * dynamic_array->element_size);
	dynamic_array->size++;
    memory_copy(dynamic_array->element_size, element, dynamic_array->element_size, dest_ptr);
}
