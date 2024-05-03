#include "../include/core_memory.h"
#include "../include/core_logger.h"
#include "../include/platform_services.h"
#include "../include/core_assert.h"

struct MemoryHeader {
	u32 allocation_size;
};

void memory_byte_advance(u32 size_in_bytes, void** data) {
	u8* base_address = ((u8*)(*data));
	*base_address += size_in_bytes;
}

void memory_byte_retreat(u32 size_in_bytes, void** data) {
	u8* base_address = ((u8*)(*data));
	*base_address -= size_in_bytes;
}

internal void _memory_insert_header(MemoryHeader header, void** data) {
	memory_copy(sizeof(header), &header, sizeof(header), (*((u8**)data)) - sizeof(header));
	memory_byte_advance(sizeof(header), data);
}

MemoryHeader memory_extract_header(void* data) {
	MemoryHeader header;
	memory_zero(sizeof(header), &header);
	memory_copy(sizeof(header), ((u8*)data) - sizeof(header), sizeof(header), &header);
	return header;
}

void* memory_allocate(u64 number_of_bytes, MemoryTag memory_tag) {
	MemoryHeader header = {0};
	header.allocation_size = sizeof(MemoryHeader) + + number_of_bytes;
    if (memory_tag == MEMORY_TAG_UNKNOWN) {
        LOG_WARN("Allocation | memory tag unknown");
    }
    global_memory_tags[memory_tag] += header.allocation_size;
	
	int* data = (int*)_platform_allocate(header.allocation_size);
	
	_memory_insert_header(header, MUTABLE_VOID_POINTER(data));
	memory_zero(number_of_bytes, data);
    return data;
}

void* memory_reallocate(u64 old_number_of_bytes, void** data, u64 new_number_of_bytes, MemoryTag memory_tag) {
    if (memory_tag == MEMORY_TAG_UNKNOWN) {
        LOG_WARN("Reallocation | memory tag unknown");
    }
    global_memory_tags[memory_tag] += new_number_of_bytes;
	void* ret_data = memory_allocate(new_number_of_bytes, memory_tag);
	memory_copy(old_number_of_bytes, *data, new_number_of_bytes, ret_data);
	memory_free(old_number_of_bytes, data, memory_tag);
    return ret_data;
}

void memory_free(u64 number_of_bytes, void** data, MemoryTag memory_tag) {
	if (memory_tag == MEMORY_TAG_UNKNOWN) {
        LOG_WARN("Free | memory tag unknown");
    }
    global_memory_tags[memory_tag] -= number_of_bytes;
	memory_zero(number_of_bytes, *data);
    _platform_free(number_of_bytes, *data);
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
    LOG_DEBUG("========================\n");
    for (int level = 0; level < MEMORY_TAG_COUNT; level++) {
        char out_message[320];
        memory_zero(sizeof(out_message), out_message);
        sprintf(out_message, "%s", known_memory_tag_strings[level]);

        char out_message2[320];
        memory_zero(sizeof(out_message2), out_message2);
        sprintf(out_message2, "%lld", global_memory_tags[level]);

        char out_message3[320];
        sprintf(out_message3, "%s%s", out_message, out_message2);
        LOG_DEBUG("%s\n", out_message3);
    }
    LOG_DEBUG("========================\n");
}
