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
static inline void _vector_insert_header(VectorHeader header, void** vector) {
    memory_copy(sizeof(header), &header, sizeof(header), *vector);
    memory_byte_advance(sizeof(header), vector);
}

static inline Boolean _vector_validate(void* vector) {
	return vector != NULL;
}

static inline void _vector_update_header(VectorHeader newHeader, void** vector) {
    u8* dest_ptr = memory_retreat_new_ptr(sizeof(newHeader), *vector); 
    memory_copy(sizeof(newHeader), &newHeader, sizeof(newHeader), dest_ptr);
}

static inline VectorHeader _vector_extract_header(const void* vector) {
    VectorHeader header;
    u8* source_ptr = memory_retreat_new_ptr(sizeof(header), vector);
    memory_copy(sizeof(header), source_ptr, sizeof(header), &header);
    return header;
}

void* _vector_create(u64 capacity, u32 type_size_in_bytes) {
	VectorHeader header;
    header.size = 0;
    header.capacity = (capacity == 0) ? VECTOR_DEFAULT_CAPACITY : capacity;
    header.type_size_in_bytes = type_size_in_bytes;

    u32 vector_allocation_size = sizeof(header) + (header.type_size_in_bytes * header.capacity);
    void* ret = memory_allocate(vector_allocation_size, MEMORY_TAG_VECTOR);
    _vector_insert_header(header, &ret);
    return ret;
}

u32 _vector_total_allocation_size(VectorHeader header) {
    return (sizeof(header) + (header.type_size_in_bytes * header.capacity));
}

u32 _vector_new_allocation_size(VectorHeader* header) {\
    header->capacity *= 2;
    return _vector_total_allocation_size(*header);
}

void* _vector_grow(void** vector) {
	VectorHeader header = _vector_extract_header(*vector);
    memory_byte_retreat(sizeof(header), vector);
    void* ret = memory_reallocate(_vector_new_allocation_size(&header), vector);
    _vector_insert_header(header, &ret);
    
    return ret;
}



u64 vector_size(void* vector) {
	assert_in_function(_vector_validate(vector), "The dynamic array size failed (freed before this call!)");
    VectorHeader header = _vector_extract_header(vector);
    return header.size;
}

void _vector_push(void** vector, const void* element) {
	assert_in_function(_vector_validate(*vector), "The vector push failed (freed before this call!)");
    VectorHeader header = _vector_extract_header(*vector);
    if (header.size >= header.capacity) {
        *vector = _vector_grow(vector);
        header = _vector_extract_header(*vector);
    }
	
    u8* dest_ptr = memory_advance_new_ptr(header.size * header.type_size_in_bytes, *vector);
	header.size++;
    memory_copy(header.type_size_in_bytes, element, header.type_size_in_bytes, dest_ptr);
    
    _vector_update_header(header, vector);
}

void* _vector_pop(void** vector) {
	assert_in_function(_vector_validate(*vector), "The vector pop failed (freed before this call!)");
    VectorHeader header = _vector_extract_header(*vector);
	assert_in_function(header.size > 0, "The vector pop failed (no elements to pop!)");
	header.size--;
    _vector_update_header(header, vector);
	return memory_advance_new_ptr(header.size * header.type_size_in_bytes, *vector);
}

void _vector_free(void** vector) {
	assert_in_function(_vector_validate(*vector), "The vector free failed (freed before this call!)");
	VectorHeader header = _vector_extract_header(*vector);
	u32 vector_allocation_size = sizeof(header) + (header.type_size_in_bytes * header.capacity);
    memory_byte_retreat(sizeof(header), vector);
	memory_free(vector);
}
//************************** End Functions **************************