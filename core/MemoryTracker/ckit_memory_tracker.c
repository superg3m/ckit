// This will do all the tag stuff

#include "./ckit_memory_tracker.h"
#include "../Collection/Vector/ckit_vector.h"
#include "../String/ckit_string.h"

CKIT_MemoryTagID reserved_tags[] = {
    TAG_CKIT_CORE_STRING,
    TAG_CKIT_CORE_ARENA,
    TAG_CKIT_CORE_VECTOR,
    TAG_CKIT_CORE_STACK,
    TAG_CKIT_CORE_LINKED_LIST,
    TAG_CKIT_CORE_QUEUE,
    TAG_CKIT_CORE_FILE_SYSTEM,

    TAG_CKIT_MODULE_LEXER,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_BMP,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_OBJ,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_PNG,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_JSON,

    TAG_CKIT_EXPECTED_USER_FREE,
    TAG_CKIT_INTERNAL,
};

char* reserved_tags_stringified[] = {
    stringify(TAG_CKIT_CORE_STRING),
    stringify(TAG_CKIT_CORE_ARENA),
    stringify(TAG_CKIT_CORE_VECTOR),
    stringify(TAG_CKIT_CORE_STACK),
    stringify(TAG_CKIT_CORE_LINKED_LIST),
    stringify(TAG_CKIT_CORE_QUEUE),
    stringify(TAG_CKIT_CORE_FILE_SYSTEM),

    stringify(TAG_CKIT_MODULE_LEXER),
    stringify(TAG_CKIT_MODULE_FILE_FORMAT_PARSER_BMP),
    stringify(TAG_CKIT_MODULE_FILE_FORMAT_PARSER_OBJ),
    stringify(TAG_CKIT_MODULE_FILE_FORMAT_PARSER_PNG),
    stringify(TAG_CKIT_MODULE_FILE_FORMAT_PARSER_JSON),

    stringify(TAG_CKIT_EXPECTED_USER_FREE),
    stringify(TAG_CKIT_INTERNAL)
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

typedef struct CKIT_MemoryTagPool {
    CKIT_MemoryTagID tag_id;
    char* tag_name;
    size_t total_pool_allocation_size;
} CKIT_MemoryTagPool;

typedef struct CKIT_MemoryHeader {
	CKIT_MemoryTag tag;
    char* magic;
} CKIT_MemoryHeader;

#define CKIT_MEMORY_MAGIC "CKIT_MAGIC_MEMORY"
#define ckit_memory_header(data) ((CKIT_MemoryHeader*)((u8*)data - sizeof(CKIT_MemoryHeader)))

internal void ckit_tracker_check_magic(void* data) {
    ckit_assert(ckit_str_equal(ckit_memory_header(data)->magic, CKIT_MEMORY_MAGIC));
}

internal CKIT_MemoryTagPool ckit_tracker_tag_pool_create(CKIT_MemoryTagID tag_id, char* name) {
    CKIT_MemoryTagPool ret;
    ret.tag_id = tag_id;
    ret.tag_name = name;
    ret.total_pool_allocation_size = 0;

    return ret;
}

internal CKIT_MemoryTagPool* global_memory_tag_pool_vector = NULLPTR;
internal u64 global_total_pool_memory_used = 0;

internal CKIT_MemoryTag ckit_tracker_memory_tag_create(CKIT_MemoryTagID tag_id, char* name) {
    CKIT_MemoryTag ret;
    ret.tag_id = tag_id;
    ret.tag_name = name;
    ret.allocation_info.allocation_size = 0;
    ret.allocation_info.file_name = NULLPTR;
    ret.allocation_info.line = -1;
    ret.allocation_info.function_name = NULLPTR;
    
    return ret;
}

internal void ckit_tracker_reserved_tag_to_string(CKIT_MemoryTagID reserved_tag_id_to_find) {
    ckit_assert(reserved_tag_id_to_find < TAG_CKIT_RESERVED_COUNT);
    return global_memory_tag_pool_vector[reserved_tag_id_to_find].tag_name;
}

internal u64 ckit_tracker_get_tag_pool_index(CKIT_MemoryTagID tag_id) {
    for (int i = TAG_CKIT_RESERVED_COUNT; i < ckit_vector_count(global_memory_tag_pool_vector); i++) {
        if (global_memory_tag_pool_vector[i].tag_id == tag_id) {
            return i;
        }
    }
    
    ckit_assert(FALSE); // don't query a not registed tag_id
    return -1; // never gets here
}

internal Boolean ckit_tracker_tag_pool_exists(CKIT_MemoryTagID tag_id) {
    for (int i = TAG_CKIT_RESERVED_COUNT; i < ckit_vector_count(global_memory_tag_pool_vector); i++) {
        if (global_memory_tag_pool_vector[i].tag_id == tag_id) {
            return TRUE;
        }
    }

    return FALSE;
}

internal char* ckit_tracker_tag_to_string(CKIT_MemoryTagID tag_id) {
    for (int i = 0; i < ckit_vector_count(global_memory_tag_pool_vector); i++) {
        if (global_memory_tag_pool_vector[i].tag_id == tag_id) {
            return global_memory_tag_pool_vector[i].tag_name;
        }
    }

    ckit_assert(FALSE); // Don't query a tag you don't have registered
}

void ckit_tracker_init() {
    global_memory_tag_pool_vector = ckit_vector_reserve(TAG_CKIT_RESERVED_COUNT, CKIT_MemoryTag);

    for (int i = 0; i < TAG_CKIT_RESERVED_COUNT; i++) {
        CKIT_MemoryTagPool tag_pool = ckit_tracker_tag_pool_create(reserved_tags[i], reserved_tags_stringified[i]);
        ckit_vector_push(global_memory_tag_pool_vector, tag_pool);
    }
}

// Date: July 22, 2024
// TODO(Jovanni): Think about this some more does this actually let me know where something is allocated?
// Maybe I need to actually have some linked list chain in the specific pool? (this would me I would know the type of the allocation because I know the pool)
CKIT_MemoryHeader ckit_tracker_header_create(CKIT_MemoryTagID tag_id, char* file_name, u64 line, char* function_name) {
    CKIT_MemoryHeader ret;
    ret.magic = CKIT_MEMORY_MAGIC;

    ret.tag.tag_id = tag_id;
    ret.tag.allocation_info.file_name = file_name;
    ret.tag.allocation_info.line = line;
    ret.tag.allocation_info.function_name = function_name;

    return ret;
}

void ckit_tracker_register_tag_pool(CKIT_MemoryTagID tag_id, const char* name) {
    ckit_assert(!ckit_tracker_tag_exists(tag_id)); // don't register a tag that already exists/has been registered

    CKIT_MemoryTagPool tag_pool = ckit_tracker_tag_pool_create(tag_id, name);
    ckit_vector_push(global_memory_tag_pool_vector, tag_pool);
}

void* memory_insert_header(void* data, CKIT_MemoryHeader header) {
  	((CKIT_MemoryHeader*)data)[0] = header;
  	data = (u8*)data + sizeof(CKIT_MemoryHeader);
  	return data;
}

void ckit_tracker_add(CKIT_MemoryTagID tag_id, CKIT_MemoryHeader header) {
  	global_memory_tag_pool_vector[TAG_CKIT_INTERNAL].total_pool_allocation_size += sizeof(header);
  	global_memory_tag_pool_vector[ckit_tracker_get_tag_pool_index(tag_id)].total_pool_allocation_size += (header.tag.allocation_info.allocation_size);
  	global_total_pool_memory_used += sizeof(header) + header.tag.allocation_info.allocation_size;
}

void ckit_tracker_remove(CKIT_MemoryTagID tag_id, CKIT_MemoryHeader header) {
  	global_memory_tag_pool_vector[TAG_CKIT_INTERNAL].total_pool_allocation_size -= sizeof(header);
  	global_memory_tag_pool_vector[ckit_tracker_get_tag_pool_index(tag_id)].total_pool_allocation_size += (header.tag.allocation_info.allocation_size);
  	global_total_pool_memory_used -= sizeof(header) + header.tag.allocation_info.allocation_size;
}

