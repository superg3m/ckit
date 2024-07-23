/*===========================================================
 * File: ckit_memory.c
 * Date: May 03, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../../ckg/Core/Memory/ckg_memory.h"



#include "../Assert/ckit_assert.h"
#include "../Logger/ckit_logger.h"

#include "../Platform/ckit_platform_services.h"

#include "./ckit_memory.h"
//========================== Begin Types ==========================

//=========================== End Types ===========================

//************************* Begin Functions *************************
void memory_init() {
	ckg_bind_alloc_callback(&platform_allocate);
	ckg_bind_free_callback(&platform_free);
}

void* MACRO_ckit_alloc(size_t byte_allocation_size, CKIT_MemoryTagID tag_id, char* file, u32 line, char* function) {
	ckit_assert_msg(byte_allocation_size > 0, "Invalid allocation size zero or below\n");

	CKIT_MemoryHeader temp_header = ckit_tracker_header_create(tag_id, byte_allocation_size, file, line, function);

	void* data = ckg_alloc(sizeof(temp_header) + byte_allocation_size);
	ckit_memory_zero(data, sizeof(temp_header) + byte_allocation_size);
	ckit_tracker_insert_header(data, temp_header);
	CKIT_MemoryHeader* real_header = ckit_tracker_get_header(data); // this is probably a bad constrain I shouldn't have to think about getting the real header
	ckit_tracker_add(real_header);

	return data;
}

void* MACRO_ckit_free(void* data) {
  	ckit_assert_msg(data, "ckit_free: Data passed is null in free\n");

  	CKIT_MemoryHeader* header = ckit_tracker_get_header(data);
	const size_t allocation_size = header->tag.allocation_info.allocation_size;
  	ckit_tracker_remove(header);
  	data = (u8*)data - sizeof(CKIT_MemoryHeader);
	ckit_memory_zero(data, sizeof(header) + allocation_size);
  	ckg_free(data);
  	return data;
}

void* MACRO_ckit_realloc(void* data, u64 new_allocation_size, char* file, u32 line, char* function) {
  	ckit_assert_msg(data, "ckit_reallocation: Data passed is null\n");
  	const CKIT_MemoryHeader* header = ckit_tracker_get_header(data);

  	void* ret_data = MACRO_ckit_alloc(sizeof(header) + new_allocation_size, header->tag.tag_id, file, line, function);
  	ckit_memory_copy(data, ret_data, header->tag.allocation_info.allocation_size, new_allocation_size);
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

/*
void ckit_memory_report(CKG_LogLevel log_level) {
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
*/

//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++











