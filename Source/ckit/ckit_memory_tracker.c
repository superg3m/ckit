// This will do all the tag stuff
#include "ckg_vector.h"

#include "ckit_string.h"
#include "ckit_assert.h"
#include "ckit_memory_tracker.h"
CKIT_MemoryTagID reserved_tags[] = {
    TAG_USER_UNKNOWN,

    TAG_CKIT_TEMP,
    TAG_CKIT_CORE_STRING,
    TAG_CKIT_CORE_ARENA,
    TAG_CKIT_CORE_VECTOR,
    TAG_CKIT_CORE_STACK,
    TAG_CKIT_CORE_LINKED_LIST,
    TAG_CKIT_CORE_QUEUE,
    TAG_CKIT_CORE_FILE_SYSTEM,
    TAG_CKIT_CORE_HASHMAP,
    TAG_CKIT_CORE_HASHSET,

    TAG_CKIT_MODULE_LEXER,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_BMP,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_OBJ,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_PNG,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_JSON,

    TAG_CKIT_EXPECTED_USER_FREE,
};

char* reserved_tags_stringified[] = {
    stringify(TAG_USER_UNKNOWN),

    stringify(TAG_CKIT_TEMP),
    stringify(TAG_CKIT_CORE_STRING),
    stringify(TAG_CKIT_CORE_ARENA),
    stringify(TAG_CKIT_CORE_VECTOR),
    stringify(TAG_CKIT_CORE_STACK),
    stringify(TAG_CKIT_CORE_LINKED_LIST),
    stringify(TAG_CKIT_CORE_QUEUE),
    stringify(TAG_CKIT_CORE_FILE_SYSTEM),
    stringify(TAG_CKIT_CORE_HASHMAP),
    stringify(TAG_CKIT_CORE_HASHSET),

    stringify(TAG_CKIT_MODULE_LEXER),
    stringify(TAG_CKIT_MODULE_FILE_FORMAT_PARSER_BMP),
    stringify(TAG_CKIT_MODULE_FILE_FORMAT_PARSER_OBJ),
    stringify(TAG_CKIT_MODULE_FILE_FORMAT_PARSER_PNG),
    stringify(TAG_CKIT_MODULE_FILE_FORMAT_PARSER_JSON),

    stringify(TAG_CKIT_EXPECTED_USER_FREE),
};

#define CKIT_MEMORY_MAGIC "CKIT_MAGIC_MEMORY"
#define ckit_memory_header(data) ((CKIT_MemoryHeader*)((u8*)data - sizeof(CKIT_MemoryHeader)))

internal CKIT_MemoryTagPool* global_memory_tag_pool_vector = NULLPTR;
internal u64 global_total_pool_memory_used = 0;

internal void ckit_tracker_check_magic(void* data) {
    ckit_assert(ckit_str_equal(ckit_memory_header(data)->magic, CKIT_MEMORY_MAGIC));
}

internal CKIT_MemoryTagPool ckit_tracker_tag_pool_create(CKIT_MemoryTagID tag_id, const char* name) {
    CKIT_MemoryTagPool ret;
    ret.tag_id = tag_id;
    ret.pool_name = name;
    ret.total_pool_allocation_size = 0;
    ret.allocated_headers = ckg_linked_list_create(CKIT_MemoryHeader, TRUE);

    return ret;
}

internal CKIT_MemoryTag ckit_tracker_memory_tag_create(CKIT_MemoryTagID tag_id, const char* name) {
    CKIT_MemoryTag ret;
    ret.tag_id = tag_id;
    ret.tag_name = name;
    ret.allocation_info.allocation_size = 0;
    ret.allocation_info.file_name = NULLPTR;
    ret.allocation_info.line = -1;
    ret.allocation_info.function_name = NULLPTR;
    
    return ret;
}

internal u64 ckit_tracker_get_tag_pool_index(CKIT_MemoryTagID tag_id) {
    for (u32 i = 0; i < ckg_vector_count(global_memory_tag_pool_vector); i++) {
        if (global_memory_tag_pool_vector[i].tag_id == tag_id) {
            return i;
        }
    }
    
    ckit_assert(FALSE); // don't query a not registed tag_id
    return -1; // never gets here
}

internal Boolean ckit_tracker_tag_pool_exists(CKIT_MemoryTagID tag_id) {
    for (u32 i = 0; i < ckg_vector_count(global_memory_tag_pool_vector); i++) {
        if (global_memory_tag_pool_vector[i].tag_id == tag_id) {
            return TRUE;
        }
    }

    return FALSE;
}

internal const char* ckit_tracker_tag_to_string(CKIT_MemoryTagID tag_id) {
    for (u32 i = 0; i < ckg_vector_count(global_memory_tag_pool_vector); i++) {
        if (global_memory_tag_pool_vector[i].tag_id == tag_id) {
            return global_memory_tag_pool_vector[i].pool_name;
        }
    }

    ckit_assert(FALSE); // Don't query a tag you don't have registered
    return NULLPTR; // never gets here
}

void ckit_tracker_init() {
    global_memory_tag_pool_vector = NULLPTR;

    for (u32 i = 0; i < TAG_CKIT_RESERVED_COUNT; i++) {
        CKIT_MemoryTagPool tag_pool = ckit_tracker_tag_pool_create(reserved_tags[i], reserved_tags_stringified[i]);
        ckg_vector_push(global_memory_tag_pool_vector, tag_pool);
    }
}

// Date: July 22, 2024
// TODO(Jovanni): Think about this some more does this actually let me know where something is allocated?
// Maybe I need to actually have some linked list chain in the specific pool? (this would me I would know the type of the allocation because I know the pool)
CKIT_MemoryHeader ckit_tracker_header_create(CKIT_MemoryTagID tag_id, size_t allocation_size, const char* file_name, const u64 line, const char* function_name) {
    CKIT_MemoryHeader ret;
    ret.magic = CKIT_MEMORY_MAGIC;

    ret.tag.tag_id = tag_id;
    ret.tag.tag_name = ckit_tracker_tag_to_string(tag_id);
    ret.tag.allocation_info.file_name = file_name;
    ret.tag.allocation_info.line = line;
    ret.tag.allocation_info.function_name = function_name;
    ret.tag.allocation_info.allocation_size = allocation_size;

    return ret;
}

void ckit_tracker_register_tag_pool(CKIT_MemoryTagID tag_id, const char* name) {
    ckit_assert(!ckit_tracker_tag_pool_exists(tag_id)); // don't register a tag that already exists/has been registered

    CKIT_MemoryTagPool tag_pool = ckit_tracker_tag_pool_create(tag_id, name);
    ckg_vector_push(global_memory_tag_pool_vector, tag_pool);
}

void* MACRO_ckit_tracker_insert_header(void* data, CKIT_MemoryHeader header) {
  	((CKIT_MemoryHeader*)data)[0] = header;
  	data = (u8*)data + sizeof(CKIT_MemoryHeader);
  	return data;
}

void ckit_tracker_add(CKIT_MemoryHeader* header) {
    u64 tag_pool_index = ckit_tracker_get_tag_pool_index(header->tag.tag_id);
    
  	global_memory_tag_pool_vector[tag_pool_index].total_pool_allocation_size += (header->tag.allocation_info.allocation_size);
  	global_total_pool_memory_used += sizeof(CKIT_MemoryHeader) + header->tag.allocation_info.allocation_size;

    header->linked_list_address = ckg_linked_list_push(global_memory_tag_pool_vector[tag_pool_index].allocated_headers, header);
}

void ckit_tracker_remove(CKIT_MemoryHeader* header) {
    u64 tag_pool_index = ckit_tracker_get_tag_pool_index(header->tag.tag_id);

  	global_memory_tag_pool_vector[tag_pool_index].total_pool_allocation_size -= (header->tag.allocation_info.allocation_size);
  	global_total_pool_memory_used -= sizeof(CKIT_MemoryHeader) + header->tag.allocation_info.allocation_size;
    
    // Date: July 23, 2024
    // TODO(Jovanni): DETERMINE IF THIS IS A POINTER TYPE BECAUSE IF IT IS NOT YOUR ARE EXPECT TO FREE THE MEMORY, BY CHANGE IT NOT TO BE A POINTER TYPE;
    u32 index = ckg_linked_list_node_to_index(global_memory_tag_pool_vector[tag_pool_index].allocated_headers,  header->linked_list_address);
  	ckg_linked_list_remove(global_memory_tag_pool_vector[tag_pool_index].allocated_headers, index); 
}

void ckit_tracker_print_header(CKIT_MemoryHeader* header, CKG_LogLevel log_level) {
    u8* data_address = (u8*)header + sizeof(CKIT_MemoryHeader);
    log_output(log_level, "=>     Address: %p | Size: %d(Bytes)\n", data_address, header->tag.allocation_info.allocation_size);
    log_output(log_level, "      - Allocation Site:\n");
    log_output(log_level, "          - File: %s:%d\n", header->tag.allocation_info.file_name, header->tag.allocation_info.line);
    log_output(log_level, "          - Function: %s\n", header->tag.allocation_info.function_name);
}

void ckit_tracker_print_pool(CKIT_MemoryTagPool* pool, CKG_LogLevel log_level) {
    LOG_PRINT("============================== POOL NAME: %s | SIZE: %d ==============================\n", pool->pool_name, pool->total_pool_allocation_size);
    u32 count = pool->allocated_headers->count;
    for (u32 i = 0; i < count; i++) {
        CKIT_MemoryHeader* current_header = (CKIT_MemoryHeader*)ckg_linked_list_pop(pool->allocated_headers).data;
        ckit_tracker_print_header(current_header, log_level);
        if (i != count - 1) {
            LOG_PRINT("\n");
        }
    }
    LOG_PRINT("========================================================================================================\n");
}

CKIT_MemoryHeader* ckit_tracker_get_header(void* data) {
    CKIT_MemoryHeader* header = (CKIT_MemoryHeader*)((u8*)data - sizeof(CKIT_MemoryHeader));
    ckit_assert(ckit_tracker_tag_pool_exists(header->tag.tag_id));
    return header;
}

void ckit_tracker_print_all_pools(CKG_LogLevel log_level) {
    if (global_total_pool_memory_used == 0) {
        LOG_SUCCESS("--- No Memory Leaks Detected --- \n");
        return;
    }

    LOG_ERROR("---------------------- Memory Leak Detected: %d(Bytes) ----------------------\n", global_total_pool_memory_used);
    u32 count = ckg_vector_capacity(global_memory_tag_pool_vector);
    Boolean has_start = FALSE; 
    Boolean has_end = FALSE; 
    for (u32 i = 0; i < count; i++) {
        CKIT_MemoryTagPool pool = global_memory_tag_pool_vector[i];
        if (pool.total_pool_allocation_size == 0) {
            continue;
        }

        if (!has_start) {
            has_start = TRUE;
        } else if (has_start && !has_end) {
            has_end = TRUE;
        }
        
        if (has_end && i != ckg_vector_capacity(global_memory_tag_pool_vector) - 1) {
            LOG_PRINT("                                               |\n");
            LOG_PRINT("                                               |\n");
            has_end = FALSE;
        }

        ckit_tracker_print_pool(&pool, log_level);

    }
    LOG_ERROR("-----------------------------------------------------------------------------------------\n");
}

CKIT_MemoryHeader** ckit_tracker_get_all_headers();
CKIT_MemoryTagPool** ckit_tracker_get_all_pools();