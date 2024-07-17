#pragma once
/*===========================================================
 * File: ckit_memory.h
 * Date: April 30, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../ckit_types.h"
#include "../Logger/ckit_logger.h"
//========================== Begin Types ==========================

typedef enum MemoryTag {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_TEMPORARY,
    MEMORY_TAG_INTERNAL,
    MEMORY_TAG_STRING,
    MEMORY_TAG_VECTOR,
    MEMORY_TAG_ARENA,
    MEMORY_TAG_COUNT
} MemoryTag;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    void memory_init();

    void* ckit_alloc(size_t number_of_bytes, MemoryTag memory_tag);
    void* MACRO_ckit_free(void* data);
    void* ckit_realloc(void* data, size_t new_number_of_bytes);
    void memory_output_allocations(CKG_LogLevel log_level);

    Boolean ckit_memory_compare(const void* buffer_one, const void* buffer_two, u32 b1_allocation_size, u32 b2_allocation_size);
    void ckit_memory_copy(const void* source, void* destination, size_t source_size, size_t destination_capacity);
    void ckit_memory_move(const void* source, void* destination, size_t source_payload_size);
    void ckit_memory_zero(void* data, size_t data_size_in_bytes);

    void MACRO_ckit_memory_delete_index(void* data, u32 data_capacity, size_t element_size_in_bytes, u32 index);
    void MACRO_ckit_memory_insert_index(void* data, u32 data_capacity, size_t element_size_in_bytes, u32 index);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#ifdef __cplusplus
	#define ckit_free(data) data = (decltype(data))MACRO_ckit_free(data);
    #define ckit_memory_delete_index(data, data_capacity, index) MACRO_ckit_memory_delete_index(data, data_capacity, sizeof(data[0]), index)
    #define ckit_memory_insert_index(data, data_capacity, element, index) MACRO_ckit_memory_delete_index(data, data_capacity, sizeof(data[0]), index); data[index] = element;
#else 
    #define ckit_free(data) data = MACRO_ckit_free(data);
    #define ckit_memory_delete_index(data, data_capacity, index) MACRO_ckit_memory_delete_index(data, data_capacity, sizeof(data[0]), index)
    #define ckit_memory_insert_index(data, data_capacity, element, index) MACRO_ckit_memory_delete_index(data, data_capacity, sizeof(data[0]), index); data[index] = element;
#endif
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++