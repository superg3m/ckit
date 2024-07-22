// This will do all the tag stuff

#include "../ckit_types.h"
#include "../Collection/Vector/ckit_vector.h"
#include "../String/ckit_string.h"

typedef enum CKIT_MemoryTagID { // Reserved tags
    CKIT_CORE_STRING_TAG,
    CKIT_CORE_ARENA_TAG,
    CKIT_CORE_VECTOR_TAG,
    CKIT_CORE_STACK_TAG,
    CKIT_CORE_LINKED_LIST_TAG,
    CKIT_CORE_QUEUE_TAG,
    CKIT_CORE_FILE_SYSTEM_TAG,

    CKIT_MODULE_LEXER_TAG,
    CKIT_MODULE_FILE_FORMAT_PARSER_BMP_TAG,
    CKIT_MODULE_FILE_FORMAT_PARSER_OBJ_TAG,
    CKIT_MODULE_FILE_FORMAT_PARSER_PNG_TAG,
    CKIT_MODULE_FILE_FORMAT_PARSER_JSON_TAG,

    CKIT_EXPECTED_USER_FREE_TAG,
    CKIT_RESERVED_TAG_COUNT
} CKIT_MemoryTagID;

CKIT_MemoryTagID reserved_tags[] = {
    CKIT_CORE_STRING_TAG,
    CKIT_CORE_ARENA_TAG,
    CKIT_CORE_VECTOR_TAG,
    CKIT_CORE_STACK_TAG,
    CKIT_CORE_LINKED_LIST_TAG,
    CKIT_CORE_QUEUE_TAG,
    CKIT_CORE_FILE_SYSTEM_TAG,

    CKIT_MODULE_LEXER_TAG,
    CKIT_MODULE_FILE_FORMAT_PARSER_BMP_TAG,
    CKIT_MODULE_FILE_FORMAT_PARSER_OBJ_TAG,
    CKIT_MODULE_FILE_FORMAT_PARSER_PNG_TAG,
    CKIT_MODULE_FILE_FORMAT_PARSER_JSON_TAG,

    CKIT_EXPECTED_USER_FREE_TAG
};

char* reserved_tags_stringified[] = {
    stringify(CKIT_CORE_STRING_TAG),
    stringify(CKIT_CORE_ARENA_TAG),
    stringify(CKIT_CORE_VECTOR_TAG),
    stringify(CKIT_CORE_STACK_TAG),
    stringify(CKIT_CORE_LINKED_LIST_TAG),
    stringify(CKIT_CORE_QUEUE_TAG),
    stringify(CKIT_CORE_FILE_SYSTEM_TAG),

    stringify(CKIT_MODULE_LEXER_TAG),
    stringify(CKIT_MODULE_FILE_FORMAT_PARSER_BMP_TAG),
    stringify(CKIT_MODULE_FILE_FORMAT_PARSER_OBJ_TAG),
    stringify(CKIT_MODULE_FILE_FORMAT_PARSER_PNG_TAG),
    stringify(CKIT_MODULE_FILE_FORMAT_PARSER_JSON_TAG),

    stringify(CKIT_EXPECTED_USER_FREE_TAG)
};


typedef struct CKIT_AllocationInfo {
    // allocation_site
    char* file_name;
    s64 line;
    char* function_name;
    size_t allocation_size;
} CKIT_AllocationInfo;

typedef struct CKIT_MemoryTag {
    CKIT_MemoryTagID tag_id;
    char* tag_name;
    CKIT_AllocationInfo allocation_info;
} CKIT_MemoryTag;

typedef struct CKIT_MemoryHeader {
	CKIT_MemoryTag tag;
    char* magic;
} CKIT_MemoryHeader;

#define CKIT_MEMORY_MAGIC "CKIT_MAGIC_MEMORY"
#define ckit_memory_header(data) ((CKIT_MemoryHeader*)((u8*)data - sizeof(CKIT_MemoryHeader)))

void ckit_tracker_check_magic(void* data) {
    ckit_assert(ckit_str_equal(ckit_memory_header(data)->magic, CKIT_MEMORY_MAGIC));
}

CKIT_MemoryHeader ckit_tracker_header_create(CKIT_MemoryTagID tag_id, char* file_name, u64 line, char* function_name) {
    CKIT_MemoryHeader ret;
    ret.magic = CKIT_MEMORY_MAGIC;

    ret.tag.tag_id = tag_id;
    ret.tag.allocation_info.file_name = file_name;
    ret.tag.allocation_info.line = line;
    ret.tag.allocation_info.function_name = function_name;

    return ret;
}

internal CKIT_MemoryTag* global_memory_tag_vector = NULLPTR;
internal u64 memory_used = 0;

CKIT_MemoryTag ckit_tracker_memory_tag_create(CKIT_MemoryTagID tag_id, char* name) {
    CKIT_MemoryTag ret;
    ret.tag_id = tag_id;
    ret.tag_name = name;
    ret.allocation_info.allocation_size = 0;
    ret.allocation_info.file_name = NULLPTR;
    ret.allocation_info.line = -1;
    ret.allocation_info.function_name = NULLPTR;
    
    return ret;
}

void ckit_tracker_init() {
    global_memory_tag_vector = ckit_vector_reserve(CKIT_RESERVED_TAG_COUNT, CKIT_MemoryTag);

    for (int i = 0; i < CKIT_RESERVED_TAG_COUNT; i++) {
        CKIT_MemoryTag current_tag = ckit_tracker_memory_tag_create(reserved_tags[i], reserved_tags_stringified[i]);
        ckit_vector_push(global_memory_tag_vector, current_tag);
    }
}

void ckit_tracker_reserved_tag_to_string(CKIT_MemoryTagID reserved_tag_id_to_find) {
    ckit_assert(reserved_tag_id_to_find < CKIT_RESERVED_TAG_COUNT);
    return global_memory_tag_vector[reserved_tag_id_to_find].tag_name;
}

Boolean ckit_tracker_tag_exists(CKIT_MemoryTagID tag_id) {
    for (int i = CKIT_RESERVED_TAG_COUNT; i < ckit_vector_count(global_memory_tag_vector); i++) {
        if (global_memory_tag_vector[i].tag_id == tag_id) {
            return TRUE;
        }
    }

    return FALSE;
}

void ckit_tracker_register_memory_tag(CKIT_MemoryTagID tag_id, const char* name) {
    ckit_assert(!ckit_tracker_tag_exists(tag_id)); // don't register a tag that already exists/has been registered

    CKIT_MemoryTag current_tag = ckit_tracker_memory_tag_create(tag_id, name);
    ckit_vector_push(global_memory_tag_vector, current_tag);
}

char* ckit_tracker_tag_to_string(CKIT_MemoryTagID tag_id) {
    for (int i = 0; i < ckit_vector_count(global_memory_tag_vector); i++) {
        if (global_memory_tag_vector[i].tag_id == tag_id) {
            return global_memory_tag_vector[i].tag_name;
        }
    }

    ckit_assert(FALSE); // Don't query a tag you don't have registered
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

