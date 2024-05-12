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

Boolean memory_tag_is_unknown(MemoryTag memory_tag) {
    return (memory_tag == MEMORY_TAG_UNKNOWN);
}

Boolean memory_tag_is_valid(MemoryTag memory_tag) {
    return (memory_tag >= 0 && memory_tag < MEMORY_TAG_COUNT);
}

u8* memory_advance_new_ptr(const void* data, u32 size_in_bytes) {
	u8* base_address = (u8*)data;
	base_address += size_in_bytes;
    return base_address;
}

u8* memory_retreat_new_ptr(const void* data, u32 size_in_bytes) {
	u8* base_address = (u8*)data;
	base_address -= size_in_bytes;
    return base_address;
}

void* MACRO_memory_byte_advance(const void* data, u32 size_in_bytes) {
	u8* base_address = (u8*)data;
	base_address += size_in_bytes;
    return base_address;
}

void* MACRO_memory_byte_retreat(const void* data, u32 size_in_bytes) {
	u8* base_address = (u8*)data;
	base_address -= size_in_bytes;
    return base_address;
}

/**
 * @brief memory Copies the header into the data and then advances the pointer
 * 
 * @param header 
 * @param data 
 * @return internal 
 */
void* MACRO_memory_insert_header(void* data, MemoryHeader header) {
    ((MemoryHeader*)data)[0] = header;
	memory_byte_advance(data, sizeof(header));
    return data;
}
#define _memory_insert_header(data, header) data = MACRO_memory_insert_header(data, header);

MemoryHeader* _memory_extract_header(void* data) {
	return &((MemoryHeader*)data)[-1];
}

void memory_register_arena(Arena** arena) {
    vector_push(arena_vector, *arena);
}


void* memory_allocate(u64 byte_allocation_size, MemoryTag memory_tag) {
    // Date: May 11, 2024
    // TODO(Jovanni): This feels like shit I would figure out a better spot for it
    // Theoretically this isn't a problem but I don't like it
    // arena_vector = vector_create(Arena*);

    assert_in_function(byte_allocation_size > 0, "Invalid allocation size zero or below\n");
    assert_in_function(memory_tag_is_valid(memory_tag), "memory_allocate: Memory tag is invalid | value: (%d)\n", memory_tag);
    if (memory_tag_is_unknown(memory_tag)) {
        LOG_WARN("memory_allocate: memory tag unknown\n");
    }
	MemoryHeader header;
    memory_zero(&header, sizeof(header));
	header.total_allocation_size = sizeof(header) + byte_allocation_size; 

    // Date: May 11, 2024
    // TODO(Jovanni): Look at this because this is really odd having to subtract sizeof(header)
    header.memory_tag = memory_tag;
    global_memory_tags[MEMORY_TAG_INTERNAL] += sizeof(header);
    global_memory_tags[memory_tag] += (header.total_allocation_size - sizeof(header));

	void* data = platform_allocate(header.total_allocation_size);
    // Date: May 09, 2024
    // TODO(Jovanni): Technically you are repeating work here
	memory_zero(data, header.total_allocation_size);
	_memory_insert_header(data, header);
    return data;
}

/**
 * @brief Returns the new allocation and frees the data provided
 * 
 * @param new_byte_allocation_size 
 * @param data 
 * @return void* 
 */
void* memory_reallocate(void* data, u64 new_byte_allocation_size) {
    LOG_DEBUG("Reallocation Triggered!\n");
    assert_in_function(data, "memory_reallocation: Data passed is null\n");

    MemoryHeader header = *_memory_extract_header(data);
    u32 old_allocation_size = header.total_allocation_size;
    // Date: May 11, 2024
    // TODO(Jovanni): I want to show the amount of internal allocations happening so i want to say
    // global_memory[MEMORY_TAG_INTERNAL] += sizeof(header) for each allocation
    //  for each free
    header.total_allocation_size = sizeof(header) + new_byte_allocation_size;
	void* ret_data = memory_allocate(new_byte_allocation_size, header.memory_tag);
    _memory_insert_header(ret_data, header);

    memory_copy(data, ret_data, header.total_allocation_size - sizeof(header), new_byte_allocation_size);
	memory_free(data);

    return ret_data;
}

/**
 * @brief Ensure that if you 
 * 
 * @param data
 */
void* MACRO_memory_free(void* data) {
    assert_in_function(data, "memory_free: Data passed is null in free\n");
    const MemoryHeader header = *_memory_extract_header(data);
    assert_in_function(memory_tag_is_valid(header.memory_tag), "memory_free: memory_tag is not valid\n");
    global_memory_tags[MEMORY_TAG_INTERNAL] -= sizeof(header);
    global_memory_tags[header.memory_tag] -= (header.total_allocation_size - sizeof(header));
    
    memory_byte_retreat(data, sizeof(header));
    // Date: May 09, 2024
    // NOTE(Jovanni): Should I actually do this? Should I zero out the memory I free?
	memory_zero(data, header.total_allocation_size);
    platform_free(data);
	data = NULLPTR;
    return data;
}

void memory_copy(const void* source, void* destination, u32 source_size, u32 destination_size) {
    assert_in_function(source, "MEMORY COPY SOURCE IS NULL\n");
    assert_in_function(destination, "MEMORY COPY SOURCE IS NULL\n");
    assert_in_function((source_size <= destination_size), "MEMORY COPY SOURCE IS TOO BIG FOR DESTINATION\n");
    for (int i = 0; i < source_size; i++) {
        ((u8*)destination)[i] = ((u8*)source)[i];
    }
}

void memory_zero(void* data, u32 data_size_in_bytes) {
    for (int i = 0; i < data_size_in_bytes; i++) {
        ((u8*)data)[i] = 0;
    }
}

Boolean memory_byte_compare(const void* buffer_one, const void* buffer_two, u32 buffer_one_size, u32 buffer_two_size) {
    assert_in_function(buffer_one, "memory_byte_compare buffer_one IS NULL\n");
    assert_in_function(buffer_two_size, "memory_byte_compare buffer_two IS NULL\n");
    assert_in_function(buffer_one == buffer_two, "memory_byte_compare buffer sizes are not equal!\n");
    u8* buffer_one_data = (u8*)buffer_one;
    u8* buffer_two_data = (u8*)buffer_two;
    for (int i = 0; i < buffer_one_size; i++) {
        if (buffer_one_data[i] != buffer_two_data[i]) {
            return FALSE;
        }
    }
    
    return TRUE;
}

void memory_write_memory_tags(LogLevel log_level) {
    char out_message[PLATFORM_CHARACTER_LIMIT];
    char out_message2[PLATFORM_CHARACTER_LIMIT];
    char out_message3[PLATFORM_CHARACTER_LIMIT];
    
    log_output(log_level, "========================\n");
    for (int level = 0; level < MEMORY_TAG_COUNT; level++) {
        memory_zero(out_message, sizeof(out_message));
        memory_zero(out_message2, sizeof(out_message2));
        memory_zero(out_message3, sizeof(out_message3));

        sprintf(out_message, "%s", known_memory_tag_strings[level]);
        sprintf(out_message2, "%lld", global_memory_tags[level]);
        sprintf(out_message3, "%s%s", out_message, out_message2);
        log_output(log_level, "%s\n", out_message3);
    }
    log_output(log_level, "========================\n");
}