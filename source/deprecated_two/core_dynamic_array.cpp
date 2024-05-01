/*===========================================================
 * File: core_dynamic_array.cpp
 * Date: April 23, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../include/deprecated/core_dynamic_array.h"
#include "../include/core_memory.h" // Actually think about this maybe don't expose this to the user?
#include "../include/core_assert.h"

#define DYNAMIC_ARRAY_DEFAULT_CAPACITY 1
//========================== Begin Structs ==========================
struct DynamicArrayHeader {
    u64 size;
    u64 capacity;
    u16 type_size_in_bytes;
};
//=========================== End Structs ===========================

//************************* Begin Functions *************************
static inline void _dynamic_array_insert_header(DynamicArrayHeader header, void** dynamic_array) {
    memory_copy(sizeof(header), &header, sizeof(header), *dynamic_array);
    u8* temp_ptr = ((u8*)(*dynamic_array));
    temp_ptr += sizeof(header);
    *dynamic_array = temp_ptr;
}

static inline Boolean _dynamic_array_validate(void* dynamic_array) {
	return dynamic_array != NULL;
}

static inline void _dynamic_array_update_header(DynamicArrayHeader newHeader, void** dynamic_array) {
    memory_copy(sizeof(newHeader), &newHeader, sizeof(newHeader), ((u8*)(*dynamic_array)) - sizeof(newHeader));
}

static inline DynamicArrayHeader _dynamic_array_extract_header(void* dynamic_array) {
    DynamicArrayHeader header;
    u8* source_ptr = ((u8*)dynamic_array) - sizeof(header);
    memory_copy(sizeof(header), source_ptr, sizeof(header), &header);
    return header;
}

void* _dynamic_array_create(u64 capacity, u32 type_size_in_bytes) {
	DynamicArrayHeader header;
    header.size = 0;
    header.capacity = (capacity == 0) ? DYNAMIC_ARRAY_DEFAULT_CAPACITY : capacity;
    header.type_size_in_bytes = type_size_in_bytes;

    u32 dynamic_array_allocation_size = sizeof(header) + (header.type_size_in_bytes * header.capacity);
    void* ret = memory_allocate(dynamic_array_allocation_size, MEMORY_TAG_DYNAMIC_ARRAY);
    _dynamic_array_insert_header(header, &ret);
    return ret;
}

u64 dynamic_array_size(void* dynamic_array) {
	assert(_dynamic_array_validate(dynamic_array), "The dynamic array size failed (freed before this call!)");
    DynamicArrayHeader header = _dynamic_array_extract_header(dynamic_array);
    return header.size;
}

void _dynamic_array_push(void** dynamic_array, void* element) {
	assert(_dynamic_array_validate(*dynamic_array), "The dynamic array push failed (freed before this call!)");
    DynamicArrayHeader header = _dynamic_array_extract_header(*dynamic_array);
    if (header.size >= header.capacity) {
        u32 old_dynamic_array_allocation_size = sizeof(header) + (header.type_size_in_bytes * header.capacity);
        header.capacity *= 2;
        u32 new_dynamic_array_allocation_size = sizeof(header) + (header.type_size_in_bytes * header.capacity);
        void* new_dynamic_array = _dynamic_array_create(header.capacity, header.type_size_in_bytes);
		_dynamic_array_update_header(header, &new_dynamic_array);
        memory_copy(old_dynamic_array_allocation_size - sizeof(header), *dynamic_array, new_dynamic_array_allocation_size - sizeof(header), new_dynamic_array);
		*((u8*)dynamic_array) -= sizeof(header); // Might be broken
        memory_free(old_dynamic_array_allocation_size, dynamic_array, MEMORY_TAG_DYNAMIC_ARRAY);
        *dynamic_array = new_dynamic_array;
    }
	
    u8* dest_ptr = (((u8*)(*dynamic_array)) + (header.size * header.type_size_in_bytes));
	header.size++;
    memory_copy(header.type_size_in_bytes, element, header.type_size_in_bytes, dest_ptr);
    _dynamic_array_update_header(header, dynamic_array);
}

void* _dynamic_array_pop(void** dynamic_array) {
	assert(_dynamic_array_validate(*dynamic_array), "The dynamic array push failed (freed before this call!)");
    DynamicArrayHeader header = _dynamic_array_extract_header(*dynamic_array);
	assert(header.size > 0, "The dynamic array push failed (no elements to pop!)");
	header.size--;
    _dynamic_array_update_header(header, dynamic_array);
	return dynamic_array[header.size];
}

void _dynamic_array_free(void** dynamic_array) {
	assert(_dynamic_array_validate(*dynamic_array), "The dynamic array free failed (freed before this call!)");
	DynamicArrayHeader header = _dynamic_array_extract_header(*dynamic_array);
	*((u8*)dynamic_array) -= sizeof(header); 
	u32 dynamic_array_allocation_size = sizeof(header) + (header.type_size_in_bytes * header.capacity);
	memory_free(dynamic_array_allocation_size, dynamic_array, MEMORY_TAG_DYNAMIC_ARRAY);
}
//************************** End Functions **************************