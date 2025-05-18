#pragma once

#ifdef __cplusplus
    #define CKIT_API extern "C"
#else
    #define CKIT_API
#endif

// Date: May 18, 2025
// NOTE(Jovanni): ANYTIME YOU USE SOMETHING FROM CKG in the tagging code! YOU HAVE TO MAKE THE ALLOCATOR NOT TAG!

#if defined(CKIT_IMPL)
    #define CKIT_IMPL_INIT
    #define CKIT_IMPL_LOGGER
    #define CKIT_IMPL_ASSERT
    #define CKIT_IMPL_MEMORY
    #define CKIT_IMPL_ARENA
    #define CKIT_IMPL_STRING
	#define CKIT_IMPL_MATH
    #define CKIT_IMPL_CHAR
    #define CKIT_IMPL_COLLECTIONS
    #define CKIT_IMPL_OS
	#define CKIT_IMPL_UTILITIES

    // Date: September 25, 2024
    // TODO(Jovanni): CKIT_IMPL_THREADING
    // TODO(Jovanni): CKIT_IMPL_SIMD
    // TODO(Jovanni): CKIT_IMPL_CUDA
    // TODO(Jovanni): CKIT_IMPL_FILE_WATCHER
    // TODO(Jovanni): CKIT_IMPL_PARSER
#endif

#define CKIT_INCLUDE_INIT
#define CKIT_INCLUDE_LOGGER
#define CKIT_INCLUDE_ASSERT
#define CKIT_INCLUDE_MEMORY
#define CKIT_INCLUDE_ARENA
#define CKIT_INCLUDE_STRING
#define CKIT_INCLUDE_MATH
#define CKIT_INCLUDE_CHAR
#define CKIT_INCLUDE_COLLECTIONS
#define CKIT_INCLUDE_OS
#define CKIT_INCLUDE_UTILITIES


#if defined(CKIT_INCLUDE_INIT)
	#include "./ckg/ckg.h"

	void ckit_init();
	void ckit_cleanup(bool generate_memory_report);
#endif

#if defined(CKIT_INCLUDE_ASSERT)
    #define CKIT_ASSERT_ENABLED true

    #if CKIT_ASSERT_ENABLED == true
        #define ckit_assert_msg(expression, message, ...) ckg_assert_msg(expression, message, ##__VA_ARGS__)
        #define ckit_assert(expression) ckg_assert(expression)
    #else
        #define ckit_assert_msg(expression, message, ...)
        #define ckit_assert(expression) 
    #endif
#endif

#if defined(CKIT_INCLUDE_LOGGER)
    typedef CKG_LogLevel CKIT_LogLevel;

    CKIT_API void MACRO_ckit_log_output(CKIT_LogLevel log_level, char* message, ...);
    #define ckit_log_output(log_level, message, ...) MACRO_ckit_log_output(log_level, message, ##__VA_ARGS__)
    #define LOG_PRINT(message, ...) ckit_log_output(LOG_LEVEL_PRINT, message, ##__VA_ARGS__)
    #define LOG_SUCCESS(message, ...) ckit_log_output(LOG_LEVEL_SUCCESS, message, ##__VA_ARGS__)
    #define LOG_DEBUG(message, ...) ckit_log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
    #define LOG_WARN(message, ...) ckit_log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__)
    #define LOG_ERROR(message, ...) ckit_log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
    #define LOG_FATAL(message, ...) ckit_log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
#endif

#if defined(CKIT_INCLUDE_MEMORY)
    internal CKG_LinkedList* registered_arenas = NULLPTR;

    // Date: March 23, 2025
    // TODO(Jovanni): Make the reserved space like 256+ so userspace has nice numbers
    typedef enum CKIT_MemoryTagID { // Reserved tags
        TAG_USER_UNKNOWN,

        TAG_CKIT_TEMP,
        TAG_CKIT_CORE_STRING,
        TAG_CKIT_CORE_ARENA,
        TAG_CKIT_CORE_VECTOR,
        TAG_CKIT_CORE_STACK,
        TAG_CKIT_CORE_LINKED_LIST,
        TAG_CKIT_CORE_QUEUE,
        TAG_CKIT_CORE_IO,
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
        const char* file_name;
        const char* function_name;
        s64 line;
        size_t allocation_size;
    } CKIT_AllocationInfo;

    typedef struct CKIT_MemoryTag {
        CKIT_MemoryTagID tag_id;
        char* tag_name;
        CKIT_AllocationInfo allocation_info;
    } CKIT_MemoryTag;

    typedef struct CKIT_MemoryHeader {
        CKIT_MemoryTag tag;
        CKG_Node* linked_list_address;
        char* magic;
    } CKIT_MemoryHeader;

    CKIT_API void ckit_memory_tracker_init();
    CKIT_API void ckit_memory_tracker_register_tag_pool(CKIT_MemoryTagID tag_id, const char* name);

    CKIT_API void ckit_memory_tracker_print_header(CKIT_MemoryHeader* header, CKIT_LogLevel log_level);
    CKIT_API void ckit_memory_tracker_print_all_pools(CKIT_LogLevel log_level);


    // Allocation
    typedef CKG_Alloc_T CKIT_Alloc_T;
    typedef CKG_Free_T CKIT_Free_T;
    CKIT_API void ckit_bind_custom_allocator(CKIT_Alloc_T* a, CKIT_Free_T* f, void* user_ctx);
    CKIT_API void* MACRO_ckit_alloc(size_t byte_allocation_size, CKIT_MemoryTagID tag_id, const char* file, const char* function, u32 line);
    CKIT_API void* MACRO_ckit_realloc(void* data, u64 new_allocation_size, const char* file, const char* function, u32 line);
    CKIT_API void* MACRO_ckit_free(void* data);

    CKIT_API void ckit_memory_report(CKIT_LogLevel log_level);

    CKIT_API bool ckit_memory_compare(void* buffer_one, void* buffer_two, u32 b1_allocation_size, u32 b2_allocation_size);
    CKIT_API void ckit_memory_copy(void* source, void* destination, size_t source_size, size_t destination_capacity);
    CKIT_API void ckit_memory_move(void* source, void* destination, size_t source_payload_size);
    CKIT_API void ckit_memory_zero(void* data, size_t data_size_in_bytes);

    CKIT_API void MACRO_ckit_memory_delete_index(void* data, u32 number_of_elements, u32 data_capacity, size_t element_size_in_bytes, u32 index);
    CKIT_API void MACRO_ckit_memory_insert_index(void* data, u32 number_of_elements, u32 data_capacity, size_t element_size_in_bytes, u32 index);
    #define ckit_alloc(number_of_bytes) MACRO_ckit_alloc(number_of_bytes, TAG_USER_UNKNOWN, __FILE__, __LINE__, __func__)
    #define ckit_alloc_custom(number_of_bytes, tag_id) MACRO_ckit_alloc(number_of_bytes, tag_id, __FILE__, __LINE__, __func__)
    #define ckit_realloc(data, new_allocation_size) MACRO_ckit_realloc(data, new_allocation_size, __FILE__, __LINE__, __func__)
    #ifdef __cplusplus
        #define ckit_free(data) data = (decltype(data))MACRO_ckit_free(data)
    #else 
        #define ckit_free(data) data = MACRO_ckit_free(data)
    #endif

    #define ckit_memory_delete_index(data, number_of_elements, data_capacity, index) MACRO_ckit_memory_delete_index(data, number_of_elements, data_capacity, sizeof(data[0]), index)
    #define ckit_memory_insert_index(data, number_of_elements, data_capacity, element, index) MACRO_ckit_memory_delete_index(data, number_of_elements, data_capacity, sizeof(data[0]), index); data[index] = element;
#endif

#if defined(CKIT_INCLUDE_ARENA)
    // Date: September 24, 2024
    // TODO(Jovanni): Just make this defines
    //========================== Begin Types ==========================
    typedef enum CKIT_ArenaFlag {
        CKIT_ARENA_FLAG_FIXED,
        CKIT_ARENA_FLAG_CIRCULAR,
        CKIT_ARENA_FLAG_EXTENDABLE_PAGES,
        CKIT_ARENA_FLAG_COUNT
    } CKIT_ArenaFlag;

    typedef struct CKIT_ArenaPage {
        u8* base_address;
        size_t capacity;
        size_t used;
    } CKIT_ArenaPage;

    typedef struct CKIT_Arena {
        char* name;
        CKG_LinkedList* pages;
        CKIT_ArenaFlag flag;
        u8 alignment;
    } CKIT_Arena;
    //=========================== End Types ===========================

    CKIT_API CKIT_Arena* MACRO_ckit_arena_create(size_t allocation_size, char* name, CKIT_ArenaFlag flag, u8 alignment);
    CKIT_API void* MACRO_ckit_arena_push(CKIT_Arena* arena, size_t element_size);	
    CKIT_API CKIT_Arena* MACRO_ckit_arena_free(CKIT_Arena* arena);
    CKIT_API void ckit_arena_clear(CKIT_Arena* arena);
    CKIT_API size_t ckit_arena_used(CKIT_Arena* arena);
    CKIT_API size_t ckit_arena_capacity(CKIT_Arena* arena);

    #define ckit_arena_create(allocation_size, name) MACRO_ckit_arena_create(allocation_size, name, CKIT_ARENA_FLAG_EXTENDABLE_PAGES, 0)
    #define ckit_arena_create_custom(allocation_size, name, flags, alignment) MACRO_ckit_arena_create(allocation_size, name, flags, alignment)
    #define ckit_arena_free(arena) arena = MACRO_ckit_arena_free(arena)
    #define ckit_arena_push(arena, type) ((type*)MACRO_ckit_arena_push(arena, sizeof(type)))
    #define ckit_arena_push_array(arena, type, element_count) ((type*)MACRO_ckit_arena_push(arena, sizeof(type) * element_count))
#endif 

#if defined(CKIT_IMPL_MEMORY)
    typedef CKG_Allocator CKIT_Allocator;
    typedef struct CKIT_Context {
        CKIT_MemoryTagID tag_id;
        size_t allocation_size;
        const char* file_name;
        const char* function_name;
        u64 line;

        void* user_ctx;
    } CKIT_Context;

    typedef struct CKIT_MemoryTagPool {
        CKIT_MemoryTagID tag_id;
        char* pool_name;
        CKG_LinkedList* allocated_headers;
        size_t total_pool_allocation_size;
    } CKIT_MemoryTagPool;

    internal void* cit_default_libc_malloc(CKIT_Allocator* allocator, size_t allocation_size) {
        (void)allocator;
        return malloc(allocation_size);
    }

    internal void ckit_default_libc_free(CKIT_Allocator* allocator, void* data) {
        (void)allocator;
        free(data);
    }

    internal CKIT_Allocator global_allocator = {cit_default_libc_malloc, ckit_default_libc_free, NULLPTR};
    // Date: May 18, 2025
    // TODO(Jovanni): Replace this with a hashmap
    internal CKIT_MemoryTagPool* global_memory_tag_pool_vector = NULLPTR;
    internal u64 global_total_pool_memory_used = 0;
    internal u64 global_total_pool_memory_internal = 0;

    // 
    // Start Memory Tracker
    //
    #define CKIT_MEMORY_MAGIC "CKIT_MAGIC_MEMORY"
    #define ckit_memory_header(data) ((CKIT_MemoryHeader*)((u8*)data - sizeof(CKIT_MemoryHeader)))

    internal void ckit_tracker_check_magic(void* data) {
        ckit_assert(ckg_str_equal(ckit_memory_header(data)->magic, sizeof(CKIT_MEMORY_MAGIC) - 1, CKG_LIT_ARG(CKIT_MEMORY_MAGIC)));
    }

    UNUSED_FUNCTION internal CKIT_MemoryTag ckit_tracker_memory_tag_create(CKIT_MemoryTagID tag_id, char* name)  {
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
        
        ckit_assert(false); // don't query a not registed tag_id
        return 0; // never gets here
    }

    internal bool ckit_tracker_tag_pool_exists(CKIT_MemoryTagID tag_id, char* name) {
        u64 name_length = ckg_cstr_length(name);

        for (u32 i = 0; i < ckg_vector_count(global_memory_tag_pool_vector); i++) {
            u64 pool_name_length = ckg_cstr_length(global_memory_tag_pool_vector[i].pool_name);
            if (global_memory_tag_pool_vector[i].tag_id == tag_id || ckg_str_equal(global_memory_tag_pool_vector[i].pool_name, pool_name_length, name, name_length)) {
                return true;
            }
        }

        return false;
    }

    internal char* ckit_tracker_tag_to_string(CKIT_MemoryTagID tag_id) {
        for (u32 i = 0; i < ckg_vector_count(global_memory_tag_pool_vector); i++) {
            if (global_memory_tag_pool_vector[i].tag_id == tag_id) {
                return global_memory_tag_pool_vector[i].pool_name;
            }
        }

        ckit_assert(false); // Don't query a tag you don't have registered
        return NULLPTR; // never gets here
    }

    internal CKIT_MemoryHeader ckit_memory_header_create(CKIT_MemoryTagID tag_id, size_t allocation_size, const char* file_name, const char* function_name, const u64 line) {
        CKIT_MemoryHeader ret;
        ret.magic = CKIT_MEMORY_MAGIC;

        ret.tag.tag_id = tag_id;
        ret.tag.tag_name = ckit_tracker_tag_to_string(tag_id);
        ret.tag.allocation_info.file_name = file_name;
        ret.tag.allocation_info.function_name = function_name;
        ret.tag.allocation_info.line = line;
        ret.tag.allocation_info.allocation_size = allocation_size;

        return ret;
    }

    
    void* ckit_allocate_wrapper(CKIT_Allocator* allocator, size_t allocation_size);
    void ckit_free_wrapper(CKIT_Allocator* allocator, void* data);
    void ckit_memory_tracker_add(CKIT_MemoryHeader* header) {
        u64 tag_pool_index = ckit_tracker_get_tag_pool_index(header->tag.tag_id);
        
        global_memory_tag_pool_vector[tag_pool_index].total_pool_allocation_size += header->tag.allocation_info.allocation_size;
        global_total_pool_memory_internal += sizeof(CKIT_MemoryHeader);
        global_total_pool_memory_used += header->tag.allocation_info.allocation_size;

        ckg_bind_custom_allocator(global_allocator.allocate, global_allocator.free, global_allocator.ctx);
        header->linked_list_address = ckg_linked_list_push(global_memory_tag_pool_vector[tag_pool_index].allocated_headers, header);
        ckg_bind_custom_allocator(ckit_allocate_wrapper, ckit_free_wrapper, global_allocator.ctx);
    }

    void ckit_memory_tracker_remove(CKIT_MemoryHeader* header) {
        u64 tag_pool_index = ckit_tracker_get_tag_pool_index(header->tag.tag_id);

        global_memory_tag_pool_vector[tag_pool_index].total_pool_allocation_size -= header->tag.allocation_info.allocation_size;
        global_total_pool_memory_internal -= sizeof(CKIT_MemoryHeader);
        global_total_pool_memory_used -= header->tag.allocation_info.allocation_size;
        
        u64 index = ckg_linked_list_node_to_index(global_memory_tag_pool_vector[tag_pool_index].allocated_headers,  header->linked_list_address);

        ckg_bind_custom_allocator(global_allocator.allocate, global_allocator.free, global_allocator.ctx);
        ckg_linked_list_remove(global_memory_tag_pool_vector[tag_pool_index].allocated_headers, index); 
        ckg_bind_custom_allocator(ckit_allocate_wrapper, ckit_free_wrapper, global_allocator.ctx);
    }

    internal void ckit_tracker_print_header(CKIT_MemoryHeader* header, CKIT_LogLevel log_level) {
        u8* data_address = (u8*)header + sizeof(CKIT_MemoryHeader);
        
        ckit_log_output(log_level, "=>  Address: %p | Size: %d(Bytes)\n", data_address, header->tag.allocation_info.allocation_size);
        ckit_log_output(log_level, "        - Allocation Site:\n");
        ckit_log_output(log_level, "            - File: %s:%d\n", header->tag.allocation_info.file_name, header->tag.allocation_info.line);
        ckit_log_output(log_level, "            - Function: %s\n", header->tag.allocation_info.function_name);
    }

    internal void ckit_tracker_print_pool(CKIT_MemoryTagPool* pool, CKIT_LogLevel log_level) {
        LOG_PRINT("============================== POOL NAME: %s | SIZE: %lld | Items: %d ==============================\n", pool->pool_name, pool->total_pool_allocation_size, pool->allocated_headers->count);
        u64 count = pool->allocated_headers->count;
        for (u64 i = 0; i < count; i++) {
            CKIT_MemoryHeader* current_header = (CKIT_MemoryHeader*)ckg_linked_list_pop(pool->allocated_headers).data;
            ckit_tracker_print_header(current_header, log_level);
            if (i != count - 1) {
                LOG_PRINT("\n");
            }
        }
        LOG_PRINT("========================================================================================================\n");
    }

    void ckit_tracker_print_all_pools(CKIT_LogLevel log_level) {
        u64 arena_allocation_capacity = 0;
        u64 arena_allocation_internal = 0;

        for (u32 i = 0; i < ckg_vector_count(global_memory_tag_pool_vector); i++) {
            CKIT_MemoryTagPool current_pool = global_memory_tag_pool_vector[i];
            if (current_pool.tag_id == TAG_CKIT_CORE_ARENA) {
                arena_allocation_capacity = current_pool.total_pool_allocation_size;
                arena_allocation_internal = current_pool.allocated_headers->count * sizeof(CKIT_MemoryHeader);
                break;
            }
        }

        if ((global_total_pool_memory_used - arena_allocation_capacity) == 0 && (global_total_pool_memory_internal - arena_allocation_internal) == 0) {
            LOG_SUCCESS("--- No Memory Leaks Detected ---\n");
            return;
        }

        u64 total_memory_without_arena = global_total_pool_memory_used - arena_allocation_capacity;
        u64 total_memory_without_arena_internal = global_total_pool_memory_internal - arena_allocation_internal;

        LOG_ERROR("---------------------- Memory Leak Detected: %d(total) + %d(internal) = %d(Bytes) ----------------------\n", total_memory_without_arena, total_memory_without_arena_internal, total_memory_without_arena + total_memory_without_arena_internal);

        u64 count = ckg_vector_capacity(global_memory_tag_pool_vector);
        bool has_start = false; 
        bool has_end = false; 
        for (u64 i = 0; i < count; i++) {
            CKIT_MemoryTagPool pool = global_memory_tag_pool_vector[i];
            if (pool.tag_id == TAG_CKIT_CORE_ARENA || pool.total_pool_allocation_size == 0) {
                continue;
            }

            if (!has_start) {
                has_start = true;
            } else if (has_start && !has_end) {
                has_end = true;
            }
            
            if (has_end && i != ckg_vector_capacity(global_memory_tag_pool_vector) - 1) {
                LOG_PRINT("                                               |\n");
                LOG_PRINT("                                               |\n");
                has_end = false;
            }

            ckit_tracker_print_pool(&pool, log_level);

        }

        LOG_ERROR("------------------------------------------------------------------------------------------------------------\n");
    }
    // 
    // End Memory Tracker
    //

    
    // 
    // Start Memory Operations
    //
bool ckit_memory_compare(void* buffer_one, void* buffer_two, u32 b1_allocation_size, u32 b2_allocation_size) {
        return ckg_memory_compare(buffer_one, buffer_two, b1_allocation_size, b2_allocation_size);
    }

    void ckit_memory_copy(void* source, void* destination, size_t source_size_in_bytes, size_t destination_size_in_bytes) {
        ckg_memory_copy(source, destination, source_size_in_bytes, destination_size_in_bytes);
    }

    void ckit_memory_zero(void* data, size_t data_size_in_bytes) {
        ckg_memory_zero(data, data_size_in_bytes);
    }

    void MACRO_ckit_memory_delete_index(void* data, u32 number_of_elements, u32 data_capacity, size_t element_size_in_bytes, u32 index) {
        MACRO_ckg_memory_delete_index(data, number_of_elements, data_capacity, element_size_in_bytes, index);
    }

    void MACRO_ckit_memory_insert_index(void* data, u32 number_of_elements, u32 data_capacity, size_t element_size_in_bytes, u32 index) {
        MACRO_ckg_memory_insert_index(data, number_of_elements, data_capacity, element_size_in_bytes, index);
    }

    void ckit_memory_report(CKIT_LogLevel log_level) {
        ckit_tracker_print_all_pools(log_level);
    }

    void ckit_memory_arena_register(CKIT_Arena* arena) {
        ckg_linked_list_push(registered_arenas, arena);
    }

    void ckit_memory_arena_unregister(CKIT_Arena* arena) {
        for (u32 i = 0; i < registered_arenas->count; i++) {
            CKIT_Arena* current_arena = (CKIT_Arena*)ckg_linked_list_get(registered_arenas, i);
            if (arena == current_arena) {
                ckg_linked_list_remove(registered_arenas, i);
                break;
            }
        }
    }

    void ckit_memory_arena_unregister_all() {
        for (u32 i = 0; i < registered_arenas->count; i++) {
            CKIT_Arena* arena = (CKIT_Arena*)ckg_linked_list_pop(registered_arenas).data;
            ckit_arena_free(arena);
        }

        ckg_linked_list_free(registered_arenas);
    }
    // 
    // End Memory Operations
    //

    // 
    // Start Allocator
    //
    void* ckit_allocate_wrapper(CKIT_Allocator* allocator, size_t allocation_size) {
        (void)allocator;
        CKIT_Context* ctx = allocator->ctx;

        u8* ret = ckg_alloc(sizeof(CKIT_MemoryHeader) + allocation_size);
        *((CKIT_MemoryHeader*)ret) = ckit_memory_header_create(ctx->tag_id, allocation_size, ctx->file_name, ctx->function_name, ctx->line);
        ckit_memory_tracker_add((CKIT_MemoryHeader*)ret);
        ret += sizeof(CKIT_MemoryHeader);
    
        return ret;
    }
    
    void ckit_free_wrapper(CKIT_Allocator* allocator, void* data) {
        (void)allocator;

        ckit_tracker_check_magic(data);
        CKIT_MemoryHeader* header = (CKIT_MemoryHeader*)((u8*)data - sizeof(CKIT_MemoryHeader));
        ckit_assert_msg(ckit_tracker_tag_pool_exists(header->tag.tag_id, header->tag.tag_name), "Tag id or tag_name must be corrupted\n");

        ckit_memory_tracker_remove(header);
        ckg_free(header);
    }

    void ckit_init() {
        CKIT_Context context = {0};
        ckg_bind_custom_allocator(ckit_allocate_wrapper, ckit_free_wrapper, &context);
        // ckit_tracker_init();
        // memory_init();
        // platform_console_init();
        // ckit_str_register_arena();
    }

    void ckit_bind_custom_allocator(CKIT_Alloc_T* a, CKIT_Free_T* f, void* user_ctx) {
        ckit_assert_msg(a, "Alloc function is NULLPTR\n");
        ckit_assert_msg(f, "Free function is NULLPTR\n");

        global_allocator.allocate = a;
        global_allocator.free = f;
        CKIT_Context* ctx = global_allocator.ctx;
        ctx->user_ctx = user_ctx;

        ckg_bind_custom_allocator(ckit_allocate_wrapper, ckit_free_wrapper, user_ctx);
    }

    void* MACRO_ckit_alloc(size_t byte_allocation_size, CKIT_MemoryTagID tag_id, const char* file, const char* function, const u32 line) {
        CKIT_Context* ctx = global_allocator.ctx;
        ctx->tag_id = tag_id;
        ctx->allocation_size = byte_allocation_size;
        ctx->file_name = file;
        ctx->function_name = function;
        ctx->line = line;


        return ckg_alloc(byte_allocation_size);
    }

    void* MACRO_ckit_free(void* data) {
        return ckg_free(data);
    }

    void* MACRO_ckit_realloc(void* data, u64 new_allocation_size, const char* file, const char* function, u32 line) {
        ckit_assert_msg(data, "ckit_reallocation: Data passed is null\n");

        CKIT_MemoryHeader* header = ckit_memory_header(data);
        void* ret_data = MACRO_ckit_alloc(new_allocation_size, header->tag.tag_id, file, function, line);
        ckit_memory_copy(data, ret_data, header->tag.allocation_info.allocation_size, new_allocation_size);
        ckit_free(data);

        return ret_data;
    }
    // 
    // End Allocator
    //
#endif
