#pragma once

#include "ckg_types.h"

//========================== Begin Types ==========================
typedef void* (CKG_MemoryAllocator)(size_t);
typedef void (CKG_MemoryFree)(void*);
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Note that the callback provided should zero out the memory allocation.
     * Failing to bind the allocation callback will result in a default allocation callback.
     */
    void ckg_bind_alloc_callback(CKG_MemoryAllocator* func_allocator);

    /**
     * @brief Failing to bind the callback will result in a default free callback.
     */
    void ckg_bind_free_callback(CKG_MemoryFree* func_allocator);

    void* MACRO_ckg_alloc(size_t allocation_size);
    void* ckg_realloc(void* data, size_t old_allocation_size, size_t new_allocation_size);
    void* MACRO_ckg_free(void* data);

    /**
     * @brief Compares the bytes in the two buffers
     * 
     * @param buffer_one 
     * @param buffer_two 
     * @param b1_allocation_size 
     * @param b2_allocation_size 
     * @return Boolean 
     */
    Boolean ckg_memory_compare(const void* buffer_one, const void* buffer_two, u32 b1_allocation_size, u32 b2_allocation_size);
    void ckg_memory_copy(const void* source, void* destination, size_t source_size_in_bytes, size_t destination_capacity_in_bytes);
    void ckg_memory_zero(void* data, size_t data_size_in_bytes);

    void MACRO_ckg_memory_delete_index(void* data, u32 data_capacity, size_t element_size_in_bytes, u32 index);
    void MACRO_ckg_memory_insert_index(void* data, u32 data_capacity, size_t element_size_in_bytes, u32 index);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

#define ckg_memory_fill(buffer, buffer_count, fill_element) \
{														\
    for (int i = 0; i < buffer_count; i++) { 			\
        buffer[i] = fill_element;                       \
    }                                                  	\
}

#ifdef __cplusplus
    #define ckg_alloc(allocation_size) (decltype(data))MACRO_ckg_alloc(allocation_size)
    #define ckg_free(data) data = (decltype(data))MACRO_ckg_free(data)
    #define ckg_memory_delete_index(data, data_capacity, index) MACRO_ckg_memory_delete_index(data, data_capacity, sizeof(data[0]), index)
    #define ckg_memory_insert_index(data, data_capacity, index, element) MACRO_ckg_memory_insert_index(data, data_capacity,  element, sizeof(data[0]), index)
#else
    #define ckg_alloc(allocation_size) MACRO_ckg_alloc(allocation_size)
    #define ckg_free(data) data = MACRO_ckg_free(data)
    #define ckg_memory_delete_index(data, data_capacity, index) MACRO_ckg_memory_delete_index(data, data_capacity, sizeof(data[0]), index)
    #define ckg_memory_insert_index(data, data_capacity, element, index) MACRO_ckg_memory_insert_index(data, data_capacity, sizeof(data[0]), index); data[index] = element;
#endif