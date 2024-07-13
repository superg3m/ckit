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
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#ifdef __cplusplus
	#define ckit_free(data) data = (decltype(data))MACRO_ckit_free(data);
#else 
    #define ckit_free(data) data = MACRO_ckit_free(data);
#endif
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++