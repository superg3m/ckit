#include "./ckit_vector.h"
#include "../../Memory/ckit_memory.h"
#include "../../String/ckit_string.h"

#define VECTOR_DEFAULT_CAPACITY 1
#define CKIT_VECTOR_MAGIC "CKIT_MAGIC_VECTOR"

internal void ckit_vector_check_magic(void* vector) {
    ckit_assert_msg(ckit_str_equal(ckit_vector_base(vector)->magic, CKIT_VECTOR_MAGIC), "Vector has wrong magic: {%s} got: {%s} \n", CKIT_VECTOR_MAGIC, ckit_vector_base(vector)->magic);
}

// Date: July 14, 2024
// TODO(Jovanni): HEY I THINK THIS SHOULD BE AN ARRENA ACTUALLY AND ITS LOCALLY SCOPED IN SOME WAY
void* ckit_vector_grow(void* vector, size_t element_size, Boolean force_grow) {
    if (vector == NULLPTR) {
        vector = ckit_alloc(sizeof(CKIT_VectorHeader) + (VECTOR_DEFAULT_CAPACITY * element_size), MEMORY_TAG_VECTOR);
        vector = (u8*)vector + sizeof(CKIT_VectorHeader);
        ckit_vector_base(vector)->count = 0;
        ckit_vector_base(vector)->capacity = VECTOR_DEFAULT_CAPACITY;
        ckit_vector_base(vector)->magic = CKIT_VECTOR_MAGIC;
    }

    ckit_vector_check_magic(vector);
    u32 count = ckit_vector_count(vector);
    u32 capactiy = ckit_vector_capacity(vector);

    if (force_grow || capactiy < count + 1) {
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
    ckit_vector_base(vector)->count = 0;
    ckit_vector_base(vector)->capacity = inital_capacity;
    ckit_vector_base(vector)->magic = CKIT_VECTOR_MAGIC;
    return vector;
}

void* MACRO_ckit_vector_free(void* vector) {
    ckit_vector_check_magic(vector);
    CKIT_VectorHeader* vector_base = ckit_vector_base(vector);
    ckit_assert_msg(vector_base, "WHAT"); 
    vector_base->count = 0;
    vector_base->capacity = 0;
    ckit_free(vector_base);

    return vector_base;
}


