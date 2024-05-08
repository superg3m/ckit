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

struct MemoryHeader {
	u32 allocation_size;
    MemoryTag memory_tag;
};

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

// Date: May 03, 2024
// TODO(Jovanni): Actually implement the memory header lmao
void* memory_allocate(u64 byte_allocation_size, MemoryTag memory_tag) {
    assert_in_function(byte_allocation_size > 0, "Invalid allocation size zero or below");
    assert_in_function(memory_tag >= 0, "Invalid memory tag value! Below Zero");
    assert_in_function(memory_tag < MEMORY_TAG_COUNT, "Invalid memory tag value! Above max count of memory tags");
    if (memory_tag == MEMORY_TAG_UNKNOWN) {
        LOG_WARN("Allocation | memory tag unknown");
    }
	MemoryHeader header;
    memory_zero(sizeof(header), &header);
	header.allocation_size = sizeof(header) + byte_allocation_size;
    header.memory_tag = memory_tag;
    global_memory_tags[memory_tag] += header.allocation_size;

	void* data = _platform_allocate(header.allocation_size);
	memory_zero(header.allocation_size, data);
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
	void* ret_data = memory_allocate(new_byte_allocation_size, header.memory_tag);

    global_memory_tags[header.memory_tag] += new_byte_allocation_size;
    memory_byte_retreat(sizeof(header), data);
    memory_byte_retreat(sizeof(header), MUTABLE_VOID_POINTER(ret_data));
	memory_copy(header.allocation_size, *data, new_byte_allocation_size, ret_data);
    memory_byte_advance(sizeof(header), data);
    memory_byte_advance(sizeof(header), MUTABLE_VOID_POINTER(ret_data));
	memory_free(data);

    global_memory_tags[header.memory_tag] -= header.allocation_size;
    header.allocation_size = new_byte_allocation_size;
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
    global_memory_tags[header.memory_tag] -= header.allocation_size;
    
    memory_byte_retreat(sizeof(header), data);
	memory_zero(header.allocation_size, *data);
    
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

void console_write_memory_tags() {
    char out_message[PLATFORM_CHARACTER_LIMIT];
    char out_message2[PLATFORM_CHARACTER_LIMIT];
    char out_message3[PLATFORM_CHARACTER_LIMIT];
    
    LOG_DEBUG("========================\n");
    for (int level = 0; level < MEMORY_TAG_COUNT; level++) {
        memory_zero(sizeof(out_message), out_message);
        memory_zero(sizeof(out_message2), out_message2);
        memory_zero(sizeof(out_message3), out_message3);

        sprintf(out_message, "%s", known_memory_tag_strings[level]);
        sprintf(out_message2, "%lld", global_memory_tags[level]);
        sprintf(out_message3, "%s%s", out_message, out_message2);
        LOG_DEBUG("%s\n", out_message3);
    }
    LOG_DEBUG("========================\n");
}
