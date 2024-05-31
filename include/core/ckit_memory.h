#pragma once
/*===========================================================
 * File: ckit_memory.h
 * Date: April 30, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "ckit_types.h"
#include "ckit_vector.h"

//========================== Begin Types ==========================
typedef struct Arena Arena;

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

    void* memory_allocate(size_t number_of_bytes, MemoryTag memory_tag);
    void* MACRO_memory_free(void* data);
    void* memory_reallocate(void* data, size_t new_number_of_bytes);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#ifdef __cplusplus
	#define memory_free(data) data = (decltype(data))MACRO_memory_free(data);
#else 
    #define memory_free(data) data = MACRO_memory_free(data);
#endif
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++