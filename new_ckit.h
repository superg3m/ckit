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
    typedef CKIT_LogLevel CKIT_LogLevel;

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
	typedef CKG_Alloc_T CKIT_Alloc_T;
	typedef CKG_Free_T CKIT_Free_T;
    typedef CKG_Allocator CKIT_Allocator;

    // Date: September 24, 2024
    // TODO(Jovanni): Just make this defines and CKIT_MemoryTagID a u8

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
        const char* tag_name;
        CKIT_AllocationInfo allocation_info;
    } CKIT_MemoryTag;

    // Date: September 24, 2024
    // TODO(Jovanni): Probably move this to the implementation you don't need this here
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

    CKIT_API void ckit_memory_tracker_init();
    CKIT_API void ckit_memory_tracker_register_tag_pool(CKIT_MemoryTagID tag_id, const char* name);
    CKIT_API void* MACRO_ckit_tracker_insert_header(void* data, CKIT_MemoryHeader header);

    CKIT_API void* memory_insert_header(void* data, CKIT_MemoryHeader header);
    CKIT_API void ckit_memory_tracker_add(CKIT_MemoryHeader* header);
    CKIT_API void ckit_memory_tracker_remove(CKIT_MemoryHeader* header);



    CKIT_API void ckit_memory_tracker_print_header(CKIT_MemoryHeader* header, CKIT_LogLevel log_level);
    CKIT_API void ckit_memory_tracker_print_pool(CKIT_MemoryTagPool* pool, CKIT_LogLevel log_level);
    CKIT_API void ckit_memory_tracker_print_all_pools(CKIT_LogLevel log_level);

    void* ckit_allocator_wrapper(CKIT_Allocator* allocator, size_t allocation_size);
    void ckit_free_wrapper(CKIT_Allocator* allocator, void* data);
#endif

void ckit_init() {
    // this needs to make the 
    ckg_bind_custom_allocator(ckit_allocator_wrapper, ckit_free_wrapper, NULLPTR);
    // ckit_tracker_init();
    // memory_init();
    // platform_console_init();
    // ckit_str_register_arena();
}

#if defined(CKIT_IMPL_MEMORY)
    internal CKIT_Allocator allocator = {ckg_default_libc_malloc, ckg_default_libc_free, NULLPTR};
    #define CKIT_MEMORY_MAGIC "CKIT_MAGIC_MEMORY"
    #define ckit_memory_header(data) ((CKIT_MemoryHeader*)((u8*)data - sizeof(CKIT_MemoryHeader)))

    internal CKIT_MemoryHeader* ckit_memory_tracker_get_header(void* data) {
        ckit_tracker_check_magic(data);
        CKIT_MemoryHeader* header = (CKIT_MemoryHeader*)((u8*)data - sizeof(CKIT_MemoryHeader));
        ckit_assert_msg(ckit_tracker_tag_pool_exists(header->tag.tag_id, header->tag.tag_name), "Tag id or tag_name must be corrupted\n");
        
        return header;
    }

    internal CKIT_MemoryHeader ckit_tracker_header_create(CKIT_MemoryTagID tag_id, size_t allocation_size, const char* file_name, const u64 line, const char* function_name) {
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

    void ckit_bind_custom_allocator(CKIT_Alloc_T* a, CKIT_Alloc_T* f, void* ctx) {
        ckit_assert_msg(a, "Alloc function is NULLPTR\n");
        ckit_assert_msg(f, "Free function is NULLPTR\n");

        allocator.allocator = a;
        allocator.free = f;
        if (ctx) {
            allocator.ctx = ctx;
        }

        ckg_bind_custom_allocator(ckit_allocator_wrapper, ckit_free_wrapper, ctx);
    }

    void ckit_tracker_add(CKIT_MemoryHeader* header) {
        u64 tag_pool_index = ckit_tracker_get_tag_pool_index(header->tag.tag_id);
        
        global_memory_tag_pool_vector[tag_pool_index].total_pool_allocation_size += header->tag.allocation_info.allocation_size;
        global_total_pool_memory_internal += sizeof(CKIT_MemoryHeader);
        global_total_pool_memory_used += header->tag.allocation_info.allocation_size;

        ckg_bind_custom_allocator(allocator.allocate, allocator.free, allocator.ctx);
        header->linked_list_address = ckg_linked_list_push(global_memory_tag_pool_vector[tag_pool_index].allocated_headers, header);
        ckg_bind_custom_allocator(ckit_allocator_wrapper, ckit_free_wrapper, allocator.ctx);
    }

    void ckit_tracker_remove(CKIT_MemoryHeader* header) {
        u64 tag_pool_index = ckit_tracker_get_tag_pool_index(header->tag.tag_id);

        global_memory_tag_pool_vector[tag_pool_index].total_pool_allocation_size -= header->tag.allocation_info.allocation_size;
        global_total_pool_memory_internal -= sizeof(CKIT_MemoryHeader);
        global_total_pool_memory_used -= header->tag.allocation_info.allocation_size;
        
        u64 index = ckg_linked_list_node_to_index(global_memory_tag_pool_vector[tag_pool_index].allocated_headers,  header->linked_list_address);

        ckg_bind_custom_allocator(allocator.allocate, allocator.free, allocator.ctx);
        ckg_linked_list_remove(global_memory_tag_pool_vector[tag_pool_index].allocated_headers, index); 
        ckg_bind_custom_allocator(ckit_allocator_wrapper, ckit_free_wrapper, allocator.ctx);
    }

    void* ckit_allocator_wrapper(CKIT_Allocator* allocator, size_t allocation_size) {
        (void)allocator;

        u8* ret = ckg_alloc(sizeof(CKIT_MemoryHeader) + allocation_size);
        *((CKIT_MemoryHeader*)ret) = ckit_tracker_header_create();
        ckit_memory_tracker_add((CKIT_MemoryHeader*)ret);
        ret += sizeof(CKIT_MemoryHeader);
    
        return ret;
    }

    void ckit_free_wrapper(CKIT_Allocator* allocator, void* data) {
        (void)allocator;

        ckit_memory_tracker_remove((CKIT_MemoryHeader*)ret);
        u8* base = (u8*)data - sizeof(CKIT_MemoryHeader);
        
        ckg_free(base);
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
#endif

