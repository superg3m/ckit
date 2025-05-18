#pragma once 

#ifdef __cplusplus
    #define CKIT_API extern "C"
#else
    #define CKIT_API
#endif

#if defined(CKIT_IMPL)
    #define CKIT_IMPL_TYPES
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

#define CKIT_INCLUDE_TYPES
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

#if defined(CKIT_INCLUDE_TYPES)
	#define CKG_EXTERN
	#include "./ckg/ckg.h"

	#define MEMORY_TAG_CHARACTER_LIMIT 16
	#define PLATFORM_CHARACTER_LIMIT 200
#endif

CKIT_API void ckit_init();
CKIT_API void ckit_cleanup(bool generate_memory_report);

// Date: May 10, 2025
// TODO(Jovanni): Fix the assert to use ckg logger
// actually maybe just want to bring this into ckg entirely
#if defined(CKIT_INCLUDE_ASSERT)
    #define CKIT_ASSERT_ENABLED true

    // Date: August 14, 2024
    // TODO(Jovanni): This has to get fixed because I have my own logging logic for ckit now! ${} needs to be handled here
    #if CKIT_ASSERT_ENABLED == true
        #define ckit_assert_msg(expression, message, ...) ckg_assert_msg(expression, message, ##__VA_ARGS__)
        #define ckit_assert(expression) ckg_assert(expression)
    #else
        #define ckit_assert_msg(expression, message, ...)
        #define ckit_assert(expression) 
    #endif
#endif 

// Date: May 10, 2025
// TODO(Jovanni): take a good look at logging between ckg and ckit
#if defined(CKIT_INCLUDE_LOGGER)
    #define LOG_LEVEL_CHARACTER_LIMIT 12

    // Log Levels
    #define LOG_LEVEL_FATAL 0
    #define LOG_LEVEL_ERROR 1
    #define LOG_LEVEL_WARN 2
    #define LOG_LEVEL_DEBUG 3
    #define LOG_LEVEL_SUCCESS 4
    #define LOG_LEVEL_PRINT 5
    #define LOG_LEVEL_COUNT 6
    typedef u8 CKIT_LogLevel;

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
        char* file_name;
        char* function_name;
        u64 line;
        size_t allocation_size;
    } CKIT_AllocationInfo;

    typedef struct CKIT_MemoryTag {
        CKIT_MemoryTagID tag_id;
        char* tag_name;
        CKIT_AllocationInfo allocation_info;
    } CKIT_MemoryTag;

    // Date: September 24, 2024
    // TODO(Jovanni): Probably move this to the implementation you don't need this here
    typedef struct CKIT_MemoryTagPool {
        CKIT_MemoryTagID tag_id;
        char* pool_name;
        CKG_LinkedList* allocated_headers;
        size_t total_pool_allocation_size;
    } CKIT_MemoryTagPool;

    typedef struct CKIT_MemoryHeader {
        CKIT_MemoryTag tag;
        CKG_Node* linked_list_address;
        char* magic;
    } CKIT_MemoryHeader;

    CKIT_API void ckit_tracker_init();
    CKIT_API void ckit_tracker_register_tag_pool(CKIT_MemoryTagID tag_id, char* name);
    CKIT_API void* MACRO_ckit_tracker_insert_header(void* data, CKIT_MemoryHeader header);

    CKIT_API CKIT_MemoryHeader ckit_tracker_header_create(CKIT_MemoryTagID tag_id, size_t allocation_size, char* file_name, u64 line, char* function_name);
    CKIT_API void* memory_insert_header(void* data, CKIT_MemoryHeader header);
    CKIT_API void ckit_tracker_add(CKIT_MemoryHeader* header);
    CKIT_API void ckit_tracker_remove(CKIT_MemoryHeader* header);
    CKIT_API CKIT_MemoryHeader* ckit_tracker_get_header(void* data);

    CKIT_API void ckit_tracker_print_header(CKIT_MemoryHeader* header, CKIT_LogLevel log_level);
    CKIT_API void ckit_tracker_print_pool(CKIT_MemoryTagPool* pool, CKIT_LogLevel log_level);
    CKIT_API void ckit_tracker_print_all_pools(CKIT_LogLevel log_level);

    // CKIT_MemoryHeader** ckit_tracker_get_all_headers();
    // CKIT_MemoryTagPool** ckit_tracker_get_all_pools();

    //
    // Memory
    //
    CKIT_API void memory_init();
    CKIT_API void* MACRO_ckit_alloc(size_t number_of_bytes, CKIT_MemoryTagID tag_id, char* file, u32 line, char* function);
    CKIT_API void* MACRO_ckit_realloc(void* data, u64 new_allocation_size, char* file, u32 line, char* function);
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

    #define ckit_tracker_insert_header(data, header) data = MACRO_ckit_tracker_insert_header(data, header)
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
        u64 capacity;
        u64 used;
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
    CKIT_API u64 ckit_arena_used(CKIT_Arena* arena);
    CKIT_API u64 ckit_arena_capacity(CKIT_Arena* arena);

    #define ckit_arena_create(allocation_size, name) MACRO_ckit_arena_create(allocation_size, name, CKIT_ARENA_FLAG_EXTENDABLE_PAGES, 0)
    #define ckit_arena_create_custom(allocation_size, name, flags, alignment) MACRO_ckit_arena_create(allocation_size, name, flags, alignment)
    #define ckit_arena_free(arena) arena = MACRO_ckit_arena_free(arena)
    #define ckit_arena_push(arena, type) ((type*)MACRO_ckit_arena_push(arena, sizeof(type)))
    #define ckit_arena_push_array(arena, type, element_count) ((type*)MACRO_ckit_arena_push(arena, sizeof(type) * element_count))
#endif 

#if defined(CKIT_INCLUDE_CHAR)
    #define ckit_char_is_digit(c) ckg_char_is_digit(c)
    #define ckit_char_is_upper(c) ckg_char_is_upper(c)
    #define ckit_char_is_lower(c) ckg_char_is_lower(c)
    #define ckit_char_is_alpha(c) ckg_char_is_alpha(c)
    #define ckit_char_is_alpha_numeric(c) ckg_char_is_alpha_numeric(c)
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

#if defined(CKIT_INCLUDE_OS)
    typedef struct CKIT_SystemObjectInfo {
        // last modified
        // created
        int a;
    } CKIT_SystemObjectInfo;

    typedef struct CKIT_File {
        char* name;
        size_t size;
        size_t cursor;
    } CKIT_File;

    typedef struct CKIT_Directory {
        struct CKIT_Directory* parent_directory;
        struct CKIT_Directory** sub_directories;
        CKIT_File* files;
        size_t size;
    } CKIT_Directory;

    CKIT_API String ckit_os_get_cwd();
    CKIT_API void ckit_os_ls();
    CKIT_API void ckit_os_get_items();
    CKIT_API void ckit_os_chdir();
    CKIT_API void ckit_os_mkdir();
    CKIT_API void ckit_os_create_file(char* path);
    CKIT_API bool ckit_os_path_exists(char* path);
    CKIT_API void ckit_os_run_subprocess();
    CKIT_API void ckit_os_get_file_info();
    CKIT_API String ckit_os_path_join(char* path, char* to_join);
    CKIT_API void ckit_os_system(char* command);

    /**
     * @brief returns the number of counts in a millisecond
     * double start_counter = ckit_os_query_performance_counter();
     * 
     * ... do some work
     * 
     * double end_counter = ckit_os_query_performance_counter();
     * double milliseconds_per_frame = end_counter - start_counter
     * double seconds_per_frame = milliseconds_per_frame / 1000.0
     * u64 fps = 1.0 / seconds_per_frame;
     * 
     * @return double 
     */
    CKIT_API double ckit_os_query_performance_counter();
    CKIT_API CKIT_File* ckit_os_file_open(char* path);
    CKIT_API String ckit_os_file_read_next_line(CKIT_File* file);
    CKIT_API String ckit_os_file_read_next_integer(CKIT_File* file);
    CKIT_API String ckit_os_file_read_next_float(CKIT_File* file);
    CKIT_API CKIT_File* ckit_os_file_close(CKIT_File* file);
    CKIT_API CKIT_File* MACRO_ckit_os_close_file(CKIT_File* file);
    CKIT_API u8* ckit_os_read_entire_file(char* path, size_t* returned_file_size);
    CKIT_API String* ckit_os_get_lines_from_file(char* path);
    CKIT_API void ckit_os_get_mouse_position(int* mouse_x, int* mouse_y);
    CKIT_API void ckit_os_push(char* path);
    CKIT_API void ckit_os_pop();
    CKIT_API void ckit_os_dir_append_subdir(CKIT_Directory* directory, CKIT_Directory* sub_directory);
    CKIT_API void ckit_os_dir_append_file(CKIT_Directory* directory, CKIT_File* file);
    // CKIT_API void ckit_os_sleep(u32 ms);

    // Date: September 24, 2024
    // TODO(Jovanni): ckit_os_alloc()
    // TODO(Jovanni): ckit_os_free()
    // TODO(Jovanni): ckit_os_console_init()
    // TODO(Jovanni): ckit_os_console_cleanup()
    // TODO(Jovanni): ckit_os_console_write()

    // =======================================================================

    #define TEXT_CLEAR     0x0000 // text color default clear
    #define TEXT_BLUE      0x0001 // text color contains blue.
    #define TEXT_GREEN     0x0002 // text color contains green.
    #define TEXT_RED       0x0004 // text color contains red.
    #define TEXT_CYAN      TEXT_BLUE|TEXT_GREEN // text color contains cyan.
    #define TEXT_PURPLE    TEXT_RED|TEXT_BLUE // text color contains purple.
    #define TEXT_WHITE     TEXT_RED|TEXT_GREEN|TEXT_BLUE // text color contains white.

    #define BACK_BLUE      0x0010 // background color contains blue.
    #define BACK_GREEN     0x0020 // background color contains green.
    #define BACK_RED       0x0040 // background color contains red.

    CKIT_API void* platform_allocate(size_t number_of_bytes);
    CKIT_API void platform_free(void* data);
    CKIT_API void platform_console_write(char* message, unsigned char color);
    CKIT_API void platform_console_init();
    CKIT_API void platform_console_shutdown();
#endif

// Collections
#if defined(CKIT_INCLUDE_COLLECTIONS)
    //
    // ========== Start CKIT_Vector ==========
    //
    typedef struct CKIT_VectorHeader {
        u32 count;
        u32 capacity;
        char* magic;
    } CKIT_VectorHeader;

    CKIT_API void* ckit_vector_grow(void* vector, size_t element_size, bool force_grow, char* file, u32 line, char* function);
    CKIT_API void* MACRO_ckit_vector_reserve(size_t element_size, u32 capacity, char* file, u32 line, char* function);
    CKIT_API void* MACRO_ckit_vector_free(void* vector);

    // Date: July 13, 2024
    // NOTE(Jovanni): This doesn't have very good checking if the vector is NULLPTR
    // Attempting to get the length/cap/pop of a NULLPTR will result in a uncontrolled crash

    // Date: September 12, 2024
    // TODO(Jovanni): MAKE SURE YOU TEST ckit_vector_remove_at() and ckit_vector_insert_at()
    #define ckit_vector_base(vector) ((CKIT_VectorHeader*)(((u8*)vector) - sizeof(CKIT_VectorHeader)))
    #define ckit_vector_count(vector) (*ckit_vector_base(vector)).count
    #define ckit_vector_capacity(vector) (*ckit_vector_base(vector)).capacity

    #ifdef __cplusplus
        #define ckit_vector_push(vector, element) vector = (decltype(vector))ckit_vector_grow(vector, sizeof(vector[0]), false, __FILE__, __LINE__, __func__); vector[ckit_vector_base(vector)->count++] = element
        #define ckit_vector_free(vector) vector = (decltype(vector))MACRO_ckit_vector_free(vector)

        #define ckit_stack_push(stack, element) stack = (decltype(stack))ckit_vector_grow(stack, sizeof(stack[0])); stack[ckit_vector_base(stack)->count++] = element
        #define ckit_stack_free(stack) stack = (decltype(stack))MACRO_ckit_vector_free(stack)
    #else 
        #define ckit_vector_push(vector, element) vector = ckit_vector_grow(vector, sizeof(vector[0]), false, __FILE__, __LINE__, __func__); vector[ckit_vector_base(vector)->count++] = element
        #define ckit_vector_free(vector) vector = MACRO_ckit_vector_free(vector)

        #define ckit_stack_push(stack, element) stack = ckit_vector_grow(stack, sizeof(stack[0]), false, __FILE__, __LINE__, __func__); stack[ckit_vector_base(stack)->count++] = element
        #define ckit_stack_free(stack) stack = MACRO_ckit_vector_free(stack)
    #endif

    #define ckit_vector_reserve(capactiy, type) (type*)MACRO_ckit_vector_reserve(sizeof(type), capactiy, __FILE__, __LINE__, __func__)
    #define ckit_vector_pop(vector) vector[--ckit_vector_base(vector)->count]
    #define ckit_vector_remove_at(vector, index) ckit_memory_delete_index(vector, ckit_vector_count(vector), ckit_vector_capacity(vector), index); ckit_vector_base(vector)->count--
    #define ckit_vector_insert_at(vector, element, index) ckit_memory_insert_index(vector, ckit_vector_count(vector), ckit_vector_capacity(vector), element, index); ckit_vector_base(vector)->count++
    #define ckit_vector_to_array(vector) MACRO_ckit_vector_to_array(vector, sizeof(vector[0]))

    #define ckit_stack_pop(stack) stack[--ckit_vector_base(stack)->count]
    #define ckit_stack_count(stack) (*ckit_vector_base(stack)).count
    #define ckit_stack_peek(stack) stack[ckit_stack_count(stack) - 1]
    #define ckit_stack_empty(stack) ckit_stack_count(stack) == 0
    //
    // ========== End CKIT_Vector ==========
    //

    //
    // ========== Start CKIT_HashMap ==========
    //
    typedef struct CKIT_HashMapEntry {
        char* key;
        void* value;
    } CKIT_HashMapEntry;

    typedef struct CKIT_HashMap {
        u32 capacity;
        u32 count;
        CKIT_HashMapEntry* entries; // not a vector
        size_t element_size;
        bool is_pointer_type;
    } CKIT_HashMap;

    CKIT_API u32 ckit_hash_value(char *str);
    CKIT_API CKIT_HashMap* MACRO_ckit_hashmap_create(u32 hashmap_capacity, size_t element_size, bool is_pointer_type);
    CKIT_API CKIT_HashMap* MACRO_ckit_hashmap_free(CKIT_HashMap* hashmap);

    // Returns previous value
    CKIT_API void* ckit_hashmap_put(CKIT_HashMap* hashmap, char* key, void* value);
    CKIT_API void* ckit_hashmap_get(CKIT_HashMap* hashmap, char* key);
    CKIT_API bool ckit_hashmap_has(CKIT_HashMap* hashmap, char* key);

    #define ckit_hashmap_create(capacity, type, is_pointer_type) MACRO_ckit_hashmap_create(capacity, sizeof(type), is_pointer_type)
    #define ckit_hashmap_free(hashmap) hashmap = MACRO_ckit_hashmap_free(hashmap)
    //
    // ========== End CKIT_HashMap ==========
    //

    //
    // ========== Start CKIT_HashSet ==========
    //
    typedef struct CKIT_HashSet {
        // CKIT_HashFunction* hash_func;
        u32 capacity;
        u32 count;
        char** entries; // not a vector
    } CKIT_HashSet;

    CKIT_API u32 ckit_hash_value(char *str);
    CKIT_API CKIT_HashSet* MACRO_ckit_hashset_create(u32 hashset_capacity);
    CKIT_API CKIT_HashSet* MACRO_ckit_hashset_free(CKIT_HashSet* hashset);

    // Returns previous value
    CKIT_API void ckit_hashset_put(CKIT_HashSet* hashset, char* key);
    CKIT_API bool ckit_hashset_has(CKIT_HashSet* hashset, char* key);

    #define ckit_hashset_create(capacity) MACRO_ckit_hashset_create(capacity)
    #define ckit_hashset_free(hashset) hashset = MACRO_ckit_hashset_free(hashset)
    //
    // ========== End CKIT_HashSet ==========
    //

    //
    // ========== Start CKIT_LinkedList ==========
    //
    typedef struct CKIT_Node {
        struct CKIT_Node* next;
        struct CKIT_Node* prev;
        size_t element_size_in_bytes;
        void* data;
    } CKIT_Node;

    typedef struct CKIT_LinkedList {
        CKIT_Node* head;
        CKIT_Node* tail;
        size_t element_size_in_bytes;
        u32 count;
        bool is_pointer_type;
    } CKIT_LinkedList;

    CKIT_API CKIT_LinkedList* MACRO_ckit_linked_list_create(size_t element_size_in_bytes, bool is_pointer_type, char* file, u32 line, char* function);
    CKIT_API CKIT_Node* ckit_linked_list_insert(CKIT_LinkedList* linked_list, u32 index, void* data);
    CKIT_API CKIT_Node* ckit_linked_list_get_node(CKIT_LinkedList* linked_list, u32 index);
    CKIT_API void* ckit_linked_list_get(CKIT_LinkedList* linked_list, u32 index);
    CKIT_API void* ckit_linked_list_head(CKIT_LinkedList* linked_list);
    CKIT_API void* ckit_linked_list_tail(CKIT_LinkedList* linked_list);
    CKIT_API CKIT_Node* ckit_linked_list_push(CKIT_LinkedList* linked_list, void* data);
    CKIT_API CKIT_Node ckit_linked_list_pop(CKIT_LinkedList* linked_list);
    CKIT_API CKIT_Node ckit_linked_list_remove(CKIT_LinkedList* linked_list, u32 index);
    CKIT_API void* MACRO_ckit_linked_list_free(CKIT_LinkedList* linked_list);
    CKIT_API u32 ckit_linked_list_node_to_index(CKIT_LinkedList* linked_list, CKIT_Node* address);
    CKIT_API void* ckit_linked_list_peek_tail(CKIT_LinkedList* linked_list);

    #define ckit_linked_list_create(type, is_pointer_type) MACRO_ckit_linked_list_create(sizeof(type), is_pointer_type, __FILE__, __LINE__, __func__)

    #ifdef __cplusplus
        #define ckit_linked_list_free(linked_list) linked_list = (decltype(linked_list))MACRO_ckit_linked_list_free(linked_list)
    #else
        #define ckit_linked_list_free(linked_list) linked_list = MACRO_ckit_linked_list_free(linked_list)
    #endif 
    //
    // ========== End CKIT_LinkedList ==========
    //


    //
    // ========== Start CKIT_Queue ==========
    //
    typedef struct CKIT_Queue {
        u32 capacity;
        u32 count;
        void* data;
        size_t element_size_in_bytes;

        u32 read_index;
        bool has_next_to_read;
        u32 write_index;
        bool is_pointer_type;
    } CKIT_Queue;

    CKIT_API CKIT_Queue* MACRO_ckit_queue_create(u32 inital_capacity, size_t element_size_in_bytes, bool is_pointer_type, char* file, u32 line, char* function);
    CKIT_API CKIT_Queue* MACRO_ckit_queue_free(CKIT_Queue* queue);
    CKIT_API void ckit_enqueue(CKIT_Queue* queue, void* element);

    // Date: September 24, 2024
    // TODO(Jovanni): You are responsible for freeing the result, if not a pointer type, I think arenas is gonna be pretty big for this whole thing.
    CKIT_API void* ckit_dequeue(CKIT_Queue* queue);
    CKIT_API u32 ckit_queue_capacity(CKIT_Queue* queue);
    CKIT_API u32 ckit_queue_count(CKIT_Queue* queue);

    #define ckit_queue_create(inital_capacity, type, is_pointer_type) MACRO_ckit_queue_create(inital_capacity, sizeof(type), is_pointer_type, __FILE__, __LINE__, __func__);
    #define ckit_queue_free(queue) MACRO_ckit_queue_free(queue);
    //
    // ========== End CKIT_Queue ==========
    //
#endif

#if defined(CKIT_INCLUDE_UTILITIES)
    //
    // ========== Start FileSystem ==========
    //
    typedef struct FileSystem {
        String file_name;
        FILE* handle;
        u8* data;
        size_t file_size;
        bool reachedEOF;
    } FileSystem;

    CKIT_API FileSystem file_system_create(String file_name);
    CKIT_API void file_open(FileSystem* file_system);
    CKIT_API void file_close(FileSystem* file_system);
    CKIT_API size_t file_size(FileSystem* file_system);
    CKIT_API String file_get_next_line(FileSystem* file_system);
    CKIT_API char file_get_next_char(FileSystem* file_system);
    //
    // ========== End FileSystem ==========
    //

    //
    // ========== Start BMP ==========
    //
	#pragma pack(push, 1)
	typedef struct BmpHeader { 	// total 40 bytes
		char signature[2]; 			// 2 bytes
		u32 file_size; 				// 4 bytes
		u32 reserved;  				// 4 bytes
		u32 data_offset; 			// 4 bytes

		u32 size;					// 4 bytes
		u32 width;					// 4 bytes
		u32 height;					// 4 bytes
		u16 planes;					// 2 bytes
		u16 bits_per_pixel; 		// 2 bytes
		u32 compression;			// 4 bytes
		u32 compressed_image_size;	// 4 bytes
		u32 x_pixels_per_meter; 	// 4 bytes // horizontal resolution: Pixels/meter
		u32 y_pixels_per_meter; 	// 4 bytes // vertical resolution: Pixels/meter
		u32 colors_used;            // 4 bytes // Number of actually used colors. For a 8-bit / pixel bitmap this will be 100h or 256.
		u32 important_colors;		// 4 bytes // Number of important colors
	} BmpHeader;
	#pragma pack(pop)

	CKIT_API CKIT_Bitmap ckit_parser_load_bmp(u8* bmp_file_data, size_t file_size);
    //
    // ========== End BMP ==========
    //
#endif

//
// ===================================================== CKIT_IMPL =====================================================
//
#if defined(CKIT_IMPL)
  void ckit_str_register_arena();
  void ckit_memory_arena_unregister_all();
  void platform_console_init();
  void platform_console_shutdown();
  void ckit_tracker_cleanup();

  void ckit_init() {
    // this needs to make the 

    ckit_tracker_init();
    memory_init();
    //platform_console_init();
    ckit_str_register_arena();
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

#if defined(CKIT_IMPL_LOGGER)
    // internal bool logging_is_initialized = false;
    #define LOGGER_START_DELIM "${"
    #define LOGGER_END_DELIM "}"

    internal char log_level_strings[LOG_LEVEL_COUNT][LOG_LEVEL_CHARACTER_LIMIT] = {
        "[FATAL]  : ",
        "[ERROR]  : ",
        "[WARN]   : ",
        "[DEBUG]  : ",
        "[SUCCESS]: ",
        "",
    };

    internal char* log_level_format[LOG_LEVEL_COUNT] = {
        CKG_RED_BACKGROUND,
        CKG_RED,
        CKG_PURPLE,
        CKG_BLUE,
        CKG_GREEN,
        CKG_COLOR_RESET,
    };

    internal bool message_has_special_delmitor(String message, u64 message_length) {
        bool start_delimitor_index = ckg_str_contains(message, message_length, LOGGER_START_DELIM, sizeof(LOGGER_START_DELIM) - 1);
        bool end_delimitor_index = ckg_str_contains(message, message_length, LOGGER_END_DELIM, sizeof(LOGGER_END_DELIM) - 1);

        return start_delimitor_index && end_delimitor_index;
    }

    internal void special_print_helper(char* message, u64 message_length, CKIT_LogLevel log_level) {
        CKG_StringView middle_to_color = ckg_sv_between_delimiters(message, message_length, LOGGER_START_DELIM, sizeof(LOGGER_START_DELIM) - 1, LOGGER_END_DELIM, sizeof(LOGGER_END_DELIM) - 1);
        if (middle_to_color.length == 0) {
            bool found = message[message_length - 1] == '\n';
            printf("%.*s", (int)(message_length - found), message);

            return;
        }

        s64 start_delimitor_index = ckg_str_index_of(message, message_length, LOGGER_START_DELIM, sizeof(LOGGER_START_DELIM) - 1);
        s64 end_delimitor_index = ckg_str_index_of(message, message_length, LOGGER_END_DELIM, sizeof(LOGGER_END_DELIM) - 1);

        CKG_StringView left_side_view = ckg_sv_create(message, start_delimitor_index);
        CKG_StringView right_side_view = ckg_sv_create(message + (end_delimitor_index + (sizeof(LOGGER_END_DELIM) - 1)), message_length);

        printf("%.*s%s%.*s%s", (int)left_side_view.length, left_side_view.data, log_level_format[log_level], (int)middle_to_color.length, middle_to_color.data, CKG_COLOR_RESET);

        special_print_helper(right_side_view.data, right_side_view.length, log_level);
    }

    void MACRO_ckit_log_output(CKIT_LogLevel log_level, char* message, ...) {
        va_list args_list;
        va_start(args_list, message);
        String out_message = ckit_str_va_sprint(NULLPTR, message, args_list);
        va_end(args_list);

        printf("%s%s%s", log_level_format[log_level], log_level_strings[log_level], CKG_COLOR_RESET);
        
        u64 out_message_length = ckit_str_length(out_message);

        if (message_has_special_delmitor(out_message, out_message_length)) {
            special_print_helper(out_message, out_message_length, log_level);
        } else {
            bool found = out_message[out_message_length - 1] == '\n';
            printf("%s%.*s%s", log_level_format[log_level], (int)(out_message_length - found), out_message, CKG_COLOR_RESET);
        }

        if (out_message[out_message_length - 1] == '\n') {
            printf("\n");
        }
    }
#endif

#if defined(CKIT_IMPL_MEMORY)
    // 
    // Memory Tracker
    //
    internal CKG_LinkedList* registered_arenas = NULLPTR;

    CKIT_MemoryTagID reserved_tags[] = {
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
        stringify(TAG_CKIT_CORE_IO),
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
    internal u64 global_total_pool_memory_internal = 0;

    internal void ckit_tracker_check_magic(void* data) {
        ckit_assert(ckg_str_equal(ckit_memory_header(data)->magic, sizeof(CKIT_MEMORY_MAGIC) - 1, CKIT_MEMORY_MAGIC, sizeof(CKIT_MEMORY_MAGIC) - 1));
    }

    internal CKIT_MemoryTagPool ckit_tracker_tag_pool_create(CKIT_MemoryTagID tag_id, char* name) {
        CKIT_MemoryTagPool ret;
        ret.tag_id = tag_id;
        ret.pool_name = name;
        ret.total_pool_allocation_size = 0;
        ret.allocated_headers = ckg_linked_list_create(CKIT_MemoryHeader, true);

        return ret;
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

    void ckit_tracker_init() {
        global_memory_tag_pool_vector = NULLPTR;

        for (u64 i = 0; i < TAG_CKIT_RESERVED_COUNT; i++) {
            CKIT_MemoryTagPool tag_pool = ckit_tracker_tag_pool_create(reserved_tags[i], reserved_tags_stringified[i]);
            ckg_vector_push(global_memory_tag_pool_vector, tag_pool);
        }
    }

    void ckit_tracker_cleanup() {
        u64 pool_count = ckg_vector_count(global_memory_tag_pool_vector);
        for (u64 i = 0 ;i < pool_count; i++) {
            ckg_linked_list_free(global_memory_tag_pool_vector[i].allocated_headers);
        }

        void* to_free = ((u8*)global_memory_tag_pool_vector) - sizeof(CKG_VectorHeader);
        ckg_free(to_free);
    }

    CKIT_MemoryHeader ckit_tracker_header_create(CKIT_MemoryTagID tag_id, size_t allocation_size, char* file_name, u64 line, char* function_name) {
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

    void ckit_tracker_register_tag_pool(CKIT_MemoryTagID tag_id, char* name) {
        ckit_assert_msg(!ckit_tracker_tag_pool_exists(tag_id, name), "Attempted to register memory tag_id: %d Number of registed tags: %d\n", tag_id, ckg_vector_count(global_memory_tag_pool_vector)); // don't register a tag that already exists/has been registered (name can't be the same either)

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
        
        global_memory_tag_pool_vector[tag_pool_index].total_pool_allocation_size += header->tag.allocation_info.allocation_size;
        global_total_pool_memory_internal += sizeof(CKIT_MemoryHeader);
        global_total_pool_memory_used += header->tag.allocation_info.allocation_size;

        header->linked_list_address = ckg_linked_list_push(global_memory_tag_pool_vector[tag_pool_index].allocated_headers, header);
    }

    void ckit_tracker_remove(CKIT_MemoryHeader* header) {
        u64 tag_pool_index = ckit_tracker_get_tag_pool_index(header->tag.tag_id);

        global_memory_tag_pool_vector[tag_pool_index].total_pool_allocation_size -= header->tag.allocation_info.allocation_size;
        global_total_pool_memory_internal -= sizeof(CKIT_MemoryHeader);
        global_total_pool_memory_used -= header->tag.allocation_info.allocation_size;
        
        u64 index = ckg_linked_list_node_to_index(global_memory_tag_pool_vector[tag_pool_index].allocated_headers,  header->linked_list_address);
        ckg_linked_list_remove(global_memory_tag_pool_vector[tag_pool_index].allocated_headers, index); 
    }

    void ckit_tracker_print_header(CKIT_MemoryHeader* header, CKIT_LogLevel log_level) {
        u8* data_address = (u8*)header + sizeof(CKIT_MemoryHeader);
        
        ckit_log_output(log_level, "=>  Address: %p | Size: %d(Bytes)\n", data_address, header->tag.allocation_info.allocation_size);
        ckit_log_output(log_level, "        - Allocation Site:\n");
        ckit_log_output(log_level, "            - File: %s:%d\n", header->tag.allocation_info.file_name, header->tag.allocation_info.line);
        ckit_log_output(log_level, "            - Function: %s\n", header->tag.allocation_info.function_name);
    }

    void ckit_tracker_print_pool(CKIT_MemoryTagPool* pool, CKIT_LogLevel log_level) {
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

    CKIT_MemoryHeader* ckit_tracker_get_header(void* data) {
        ckit_tracker_check_magic(data);
        CKIT_MemoryHeader* header = (CKIT_MemoryHeader*)((u8*)data - sizeof(CKIT_MemoryHeader));
        ckit_assert_msg(ckit_tracker_tag_pool_exists(header->tag.tag_id, header->tag.tag_name), "Tag id or tag_name must be corrupted\n");
        
        return header;
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
    // Memory
    //
    void memory_init() {
        registered_arenas = ckg_linked_list_create(CKIT_Arena*, true);

        // ckg_bind_custom_allocator(&platform_allocate, &platform_free, &arena);
    }

    void* MACRO_ckit_alloc(size_t byte_allocation_size, CKIT_MemoryTagID tag_id, char* file, u32 line, char* function) {
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
        ckit_assert_msg(data, "ckit_free: Data passed is null in free\n");\

        CKIT_MemoryHeader* header = ckit_tracker_get_header(data);
        size_t allocation_size = header->tag.allocation_info.allocation_size;
        ckit_tracker_remove(header);
        data = (u8*)data - sizeof(CKIT_MemoryHeader);
        ckit_memory_zero(data, sizeof(header) + allocation_size);
        ckg_free(data);
        return data;
    }

    void* MACRO_ckit_realloc(void* data, u64 new_allocation_size, char* file, u32 line, char* function) {
        ckit_assert_msg(data, "ckit_reallocation: Data passed is null\n");

        CKIT_MemoryHeader* header = ckit_tracker_get_header(data);

        void* ret_data = MACRO_ckit_alloc(sizeof(header) + new_allocation_size, header->tag.tag_id, file, line, function);
        ckit_memory_copy(data, ret_data, header->tag.allocation_info.allocation_size, new_allocation_size);
        ckit_free(data);

        return ret_data;
    }

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

#if defined(CKIT_IMPL_MATH)
    int int_abs(int a) {
        return a < 0 ? (a * -1) : a;
    }

    double float_abs(double a) {
        return a < 0 ? (a * -1) : a;
    }

    double ckit_lerp(double a, double b, double t) {
        return b + ((a - b) * t);
    }

    CKIT_Vector2 ckit_vector2_lerp(CKIT_Vector2 a, CKIT_Vector2 b, double t) {
        CKIT_Vector2 vec_ret = {0};
        vec_ret.x = ckit_lerp(a.x, b.x, t); 
        vec_ret.y = ckit_lerp(a.y, b.y, t); 
        return vec_ret;
    }

    CKIT_Vector3 ckit_vector3_lerp(CKIT_Vector3 a, CKIT_Vector3 b, double t) {
        CKIT_Vector3 vec_ret = {0};
        vec_ret.x = ckit_lerp(a.x, b.x, t);
        vec_ret.y = ckit_lerp(a.y, b.y, t);
        vec_ret.z = ckit_lerp(a.z, b.z, t);

        return vec_ret;
    }

    CKIT_Vector4 ckit_vector4_lerp(CKIT_Vector4 a, CKIT_Vector4 b, double t) {
        CKIT_Vector4 vec_ret = {0};
        vec_ret.x = ckit_lerp(a.x, b.x, t);
        vec_ret.y = ckit_lerp(a.y, b.y, t);
        vec_ret.z = ckit_lerp(a.z, b.z, t);
        vec_ret.w = ckit_lerp(a.w, b.w, t);

        return vec_ret;
    }


    CKIT_Vector2 ckit_vector2_projection(CKIT_Vector2 a, CKIT_Vector2 b) {
        CKIT_Vector2 vec_ret = {0};
        (void)a;
        (void)b;

        return vec_ret;
    }

    CKIT_Vector2 ckit_vector2_noramlize(CKIT_Vector2 a) {
        CKIT_Vector2 vec_ret = {0};
        (void)a;

        return vec_ret;
    }

    CKIT_Vector2 ckit_vector2_perpendicular(CKIT_Vector2 a) {
        CKIT_Vector2 vec_ret = {0};
        (void)a;

        return vec_ret;
    }

    void ckit_vector2_print(CKIT_Vector2 point) {
        LOG_PRINT("(%f, %f)\n", point.x, point.y);
    }

    CKIT_Vector2 ckit_vector2_spline_point(CKIT_Vector2* spline_points, u32 spline_points_count, double t) {
        if (spline_points_count == 1) {
            return spline_points[0];
        }

        CKIT_Vector2* points_vector = NULLPTR;
        for (u32 i = 0; i < spline_points_count - 1; i++) { // get lerp points
            CKIT_Vector2 lerp_point = ckit_vector2_lerp(spline_points[i + 1], spline_points[i], t);
            ckit_vector_push(points_vector, lerp_point);
        }

        CKIT_Vector2 ret = ckit_vector2_spline_point(points_vector, ckit_vector_count(points_vector), t); // feed back points
        ckit_vector_free(points_vector);
        return ret;
    }

    CKIT_Rectangle2D ckit_rectangle2d_create(s32 x, s32 y, u32 width, u32 height) {
        CKIT_Rectangle2D ret = {0};
        ret.position.x = x;
        ret.position.y = y;
        ret.width = width;
        ret.height = height;

        return ret;
    }

    CKIT_Rectangle3D ckit_rectangle3d_create(s32 x, s32 y, s32 z, u32 length, u32 width, u32 height) {
        CKIT_Rectangle3D ret = {0};
        ret.position.x = x;
        ret.position.y = y;
        ret.position.z = z;
        ret.length = length;
        ret.width = width;
        ret.height = height;

        return ret;
    }

    bool ckit_rectangle_check_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2) {
        if (rect1.position.x < rect2.position.x + rect2.width && rect1.position.x + rect1.width > rect2.position.x &&
            rect1.position.y < rect2.position.y + rect2.height && rect1.position.y + rect1.height > rect2.position.y) {
            return true;
        }

        return false;
    }

    CKIT_Rectangle2D ckit_rectangle_get_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2) {
        CKIT_Rectangle2D ret = {0};
        (void)rect1;
        (void)rect2;
        
        return ret;
    }

    CKIT_Circle2D ckit_circle2d_create(s32 x, s32 y, u32 radius) {
        CKIT_Circle2D ret = {0};
        ret.position.x = x;
        ret.position.y = y;
        ret.radius = radius;

        return ret;
    }

    CKIT_Circle3D ckit_circle3d_create(s32 x, s32 y, s32 z, u32 radius) {
        CKIT_Circle3D ret = {0};
        ret.position.x = x;
        ret.position.y = y;
        ret.position.z = z;
        ret.radius = radius;

        return ret;
    }

    // Eventually I want to do some physics here Acceleration, Velocity, Position
#endif

#if defined(CKIT_IMPL_OS)
    internal String cwd = NULLPTR;
    // internal String cached_directory = NULLPTR;

    // just asserts because I don't like handling errors

    void ckit_os_system(char* command) {
        ckit_assert(system(command));
    }

    String ckit_os_path_join(char* path, char* to_join) {
        String ret = ckit_str_create(path);
        ckit_str_append_char(ret, OS_DELIMITER);
        ckit_str_append(ret, to_join);
        return ret;
    }

    String* ckit_os_get_lines_from_file(char* path) {
        size_t file_size = 0;
        u8* file_data = ckit_os_read_entire_file(path, &file_size);

        String* ret = NULLPTR;

        String current_line = ckit_str_create("");
        for (size_t i = 0; i < file_size; i++) {
            char current_char = file_data[i];
            if (current_char == '\n' || current_char == '\0') {
                ckit_vector_push(ret, current_line);
                current_line = ckit_str_create("");
                continue;
            }

            ckit_str_append_char(current_line, current_char);
        }

        ckit_free(file_data);

        return ret;
    }

    #if defined(PLATFORM_WINDOWS)
        void ckit_os_init() {
            cwd = ckit_str_create_custom("", sizeof("") - 1, PLATFORM_MAX_PATH);
        }

        String ckit_os_get_cwd() {
            char buffer[MAX_PATH];
            GetCurrentDirectoryA(MAX_PATH, buffer);

            String ret = ckit_str_create(buffer);

            return ret;
        }

        // Date: September 25, 2024
        // TODO(Jovanni): ckit_os_query_cycle_counter()
        u64 ckit_os_query_cycle_counter() {
            LARGE_INTEGER counter;
            QueryPerformanceCounter(&counter);

            // Date: September 25, 2024
            // TODO(Jovanni): idk if this is right?
            u64 cycles = counter.QuadPart;

            return cycles;
        }

        double ckit_os_query_performance_counter() {
            LARGE_INTEGER performance_counter_frequency;
            QueryPerformanceFrequency(&performance_counter_frequency);

            LARGE_INTEGER counter;
            QueryPerformanceCounter(&counter);
            double milliseconds_elapsed = ((double)(counter.QuadPart * 1000.0) / (double)performance_counter_frequency.QuadPart);

            return milliseconds_elapsed;
        }

        void ckit_os_ls();
        void ckit_os_get_items();
        void ckit_os_chdir(char* path) {
            ckit_os_path_exists(path);
            SetCurrentDirectoryA(path);
        }
        void ckit_os_mkdir();
        void ckit_os_create_file(char* path) {
            HANDLE file_handle = CreateFileA(path, GENERIC_READ, 0, NULLPTR, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULLPTR);
            ckit_assert(CloseHandle(file_handle));
        }

        bool ckit_os_path_exists(char* path) {
            return (GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES);
        }
        void ckit_os_run_subprocess(); // runs on seperate thread?
        void ckit_os_get_file_info();
        u8* ckit_os_read_entire_file(char* path, size_t* returned_file_size) {
            ckit_assert(ckit_os_path_exists(path));

            HANDLE file_handle = CreateFileA(path, GENERIC_READ, 0, NULLPTR, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULLPTR);
            if (file_handle == INVALID_HANDLE_VALUE) {
                return NULLPTR; // Failed to open file
            }

            LARGE_INTEGER large_int = {0};
            BOOL success = GetFileSizeEx(file_handle, &large_int);
            ckit_assert(success);

            uint64_t file_size = large_int.QuadPart + 1;
            if (file_size > SIZE_MAX) {
                CloseHandle(file_handle);
                return NULLPTR; // File too large to handle
            }

            u8* file_data = (u8*)ckit_alloc_custom((size_t)file_size, TAG_CKIT_EXPECTED_USER_FREE); // +1 for optional null-terminator
            if (file_data == NULLPTR) {
                CloseHandle(file_handle);
                return NULLPTR; // Memory allocation failed
            }

            DWORD bytes_read = 0;
            success = ReadFile(file_handle, file_data, (DWORD)file_size, &bytes_read, NULLPTR);
            CloseHandle(file_handle);

            ckit_assert(success && bytes_read == (file_size - 1));

            if (returned_file_size) {
                *returned_file_size = (size_t)file_size;
            }

            return file_data;
        }

        void ckit_os_get_mouse_position(int* mouse_x, int* mouse_y) {
            POINT point;
            ckit_assert(GetCursorPos(&point));
            *mouse_x = point.x;
            *mouse_y = point.y;
        }

        void ckit_os_push(char* path);
        void ckit_os_pop();

        void ckit_os_dir_append_subdir(CKIT_Directory* directory, CKIT_Directory* sub_directory);
        void ckit_os_dir_append_file(CKIT_Directory* directory, CKIT_File* file);
    #else
        void ckit_os_init() {
            cwd = ckit_str_create_custom("", 0, PLATFORM_MAX_PATH);
        }

        String ckit_os_get_cwd() {
            // return cwd;
            return "";
        }

        void ckit_os_ls();
        void ckit_os_get_items();
        void ckit_os_chdir();
        void ckit_os_mkdir();
        void ckit_os_create_file(char* path) {
            
        }

        bool ckit_os_path_exists(char* path) {
            FILE *fptr = fopen(path, "r");

            if (fptr == NULL) {
                return false;
            }

            fclose(fptr);

            return true;
        }

        double ckit_os_query_performance_counter() {
            ckit_assert_msg(false, "NOT IMPLEMENTED YET!");
            return 0.0;
        }

        void ckit_os_push(char* path) {

        }

        void ckit_os_pop() {

        }
        
        void ckit_os_run_subprocess() {

        }

        void ckit_os_get_file_info() {

        }

        u8* ckit_os_read_entire_file(char* path, size_t* returned_file_size) {
            ckit_assert_msg(ckit_os_path_exists(path), "Path doesn't exist\n");

            FILE* file_handle = fopen(path, "rb");
            if (file_handle == NULLPTR) {
                return NULLPTR;
            }

            fseek(file_handle, 0L, SEEK_END);
            size_t file_size = ftell(file_handle);
            rewind(file_handle);

            if (file_size == 0) {
                fclose(file_handle);
                if (returned_file_size) {
                    *returned_file_size = 0;
                }
                return NULLPTR;
            }

            u8* file_data = ckit_alloc(file_size + 1); // +1 for null terminator
            if (file_data == NULLPTR) {
                fclose(file_handle);
                return NULLPTR;
            }

            if (fread(file_data, file_size, 1, file_handle) != 1) {
                fclose(file_handle);
                ckit_free(file_data);
                ckit_assert_msg(false, "Error reading file");
                return NULLPTR;
            }

            fclose(file_handle);

            if (returned_file_size) {
                *returned_file_size = file_size + 1;
            }

            return file_data;
        }

        void ckit_os_get_mouse_position(int* mouse_x, int* mouse_y) {
            ckit_assert_msg(false, "NOT IMPLETMENTED FOR LINUX YET");
        }

        void ckit_os_dir_append_subdir(CKIT_Directory* directory, CKIT_Directory* sub_directory) {

        }
        void ckit_os_dir_append_file(CKIT_Directory* directory, CKIT_File* file) {

        }
    #endif
    
    // =============================================================================

    #if defined(PLATFORM_WINDOWS)
        void* platform_allocate(size_t number_of_bytes) {
            return VirtualAlloc(NULL, number_of_bytes, MEM_COMMIT, PAGE_READWRITE); 
        }

        /**
         * @brief Keep in mind that free only need a pointer that is pointign the the right data
         * to free so it doesn't matter if its a copy or not because the copy is pointing the he same data
         * 
         * @param data 
         */
        void platform_free(void* data) {
            VirtualFree(data, 0, MEM_RELEASE); // This is really interesting
            // Date: May 08, 2024
            // TODO(Jovanni): Look into VirtualProtect() this allows you to change memory access to NO_ACCESS
            // can help find use after free bugs interesting concept
        }

        void platform_console_init() {
            AllocConsole();

            HANDLE console_handle = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (console_handle == INVALID_HANDLE_VALUE) {
                LOG_ERROR("platform_console_init: ${FAILED}");
                platform_console_shutdown();
                return;
            }

            DWORD dwMode = 0;
            GetConsoleMode(console_handle, &dwMode);
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(console_handle, dwMode);

            ckit_assert(CloseHandle(console_handle));

            freopen("CONOUT$", "w", stdout);
            freopen("CONIN$", "r", stdin);
            freopen("CONOUT$", "w", stderr);
        }

        void platform_console_shutdown() {
            FreeConsole();
        }
    #elif defined(PLATFORM_LINUX) || defined(PLATFORM_APPLE)
        #define COLOR_RESET "\033[0"
        #define BLU "\033[34"
        #define GRN "\033[32"
        #define RED "\033[31"
        #define MAG "\033[35"
        #define WHT "\033[37"
        #define BLKB "\033[40"
        #define GRNB "\033[42"
        #define REDB "\033[41"

        int translate_color(unsigned char color) {
            switch (color) {
                case 0x0000: { // text color default clear.
                    return 0;
                } break;

                case 0x0001: { // text color contains blue.
                    return 1;
                } break;

                case 0x0002: { // text color contains green.
                    return 2;
                } break;

                case 0x0004: { // text color contains red.
                    return 3;
                } break;

                case (0x0001|0x0004): { // background color contains purple.
                    return 4;
                } break;

                case (0x0001|0x0002|0x0004): { // background color contains red.
                    return 5;
                } break;

                case 0x0010: { // background color contains blue.
                    return 6;
                } break;

                case 0x0020: { // background color contains green.
                    return 7;
                } break;

                case 0x0040: { // background color contains red.
                    return 8;
                } break;
            }

            ckit_assert(false);
            return -1;
        }

        void* platform_allocate(size_t number_of_bytes) {
            ckit_assert(number_of_bytes != 0);
            return malloc(number_of_bytes);
        }

        void platform_free(void* data) {
            free(data);
        }

        void platform_console_init() {

        }
        
        void platform_console_write(char* message, unsigned char color) {
            char* color_strings[] = {COLOR_RESET, BLU, GRN, RED, MAG, WHT, BLKB, GRNB, REDB};
            printf("\033[%sm%s\033[0m", color_strings[translate_color(color)], message);
        }
    #endif
#endif


#if defined(CKIT_IMPL_COLLECTIONS)
    //
    // ========== Start CKIT_Vector ==========
    //

    // Start experimenting with returning an index back
    // - [ ] Serializable
    // - [ ] Deletable
    // - [ ] Relocatable (shuffuling the data) (update the referece so you have the right index?)
    // - [ ] No dangling pointers

    // typedef struct CKIT_MemoryIndex {
    //     u32 index;
    // } CKIT_MemoryIndex;

    #define VECTOR_DEFAULT_CAPACITY 1
    #define CKIT_VECTOR_MAGIC "CKIT_MAGIC_VECTOR"

    internal void ckit_vector_check_magic(void* vector) {
        ckit_assert_msg(ckg_str_equal(ckit_vector_base(vector)->magic, sizeof(CKIT_VECTOR_MAGIC) - 1, CKIT_VECTOR_MAGIC, sizeof(CKIT_VECTOR_MAGIC) - 1), "Vector has wrong magic: {%s} got: {%s} \n", CKIT_VECTOR_MAGIC, ckit_vector_base(vector)->magic);
    }

    void* ckit_vector_grow(void* vector, size_t element_size, bool force_grow, char* file, u32 line, char* function) {
        if (vector == NULLPTR) {
            vector = ckit_alloc_custom(sizeof(CKIT_VectorHeader) + (VECTOR_DEFAULT_CAPACITY * element_size), TAG_CKIT_CORE_VECTOR);
            vector = (u8*)vector + sizeof(CKIT_VectorHeader);
            ckit_vector_base(vector)->count = 0;
            ckit_vector_base(vector)->capacity = VECTOR_DEFAULT_CAPACITY;
            ckit_vector_base(vector)->magic = CKIT_VECTOR_MAGIC;
        }

        ckit_vector_check_magic(vector);
        u32 count = ckit_vector_count(vector);
        u32 capactiy = ckit_vector_capacity(vector);

        if (force_grow || capactiy < count + 1) {
            u32 new_capactiy = capactiy * 2;
            size_t new_allocation_size = sizeof(CKIT_VectorHeader) + (new_capactiy * element_size);

            vector = MACRO_ckit_realloc(ckit_vector_base(vector), new_allocation_size, file, line, function);
            vector = (u8*)vector + sizeof(CKIT_VectorHeader);

            ckit_vector_base(vector)->count = count;
            ckit_vector_base(vector)->capacity = new_capactiy;
        }

        return vector;
    }
    
    void* MACRO_ckit_vector_to_array(void* vector, size_t element_size) {
        size_t total_size = ckit_vector_count(vector) * element_size;
        u8* ret = (u8*)ckit_alloc(ckit_vector_count(vector) * element_size);
        ckit_memory_copy(vector, ret, total_size, total_size);

        return ret;
    }

    void* MACRO_ckit_vector_reserve(size_t element_size, u32 inital_capacity, char* file, u32 line, char* function) {
        void* vector = MACRO_ckit_alloc(sizeof(CKIT_VectorHeader) + (inital_capacity * element_size), TAG_CKIT_CORE_VECTOR, file, line, function);
        vector = (u8*)vector + sizeof(CKIT_VectorHeader);
        ckit_vector_base(vector)->count = 0;
        ckit_vector_base(vector)->capacity = inital_capacity;
        ckit_vector_base(vector)->magic = CKIT_VECTOR_MAGIC;
        
        return vector;
    }

    void* MACRO_ckit_vector_free(void* vector) {
        ckit_vector_check_magic(vector);
        CKIT_VectorHeader* vector_base = ckit_vector_base(vector);
        vector_base->count = 0;
        vector_base->capacity = 0;
        ckit_free(vector_base);

        return vector_base;
    }
    //
    // ========== End CKIT_Vector ==========
    //

    //
    // ========== Start CKIT_HashMap ==========
    //
    #define CKIT_HASHMAP_DEFAULT_LOAD_FACTOR 0.75

    u32 ckit_hash_value(char *str) {
        unsigned long hash = 5381;
        int c;

        while ((c = *str++)) {
            hash = ((hash << 5) + hash) + c;
        }

        return hash;
    }

    float ckit_hashmap_load_factor(CKIT_HashMap* hashmap) {
        return (float)hashmap->count / (float)hashmap->capacity;
    }

    internal u64 ckit_hashmap_resolve_collision(CKIT_HashMap* hashmap, char* key, u64 inital_hash_index) {
        u64 cannonical_hash_index = inital_hash_index;

        while (hashmap->entries[cannonical_hash_index].key && !ckg_str_equal(hashmap->entries[cannonical_hash_index].key, ckg_cstr_length(hashmap->entries[cannonical_hash_index].key), key, ckg_cstr_length(key))) {
            cannonical_hash_index++;
            cannonical_hash_index = cannonical_hash_index % hashmap->capacity;
        }
        return cannonical_hash_index;
    }

    void ckit_hashmap_grow(CKIT_HashMap* hashmap) {
        if (ckit_hashmap_load_factor(hashmap) < CKIT_HASHMAP_DEFAULT_LOAD_FACTOR) {
            return;
        }

        u32 old_capacity = hashmap->capacity;
        hashmap->capacity *= 2;
        CKIT_HashMapEntry* new_entries = (CKIT_HashMapEntry*)ckit_alloc_custom(sizeof(CKIT_HashMapEntry) * hashmap->capacity, TAG_CKIT_CORE_HASHMAP);
        
        // rehash
        for (u32 i = 0; i < old_capacity; i++) {
            if (hashmap->entries[i].key != NULLPTR) {
                u32 index = ckit_hash_value(hashmap->entries[i].key) % hashmap->capacity;
                CKIT_HashMapEntry* cached_entries = hashmap->entries; // Have to cache the original hashmap entries because I need a pointer to it
                hashmap->entries = new_entries; // This is needed, otherwise resolve collisions is looking at the wrong entries buffer
                u64 real_index = ckit_hashmap_resolve_collision(hashmap, cached_entries[i].key, index);
                hashmap->entries = cached_entries;
                new_entries[real_index] = hashmap->entries[i];
            }
        }

        ckit_free(hashmap->entries);
        hashmap->entries = new_entries;
    }

    CKIT_HashMap* MACRO_ckit_hashmap_create(u32 hashmap_capacity, size_t element_size, bool is_pointer_type) {
        CKIT_HashMap* ret = (CKIT_HashMap*)ckit_alloc_custom(sizeof(CKIT_HashMap), TAG_CKIT_CORE_HASHMAP);
        ret->capacity = 1;
        ret->entries = (CKIT_HashMapEntry*)ckit_alloc_custom(sizeof(CKIT_HashMapEntry) * hashmap_capacity, TAG_CKIT_CORE_HASHMAP);
        ret->element_size = element_size;
        ret->count = 0;
        ret->capacity = hashmap_capacity;
        ret->is_pointer_type = is_pointer_type;

        return ret;
    }

    CKIT_HashMapEntry ckit_hashmap_entry_create(CKIT_HashMap* hashmap, char* key, void* value) {
        CKIT_HashMapEntry entry;
        entry.key = key;
        if (hashmap->is_pointer_type) {
            entry.value = value;
        } else {
            entry.value = ckit_alloc_custom(hashmap->element_size, TAG_CKIT_EXPECTED_USER_FREE); 
            // The reason why this is EXPECTED USER FREE IS BECAUSE YOU ARE supposed to free it from ckit_hashmap_put!
            ckit_memory_copy(value, entry.value, hashmap->element_size, hashmap->element_size); 
        }

        return entry;
    }

    CKIT_HashMap* MACRO_ckit_hashmap_free(CKIT_HashMap* hashmap) {
        if (!hashmap->is_pointer_type) {
            for (u32 i = 0; i < hashmap->capacity; i++) {
                if (hashmap->entries[i].value) {
                    ckit_free(hashmap->entries[i].value);
                }
            }
        }
        ckit_free(hashmap->entries);
        ckit_free(hashmap);

        return hashmap;
    }


    bool ckit_hashmap_entry_exists(CKIT_HashMap* hashmap, u32 index) {
        return hashmap->entries[index].key != NULLPTR;
    }

    void* ckit_hashmap_put(CKIT_HashMap* hashmap, char* key, void* value) {
        ckit_hashmap_grow(hashmap);

        void* ret = NULLPTR;

        u32 index =  ckit_hash_value(key) % hashmap->capacity;
        u64 real_index = ckit_hashmap_resolve_collision(hashmap, key, index);

        if (ckit_hashmap_entry_exists(hashmap, (u32)real_index)) {
            ret = hashmap->entries[real_index].value; // get previous value
            hashmap->entries[real_index] = ckit_hashmap_entry_create(hashmap, key, value);
        } else { // don't have the value
            hashmap->count++;
            hashmap->entries[real_index] = ckit_hashmap_entry_create(hashmap, key, value);
        }

        return ret;
    }

    void* ckit_hashmap_get(CKIT_HashMap* hashmap, char* key) {
        u32 index =  ckit_hash_value(key) % hashmap->capacity;
        u64 real_index = ckit_hashmap_resolve_collision(hashmap, key, index);

        return hashmap->entries[real_index].value;
    }

    bool ckit_hashmap_has(CKIT_HashMap* hashmap, char* key) {
        u32 index =  ckit_hash_value(key) % hashmap->capacity;
        u64 real_index = ckit_hashmap_resolve_collision(hashmap, key, index);

        return hashmap->entries[real_index].key != NULLPTR;
    }

    // open addressing
    // probing (linear or with double hashing)
    //
    // ========== End CKIT_HashMap ==========
    //

    //
    // ========== Start CKIT_HashSet ==========
    //
    #define CKIT_HASHSET_DEFAULT_LOAD_FACTOR 0.75

    float ckit_hashset_load_factor(CKIT_HashSet* hashset) {
        return (float)hashset->count / (float)hashset->capacity;
    }

    internal u64 ckit_hashset_resolve_collision(CKIT_HashSet* hashset, char* key, u64 inital_hash_index) {
        u64 cannonical_hash_index = inital_hash_index;
        while (hashset->entries[cannonical_hash_index] && !ckg_str_equal(hashset->entries[cannonical_hash_index], ckg_cstr_length(hashset->entries[cannonical_hash_index]), key, ckg_cstr_length(key))) {
            cannonical_hash_index++;
            cannonical_hash_index = cannonical_hash_index % hashset->capacity;
        }
        
        return cannonical_hash_index;
    }

    void ckit_hashset_grow(CKIT_HashSet* hashset) {
        if (ckit_hashset_load_factor(hashset) < CKIT_HASHSET_DEFAULT_LOAD_FACTOR) {
            return;
        }

        u32 old_capacity = hashset->capacity;
        hashset->capacity *= 2;
        char** temp_entries = (char**)ckit_alloc_custom(sizeof(char*) * hashset->capacity, TAG_CKIT_TEMP);
        
        // rehash
        for (u32 i = 0; i < old_capacity; i++) {
            if (hashset->entries[i] != NULLPTR) {
                u32 index =  ckit_hash_value(hashset->entries[i]) % hashset->capacity;
                char** cached_ptr = hashset->entries;
                hashset->entries = temp_entries;
                u64 real_index = ckit_hashset_resolve_collision(hashset, cached_ptr[i], index);
                hashset->entries = cached_ptr;

                temp_entries[real_index] = hashset->entries[i];
            }
        }

        ckit_free(hashset->entries);
        hashset->entries = temp_entries;
    }

    CKIT_HashSet* MACRO_ckit_hashset_create(u32 hashset_capacity) {
        CKIT_HashSet* ret = (CKIT_HashSet*)ckit_alloc_custom(sizeof(CKIT_HashSet), TAG_CKIT_CORE_HASHSET);
        ret->capacity = 1;
        ret->entries = (char**)ckit_alloc_custom(sizeof(char*) * hashset_capacity, TAG_CKIT_CORE_HASHSET);
        ret->count = 0;
        ret->capacity = hashset_capacity;

        return ret;
    }

    CKIT_HashSet* MACRO_ckit_hashset_free(CKIT_HashSet* hashset) {
        ckit_free(hashset->entries);
        ckit_free(hashset);

        return hashset;
    }


    bool ckit_hashset_entry_exists(CKIT_HashSet* hashset, u32 index) {
        return hashset->entries[index] != NULLPTR;
    }

    void ckit_hashset_put(CKIT_HashSet* hashset, char* key) {
        ckit_hashset_grow(hashset);

        u32 index =  ckit_hash_value(key) % hashset->capacity;
        u64 real_index = ckit_hashset_resolve_collision(hashset, key, index);
        if (hashset->entries[real_index] == NULLPTR) {
            hashset->entries[real_index] = key;
        }
    }

    bool ckit_hashset_has(CKIT_HashSet* hashset, char* key) {
        u32 index =  ckit_hash_value(key) % hashset->capacity;
        u64 real_index = ckit_hashset_resolve_collision(hashset, key, index);

        return hashset->entries[real_index] != NULLPTR;
    }
    //
    // ========== End CKIT_HashSet ==========
    //

    //
    // ========== Start CKIT_LinkedList ==========
    //
    CKIT_LinkedList* MACRO_ckit_linked_list_create(size_t element_size_in_bytes, bool is_pointer_type, char* file, u32 line, char* function) {
        CKIT_LinkedList* ret = (CKIT_LinkedList*)MACRO_ckit_alloc(sizeof(CKIT_LinkedList), TAG_CKIT_CORE_LINKED_LIST, file, line, function);
        ret->count = 0;
        ret->element_size_in_bytes = element_size_in_bytes;
        ret->head = NULLPTR;
        ret->tail = NULLPTR;
        ret->is_pointer_type = is_pointer_type;
        return ret;
    }

    CKIT_Node* MACRO_ckit_node_create(CKIT_LinkedList* linked_list, void* data) {
        CKIT_Node* ret = (CKIT_Node*)ckit_alloc_custom(sizeof(CKIT_Node), TAG_CKIT_CORE_LINKED_LIST);
        if (linked_list->is_pointer_type) {
            ret->data = data;
        } else {
            ret->data = ckit_alloc_custom(linked_list->element_size_in_bytes, TAG_CKIT_EXPECTED_USER_FREE); // user has to free
            ckit_memory_copy(data, ret->data, linked_list->element_size_in_bytes, linked_list->element_size_in_bytes); 
        }

        ret->element_size_in_bytes = linked_list->element_size_in_bytes;
        ret->next = NULLPTR;
        ret->prev = NULLPTR;
        return ret;
    }
    #define ckit_node_create(linked_list, data) MACRO_ckit_node_create(linked_list, data)

    /**
     * @brief returns a null ptr
     * 
     * @param node 
     * @return CKIT_Node* 
     */
    internal CKIT_Node* MACRO_ckit_node_free(CKIT_LinkedList* linked_list, CKIT_Node* node) {
        ckit_assert(linked_list);
        ckit_assert(node);
        node->element_size_in_bytes = 0;
        node->next = NULLPTR;
        node->prev = NULLPTR;
        ckit_free(node);

        return node;
    }
    #define ckit_node_free(linked_list, node) node = MACRO_ckit_node_free(linked_list, node)


    /**
     * @brief returns a null ptr
     * 
     * @param node 
     * @return CKIT_Node* 
     */
    internal CKIT_Node* MACRO_ckit_node_data_free(CKIT_LinkedList* linked_list, CKIT_Node* node) {
        ckit_assert(linked_list);
        ckit_assert(node);
        ckit_assert(node->data);
        node->element_size_in_bytes = 0;
        node->next = NULLPTR;
        node->prev = NULLPTR;
        if (!linked_list->is_pointer_type) {
            ckit_free(node->data);
        }
        ckit_free(node);

        return node;
    }
    #define ckit_node_data_free(linked_list, node) node = MACRO_ckit_node_data_free(linked_list, node)

    CKIT_Node* ckit_linked_list_insert(CKIT_LinkedList* linked_list, u32 index, void* data) {
        ckit_assert(linked_list);
        ckit_assert(data);
        ckit_assert(index >= 0);

        u32 old_count = linked_list->count++;
        if (linked_list->head == NULLPTR) { // there is not head and by definition no tail
            CKIT_Node* new_node_to_insert = ckit_node_create(linked_list, data);
            linked_list->head = new_node_to_insert;
            linked_list->tail = new_node_to_insert;

            return linked_list->head;
        }

        ckit_assert(index <= old_count);
        CKIT_Node* new_node_to_insert = ckit_node_create(linked_list, data);

        if (index == 0) { // insert at head
            linked_list->head->prev = new_node_to_insert;
            new_node_to_insert->next = linked_list->head;
            linked_list->head = new_node_to_insert;
            
            return new_node_to_insert;
        }

        if (index == old_count) { // insert at tail
            linked_list->tail->next = new_node_to_insert;
            new_node_to_insert->prev = linked_list->tail;
            linked_list->tail = new_node_to_insert;

            return new_node_to_insert;
        }

        CKIT_Node* current_node = linked_list->head; 
        for (u32 i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        new_node_to_insert->prev = current_node;
        new_node_to_insert->next = current_node->next;

        current_node->next->prev = new_node_to_insert;
        current_node->next = new_node_to_insert;

        return new_node_to_insert;
    }

    CKIT_Node* ckit_linked_list_get_node(CKIT_LinkedList* linked_list, u32 index) {
        ckit_assert(linked_list);
        CKIT_Node* current_node = linked_list->head; 
        for (u32 i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        return current_node;
    }

    void* ckit_linked_list_get(CKIT_LinkedList* linked_list, u32 index) {
        return ckit_linked_list_get_node(linked_list, index)->data;
    }

    void* ckit_linked_list_peek_head(CKIT_LinkedList* linked_list) {
        return ckit_linked_list_get_node(linked_list, 0)->data;
    }

    void* ckit_linked_list_peek_tail(CKIT_LinkedList* linked_list) {
        return ckit_linked_list_get_node(linked_list, linked_list->count - 1)->data;
    }

    CKIT_Node* ckit_linked_list_push(CKIT_LinkedList* linked_list, void* data) {
        return ckit_linked_list_insert(linked_list, linked_list->count, data);
    }

    u32 ckit_linked_list_node_to_index(CKIT_LinkedList* linked_list, CKIT_Node* address) {
        CKIT_Node* current_node = linked_list->head; 
        for (u32 i = 0; i < linked_list->count + 1; i++) {
            if (current_node == address) {
                return i;
            }
            current_node = current_node->next;
        }

        ckit_assert(false); // couldn't match a node to an address
        return 0; // should never get here
    }

    CKIT_Node ckit_linked_list_pop(CKIT_LinkedList* linked_list) {
        return ckit_linked_list_remove(linked_list, linked_list->count - 1);
    }

    CKIT_Node ckit_linked_list_remove(CKIT_LinkedList* linked_list, u32 index) {
        ckit_assert(linked_list); 
        ckit_assert(linked_list->count > 0); 
        ckit_assert(index >= 0);

        u32 old_count = linked_list->count--;
        if (index == 0 && old_count == 1) { // removing the head fully
            CKIT_Node ret = *linked_list->head;
            ckit_node_free(linked_list, linked_list->head);
            linked_list->head = NULLPTR;

            return ret;
        }

        if (index == 0) { // remove head
            CKIT_Node* cached_head = linked_list->head;
            linked_list->head = linked_list->head->next;
            CKIT_Node ret = *cached_head; 
            ckit_node_free(linked_list, cached_head);

            return ret;
        }

        ckit_assert(index < old_count);
        if (index == (old_count - 1)) { // remove tail
            CKIT_Node* cached_tail = linked_list->tail;
            linked_list->tail = linked_list->tail->prev;
            CKIT_Node ret = *cached_tail; 
            ckit_node_free(linked_list, cached_tail);

            return ret;
        }

        CKIT_Node* current_node = linked_list->head; 
        for (u32 i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        current_node->next->prev = current_node->prev;
        current_node->prev->next = current_node->next;
        CKIT_Node ret = *current_node; 
        ckit_node_free(linked_list, current_node);

        return ret;
    }

    void* MACRO_ckit_linked_list_free(CKIT_LinkedList* linked_list) {
        ckit_assert(linked_list); 
        CKIT_Node* current_node = linked_list->head; 
        CKIT_Node* next_node = NULLPTR; 
        for (u32 i = 0; i < linked_list->count; i++) {
            next_node = current_node->next;
            ckit_node_data_free(linked_list, current_node);
            current_node = next_node;
        }
        ckit_free(linked_list);

        return linked_list;
    }
    //
    // ========== End CKIT_LinkedList ==========
    //

    //
    // ========== Start CKIT_Queue ==========
    //
    CKIT_Queue* MACRO_ckit_queue_create(u32 inital_capacity, size_t element_size_in_bytes, bool is_pointer_type, char* file, u32 line, char* function) {
        CKIT_Queue* ret = (CKIT_Queue*)MACRO_ckit_alloc(sizeof(CKIT_Queue), TAG_CKIT_CORE_QUEUE, file, line, function);
        ret->element_size_in_bytes = element_size_in_bytes;
        ret->capacity = inital_capacity;
        ret->count = 0;
        ret->is_pointer_type = is_pointer_type;
        ret->data = MACRO_ckit_alloc(element_size_in_bytes * ret->capacity, TAG_CKIT_CORE_QUEUE, file, line, function);
        return ret;
    }

    CKIT_Queue* MACRO_ckit_queue_free(CKIT_Queue* queue) {
        queue->element_size_in_bytes = 0;
        queue->capacity = 0;
        queue->count = 0;
        ckit_free(queue->data);
        ckit_free(queue);
        return queue;
    }


    // Date: July 19, 2024
    // NOTE(Jovanni): If you have a count it kind of defeats the purpose sort of
    void* ckit_dequeue(CKIT_Queue* queue) {
        ckit_assert(queue->count > 0);
        ckit_assert(queue->has_next_to_read || queue->read_index != queue->write_index);
        queue->has_next_to_read = false;

        void* ret = NULLPTR;
        
        queue->read_index = queue->read_index % queue->capacity;
        if (queue->is_pointer_type) {
            u8** temp_ptr = (u8**)((u8*)queue->data + (queue->element_size_in_bytes * queue->read_index++));
            ret = *temp_ptr;
        } else {
            ret = ckit_alloc_custom(queue->element_size_in_bytes, TAG_CKIT_EXPECTED_USER_FREE);
            ckit_memory_copy((u8*)queue->data + (queue->element_size_in_bytes * queue->read_index++), ret, queue->element_size_in_bytes, queue->element_size_in_bytes);
        }
        queue->count--;

        return ret;
    }

    void ckit_enqueue(CKIT_Queue* queue, void* element) {
        ckit_assert(queue->count < queue->capacity);
        ckit_assert(!queue->has_next_to_read || queue->read_index != queue->write_index);
        queue->has_next_to_read = true;

        queue->write_index = queue->write_index % queue->capacity;
        if (queue->is_pointer_type) {
            u8** temp_ptr = (u8**)((u8*)queue->data + (queue->element_size_in_bytes * queue->write_index++));
            *temp_ptr = (u8*)element;
        }  else {
            ckit_memory_copy(element, (u8*)queue->data + (queue->element_size_in_bytes * queue->write_index++), queue->element_size_in_bytes, queue->element_size_in_bytes);
        }

        queue->count++;
    }

    u32 ckit_queue_capacity(CKIT_Queue* queue) {
        return queue->capacity;
    }

    u32 ckit_queue_count(CKIT_Queue* queue) {
        return queue->count;
    }
    //
    // ========== End CKIT_Queue ==========
    //
#endif

#if defined(CKIT_IMPL_UTILITIES)
    u8* file_data(FILE* handle, size_t file_size) {
        u8* buffer = (u8*)ckit_alloc_custom(file_size, TAG_CKIT_CORE_IO);
        ckit_assert_msg(fread(buffer, file_size, 1 , handle) != file_size, "Error reading file");
        rewind(handle);
        return buffer;
    }

    void file_open(FileSystem* file_system) {
        file_system->handle = fopen(file_system->file_name, "r");
        ckit_assert_msg(file_system->handle != NULLPTR, "FILE HANDLE IS NULL, CHECK INITIAL FILE NAME\n");
        fseek(file_system->handle, 0L, SEEK_END);
        file_system->file_size = ftell(file_system->handle);
        rewind(file_system->handle);

        file_system->data = file_data(file_system->handle, file_system->file_size);
    }

    String file_get_next_line(FileSystem* file_system) {
        String line = ckit_str_create("");
        char c;
        do {
            c = (char)fgetc(file_system->handle);
            if (c != '\n' && c != EOF) {
            ckit_str_append_char(line, c);
            }
            if (c == EOF) {
            file_system->reachedEOF = true;
            }
        } while (c != '\n' && c != EOF);
        return line;
    }

    char file_get_next_char(FileSystem* file_system) {
        return (char)fgetc(file_system->handle);
    }

    void file_close(FileSystem* file_system) { 
        ckit_free(file_system->data);
        fclose(file_system->handle);
    }

    FileSystem file_system_create(String file_name) {
        FileSystem file_system;
        file_system.handle = NULLPTR;
        file_system.reachedEOF = false;
        file_system.file_name = file_name;
        file_system.data = NULLPTR;
        return file_system;
    }

    // ======================================================================================
#endif