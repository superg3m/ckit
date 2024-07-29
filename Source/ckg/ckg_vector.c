#include "ckg_vector.h"
#include "ckg_memory.h"

void* ckg_vector_grow(void* vector, size_t element_size) {
    if (vector == NULLPTR) {
        vector = ckg_alloc(sizeof(CKG_VectorHeader) + (VECTOR_DEFAULT_CAPACITY * element_size));
        vector = (u8*)vector + sizeof(CKG_VectorHeader);
        ckg_vector_capacity(vector) = VECTOR_DEFAULT_CAPACITY;
    }

    u32 count = ckg_vector_count(vector);
    u32 capactiy = ckg_vector_capacity(vector);

    if (capactiy < count + 1) {
        size_t old_allocation_size = sizeof(CKG_VectorHeader) + (capactiy * element_size);
        u32 new_capactiy = capactiy * 2;
        size_t new_allocation_size = sizeof(CKG_VectorHeader) + (new_capactiy * element_size);

        vector = ckg_realloc(ckg_vector_header_base(vector), old_allocation_size, new_allocation_size);
        vector = (u8*)vector + sizeof(CKG_VectorHeader);

        ckg_vector_header_base(vector)->count = count;
        ckg_vector_header_base(vector)->capacity = new_capactiy;
    }

    return vector;
}