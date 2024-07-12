/*===========================================================
 * File: ckit_memory.c
 * Date: May 03, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../../ckg/Core/Memory/ckg_memory.h"

#include "./ckit_memory.h"
#include "../Assert/ckit_assert.h"
#include "../Logger/ckit_logger.h"
#include "../Platform/ckit_platform_services.h"
//========================== Begin Types ==========================

// Date: May 25, 2024
// TODO(Jovanni): Figure out a better way to do headers and pointer math

typedef struct MemoryHeader {
	u32 allocation_size_without_header;
    MemoryTag memory_tag;
} MemoryHeader;

typedef enum LogLevel LogLevel;
typedef struct Arena Arena;

internal u64 memory_used = 0; 
internal u64 global_memory_tags[MEMORY_TAG_COUNT];
char known_memory_tag_strings[MEMORY_TAG_COUNT][MEMORY_TAG_CHARACTER_LIMIT] = {
    "UNKNOWN      : ",
    "TEMPORARY    : ",
    "INTERNAL     : ",
    "STRING       : ",
    "VECTOR       : ",
    "ARENA        : ",
};
//=========================== End Types ===========================

//************************* Begin Functions *************************
internal void _memory_track_add(MemoryHeader header, MemoryTag memory_tag) {
  	global_memory_tags[MEMORY_TAG_INTERNAL] += sizeof(header);
  	global_memory_tags[memory_tag] += (header.allocation_size_without_header);
  	memory_used += sizeof(header) + header.allocation_size_without_header;
}

internal void _memory_track_remove(MemoryHeader header, MemoryTag memory_tag) {
  	global_memory_tags[MEMORY_TAG_INTERNAL] -= sizeof(header);
  	global_memory_tags[memory_tag] -= (header.allocation_size_without_header);
  	memory_used -= sizeof(header) + header.allocation_size_without_header;
}

internal void* MACRO_memory_insert_header(void* data, MemoryHeader header) {
  	((MemoryHeader*)data)[0] = header;
  	data = (u8*)data + sizeof(header);
  	return data;
}
#define _memory_insert_header(data, header) data = MACRO_memory_insert_header(data, header);

Boolean memory_tag_is_valid(MemoryTag memory_tag) {
  	return (memory_tag >= 0 && memory_tag < MEMORY_TAG_COUNT);
}

internal MemoryHeader* _memory_extract_header(void* data) {
	return &((MemoryHeader*)data)[-1];
}

void memory_init() {
	ckg_bind_alloc_callback(&platform_allocate);
	ckg_bind_free_callback(&platform_free);
}

void* memory_allocate(size_t byte_allocation_size, MemoryTag memory_tag) {
	ckit_assert_msg(byte_allocation_size > 0, "Invalid allocation size zero or below\n");
	ckit_assert_msg(memory_tag_is_valid(memory_tag), "memory_allocate: Memory tag is invalid | value: (%d)\n", memory_tag);
	if (memory_tag == MEMORY_TAG_UNKNOWN) {
		LOG_WARN("memory_allocate: memory tag unknown\n");
	}

	MemoryHeader header;
	header.allocation_size_without_header = byte_allocation_size;
	header.memory_tag = memory_tag;

	_memory_track_add(header, memory_tag);

	u32 total_allocation_size = sizeof(header) + header.allocation_size_without_header;

	void* data = ckg_alloc(total_allocation_size);
	// Date: May 09, 2024
	// NOTE(Jovanni): Technically you are repeating work here
	ckg_memory_zero(data, total_allocation_size);
	_memory_insert_header(data, header);

	return data;
}

void* MACRO_memory_free(void* data) {
  	ckit_assert_msg(data, "memory_free: Data passed is null in free\n");
  	const MemoryHeader header = *_memory_extract_header(data);
  	ckit_assert_msg(memory_tag_is_valid(header.memory_tag), "memory_free: memory_tag is not valid\n");

  	_memory_track_remove(header, header.memory_tag);

  	data = (u8*)data - sizeof(header);
	ckg_memory_zero(data, sizeof(header) + header.allocation_size_without_header);
  	ckg_free(data);
  	return data;
}

void* memory_reallocate(void* data, u64 new_byte_allocation_size) {
  	LOG_DEBUG("Reallocation Triggered!\n");
  	ckit_assert_msg(data, "memory_reallocation: Data passed is null\n");

  	MemoryHeader header = *_memory_extract_header(data);

	u32 old_total_allocation_size = sizeof(header) + header.allocation_size_without_header;
  	u32 new_total_allocation_size = sizeof(header) + new_byte_allocation_size;

	header.allocation_size_without_header = new_byte_allocation_size;

  	void* ret_data = memory_allocate(new_total_allocation_size, header.memory_tag);

  	ckg_memory_copy(data, ret_data, old_total_allocation_size - sizeof(header), new_total_allocation_size - sizeof(header));
  	memory_free(data);

  	return ret_data;
}

void memory_output_allocations(LogLevel log_level) {
    if (memory_used == 0) {
        log_output(log_level, "No memory allocations!\n");
        return;
    }
    char out_message[PLATFORM_CHARACTER_LIMIT];
    char out_message2[PLATFORM_CHARACTER_LIMIT];
    char out_message3[PLATFORM_CHARACTER_LIMIT];
    
    log_output(log_level, "========================\n");
    for (int level = 0; level < MEMORY_TAG_COUNT; level++) {
     	if (global_memory_tags[level] == 0) {
    		continue;
     	} 

     	ckg_memory_zero(out_message, sizeof(out_message));
     	ckg_memory_zero(out_message2, sizeof(out_message2));
     	ckg_memory_zero(out_message3, sizeof(out_message3));

     	sprintf(out_message, "%s", known_memory_tag_strings[level]);
     	sprintf(out_message2, "%lld", global_memory_tags[level]);
     	sprintf(out_message3, "%s%s", out_message, out_message2);
     	log_output(log_level, "%s\n", out_message3);
    }
    log_output(log_level, "========================\n");
}
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++











