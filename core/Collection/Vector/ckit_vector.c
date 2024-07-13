#include "./ckit_vector.h"
#include "../../Memory/ckit_memory.h"

#define VECTOR_DEFAULT_CAPACITY 1
void* ckit_vector_grow(void* vector, size_t element_size) {
    if (vector == NULLPTR) {
        vector = ckit_alloc(sizeof(CKIT_VectorHeader) + (VECTOR_DEFAULT_CAPACITY * element_size), MEMORY_TAG_VECTOR);
        vector = (u8*)vector + sizeof(CKIT_VectorHeader);
        ckit_vector_base(vector)->capacity = VECTOR_DEFAULT_CAPACITY;
    }

    u32 count = ckit_vector_count(vector);
    u32 capactiy = ckit_vector_capacity(vector);

    if (capactiy < count + 1) {
        size_t old_allocation_size = sizeof(CKIT_VectorHeader) + (capactiy * element_size);
        u32 new_capactiy = capactiy * 2;
        size_t new_allocation_size = sizeof(CKIT_VectorHeader) + (new_capactiy * element_size);

        vector = ckit_realloc(ckit_vector_base(vector), new_allocation_size);
        vector = (u8*)vector + sizeof(CKIT_VectorHeader);

        ckit_vector_base(vector)->count = count;
        ckit_vector_base(vector)->capacity = new_capactiy;
    }

    return vector;
}

void* MACRO_ckit_vector_reserve(size_t element_size, u32 inital_capacity) {
    void* vector = ckit_alloc(sizeof(CKIT_VectorHeader) + (inital_capacity * element_size), MEMORY_TAG_VECTOR);
    vector = (u8*)vector + sizeof(CKIT_VectorHeader);
    ckit_vector_base(vector)->capacity = inital_capacity;

    return vector;
}

void* MACRO_ckit_vector_free(void* vector) {
    CKIT_VectorHeader* vector_base = ckit_vector_base(vector);
    ckit_assert_msg(vector_base, "WHAT"); 
    vector_base->count = 0;
    vector_base->capacity = 0;
    ckit_free(vector_base);

    return vector_base;
}


