/*===========================================================
 * File: core_memory.cpp
 * Date: May 03, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

/*
Be aware anything allocated from this has a header
*/

#include "../include/core_memory.h"
#include "../include/core_logger.h"
#include "../include/platform_services.h"
#include "../include/core_assert.h"



typedef struct MemoryHeader {
    u32 total_allocation_size;
    MemoryTag memory_tag;
} MemoryHeader;

u8* memory_advance_new_ptr(u32 size_in_bytes, const void* data) {
	u8* base_address = (u8*)data;
	base_address += size_in_bytes;
    return base_address;
}

u8* memory_retreat_new_ptr(u32 size_in_bytes, const void* data) {
	u8* base_address = (u8*)data;
	base_address -= size_in_bytes;
    return base_address;
}

void memory_byte_advance(u32 size_in_bytes, void** data) {
	u8* base_address = (u8*)*data;
	base_address += size_in_bytes;
    *data = base_address;
}

void memory_byte_retreat(u32 size_in_bytes, void** data) {
	u8* base_address = (u8*)*data;
	base_address -= size_in_bytes;
    *data = base_address;
}

/**
 * @brief memory Copies the header into the data and then advances the pointer
 * 
 * @param header 
 * @param data 
 * @return internal 
 */
internal void _memory_insert_header(MemoryHeader header, void** data) {
	memory_copy(sizeof(header), &header, sizeof(header), *data);
	memory_byte_advance(sizeof(header), data);
}

/**
 * @brief just update don't advance
 * 
 * @param header 
 * @param data 
 * @return internal 
 */
internal void _memory_update_header(MemoryHeader header, void** data) {
    memory_byte_retreat(sizeof(header), data);
	memory_copy(sizeof(header), &header, sizeof(header), data);
	memory_byte_advance(sizeof(header), data);
}

MemoryHeader _memory_extract_header(void* data) {
	MemoryHeader header;
	memory_zero(sizeof(header), &header);
    memory_byte_retreat(sizeof(header), MUTABLE_VOID_POINTER(data));
	memory_copy(sizeof(header), data, sizeof(header), &header);
    memory_byte_advance(sizeof(header), MUTABLE_VOID_POINTER(data));
	return header;
}

void* memory_allocate(u64 byte_allocation_size, MemoryTag memory_tag) {
    assert_in_function(byte_allocation_size > 0, "Invalid allocation size zero or below");
    assert_in_function(memory_tag >= 0, "Invalid memory tag value! Below Zero");
    assert_in_function(memory_tag < MEMORY_TAG_COUNT, "Invalid memory tag value! Above max count of memory tags");
    if (memory_tag == MEMORY_TAG_UNKNOWN) {
        LOG_WARN("Allocation | memory tag unknown");
    }
	MemoryHeader header;
    memory_zero(sizeof(header), &header);
	header.total_allocation_size = sizeof(header) + byte_allocation_size; 

    header.memory_tag = memory_tag;
    global_memory_tags[memory_tag] += header.total_allocation_size;

	void* data = _platform_allocate(header.total_allocation_size);
    // Date: May 09, 2024
    // TODO(Jovanni): Technically you are repeating work here
	memory_zero(header.total_allocation_size, data);
	_memory_insert_header(header, MUTABLE_VOID_POINTER(data));
    return data;
}

/**
 * @brief Returns the new allocation and frees the data provided
 * 
 * @param new_byte_allocation_size 
 * @param data 
 * @return void* 
 */
void* memory_reallocate(u64 new_byte_allocation_size, void** data) {
    LOG_DEBUG("Reallocation Triggered!\n");
    assert_in_function(data, "Data passed is null in reallocation");

    MemoryHeader header = _memory_extract_header(*data);
    u32 old_allocation_size = header.total_allocation_size;
    header.total_allocation_size = sizeof(header) + new_byte_allocation_size;
	void* ret_data = memory_allocate(new_byte_allocation_size, header.memory_tag);
    _memory_insert_header(header, MUTABLE_VOID_POINTER(ret_data));

    memory_copy(header.total_allocation_size - sizeof(header), *data, new_byte_allocation_size, ret_data);
	memory_free(data);

    return ret_data;
}

/**
 * @brief Ensure that if you 
 * 
 * @param data 
 */
void memory_free(void** data) {
    assert_in_function(data != NULL && *data != NULL, "Data passed is null in free");
    MemoryHeader header = _memory_extract_header(*data);
    global_memory_tags[header.memory_tag] -= header.total_allocation_size;
    
    memory_byte_retreat(sizeof(header), data);
    // Date: May 09, 2024
    // NOTE(Jovanni): Should I actually do this? Should I zero out the memory I free?
	memory_zero(header.total_allocation_size , *data);
    
    _platform_free(data);
	*data = NULL;
}

void memory_copy(u32 source_size, const void* source, u32 destination_size, void* destination) {
    assert_in_function((source_size <= destination_size), "MEMORY COPY SOURCE IS TOO BIG FOR DESTINATION\n");
    for (int i = 0; i < source_size; i++) {
        ((u8*)destination)[i] = ((u8*)source)[i];
    }
}

void memory_zero(u32 data_size_in_bytes, void* data) {
    for (int i = 0; i < data_size_in_bytes; i++) {
        ((u8*)data)[i] = 0;
    }
}

Boolean memory_byte_compare(u32 buffer_one_size, const void* buffer_one, u32 buffer_two_size, const void* buffer_two) {
    assert_in_function(buffer_one == buffer_two, "memory_byte_compare buffer sizes are not equal!");
    u8* buffer_one_data = (u8*)buffer_one;
    u8* buffer_two_data = (u8*)buffer_two;
    for (int i = 0; i < buffer_one_size; i++) {
        if (buffer_one_data[i] != buffer_two_data[i]) {
            return FALSE;
        }
    }
    
    return TRUE;
}

void console_write_memory_tags(LogLevel log_level) {
    char out_message[PLATFORM_CHARACTER_LIMIT];
    char out_message2[PLATFORM_CHARACTER_LIMIT];
    char out_message3[PLATFORM_CHARACTER_LIMIT];
    
    log_output(log_level, "========================\n");
    for (int level = 0; level < MEMORY_TAG_COUNT; level++) {
        memory_zero(sizeof(out_message), out_message);
        memory_zero(sizeof(out_message2), out_message2);
        memory_zero(sizeof(out_message3), out_message3);

        sprintf(out_message, "%s", known_memory_tag_strings[level]);
        sprintf(out_message2, "%lld", global_memory_tags[level]);
        sprintf(out_message3, "%s%s", out_message, out_message2);
        log_output(log_level, "%s\n", out_message3);
    }
    log_output(log_level, "========================\n");
}
