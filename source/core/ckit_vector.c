/*===========================================================
 * File: ckit_vector.cpp
 * Date: April 23, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../../include/core/ckit_vector.h"
#include "../../include/core/ckit_memory.h" // Actually think about this maybe don't expose this to the user?
#include "../../include/core/ckit_assert.h"

// Look up stetch buffers because it seems like I did a lot more work than I should have
/* 
typedef struct VectorHeader { // This  is all I should need somehow?
    u64 size;
    u64 capacity;
    u8 buffer[0]; // Apparently this is possible and is good?
} VectorHeader;
*/



#define VECTOR_DEFAULT_CAPACITY 1
//========================== Begin Types ==========================
typedef struct VectorHeader {
    u64 size;
    u64 capacity;
    size_t type_size_in_bytes;
} VectorHeader;
//=========================== End Types ===========================

//************************* Begin Functions *************************
internal void* MACRO_vector_insert_header(void* vector, VectorHeader header) {
    ((VectorHeader*)vector)[0] = header;
    vector = (u8*)vector + sizeof(header);
    return vector;
}
#define _vector_insert_header(vector, header) vector = MACRO_vector_insert_header(vector, header);

internal inline VectorHeader* _vector_extract_header(const void* vector) {
    return &((VectorHeader*)vector)[-1];
}

void* MACRO_vector_create(u32 size, u64 capacity, size_t type_size_in_bytes) {
	VectorHeader header;
    header.size = size;
    header.capacity = (capacity == 0) ? VECTOR_DEFAULT_CAPACITY : capacity;
    header.type_size_in_bytes = type_size_in_bytes;

    u32 vector_allocation_size = sizeof(header) + (header.type_size_in_bytes * header.capacity);
    void* ret = memory_allocate(vector_allocation_size, MEMORY_TAG_VECTOR);
    assert_in_function(ret, "vector_create: failed to allocate - %d bytes", vector_allocation_size);
    _vector_insert_header(ret, header);
    return ret;
}

internal u32 _vector_total_allocation_size(VectorHeader header) {
    return (sizeof(header) + (header.type_size_in_bytes * header.capacity));
}

void* vector_grow(void* vector) {
    // Date: May 11, 2024
    // NOTE(Jovanni): Need to deference because the header memory location will be freed after reallocation
	VectorHeader header = *_vector_extract_header(vector);
    u32 old_allocation_size = header.capacity * header.type_size_in_bytes;
    header.capacity *= 2;
    u32 new_allocation_size = header.capacity * header.type_size_in_bytes;

    void* ret = MACRO_vector_create(header.size, header.capacity, header.type_size_in_bytes);
    ckg_memory_copy(vector, ret, old_allocation_size, new_allocation_size);
    vector_free(vector);

    return ret;
}

u64 vector_size(void* vector) {
	assert_in_function(vector, "The dynamic array size failed (freed before this call!)\n");
    VectorHeader* header = _vector_extract_header(vector);
    return header->size;
}

u64 vector_capacity(void* vector) {
	assert_in_function(vector, "The dynamic array size failed (freed before this call!)\n");
    VectorHeader* header = _vector_extract_header(vector);
    return header->capacity;
}

void* MACRO_vector_push(void* vector, const void* element) {
	assert_in_function(vector, "The vector push failed (freed before this call!)");
    VectorHeader* header = _vector_extract_header(vector);
    if (header->size >= header->capacity) {
        vector = vector_grow(vector);
        header = _vector_extract_header(vector);
    }
	
    u8* dest_ptr = (u8*)vector + (header->size * header->type_size_in_bytes);
	header->size++;
    ckg_memory_copy(element, dest_ptr, header->type_size_in_bytes, header->type_size_in_bytes);
    
    return vector;
}

void* MACRO_vector_pop(void* vector) {
	assert_in_function(vector, "The vector pop failed (freed before this call!)\n");
    VectorHeader* header = _vector_extract_header(vector);
	assert_in_function(header->size > 0, "The vector pop failed (no elements to pop!)\n");
	header->size--;
	return (u8*)vector + (header->size * header->type_size_in_bytes);
}

void* MACRO_vector_free(void* vector) {
	assert_in_macro(vector, "The vector free failed (freed before this call!)\n");
	VectorHeader header = *_vector_extract_header(vector);
	u32 vector_allocation_size = sizeof(header) + (header.type_size_in_bytes * header.capacity);
    vector = (u8*)vector - sizeof(header);
	memory_free(vector);

    return vector;
}
//************************** End Functions **************************