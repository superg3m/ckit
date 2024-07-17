/*===========================================================
 * File: ckit_memory.c
 * Date: May 03, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../../ckg/Core/Memory/ckg_memory.h"

#include "./ckit_memory.h"
#include "./ckit_arena.h"
#include "../Assert/ckit_assert.h"
#include "../Logger/ckit_logger.h"
#include "../Collection/Vector/ckit_vector.h"
#include "../Platform/ckit_platform_services.h"
//========================== Begin Types ==========================
typedef enum CKG_LogLevel CKG_LogLevel;
typedef struct CKIT_Arena CKIT_Arena;

typedef struct MemoryHeader {
	u32 allocation_size_without_header;
    MemoryTag memory_tag;
} MemoryHeader;


internal u64 memory_used = 0; 
internal u64 global_memory_tags[MEMORY_TAG_COUNT];
internal CKIT_Arena** registered_arenas = NULLPTR;
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
internal void memory_track_add(MemoryHeader header, MemoryTag memory_tag) {
  	global_memory_tags[MEMORY_TAG_INTERNAL] += sizeof(header);
  	global_memory_tags[memory_tag] += (header.allocation_size_without_header);
  	memory_used += sizeof(header) + header.allocation_size_without_header;
}

internal void memory_track_remove(MemoryHeader header, MemoryTag memory_tag) {
  	global_memory_tags[MEMORY_TAG_INTERNAL] -= sizeof(header);
  	global_memory_tags[memory_tag] -= (header.allocation_size_without_header);
  	memory_used -= sizeof(header) + header.allocation_size_without_header;
}

internal void* memory_insert_header(void* data, MemoryHeader header) {
  	((MemoryHeader*)data)[0] = header;
  	data = (u8*)data + sizeof(header);
  	return data;
}

#define ckit_memory_base(data) (MemoryHeader*)((u8*)data - sizeof(MemoryHeader))

Boolean memory_tag_is_valid(MemoryTag memory_tag) {
  	return (memory_tag >= 0 && memory_tag < MEMORY_TAG_COUNT);
}

void memory_init() {
	ckg_bind_alloc_callback(&platform_allocate);
	ckg_bind_free_callback(&platform_free);
}

void* ckit_alloc(size_t byte_allocation_size, MemoryTag memory_tag) {
	ckit_assert_msg(byte_allocation_size > 0, "Invalid allocation size zero or below\n");
	ckit_assert_msg(memory_tag_is_valid(memory_tag), "ckit_alloc: Memory tag is invalid | value: (%d)\n", memory_tag);
	if (memory_tag == MEMORY_TAG_UNKNOWN) {
		LOG_WARN("ckit_alloc: memory tag unknown\n");
	}

	MemoryHeader header;
	header.allocation_size_without_header = byte_allocation_size;
	header.memory_tag = memory_tag;

	memory_track_add(header, memory_tag);
	void* data = ckg_alloc(sizeof(header) + header.allocation_size_without_header);
	ckit_memory_zero(data, sizeof(header) + header.allocation_size_without_header);
	// Date: July 16, 2024
	// NOTE(Jovanni): Somehow the memory we get from this isn't zeroed lmao?
	data = memory_insert_header(data, header);

	return data;
}

// Date: July 16, 2024
// TODO(Jovanni): This can be so much more powerful because I can use the tag system to see if a tag is registered and used that free method that is provided
// this is really awesome because this means you have generic frees for everything.
void* MACRO_ckit_free_tag_check(void* data) { 
	ckit_assert_msg(data, "ckit_free: Data passed is null in free\n");
  	const MemoryHeader header = *ckit_memory_base(data);
	switch (header.memory_tag) {
		case MEMORY_TAG_STRING: {
			#include "../String/ckit_string.h"
			ckit_str_free(data);
		} break;

		case MEMORY_TAG_VECTOR: {
			#include "../Collection/Vector/ckit_vector.h"
			ckit_vector_free(data);
		} break;

		case MEMORY_TAG_ARENA: {
			#include "./ckit_arena.h"
			ckit_arena_free(data);
		} break;

		default : {
			ckit_free(data);
		} break;
	}

	return data;
}

void* MACRO_ckit_free(void* data) {
  	ckit_assert_msg(data, "ckit_free: Data passed is null in free\n");
  	const MemoryHeader header = *ckit_memory_base(data);
  	ckit_assert_msg(memory_tag_is_valid(header.memory_tag), "ckit_free: memory_tag is not valid\n");

  	memory_track_remove(header, header.memory_tag);

  	data = ckit_memory_base(data);
	ckg_memory_zero(data, sizeof(header) + header.allocation_size_without_header);
  	ckg_free(data);
  	return data;
}

void* ckit_realloc(void* data, u64 new_allocation_size) {
  	ckit_assert_msg(data, "ckit_reallocation: Data passed is null\n");
  	MemoryHeader header = *ckit_memory_base(data);

  	void* ret_data = ckit_alloc(sizeof(header) + new_allocation_size, header.memory_tag);
  	ckg_memory_copy(data, ret_data, header.allocation_size_without_header, new_allocation_size);
  	ckit_free(data);

  	return ret_data;
}

Boolean ckit_memory_compare(const void* buffer_one, const void* buffer_two, u32 b1_allocation_size, u32 b2_allocation_size) {
	return ckg_memory_compare(buffer_one, buffer_two, b1_allocation_size, b2_allocation_size);
}
void ckit_memory_copy(const void* source, void* destination, size_t source_size_in_bytes, size_t destination_size_in_bytes) {
	ckg_memory_copy(source, destination, source_size_in_bytes, destination_size_in_bytes);
}

void ckit_memory_zero(void* data, size_t data_size_in_bytes) {
	ckg_memory_zero(data, data_size_in_bytes);
}

void MACRO_ckit_memory_delete_index(void* data, u32 data_capacity, size_t element_size_in_bytes, u32 index) {
	MACRO_ckg_memory_delete_index(data, data_capacity, element_size_in_bytes, index);
}

void MACRO_ckit_memory_insert_index(void* data, u32 data_capacity, size_t element_size_in_bytes, u32 index) {
	MACRO_ckg_memory_insert_index(data, data_capacity, element_size_in_bytes, index);
}

void ckit_memory_arena_register(CKIT_Arena* arena) {
	ckit_vector_push(registered_arenas, arena);
}

void ckit_memory_arena_unregister(CKIT_Arena* arena) {
	for (int i = 0; i < ckit_vector_count(registered_arenas); i++) {
		if (arena == registered_arenas[i]) {
			ckit_vector_remove_at(registered_arenas, i);
			break;
		}
	}
}

void ckit_memory_arena_unregister_all() {
	for (int i = ckit_vector_count(registered_arenas) - 1; i >= 0; i--) {
		ckit_arena_free(registered_arenas[i]);
	}

	ckit_vector_free(registered_arenas);
}

void memory_output_allocations(CKG_LogLevel log_level) {
    if (memory_used == 0) {
        log_output(log_level, "No memory allocations!\n");
        return;
    }
    char out_message[PLATFORM_CHARACTER_LIMIT];
    char out_message2[PLATFORM_CHARACTER_LIMIT];
    char out_message3[PLATFORM_CHARACTER_LIMIT];
    
    log_output(log_level, "============ MEMORY LEAK PRESENT ============\n");
    for (int level = 0; level < MEMORY_TAG_COUNT; level++) {
     	if (global_memory_tags[level] == 0) {
    		continue;
     	} 

     	ckg_memory_zero(out_message, sizeof(out_message));
     	ckg_memory_zero(out_message2, sizeof(out_message2));
     	ckg_memory_zero(out_message3, sizeof(out_message3));

     	sprintf(out_message, "%s", known_memory_tag_strings[level]);
     	sprintf(out_message2, "%lld bytes", global_memory_tags[level]);
     	sprintf(out_message3, "%s%s", out_message, out_message2);
     	log_output(log_level, "%s\n", out_message3);
		if (level == MEMORY_TAG_ARENA) {
			for (int i = 0; i < ckit_vector_count(registered_arenas); i++) {
				ckit_arena_output_allocations(registered_arenas[i], log_level);
			}
		}
    }
    log_output(log_level, "=============================================\n");
}
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++











