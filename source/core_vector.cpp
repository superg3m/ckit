/*===========================================================
 * File: core_vector.cpp
 * Date: April 23, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../include/core_vector.h"
#include "../include/core_memory.h" // Actually think about this maybe don't expose this to the user?
#include "../include/core_assert.h"

#define VECTOR_DEFAULT_CAPACITY 1
//========================== Begin Structs ==========================
typedef struct VectorHeader {
    u64 size;
    u64 capacity;
    u16 type_size_in_bytes;
} VectorHeader;
//=========================== End Structs ===========================

//************************* Begin Functions *************************
internal void* MACRO_vector_insert_header(void* vector, VectorHeader header) {
    ((VectorHeader*)vector)[0] = header;
    memory_byte_advance(vector, sizeof(header));
    return vector;
}
#define _vector_insert_header(vector, header) vector = MACRO_vector_insert_header(vector, header);

internal inline Boolean _vector_validate(void* vector) {
	return vector != NULL;
}

internal inline VectorHeader* _vector_extract_header(const void* vector) {
    return &((VectorHeader*)vector)[-1];
}

void* MACRO_vector_create(u64 capacity, u32 type_size_in_bytes) {
	VectorHeader header;
    header.size = 0;
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

internal void* _vector_grow(void* vector) {
    // Date: May 11, 2024
    // NOTE(Jovanni): Need to deference because the memory location will freed not after reallocation
	VectorHeader header = *_vector_extract_header(vector);
    header.capacity *= 2;
    memory_byte_retreat(vector, sizeof(header));
    void* ret = memory_reallocate(vector, _vector_total_allocation_size(header));
    _vector_insert_header(ret, header);
    
    return ret;
}

u64 vector_size(void* vector) {
	assert_in_function(_vector_validate(vector), "The dynamic array size failed (freed before this call!)\n");
    VectorHeader* header = _vector_extract_header(vector);
    return header->size;
}

void* MACRO_vector_push(void* vector, const void* element) {
	assert_in_function(_vector_validate(vector), "The vector push failed (freed before this call!)");
    VectorHeader* header = _vector_extract_header(vector);
    if (header->size >= header->capacity) {
        vector = _vector_grow(vector);
        header = _vector_extract_header(vector);
    }
	
    u8* dest_ptr = memory_advance_new_ptr(vector, header->size * header->type_size_in_bytes);
	header->size++;
    memory_copy(element, dest_ptr, header->type_size_in_bytes, header->type_size_in_bytes);
    
    return vector;
}

void* MACRO_vector_pop(void* vector) {
	assert_in_function(_vector_validate(vector), "The vector pop failed (freed before this call!)\n");
    VectorHeader* header = _vector_extract_header(vector);
	assert_in_function(header->size > 0, "The vector pop failed (no elements to pop!)\n");
	header->size--;
	return memory_advance_new_ptr(vector, header->size * header->type_size_in_bytes);
}

void* MACRO_vector_free(void* vector) {
	assert_in_macro(_vector_validate(vector), "The vector free failed (freed before this call!)\n");
	VectorHeader header = *_vector_extract_header(vector);
	u32 vector_allocation_size = sizeof(header) + (header.type_size_in_bytes * header.capacity);
    memory_byte_retreat(vector, sizeof(header));
	memory_free(vector);

    return vector;
}
//************************** End Functions **************************