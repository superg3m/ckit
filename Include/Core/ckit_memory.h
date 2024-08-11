#pragma once

#include "ckit_types.h"
#include "ckit_logger.h"
//========================== Begin Types ==========================
typedef enum CKIT_MemoryTagID { // Reserved tags
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

    TAG_CKIT_RESERVED_COUNT
} CKIT_MemoryTagID;

typedef struct CKG_LinkedList CKG_LinkedList;
typedef struct CKG_Node CKG_Node;

typedef struct CKIT_AllocationInfo {
    // allocation_site
    const char* file_name;
    s64 line;
    const char* function_name;
    size_t allocation_size;
} CKIT_AllocationInfo;

typedef struct CKIT_MemoryTag {
    CKIT_MemoryTagID tag_id;
    const char* tag_name;
    CKIT_AllocationInfo allocation_info;
} CKIT_MemoryTag;

typedef struct CKIT_MemoryTagPool {
    CKIT_MemoryTagID tag_id;
    const char* pool_name;
    CKG_LinkedList* allocated_headers;
    size_t total_pool_allocation_size;
} CKIT_MemoryTagPool;

typedef struct CKIT_MemoryHeader {
    CKIT_MemoryTag tag;
    CKG_Node* linked_list_address;
    const char* magic;
} CKIT_MemoryHeader;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif

    //
    // Memory Tracker
    //
    void ckit_tracker_init();
	void ckit_tracker_register_tag_pool(CKIT_MemoryTagID tag_id, const char* name);
    void* MACRO_ckit_tracker_insert_header(void* data, CKIT_MemoryHeader header);

    CKIT_MemoryHeader ckit_tracker_header_create(CKIT_MemoryTagID tag_id, size_t allocation_size, const char* file_name, const u64 line, const char* function_name);
	void* memory_insert_header(void* data, CKIT_MemoryHeader header);
    void ckit_tracker_add(CKIT_MemoryHeader* header);
    void ckit_tracker_remove(CKIT_MemoryHeader* header);
    CKIT_MemoryHeader* ckit_tracker_get_header(void* data);

    void ckit_tracker_print_header(CKIT_MemoryHeader* header, CKG_LogLevel log_level);
    void ckit_tracker_print_pool(CKIT_MemoryTagPool* pool, CKG_LogLevel log_level);

    void ckit_tracker_print_all_pools(CKG_LogLevel log_level);

    // CKIT_MemoryHeader** ckit_tracker_get_all_headers();
    // CKIT_MemoryTagPool** ckit_tracker_get_all_pools();

    //
    // Memory
    //
    void memory_init();
    void* MACRO_ckit_alloc(size_t number_of_bytes, CKIT_MemoryTagID tag_id, const char* file, const u32 line, const char* function);
    void* MACRO_ckit_realloc(void* data, u64 new_allocation_size, const char* file, const u32 line, const char* function);
    void* MACRO_ckit_free(void* data);

    void ckit_memory_report(CKG_LogLevel log_level);

    Boolean ckit_memory_compare(const void* buffer_one, const void* buffer_two, u32 b1_allocation_size, u32 b2_allocation_size);
    void ckit_memory_copy(const void* source, void* destination, size_t source_size, size_t destination_capacity);
    void ckit_memory_move(const void* source, void* destination, size_t source_payload_size);
    void ckit_memory_zero(void* data, size_t data_size_in_bytes);

    void MACRO_ckit_memory_delete_index(void* data, u32 data_capacity, size_t element_size_in_bytes, u32 index);
    void MACRO_ckit_memory_insert_index(void* data, u32 data_capacity, size_t element_size_in_bytes, u32 index);


#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_alloc(number_of_bytes) MACRO_ckit_alloc(number_of_bytes, TAG_USER_UNKNOWN, __FILE__, __LINE__, __func__)
#define ckit_alloc_custom(number_of_bytes, tag_id) MACRO_ckit_alloc(number_of_bytes, tag_id, __FILE__, __LINE__, __func__)
#define ckit_realloc(data, new_allocation_size) MACRO_ckit_realloc(data, new_allocation_size, __FILE__, __LINE__, __func__)
#define ckit_free(data) data = MACRO_ckit_free(data)

#define ckit_memory_delete_index(data, data_capacity, index) MACRO_ckit_memory_delete_index(data, data_capacity, sizeof(data[0]), index)
#define ckit_memory_insert_index(data, data_capacity, element, index) MACRO_ckit_memory_delete_index(data, data_capacity, sizeof(data[0]), index); data[index] = element;

#define ckit_tracker_insert_header(data, header) data = MACRO_ckit_tracker_insert_header(data, header)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

#if defined(CKIT_IMPL)
    #include "ckit_assert.h"
    #include "ckit_logger.h"
    #include "ckit_platform_services.h"
    #include "../../ckg/Include/ckg_memory.h"

        // 
    // Memory Tracker
    //
    #include "../../ckg/Include/ckg_linked_list.h"
    #include "../../ckg/Include/ckg_vector.h"
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
        ckit_assert(ckg_cstr_equal(ckit_memory_header(data)->magic, CKIT_MEMORY_MAGIC));
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

    internal Boolean ckit_tracker_tag_pool_exists(CKIT_MemoryTagID tag_id, const char* name) {
        for (u32 i = 0; i < ckg_vector_count(global_memory_tag_pool_vector); i++) {
            if (global_memory_tag_pool_vector[i].tag_id == tag_id || ckg_cstr_equal(global_memory_tag_pool_vector[i].pool_name, name)) {
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
        ckit_assert(!ckit_tracker_tag_pool_exists(tag_id, name)); // don't register a tag that already exists/has been registered (name can't be the same either)

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
        ckit_assert(ckit_tracker_tag_pool_exists(header->tag.tag_id, header->tag.tag_name));
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

    // CKIT_MemoryHeader** ckit_tracker_get_all_headers();
    // CKIT_MemoryTagPool** ckit_tracker_get_all_pools();

    //
    // Memory
    //
    void memory_init() {
        ckg_bind_alloc_callback(&platform_allocate);
        ckg_bind_free_callback(&platform_free);
    }

    void* MACRO_ckit_alloc(size_t byte_allocation_size, CKIT_MemoryTagID tag_id, const char* file, const u32 line, const char* function) {
        ckit_assert_msg(byte_allocation_size > 0, "Invalid allocation size zero or below\n");

        CKIT_MemoryHeader temp_header = ckit_tracker_header_create(tag_id, byte_allocation_size, file, line, function);

        void* data = ckg_alloc(sizeof(temp_header) + byte_allocation_size);
        ckit_memory_zero(data, sizeof(temp_header) + byte_allocation_size);
        ckit_tracker_insert_header(data, temp_header);
        CKIT_MemoryHeader* real_header = ckit_tracker_get_header(data); // this is probably a bad constraint I shouldn't have to think about getting the real header
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

    void* MACRO_ckit_realloc(void* data, u64 new_allocation_size, const char* file, const u32 line, const char* function) {
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


    void ckit_memory_report(CKG_LogLevel log_level) {
        /*
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
        */

        ckit_tracker_print_all_pools(log_level);
    }

    /*
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
#endif // CKIT_IMPL