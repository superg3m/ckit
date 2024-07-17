// This will do all the tag stuff

#include "../ckit_types.h"
#include "../Collection/Vector/ckit_vector.h"

typedef enum MemoryTag { // Reserved tags
    MEMORY_TAG_CKIT_TEMPORARY,
    MEMORY_TAG_CKIT_INTERNAL,
    MEMORY_TAG_CKIT_STRING,
    MEMORY_TAG_CKIT_VECTOR,
    MEMORY_TAG_CKIT_ARENA,
    MEMORY_TAG_CKIT_COUNT
} MemoryTag;

typedef struct MemoryHeader {
	u32 allocation_size_without_header;
    MemoryTag memory_tag;
} MemoryHeader;

internal s64* global_memory_tags = NULLPTR; // this might be a hashmap
internal u64 memory_used = 0; 

void memory_tracker_init() {
	global_memory_tags = ckit_vector_reserve(MEMORY_TAG_COUNT, s64);
}

void register_memory_tag(u32 tag_id, const char* name, void* free_method) {
    if (global_memory_tags[tag_id] == -1) { // this is prob a perfect use 

    }

    // bro when you register a tag you can also have the free for that tag holy shit
	// for (int i = 0; )
}

char known_memory_tag_strings[MEMORY_TAG_COUNT][MEMORY_TAG_CHARACTER_LIMIT] = {
    "UNKNOWN      : ",
    "TEMPORARY    : ",
    "INTERNAL     : ",
    "STRING       : ",
    "VECTOR       : ",
    "ARENA        : ",
};

MemoryHeader memory_tracker_create_header(size_t allocation_size, ) {

}

internal void* memory_insert_header(void* data, MemoryHeader header) {
  	((MemoryHeader*)data)[0] = header;
  	data = (u8*)data + sizeof(header);
  	return data;
}

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

