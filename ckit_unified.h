#pragma once 

#if defined(CKIT_EXTERN)
    #define CKIT_API extern
#else
    #define CKIT_API
#endif

#if defined(CKIT_IMPL)
    #define CKIT_IMPL_TYPES
    #define CKIT_IMPL_LOGGER
    #define CKIT_IMPL_ASSERT
    #define CKIT_IMPL_MEMORY
    #define CKIT_IMPL_ARENA
    #define CKIT_IMPL_CSTRING
    #define CKIT_IMPL_CHAR
    #define CKIT_IMPL_COLLECTIONS
    #define CKIT_IMPL_PLATFORM
#endif

#define CKIT_INCLUDE_TYPES
#define CKIT_INCLUDE_LOGGER
#define CKIT_INCLUDE_ASSERT
#define CKIT_INCLUDE_MEMORY
#define CKIT_INCLUDE_ARENA
#define CKIT_INCLUDE_STRING
#define CKIT_INCLUDE_CHAR
#define CKIT_INCLUDE_COLLECTIONS
#define CKIT_INCLUDE_PLATFORM

CKIT_API void ckit_init();
CKIT_API void ckit_cleanup();

#if defined(CKIT_INCLUDE_TYPES)
	#define CKG_EXTERN
	#include "../ckg/ckg.h"

	typedef struct CKIT_Bitmap {
		u32 height;
		u32 width;
		u8 bytes_per_pixel;
		u8* memory;
	} CKIT_Bitmap;

	#define MEMORY_TAG_CHARACTER_LIMIT 16
	#define PLATFORM_CHARACTER_LIMIT 200
#endif

#if defined(CKIT_INCLUDE_ASSERT)
    #define CKIT_ASSERT_ENABLED TRUE

    // Date: August 14, 2024
    // TODO(Jovanni): This has to get fixed because I have my own logging logic for ckit now! ${} needs to be handled here
    #if CKIT_ASSERT_ENABLED == TRUE
        #define ckit_assert_msg(expression, message, ...) ckg_assert_msg(expression, message, ##__VA_ARGS__)
        #define ckit_assert(expression) ckg_assert(expression)
    #else
            #define ckit_assert_msg(expression, message, ...)
            #define ckit_assert(expression) 
    #endif
#endif 

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

    CKIT_API void MACRO_ckit_log_output(CKIT_LogLevel log_level, const char* message, ...);
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
        s64 line;
        const char* function_name;
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

    CKIT_API void ckit_tracker_init();
    CKIT_API void ckit_tracker_register_tag_pool(CKIT_MemoryTagID tag_id, const char* name);
    CKIT_API void* MACRO_ckit_tracker_insert_header(void* data, CKIT_MemoryHeader header);

    CKIT_API CKIT_MemoryHeader ckit_tracker_header_create(CKIT_MemoryTagID tag_id, size_t allocation_size, const char* file_name, const u64 line, const char* function_name);
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
    CKIT_API void* MACRO_ckit_alloc(size_t number_of_bytes, CKIT_MemoryTagID tag_id, const char* file, const u32 line, const char* function);
    CKIT_API void* MACRO_ckit_realloc(void* data, u64 new_allocation_size, const char* file, const u32 line, const char* function);
    CKIT_API void* MACRO_ckit_free(void* data);

    CKIT_API void ckit_memory_report(CKIT_LogLevel log_level);

    CKIT_API Boolean ckit_memory_compare(const void* buffer_one, const void* buffer_two, u32 b1_allocation_size, u32 b2_allocation_size);
    CKIT_API void ckit_memory_copy(const void* source, void* destination, size_t source_size, size_t destination_capacity);
    CKIT_API void ckit_memory_move(const void* source, void* destination, size_t source_payload_size);
    CKIT_API void ckit_memory_zero(void* data, size_t data_size_in_bytes);

    CKIT_API void MACRO_ckit_memory_delete_index(void* data, u32 number_of_elements, u32 data_capacity, size_t element_size_in_bytes, u32 index);
    CKIT_API void MACRO_ckit_memory_insert_index(void* data, u32 number_of_elements, u32 data_capacity, size_t element_size_in_bytes, u32 index);

    #define ckit_alloc(number_of_bytes) MACRO_ckit_alloc(number_of_bytes, TAG_USER_UNKNOWN, __FILE__, __LINE__, __func__)
    #define ckit_alloc_custom(number_of_bytes, tag_id) MACRO_ckit_alloc(number_of_bytes, tag_id, __FILE__, __LINE__, __func__)
    #define ckit_realloc(data, new_allocation_size) MACRO_ckit_realloc(data, new_allocation_size, __FILE__, __LINE__, __func__)
    #define ckit_free(data) data = MACRO_ckit_free(data)

    #define ckit_memory_delete_index(data, number_of_elements, data_capacity, index) MACRO_ckit_memory_delete_index(data, number_of_elements, data_capacity, sizeof(data[0]), index)
    #define ckit_memory_insert_index(data, number_of_elements, data_capacity, element, index) MACRO_ckit_memory_delete_index(data, number_of_elements, data_capacity, sizeof(data[0]), index); data[index] = element;

    #define ckit_tracker_insert_header(data, header) data = MACRO_ckit_tracker_insert_header(data, header)
#endif

#if defined(CKIT_INCLUDE_ARENA)
    // Date: September 24, 2024
    // TODO(Jovanni): Just make this defines
    typedef enum CKIT_ArenaFlag {
    CKIT_ARENA_FLAG_FIXED,
    CKIT_ARENA_FLAG_CIRCULAR,
    CKIT_ARENA_FLAG_EXTENDABLE_PAGES,
    CKIT_ARENA_FLAG_COUNT
    } CKIT_ArenaFlag;

    typedef struct CKIT_Arena CKIT_Arena;

    CKIT_API CKIT_Arena* MACRO_ckit_arena_create(size_t allocation_size, const char* name, CKIT_ArenaFlag flag, u8 alignment);
    CKIT_API void* MACRO_ckit_arena_push(CKIT_Arena* arena, size_t element_size);	
    CKIT_API CKIT_Arena* MACRO_ckit_arena_free(CKIT_Arena* arena);
    CKIT_API void ckit_arena_clear(CKIT_Arena* arena);

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
    typedef char* String;

    CKIT_API String ckit_str_create_custom(const char* c_str, size_t capacity);
    CKIT_API u32 ckit_cstr_length(const char* str);
    CKIT_API u32 ckit_str_length(const String str);
    CKIT_API Boolean ckit_str_equal(const char* str1, const char* str2);
    CKIT_API void ckit_str_copy(String str, const char* source);

    CKIT_API String MACRO_ckit_str_append(String str, const char* source);
    CKIT_API String MACRO_ckit_str_append_char(String str, const char source);

    CKIT_API String MACRO_ckit_str_insert(String str, const char* to_insert, const u32 index);
    CKIT_API String MACRO_ckit_str_insert_char(String str, const char to_insert, const u32 index);
    CKIT_API void ckit_str_clear(char* str1);

    // If you are copying data to the string and need to update the header state specifically for length
    CKIT_API void ckit_str_recanonicalize_header_length(String str);

    CKIT_API String ckit_substring(const char* string_buffer, u32 start_range, u32 end_range);

    // Little bit tricky. This method returns a vector of strings so 
    // ckit_vector_count(): to get the number of strings it returned
    //
    //  for (u32 i = 0; i < ckit_vector_count(string_vector); i++) {
    //     LOG_PRINT("%s\n", string_vector[i]);
    //  }
    //  ckit_vector_free(string_vector);
    CKIT_API String* ckit_str_split(const char* string_buffer, const char* delimitor);
    // Date: September 09, 2024
    // TODO(Jovanni): String* ckit_str_split_with_char(const char* string_buffer, const char delimitor);

    CKIT_API Boolean ckit_str_contains(const char* string_buffer, const char* contains);
    CKIT_API s32 ckit_str_index_of(const char* string_buffer, const char* sub_string);
    CKIT_API s32 ckit_str_last_index_of(const char* string_buffer, const char* sub_string);
    CKIT_API Boolean ckit_str_starts_with(const char* string_buffer, const char* starts_with);
    CKIT_API Boolean ckit_str_ends_with(const char* string_buffer, const char* ends_with);
    CKIT_API String ckit_str_reverse(const char* string_buffer);
    CKIT_API String ckit_str_int_to_str(int number);
    CKIT_API int ckit_str_to_int(const char* ascii_number);
    CKIT_API String ckit_str_between_delimiters(const char* str, const char* start_delimitor, const char* end_delimitor);

    #define ckit_str_create(str) ckit_str_create_custom(str, 0)
    #define ckit_str_insert(str, source, index) str = MACRO_ckit_str_insert(str, source, index);
    #define ckit_str_insert_char(str, source, index) str = MACRO_ckit_str_insert_char(str, source, index);
    #define ckit_str_append(str, source) str = MACRO_ckit_str_append(str, source);
    #define ckit_str_append_char(str, source) str = MACRO_ckit_str_append_char(str, source);
#endif

#if defined(CKIT_INCLUDE_MATH)
    #include <math.h>
    typedef struct CKIT_Vector2 {
        union {
            struct {
                double x;
                double y;
            };
            double v[2];
        };
    } CKIT_Vector2;

    typedef struct CKIT_Vector3 {
        union {
            struct {
                double x;
                double y;
                double z;
            };
            struct {
                double r;
                double g;
                double b;
            };
            double v[3];
        };
    } CKIT_Vector3;

    typedef struct CKIT_Vector4 {
        union {
            struct {
                double x;
                double y;
                double z;
                double w;
            };
            struct {
                double r;
                double g;
                double b;
                double a;
            };
            double v[4];
        };
    } CKIT_Vector4;

    typedef struct Mat2x2 {
        CKIT_Vector2 data[2];
    } Mat2x2;

    typedef struct Mat3x3 {
        CKIT_Vector3 data[3];
    } Mat3x3;

    typedef struct Mat4x4 {
        CKIT_Vector4 data[4];
    } Mat4x4;

    typedef struct CKIT_Rectangle2D {
        CKIT_Vector2 position;
        u32 width;
        u32 height;
    } CKIT_Rectangle2D;

    typedef struct CKIT_Rectangle3D {
        CKIT_Vector3 position;
        u32 length;
        u32 width;
        u32 height;
    } CKIT_Rectangle3D;

    typedef struct CKIT_Circle2D {
        CKIT_Vector2 position;
        u32 radius;
    } CKIT_Circle2D;

    typedef struct CKIT_Circle3D {
        CKIT_Vector3 position;
        u32 radius;
    } CKIT_Circle3D;

    CKIT_API int int_abs(int a);
    CKIT_API double float_abs(double a);
    CKIT_API double lerp(double a, double b, double t);
    CKIT_API CKIT_Vector2 CKIT_Vector2_lerp(CKIT_Vector2 a, CKIT_Vector2 b, double t);
    CKIT_API CKIT_Vector3 CKIT_Vector3_lerp(CKIT_Vector3 a, CKIT_Vector3 b, double t);
    CKIT_API CKIT_Vector4 CKIT_Vector4_lerp(CKIT_Vector4 a, CKIT_Vector4 b, double t);
    CKIT_API void CKIT_Vector2_print(CKIT_Vector2 point);

    CKIT_API CKIT_Vector2 CKIT_Vector2_spline_point(CKIT_Vector2* spline_points, u32 spline_points_count, double t);

    CKIT_API Mat2x2 ckit_mat2x2_projection_mat();
    CKIT_API Mat2x2 ckit_mat2x2_rotation_mat();
    CKIT_API Mat2x2 ckit_mat2x2_transposition();
    CKIT_API Mat2x2 ckit_mat2x2_mult();
    CKIT_API Mat2x2 ckit_mat2x2_add();

    CKIT_API Mat3x3 ckit_mat3x3_projection_mat();
    CKIT_API Mat3x3 ckit_mat3x3_rotation_mat();
    CKIT_API Mat3x3 ckit_mat3x3_transposition();
    CKIT_API Mat3x3 ckit_mat3x3_mult();
    CKIT_API Mat3x3 ckit_mat3x3_add();

    CKIT_API Mat4x4 ckit_mat4x4_projection_mat();
    CKIT_API Mat4x4 ckit_mat4x4_rotation_mat();
    CKIT_API Mat4x4 ckit_mat4x4_transposition();
    CKIT_API Mat4x4 ckit_mat4x4_mult();
    CKIT_API Mat4x4 ckit_mat4x4_add();

    CKIT_API CKIT_Rectangle2D ckit_rectangle2d_create(s32 x, s32 y, u32 width, u32 height);
    CKIT_API Boolean ckit_rectangle_check_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2);
    CKIT_API CKIT_Rectangle2D ckit_rectangle_get_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2);
    CKIT_API CKIT_Rectangle3D ckit_rectangle3d_create(s32 x, s32 y, s32 z, u32 length, u32 width, u32 height);
    CKIT_API CKIT_Circle2D ckit_circle2d_create(s32 x, s32 y, u32 radius);
    CKIT_API CKIT_Circle3D ckit_circle3d_create(s32 x, s32 y, s32 z, u32 radius);
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
    CKIT_API void ckit_os_create_file(const char* path);
    CKIT_API Boolean ckit_os_path_exists(const char* path);
    CKIT_API void ckit_os_run_subprocess();
    CKIT_API void ckit_os_get_file_info();
    CKIT_API String ckit_os_path_join(char* path, const char* to_join);
    CKIT_API void ckit_os_system(const char* command);

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
    CKIT_API CKIT_File* ckit_os_file_open(const char* path);
    CKIT_API String ckit_os_file_read_next_line(CKIT_File* file);
    CKIT_API String ckit_os_file_read_next_integer(CKIT_File* file);
    CKIT_API String ckit_os_file_read_next_float(CKIT_File* file);
    CKIT_API CKIT_File* ckit_os_file_close(CKIT_File* file);
    CKIT_API CKIT_File* MACRO_ckit_os_close_file(CKIT_File* file);
    CKIT_API u8* ckit_os_read_entire_file(const char* path, size_t* returned_file_size);
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
    CKIT_API void platform_console_write(const char* message, unsigned char color);
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

    CKIT_API void* ckit_vector_grow(void* vector, size_t element_size, Boolean force_grow, const char* file, const u32 line, const char* function);
    CKIT_API void* MACRO_ckit_vector_reserve(size_t element_size, u32 capacity, const char* file, const u32 line, const char* function);
    CKIT_API void* MACRO_ckit_vector_free(void* vector);

    // Date: July 13, 2024
    // NOTE(Jovanni): This doesn't have very good checking if the vector is NULLPTR
    // Attemptying to get the length/cap/pop of a NULLPTR will result in a uncontrolled crash

    // Date: September 12, 2024
    // TODO(Jovanni): MAKE SURE YOU TEST ckit_vector_remove_at() and ckit_vector_insert_at()
    #define ckit_vector_base(vector) ((CKIT_VectorHeader*)(((u8*)vector) - sizeof(CKIT_VectorHeader)))
    #define ckit_vector_count(vector) (*ckit_vector_base(vector)).count
    #define ckit_vector_capacity(vector) (*ckit_vector_base(vector)).capacity
    #define ckit_vector_push(vector, element) vector = ckit_vector_grow(vector, sizeof(element), FALSE, __FILE__, __LINE__, __func__); vector[ckit_vector_base(vector)->count++] = element
    #define ckit_vector_reserve(capactiy, type) (type*)MACRO_ckit_vector_reserve(sizeof(type), capactiy, __FILE__, __LINE__, __func__)
    #define ckit_vector_pop(vector) vector[--ckit_vector_base(vector)->count]
    #define ckit_vector_remove_at(vector, index) ckit_memory_delete_index(vector, ckit_vector_count(vector), ckit_vector_capacity(vector), index); ckit_vector_base(vector)->count--
    #define ckit_vector_insert_at(vector, element, index) ckit_memory_insert_index(vector, ckit_vector_count(vector), ckit_vector_capacity(vector), element, index); ckit_vector_base(vector)->count++
    #define ckit_vector_free(vector) vector = MACRO_ckit_vector_free(vector)
    //
    // ========== End CKIT_Vector ==========
    //

    //
    // ========== Start CKIT_HashMap ==========
    //
    #define CKIT_HASHMAP_DEFAULT_LOAD_FACTOR 0.75     // TODO(Jovanni): Move this to IMPL

    typedef struct CKIT_HashMapEntry {
        String key;
        void* value;
    } CKIT_HashMapEntry;

    typedef struct CKIT_HashMap {
        u32 capacity;
        u32 count;
        CKIT_HashMapEntry* entries; // not a vector
        size_t element_size;
        Boolean is_pointer_type;
    } CKIT_HashMap;

    CKIT_API u32 ckit_hash_value(char *str);
    CKIT_API CKIT_HashMap* MACRO_ckit_hashmap_create(u32 hashmap_capacity, size_t element_size, Boolean is_pointer_type);
    CKIT_API CKIT_HashMap* MACRO_ckit_hashmap_free(CKIT_HashMap* hashmap);

    // Returns previous value
    CKIT_API void* ckit_hashmap_put(CKIT_HashMap* hashmap, char* key, void* value);
    CKIT_API void* ckit_hashmap_get(CKIT_HashMap* hashmap, char* key);
    CKIT_API Boolean ckit_hashmap_has(CKIT_HashMap* hashmap, char* key);

    #define ckit_hashmap_create(capacity, type, is_pointer_type) MACRO_ckit_hashmap_create(capacity, sizeof(type), is_pointer_type)
    #define ckit_hashmap_free(hashmap) hashmap = MACRO_ckit_hashmap_free(hashmap)
    //
    // ========== End CKIT_HashMap ==========
    //

    //
    // ========== Start CKIT_HashSet ==========
    //
    #define CKIT_HASHSET_DEFAULT_LOAD_FACTOR 0.75 // TODO(Jovanni): MOVE THIS TO IMPL

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
    CKIT_API Boolean ckit_hashset_has(CKIT_HashSet* hashset, char* key);

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
        Boolean is_pointer_type;
    } CKIT_LinkedList;

    CKIT_API CKIT_LinkedList* MACRO_ckit_linked_list_create(size_t element_size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function);
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
    #define ckit_linked_list_free(linked_list) linked_list = MACRO_ckit_linked_list_free(linked_list)
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
        Boolean has_next_to_read;
        u32 write_index;
        Boolean is_pointer_type;
    } CKIT_Queue;

    CKIT_API CKIT_Queue* MACRO_ckit_queue_create(u32 inital_capacity, size_t element_size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function);
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

    //
    // ========== Start CKIT_Stack ==========
    //
    typedef struct CKIT_Stack CKIT_Stack;

    CKIT_API CKIT_Stack* MACRO_ckit_stack_create(size_t size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function);
    CKIT_API void* ckit_stack_push(CKIT_Stack* stack, void* data);
    // Date: July 22, 2024
    // NOTE(Jovanni): If pointer type is true you must free this yourself, trivially this should be able to go into an arena.
    CKIT_API void* ckit_stack_pop(CKIT_Stack* stack);
    CKIT_API void* MACRO_ckit_stack_free(CKIT_Stack* stack);

    #define ckit_stack_create(type, is_pointer_type) MACRO_ckit_stack_create(sizeof(type), is_pointer_type, __FILE__, __LINE__, __func__)
    #define ckit_stack_free(stack) MACRO_ckit_stack_free(stack)
    //
    // ========== End CKIT_Stack ==========
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
        Boolean reachedEOF;
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
    // ========== Start CKIT_Color ==========
    //
    typedef CKIT_Vector4 CKIT_Color;

	CKIT_API u32 ckit_color_to_u32(CKIT_Color color);
	CKIT_API CKIT_Color ckit_color_from_u32(u32 color);
	CKIT_API CKIT_Color ckit_color_alpha_blend(CKIT_Color front_color, CKIT_Color back_color);
	CKIT_API CKIT_Color ckit_color_u32_alpha_blend(u32 front_color_u32, u32 back_color_u32);

	/**
	 * @brief value from 0.0 to 1.0
	 * 
	 * @param color 
	 * @param value 
	 * @return CKIT_Color 
	 */
	CKIT_API CKIT_Color ckit_color_multiply(CKIT_Color color, float value);

    #define CKIT_COLOR_BLACK ((CKIT_Color){0, 0, 0, 255})
    #define CKIT_COLOR_RED ((CKIT_Color){255, 0, 0, 255})
    #define CKIT_COLOR_BLUE ((CKIT_Color){0, 0, 255, 255})
    #define CKIT_COLOR_GREEN ((CKIT_Color){0, 255, 0, 255})
    #define CKIT_COLOR_WHITE ((CKIT_Color){255, 255, 255, 255})
    #define CKIT_COLOR_PINK ((CKIT_Color){255, 105, 180, 255})
    #define CKIT_COLOR_LIME ((CKIT_Color){0, 255, 128, 255})
    #define CKIT_COLOR_CYAN ((CKIT_Color){0, 255, 255, 255})
    #define CKIT_COLOR_PURPLE ((CKIT_Color){128, 0, 128, 255})
    #define CKIT_COLOR_YELLOW ((CKIT_Color){255, 255, 0, 255})
    //
    // ========== End CKIT_Color ==========
    //
#endif

// Date: September 24, 2024
// TODO(Jovanni): Make this a module
#if defined(CKIT_INCLUDE_PARSER)
    #include "./Include/Parser_and_Lexer/ckit_bmp_parser.h"
#endif 

//
// ===================================================== CKG_IMPL =====================================================
//

