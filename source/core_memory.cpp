#include "../include/core_memory.h"
#include "../include/core_logger.h"
#include "../../platform/platform_services.h"

void* memory_allocate(u64 number_of_bytes, MemoryTag memory_tag) {
    if (memory_tag == MEMORY_TAG_UNKNOWN) {
        LOG_WARN("Allocation | memory tag unknown");
    }
    global_memory_tags[memory_tag] += number_of_bytes;
	void* data = _platform_allocate(number_of_bytes);
	memory_zero(number_of_bytes, data);
    return data;
}
void memory_free(u64 number_of_bytes, void** data, MemoryTag memory_tag) {
    global_memory_tags[memory_tag] -= number_of_bytes;
	memory_zero(number_of_bytes, *data);
    _platform_free(number_of_bytes, *data);
	*data = NULL;
}

void memory_copy(u32 source_size, const void* source, u32 destination_size, void* destination) {
    assert((source_size <= destination_size), "MEMORY COPY SOURCE IS TOO BIG FOR DESTINATION\n");
    for (int i = 0; i < source_size; i++) {
        ((u8*)destination)[i] = ((u8*)source)[i];
    }
}

#define memory_set(data_size_in_bytes, data, element) \
	decltype(element) e = element; \
	for (int i = 0; i < data_size_in_bytes; i++) { \
			((u8*)data)[i] = e; \
	} \

void memory_zero(u32 data_size_in_bytes, void* data) {
    for (int i = 0; i < data_size_in_bytes; i++) {
        ((u8*)data)[i] = 0;
    }
}

void console_write_memory_tags() {
    LOG_PRINT("");
    LOG_DEBUG("========================");
    for (int level = 0; level < MEMORY_TAG_COUNT; level++) {
        char out_message[320];
        memory_zero(sizeof(out_message), out_message);
        sprintf(out_message, "%s", known_memory_tag_strings[level]);

        char out_message2[320];
        memory_zero(sizeof(out_message2), out_message2);
        sprintf(out_message2, "%ld", global_memory_tags[level]);

        char out_message3[320];
        sprintf(out_message3, "%s%s", out_message, out_message2);
        LOG_DEBUG("%s", out_message3);
    }
    LOG_DEBUG("========================\n");
}
