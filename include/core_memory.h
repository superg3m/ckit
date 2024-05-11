#pragma once
/*===========================================================
 * File: core_memory.h
 * Date: April 30, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_types.h"


//========================== Begin Constants ==========================
#define MEMORY_TAG_CHARACTER_LIMIT 16
typedef enum MemoryTag {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_STRING,
    MEMORY_TAG_VECTOR,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_ENGINE,
    MEMORY_TAG_TEMPORARY,
    MEMORY_TAG_ARENA,
    MEMORY_TAG_INTERNAL,
    MEMORY_TAG_COUNT
} MemoryTag;

static char known_memory_tag_strings[MEMORY_TAG_COUNT][MEMORY_TAG_CHARACTER_LIMIT] = {
    "UNKNOWN      : ",
    "STRING       : ",
    "VECTOR       : ",
    "APPLICATION  : ",
    "ENGINE       : ",
    "TEMPORARY    : ",
    "ARENA        : ",
    "INTERNAL     : ",
};

static u64 global_memory_tags[MEMORY_TAG_COUNT];
//=========================== End Constants ===========================


typedef enum LogLevel LogLevel;
void memory_write_memory_tags(LogLevel log_level);

//************************* Begin Functions *************************
void* memory_allocate(u64 number_of_bytes, MemoryTag memory_tag);
void* memory_reallocate(void* data, u64 new_number_of_bytes);
void* MACRO_memory_free(void* data);

Boolean memory_byte_compare(const void* buffer_one, const void* buffer_two, u32 buffer_one_size, u32 buffer_two_size);
void memory_copy(const void* source, void* destination, u32 source_size, u32 destination_size);
void memory_zero(void* data, u32 data_size_in_bytes);

u8* memory_advance_new_ptr(const void* data, u32 size_in_bytes);
u8* memory_retreat_new_ptr(const void* data, u32 size_in_bytes);
void* MACRO_memory_byte_advance(const void* data, u32 size_in_bytes);
void* MACRO_memory_byte_retreat(const void* data, u32 size_in_bytes);
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define memory_free(data) data = (decltype(data))MACRO_memory_free(data);

#define memory_byte_advance(data, size_in_bytes) data = (decltype(data))MACRO_memory_byte_advance(data, size_in_bytes);
#define memory_byte_retreat(data, size_in_bytes) data = (decltype(data))MACRO_memory_byte_retreat(data, size_in_bytes);
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
