#pragma once
/*===========================================================
 * File: core_memory.h
 * Date: April 30, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_types.h"

enum MemoryTag {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_STRING,
    MEMORY_TAG_VECTOR,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_ENGINE,
    MEMORY_TAG_TEMPORARY,
    MEMORY_TAG_COUNT
};

#define MEMORY_TAG_CHARACTER_LIMIT 16
static char known_memory_tag_strings[MEMORY_TAG_COUNT][MEMORY_TAG_CHARACTER_LIMIT] = {
    "UNKNOWN      : ",
    "STRING       : ",
    "DYNAMIC_ARRAY: ",
    "APPLICATION  : ",
    "ENGINE       : ",
    "TEMPORARY    : ",
};

static u64 global_memory_tags[MEMORY_TAG_COUNT];

void* memory_allocate(u64 number_of_bytes, MemoryTag memory_tag);
void* memory_reallocate(u64 new_number_of_bytes, void** data);
void memory_free(void** data);

Boolean memory_byte_compare(u32 buffer_one_size, const void* buffer_one, u32 buffer_two_size, const void* buffer_two);
void memory_copy(u32 source_size, const void* source, u32 destination_size, void* destination);
//void memory_set(u32 data_size_in_bytes, void* data, const void* element);
void memory_zero(u32 data_size_in_bytes, void* data);


void memory_byte_advance(u32 size_in_bytes, void** data);
void memory_byte_retreat(u32 size_in_bytes, void** data);
u8* memory_advance_new_ptr(u32 size_in_bytes, const void* data);
u8* memory_retreat_new_ptr(u32 size_in_bytes, const void* data);

enum LogLevel;
void console_write_memory_tags(LogLevel log_level);

// Memory Arena
