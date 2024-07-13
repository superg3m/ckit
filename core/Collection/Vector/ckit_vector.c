#include "./ckit_vector.h"

#include "../../Memory/ckit_memory.h"
void* ckit_vector_grow(void* vector, size_t element_size) {
    if (vector == NULLPTR) {
        vector = ckit_alloc(sizeof(CKIT_VectorHeader) + (VECTOR_DEFAULT_CAPACITY * element_size));
        vector = (u8*)vector + sizeof(CKIT_VectorHeader);
        ckit_vector_header_base(vector)->capacity = VECTOR_DEFAULT_CAPACITY;
    }

    u32 length = ckg_vector_length(vector);
    u32 capactiy = ckg_vector_capacity(vector);

    if (capactiy < length + 1) {
        size_t old_allocation_size = sizeof(CKIT_VectorHeader) + (capactiy * element_size);
        u32 new_capactiy = capactiy * 2;
        size_t new_allocation_size = sizeof(CKIT_VectorHeader) + (new_capactiy * element_size);

        vector = ckit_realloc(ckg_vector_header_base(vector), old_allocation_size, new_allocation_size);
        vector = (u8*)vector + sizeof(CKIT_VectorHeader);

        ckit_vector_header_base(vector)->length = length;
        ckit_vector_header_base(vector)->capacity = new_capactiy;
    }

    return vector;
}