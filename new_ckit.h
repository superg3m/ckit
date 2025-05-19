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

#if defined(CKIT_INCLUDE_STRING)
    typedef struct CKIT_StringHeader {
        u64 length; 
        u64 capacity;
        char* magic; 
    } CKIT_StringHeader;
    
    typedef char* String;

    CKIT_API String ckit_str_create_custom(char* c_str, u64 length, u64 capacity);
    CKIT_API u64 ckit_str_length(String str);
    CKIT_API bool ckit_str_equal(String str1, String str2);
    CKIT_API void ckit_str_copy(String str, char* source);

    // Date: May 10, 2025
    // TODO(Jovanni): I think all cstr stuff will be in ckg?
    CKIT_API char* ckit_cstr_va_sprint(u64* allocation_size_ptr, char* fmt, va_list args);
    CKIT_API char* MACRO_ckit_cstr_sprint(u64* allocation_size_ptr, char* fmt, ...);

    CKIT_API String ckit_str_va_sprint(u64* allocation_size_ptr, char* fmt, va_list args);
    CKIT_API String MACRO_ckit_str_sprint(u64* allocation_size_ptr, char* fmt, ...);

    CKIT_API String MACRO_ckit_str_append(String str, char* source);
    CKIT_API String MACRO_ckit_str_append_char(String str, char source);

    CKIT_API String MACRO_ckit_str_insert(String str, char* to_insert, u64 to_insert_length, u64 index);
    CKIT_API String MACRO_ckit_str_insert_char(String str, char to_insert, u64 index);

    // Date: September 27, 2024
    // NOTE(Jovanni): This is a bit scary if you clear with the wrong one the length with be wrong!

    // Date: May 10, 2025
    // NOTE(Jovanni): This is also why its probably best if cstr stuff is under a different namespace ckg
    CKIT_API void ckit_str_clear(String str1);
    CKIT_API void ckit_cstr_clear(char* str1);

    // If you are copying data to the string and need to update the header state specifically for length
    CKIT_API void ckit_str_recanonicalize_header_length(String str);

    // Date: May 10, 2025
    // TODO(Jovanni): There should be very few things that allocate everything else should be a CKG_StringView
    CKIT_API String ckit_substring(char* string_buffer, u64 start_range, u64 end_range);

    // Little bit tricky. This method returns a vector of strings so 
    // ckit_vector_count(): to get the number of strings it returned
    //
    //  for (u32 i = 0; i < ckit_vector_count(string_vector); i++) {
    //     LOG_PRINT("%s\n", string_vector[i]);
    //  }
    //  ckit_vector_free(string_vector);
    CKIT_API String* ckit_str_split(char* string_buffer, char* delimitor);
    // Date: September 09, 2024
    // TODO(Jovanni): String* ckit_str_split_with_char(char* string_buffer, char delimitor);

    CKIT_API String ckit_str_reverse(char* str, u64 str_length);
    CKIT_API String ckit_str_int_to_str(int number);
    CKIT_API String ckit_str_between_delimiters(char* str, u64 str_length, char* start_delimitor, u64 start_delimitor_length, char* end_delimitor, u64 end_delimitor_length);

    #define ckit_str_create(str) ckit_str_create_custom(str, 0, 0)
    #define ckit_str_insert(str, source, index) str = MACRO_ckit_str_insert(str, source, index);
    #define ckit_str_insert_char(str, source, index) str = MACRO_ckit_str_insert_char(str, source, index);
    #define ckit_str_append(str, source) str = MACRO_ckit_str_append(str, source);
    #define ckit_str_append_char(str, source) str = MACRO_ckit_str_append_char(str, source);

    #define ckit_str_sprint(allocation_size_ptr, fmt, ...) MACRO_ckit_str_sprint(allocation_size_ptr, fmt, ##__VA_ARGS__)
    #define ckit_cstr_sprint(allocation_size_ptr, fmt, ...) MACRO_ckit_cstr_sprint(allocation_size_ptr, fmt, ##__VA_ARGS__)
#endif

//
// ===================================================== CKIT_IMPL =====================================================
//

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

#if defined(CKIT_IMPL_ARENA)
    #define ARENA_DEFAULT_ALLOCATION_SIZE MegaBytes(1)

    void ckit_memory_arena_register(CKIT_Arena* arena);
    void ckit_memory_arena_unregister(CKIT_Arena* arena);

    bool ckit_arena_flag_is_set(CKIT_Arena* arena, CKIT_ArenaFlag flag) {
        return arena->flag == flag;
    }

    internal CKIT_ArenaPage* ckit_arena_page_create(size_t allocation_size) {
        CKIT_ArenaPage* ret = (CKIT_ArenaPage*)ckit_alloc_custom(sizeof(CKIT_ArenaPage), TAG_CKIT_CORE_ARENA);
        ret->used = 0;
        ret->capacity = allocation_size != 0 ? allocation_size : ARENA_DEFAULT_ALLOCATION_SIZE;
        ret->base_address = (u8*)ckit_alloc_custom(ret->capacity, TAG_CKIT_CORE_ARENA);

        return ret;
    }

    CKIT_Arena* MACRO_ckit_arena_create(size_t allocation_size, char* name, CKIT_ArenaFlag flag, u8 alignment) {
        CKIT_Arena* arena = (CKIT_Arena*)ckit_alloc_custom(sizeof(CKIT_Arena), TAG_CKIT_CORE_ARENA);
        arena->alignment = alignment == 0 ? 8 : alignment;
        arena->name = name;
        arena->flag = flag;
        arena->pages = ckg_linked_list_create(CKIT_ArenaPage*, true);
        CKIT_ArenaPage* inital_page = ckit_arena_page_create(allocation_size);
        ckg_linked_list_push(arena->pages, inital_page);
        ckit_memory_arena_register(arena);

        return arena;
    }

    CKIT_Arena* MACRO_ckit_arena_free(CKIT_Arena* arena) {
        ckit_assert(arena);

        u64 cached_count = arena->pages->count;
        for (u64 i = 0; i < cached_count; i++) {
            CKIT_ArenaPage* page = (CKIT_ArenaPage*)ckg_linked_list_remove(arena->pages, 0).data;
            ckit_assert(page->base_address);
            ckit_free(page->base_address);
            ckit_free(page);
        }
        ckg_linked_list_free(arena->pages);

        ckit_memory_arena_unregister(arena);
        ckit_free(arena);

        return arena;
    }

    void ckit_arena_clear(CKIT_Arena* arena) {
        ckit_assert(arena);

        for (u64 i = 0; i < arena->pages->count; i++) {
            CKIT_ArenaPage* page = (CKIT_ArenaPage*)ckg_linked_list_get(arena->pages, i);
            ckit_assert(page->base_address);
            ckit_memory_zero(page->base_address, page->used);
            page->used = 0;
        }
    }

    CKIT_API u64 ckit_arena_used(CKIT_Arena* arena) {
        u64 total_used = 0;
        for (u64 i = 0; i < arena->pages->count; i++) {
            CKIT_ArenaPage* page = (CKIT_ArenaPage*)ckg_linked_list_get(arena->pages, i);
            total_used += page->used;
        }

        return total_used;
    }

    CKIT_API u64 ckit_arena_capacity(CKIT_Arena* arena) {
        u64 total_capacity = 0;
        for (u64 i = 0; i < arena->pages->count; i++) {
            CKIT_ArenaPage* page = (CKIT_ArenaPage*)ckg_linked_list_get(arena->pages, i);
            total_capacity += page->capacity;
        }

        return total_capacity;
    }

    void* MACRO_ckit_arena_push(CKIT_Arena* arena, size_t element_size) {
        ckit_assert(arena);

        CKIT_ArenaPage* last_page = (CKIT_ArenaPage*)ckg_linked_list_peek_tail(arena->pages);
        if (ckit_arena_flag_is_set(arena, CKIT_ARENA_FLAG_FIXED)) { // single page assert if you run out of memory
            ckit_assert((last_page->used + element_size <= last_page->capacity));
        } else if (ckit_arena_flag_is_set(arena, CKIT_ARENA_FLAG_CIRCULAR)) { // single page circle around if you run out of memory
            if ((last_page->used + element_size > last_page->capacity)) {
                last_page->used = 0;
                ckit_assert((last_page->used + element_size <= last_page->capacity));
            }
        } else if (ckit_arena_flag_is_set(arena, CKIT_ARENA_FLAG_EXTENDABLE_PAGES)) { // Allocate extra pages of memory, doesn't invalidate pointers
            ckit_assert(last_page->base_address);
            if ((last_page->used + element_size > last_page->capacity)) {
                CKIT_ArenaPage* next_page = ckit_arena_page_create((last_page->capacity + element_size) * 2);
                ckit_assert(next_page->base_address);
                ckg_linked_list_push(arena->pages, next_page);
            }
        } else {
            ckit_assert(false);
        }

        last_page = (CKIT_ArenaPage*)ckg_linked_list_peek_tail(arena->pages); // tail might change

        u8* ret = last_page->base_address + last_page->used;
        last_page->used += element_size;

        u64 ALIGNMENT_MASK = (arena->alignment - 1);
        u64 REMAINDER = (last_page->used & ALIGNMENT_MASK);

        if (REMAINDER != 0) {
            last_page->used += (arena->alignment - REMAINDER);
        }

        return ret;
    }
#endif

#if defined(CKIT_IMPL_STRING)
    #define ckit_str_header(string) ((CKIT_StringHeader*)(string - sizeof(CKIT_StringHeader)))
    #define CKIT_STR_MAGIC "CKIT_MAGIC_STRING"

    CKIT_Arena* string_arena;

    void ckit_str_register_arena() {
        #define STRING_ARENA_DEFAULT_CAPACITY MegaBytes(2)
        string_arena = ckit_arena_create_custom(STRING_ARENA_DEFAULT_CAPACITY, "String Arena", CKIT_ARENA_FLAG_EXTENDABLE_PAGES, sizeof(char));
    }

    internal void ckit_str_check_magic(String str) {
        ckit_assert_msg(str, "String: %s is null can't check magic: (%s) Likely not a CKIT_String\n", str, CKIT_STR_MAGIC);
        ckit_assert_msg(ckg_str_equal(ckit_str_header(str)->magic, sizeof(CKIT_STR_MAGIC) - 1, CKIT_STR_MAGIC, sizeof(CKIT_STR_MAGIC) - 1), "String: %s has the wrong magic: {%s} got: {%s} \n", str, CKIT_STR_MAGIC, ckit_str_header(str)->magic);
    }

    internal inline String ckit_str_grow(String str, u64 new_allocation_size) {
        ckit_str_check_magic(str);
        CKIT_StringHeader header = *ckit_str_header(str);
        header.capacity = new_allocation_size;
        String ret = ckit_str_create_custom(str, header.length, header.capacity);
        
        return ret;
    }

    String ckit_str_create_custom(char* c_string, u64 length, u64 capacity) {
        u64 c_str_length = length ? length : ckg_cstr_length(c_string);
        u64 true_capacity = capacity != 0 ? capacity : sizeof(char) * (c_str_length + 1);
        CKIT_StringHeader* header = (CKIT_StringHeader*)MACRO_ckit_arena_push(string_arena, sizeof(CKIT_StringHeader) + true_capacity);
        header->length = c_str_length;
        header->capacity = true_capacity;
        header->magic = CKIT_STR_MAGIC;

        String ret = (String)((u8*)header + sizeof(CKIT_StringHeader));

        ckg_str_copy(ret, header->capacity, c_string, header->length);
        return ret;
    }

    bool ckit_str_equal(String str1, String str2) {
        ckit_str_check_magic(str1);
        ckit_str_check_magic(str2);

        return ckg_str_equal(str1, ckit_str_length(str1), str2, ckit_str_length(str2));
    }

    String MACRO_ckit_str_insert(String str, char* to_insert, u64 to_insert_length, u64 index) {
        ckit_str_check_magic(str);

        u64 to_insert_capacity = to_insert_length + 1;
        CKIT_StringHeader* header = ckit_str_header(str);
        if (header->length + to_insert_capacity >= header->capacity) {
            str = ckit_str_grow(str, (header->length + to_insert_capacity) * 2);
            header = ckit_str_header(str);
        }

        ckg_str_insert(str, ckit_str_length(str), header->capacity, to_insert, to_insert_length, index);
        header->length += to_insert_capacity - 1;
        return str;
    }

    String MACRO_ckit_str_insert_char(String str, char to_insert, u64 index) {
        ckit_str_check_magic(str);

        u8 source_size = 1;
        CKIT_StringHeader* header = ckit_str_header(str);
        if (header->length + source_size >= header->capacity) {
            str = ckit_str_grow(str, (header->length + source_size) * 2);
            header = ckit_str_header(str);
        }

        ckg_str_insert_char(str, ckit_str_length(str), header->capacity, to_insert, index);
        header->length++;
        return str;
    }

    void ckit_str_clear(String str1) {
        ckit_memory_zero(str1, ckit_str_length(str1));
        ckit_str_recanonicalize_header_length(str1);
    }

    void ckit_str_recanonicalize_header_length(String str) {
        u64 actual_length = ckit_str_length(str);
        ckit_str_header(str)->length = actual_length;
    }

    void ckit_str_copy(String str1, char* source) {
        ckit_str_clear(str1);
        ckit_str_append(str1, source);
    }

    u64 ckit_str_length(String str) {
        ckit_str_check_magic(str);
        return ckit_str_header(str)->length;
    }

    String MACRO_ckit_str_append(String str, char* source) {
        ckit_str_check_magic(str);
        ckit_assert_msg(str, "ckit_str_append: String passed is null\n");
        ckit_assert_msg(source, "ckit_str_append: Source passed is null\n");

        u64 source_length = ckg_cstr_length(source); 
        u64 source_capacity = source_length + 1; 
        CKIT_StringHeader* header = ckit_str_header(str);
        if (header->length + source_capacity >= header->capacity) {
            str = ckit_str_grow(str, (header->length + source_capacity) * 2);
            header = ckit_str_header(str);
        }

        ckg_str_append(str, ckit_str_length(str), header->capacity, source, source_length);
        header->length += source_capacity - 1;
        return str;
    }

    char* MACRO_ckit_cstr_va_sprint(u64* allocation_size_ptr, char* fmt, va_list args) {
        va_list args_copy;
        va_copy(args_copy, args);
        u64 allocation_size = vsnprintf(NULLPTR, 0, fmt, args_copy) + 1; // + 1 because null terminator
        va_end(args_copy);

        char* ret = ckit_alloc(allocation_size);

        va_copy(args_copy, args);
        vsnprintf(ret, allocation_size, fmt, args_copy);
        va_end(args_copy);
        
        if (allocation_size_ptr != NULLPTR) {
            *allocation_size_ptr = allocation_size;
        } 

        return ret;
    }

    char* MACRO_ckit_cstr_sprint(u64* allocation_size_ptr, char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        char* ret = MACRO_ckit_cstr_va_sprint(allocation_size_ptr, fmt, args);
        va_end(args);

        return ret;
    }

    String ckit_str_va_sprint(u64* allocation_size_ptr, char* fmt, va_list args) {
        va_list args_copy;
        va_copy(args_copy, args);
        u64 allocation_ret = vsnprintf(NULLPTR, 0, fmt, args_copy) + 1; // +1 for null terminator
        va_end(args_copy);

        char* buffer = ckit_alloc(allocation_ret);

        va_copy(args_copy, args);
        vsnprintf(buffer, allocation_ret, fmt, args_copy);
        va_end(args_copy);

        String ret = ckit_str_create(buffer);
        ckit_free(buffer);

        if (allocation_size_ptr != NULLPTR) {
            *allocation_size_ptr = allocation_ret;
        } 
        
        return ret;
    }


    String MACRO_ckit_str_sprint(u64* allocation_size_ptr, char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        String ret = ckit_str_va_sprint(allocation_size_ptr, fmt, args);
        va_end(args);
        
        return ret;
    }
    
    String MACRO_ckit_str_append_char(String str, char source) {
        ckit_str_check_magic(str);
        ckit_assert_msg(str, "ckit_str_append_char: String passed is null\n");
        ckit_assert_msg(source, "ckit_str_append_char: Source passed is null\n");

        u32 source_size = 1;
        CKIT_StringHeader* header = ckit_str_header(str);
        if (header->length + source_size >= header->capacity) {
            str = ckit_str_grow(str, (header->length + source_size) * 2);
            header = ckit_str_header(str);
        }

        str[header->length] = source;
        header->length++;

        return str;
    }

    String* ckit_str_split(char* str, char* delimitor) {
        ckit_assert(str);
        ckit_assert(delimitor);

        u64 str_length = ckg_cstr_length(str);
        u64 delimitor_length = ckg_cstr_length(delimitor);
        ckit_assert_msg(delimitor_length > 0, "delimitor can not be a empty string!\n");

        if (str_length == 0) {
            String* ret_vector = NULLPTR;
            String current = ckit_str_create(str);
            ckit_vector_push(ret_vector, current);

            return ret_vector;
        }

        String* ret_vector = NULLPTR;
        CKG_StringView str_view = ckg_sv_create(str, str_length);
        while (true) {
            s64 found_index = ckg_str_index_of(str_view.data, str_view.length, delimitor, delimitor_length);
            if (found_index == -1) {
                String substring = ckit_str_create_custom(str_view.data, str_view.length, str_view.length + 1);
                ckit_vector_push(ret_vector, substring);
                return ret_vector;
            }

            if (found_index == 0) {
                String empty_string = ckit_str_create("");
                ckit_vector_push(ret_vector, empty_string);

                str_view.data += (found_index + 1);
                str_view.length -= (found_index + 1);
                
                continue;
            }

            String substring = ckit_str_create_custom(str_view.data, found_index, (found_index) + 1);
            ckit_vector_push(ret_vector, substring);

            str_view.data += (found_index + 1);
            str_view.length -= (found_index + 1);
        }

        return ret_vector;
    }

    String ckit_str_reverse(char* str, u64 str_length) {
        ckit_assert(str);

        String reversed_string_buffer = ckit_str_create_custom("", 0, str_length + 1);
        ckg_str_reverse(str, str_length, reversed_string_buffer, str_length + 1);
        return reversed_string_buffer;
    }

    String ckit_str_int_to_str(int number) {
        String ret = ckit_str_create("");

        while (number != 0) {
            char c = '0' + FIRST_DIGIT(number);
            ckit_str_insert_char(ret, c, 0);
            number /= (int)10;
        }

        return ret;
    }

    String ckit_str_between_delimiters(char* str, u64 str_length, char* start_delimitor, u64 start_delimitor_length, char* end_delimitor, u64 end_delimitor_length) {
        ckit_assert(str);
        ckit_assert(start_delimitor);
        ckit_assert(end_delimitor);
        ckit_assert(!ckg_str_equal(start_delimitor, start_delimitor_length, end_delimitor, end_delimitor_length));

        s64 start_delimitor_index = ckg_str_index_of(str, str_length, start_delimitor, start_delimitor_length); 
        s64 end_delimitor_index = ckg_str_index_of(str, str_length, end_delimitor, end_delimitor_length);
        if (start_delimitor_index == -1 || end_delimitor_index == -1) {
            return NULLPTR;
        }

        String ret = ckit_str_create("");

        if (start_delimitor_index == -1 || end_delimitor_index == -1) {
            return NULLPTR;
        } else if (start_delimitor_index > end_delimitor_index) {
            return NULLPTR; // The start delimtor is after the end delimitor
        }

        u64 i = (u64)(start_delimitor_index + start_delimitor_length);

        while (i < (u64)end_delimitor_index) {
            ckit_str_append_char(ret, str[i++]);
        }

        return ret;
    }
#endif

#if defined(CKIT_IMPL_INIT)
	void ckit_init() {
        CKIT_Context context = {0};
        ckg_bind_custom_allocator(ckit_allocate_wrapper, ckit_free_wrapper, &context);
        // ckit_tracker_init();
        // memory_init();
        // platform_console_init();
        // ckit_str_register_arena();
    }

	void ckit_cleanup(bool generate_memory_report) {
        if (generate_memory_report) {
            ckit_memory_report(LOG_LEVEL_WARN); 
            // Evenetually this should happen before arenas are unregistered
        }

        //platform_console_shutdown();
        ckit_memory_arena_unregister_all();
        ckit_tracker_cleanup();
    }
#endif