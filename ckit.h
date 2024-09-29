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

CKIT_API void ckit_init();
CKIT_API void ckit_cleanup();

#if defined(CKIT_INCLUDE_TYPES)
	#define CKG_EXTERN
	#include "./ckg/ckg.h"

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
    typedef struct CKIT_StringHeader {
        u32 length; 
        size_t capacity;
        char* magic; 
    } CKIT_StringHeader;
    
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
    // Date: September 27, 2024
    // NOTE(Jovanni): This is a bit scary if you clear with the wrong one the length with be wrong!
    CKIT_API void ckit_str_clear(String str1);
    CKIT_API void ckit_cstr_clear(char* str1);

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
    CKIT_API CKIT_Vector2 ckit_vector2_lerp(CKIT_Vector2 a, CKIT_Vector2 b, double t);
    CKIT_API CKIT_Vector3 ckit_vector3_lerp(CKIT_Vector3 a, CKIT_Vector3 b, double t);
    CKIT_API CKIT_Vector4 ckit_vector4_lerp(CKIT_Vector4 a, CKIT_Vector4 b, double t);
    CKIT_API void ckit_vector2_print(CKIT_Vector2 point);

    CKIT_API CKIT_Vector2 ckit_vector2_spline_point(CKIT_Vector2* spline_points, u32 spline_points_count, double t);

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
    CKIT_API String* ckit_os_get_lines_from_file(const char* path);
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

// Date: September 24, 2024
// TODO(Jovanni): Make this a module
#if defined(CKIT_INCLUDE_PARSER)
    #include "./Include/Parser_and_Lexer/ckit_bmp_parser.h"
#endif 

//
// ===================================================== CKG_IMPL =====================================================
//

#if defined(CKIT_IMPL)
  void ckit_str_register_arena();
  void ckit_memory_arena_unregister_all();
  void platform_console_init();
  void platform_console_shutdown();

  void ckit_init() {
    ckit_tracker_init();
    memory_init();
    //platform_console_init();
    ckit_str_register_arena();
  }

  void ckit_cleanup() {
    ckit_memory_arena_unregister_all();
    //platform_console_shutdown();

    ckit_memory_report(LOG_LEVEL_WARN);
  }
#endif

#if defined(CKIT_IMPL_LOGGER)
    internal Boolean logging_is_initialized = FALSE;
    internal const char* logger_start_delimitor = "${";
    internal const char* logger_end_delimitor = "}";

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

    #if defined(PLATFORM_WINDOWS)
        #include <windows.h>
    #endif

    Boolean message_has_special_delmitor(const char* message) {
        s32 start_delimitor_index = ckit_str_index_of(message, logger_start_delimitor);
        s32 end_delimitor_index = ckit_str_index_of(message, logger_end_delimitor);
        return start_delimitor_index != -1 && end_delimitor_index != -1;
    }

    internal void special_print_helper(const char* message, CKIT_LogLevel log_level) {
        String middle_to_color = ckit_str_between_delimiters(message, logger_start_delimitor, logger_end_delimitor);
        if (!middle_to_color) {
            u32 message_length = ckit_cstr_length(message);
            Boolean found = message[message_length - 1] == '\n';
            printf("%.*s", message_length - found, message);
            return;
        }

        u32 start_delimitor_index = ckit_str_index_of(message, logger_start_delimitor);
        u32 end_delimitor_index = ckit_str_index_of(message, logger_end_delimitor);

        String left_side = ckit_substring(message, 0, start_delimitor_index - 1);
        String right_side = ckit_substring(message, end_delimitor_index + ckit_cstr_length(logger_end_delimitor), ckit_cstr_length(message) - 1);

        printf("%s%s%s%s", left_side, log_level_format[log_level], middle_to_color, CKG_COLOR_RESET);

        special_print_helper(right_side, log_level);

        return;
    }

    void MACRO_ckit_log_output(CKIT_LogLevel log_level, const char* message, ...) {
        char out_message[CKG_PLATFORM_CHARACTER_LIMIT];
        ckit_memory_zero(out_message, sizeof(out_message));
        
        va_list args_list;
        va_start(args_list, message);
        vsnprintf(out_message, CKG_PLATFORM_CHARACTER_LIMIT, message, args_list);
        va_end(args_list);

        printf("%s%s%s", log_level_format[log_level], log_level_strings[log_level], CKG_COLOR_RESET);
        
        u32 out_message_length = ckit_cstr_length(out_message);

        if (message_has_special_delmitor(out_message)) {
            special_print_helper(out_message, log_level);
        } else {
            Boolean found = out_message[out_message_length - 1] == '\n';
            printf("%s%.*s%s", log_level_format[log_level], out_message_length - found, out_message, CKG_COLOR_RESET);
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
        return 0; // never gets here
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
        
        global_memory_tag_pool_vector[tag_pool_index].total_pool_allocation_size += header->tag.allocation_info.allocation_size;
        global_total_pool_memory_internal += sizeof(CKIT_MemoryHeader);
        global_total_pool_memory_used += sizeof(CKIT_MemoryHeader) + header->tag.allocation_info.allocation_size;

        header->linked_list_address = ckg_linked_list_push(global_memory_tag_pool_vector[tag_pool_index].allocated_headers, header);
    }

    void ckit_tracker_remove(CKIT_MemoryHeader* header) {
        u64 tag_pool_index = ckit_tracker_get_tag_pool_index(header->tag.tag_id);

        global_memory_tag_pool_vector[tag_pool_index].total_pool_allocation_size -= header->tag.allocation_info.allocation_size;
        global_total_pool_memory_internal -= sizeof(CKIT_MemoryHeader);
        global_total_pool_memory_used -= sizeof(CKIT_MemoryHeader) + header->tag.allocation_info.allocation_size;
        
        u32 index = ckg_linked_list_node_to_index(global_memory_tag_pool_vector[tag_pool_index].allocated_headers,  header->linked_list_address);
        ckg_linked_list_remove(global_memory_tag_pool_vector[tag_pool_index].allocated_headers, index); 
    }

    void ckit_tracker_print_header(CKIT_MemoryHeader* header, CKIT_LogLevel log_level) {
        u8* data_address = (u8*)header + sizeof(CKIT_MemoryHeader);
        ckit_log_output(log_level, "=>     Address: %p | Size: %d(Bytes)\n", data_address, header->tag.allocation_info.allocation_size);
        ckit_log_output(log_level, "      - Allocation Site:\n");
        ckit_log_output(log_level, "          - File: %s:%d\n", header->tag.allocation_info.file_name, header->tag.allocation_info.line);
        ckit_log_output(log_level, "          - Function: %s\n", header->tag.allocation_info.function_name);
    }

    void ckit_tracker_print_pool(CKIT_MemoryTagPool* pool, CKIT_LogLevel log_level) {
        LOG_PRINT("============================== POOL NAME: %s | SIZE: %d | Items: %d ==============================\n", pool->pool_name, pool->total_pool_allocation_size, pool->allocated_headers->count);
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

    void ckit_tracker_print_all_pools(CKIT_LogLevel log_level) {
        if (global_total_pool_memory_used == 0) {
            LOG_SUCCESS("--- No Memory Leaks Detected --- \n");
            return;
        }

        LOG_ERROR("---------------------- Memory Leak Detected: %d(total) - %d(internal) = %d(Bytes) ----------------------\n", global_total_pool_memory_used, global_total_pool_memory_internal, global_total_pool_memory_used - global_total_pool_memory_internal);
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
        LOG_ERROR("------------------------------------------------------------------------------------------------------------\n");
    }

    //
    // Memory
    //

    internal CKG_LinkedList* registered_arenas = NULLPTR;

    void memory_init() {
        registered_arenas = ckg_linked_list_create(CKIT_Arena*, TRUE);

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
            CKIT_Arena* current_arena = ckg_linked_list_get(registered_arenas, i);
            if (arena == current_arena) {
                ckg_linked_list_remove(registered_arenas, i);
                break;
            }
        }
    }

    void ckit_memory_arena_unregister_all() {
        for (u32 i = 0; i < registered_arenas->count; i++) {
            CKIT_Arena* arena = ckg_linked_list_pop(registered_arenas).data;
            ckit_arena_free(arena);
        }

        ckg_linked_list_free(registered_arenas);
    }
#endif

#if defined(CKIT_IMPL_ARENA)
    #define ARENA_DEFAULT_ALLOCATION_SIZE MegaBytes(1)

    //========================== Begin Types ==========================
    typedef struct CKIT_ArenaPage {
        void* base_address;
        u64 capacity;
        u64 used;
    } CKIT_ArenaPage;

    typedef struct CKIT_Arena {
        const char* name;
        CKIT_LinkedList* pages;
        CKIT_ArenaFlag flag;
        u8 alignment;
    } CKIT_Arena;
    //=========================== End Types ===========================

    void ckit_memory_arena_register(CKIT_Arena* arena);
    void ckit_memory_arena_unregister(CKIT_Arena* arena);

    Boolean ckit_CKIT_ARENA_FLAG_is_set(CKIT_Arena* arena, CKIT_ArenaFlag flag) {
        return arena->flag == flag;
    }

    internal CKIT_ArenaPage* ckit_arena_page_create(size_t allocation_size) {
        CKIT_ArenaPage* ret = ckit_alloc_custom(sizeof(CKIT_ArenaPage), TAG_CKIT_CORE_ARENA);
        ret->used = 0;
        ret->capacity = allocation_size;
        ret->base_address = ckit_alloc_custom(allocation_size != 0 ? allocation_size : ARENA_DEFAULT_ALLOCATION_SIZE, TAG_CKIT_CORE_ARENA);

        return ret;
    }

    CKIT_Arena* MACRO_ckit_arena_create(size_t allocation_size, const char* name, CKIT_ArenaFlag flag, u8 alignment) {
        CKIT_Arena* arena = ckit_alloc_custom(sizeof(CKIT_Arena), TAG_CKIT_CORE_ARENA);
        arena->alignment = alignment == 0 ? 8 : alignment;
        arena->name = name;
        arena->flag = flag;
        arena->pages = ckit_linked_list_create(CKIT_ArenaPage*, TRUE);
        CKIT_ArenaPage* inital_page = ckit_arena_page_create(allocation_size);
        ckit_linked_list_push(arena->pages, inital_page);
        ckit_memory_arena_register(arena);

        return arena;
    }

    CKIT_Arena* MACRO_ckit_arena_free(CKIT_Arena* arena) {
        ckit_assert(arena);

        u32 cached_count = arena->pages->count;
        for (u32 i = 0; i < cached_count; i++) {
            CKIT_ArenaPage* page = ckit_linked_list_remove(arena->pages, 0).data;
            ckit_assert(page->base_address);
            ckit_free(page->base_address);
            ckit_free(page);
        }
        ckit_linked_list_free(arena->pages);

        ckit_memory_arena_unregister(arena);
        ckit_free(arena);

        return arena;
    }

    void ckit_arena_clear(CKIT_Arena* arena) {
        ckit_assert(arena);

        for (u32 i = 0; i < arena->pages->count; i++) {
            CKIT_ArenaPage* page = ckit_linked_list_get(arena->pages, i);
            ckit_assert(page->base_address);
            ckit_memory_zero(page->base_address, page->used);
            page->used = 0;
        }
    }

    void* MACRO_ckit_arena_push(CKIT_Arena* arena, size_t element_size) {
        ckit_assert(arena);

        CKIT_ArenaPage* last_page = ckit_linked_list_peek_tail(arena->pages);
        if (ckit_CKIT_ARENA_FLAG_is_set(arena, CKIT_ARENA_FLAG_FIXED)) { // single page assert if you run out of memory
            ckit_assert((last_page->used + element_size <= last_page->capacity));
        } else if (ckit_CKIT_ARENA_FLAG_is_set(arena, CKIT_ARENA_FLAG_CIRCULAR)) { // single page circle around if you run out of memory
            if ((last_page->used + element_size > last_page->capacity)) {
                last_page->used = 0;
                ckit_assert((last_page->used + element_size <= last_page->capacity));
            }
        } else if (ckit_CKIT_ARENA_FLAG_is_set(arena, CKIT_ARENA_FLAG_EXTENDABLE_PAGES)) { // Allocate memory, doesn't invalidate pointers
            ckit_assert(last_page->base_address);
            if ((last_page->used + element_size > last_page->capacity)) {
                CKIT_ArenaPage* next_page = ckit_arena_page_create((last_page->capacity + element_size) * 2);
                ckit_assert(next_page->base_address);
                ckit_linked_list_push(arena->pages, next_page);
            }
        } else {
            ckit_assert(FALSE);
        }

        last_page = ckit_linked_list_peek_tail(arena->pages); // tail might change

        u8* ret = ((u8*)last_page->base_address) + last_page->used;
        last_page->used += element_size;
        if ((last_page->used & (arena->alignment - 1)) != 0) { // if first bit is set then its not aligned
            last_page->used += (arena->alignment - (last_page->used & (arena->alignment - 1)));
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
        ckit_assert_msg(ckit_str_equal(ckit_str_header(str)->magic, CKIT_STR_MAGIC), "String: %s has the wrong magic: {%s} got: {%s} \n", str, CKIT_STR_MAGIC, ckit_str_header(str)->magic);
    }

    internal void ckit_str_insert_header(char** str, CKIT_StringHeader header) {
        ckit_memory_copy(&header, *str,  sizeof(header),  sizeof(header) + header.capacity);
        *str = (char*)(((u8*)(*str)) + sizeof(header));
    }

    internal inline String ckit_str_grow(String str, size_t new_allocation_size) {
        ckit_str_check_magic(str);
        CKIT_StringHeader header = *ckit_str_header(str);
        header.capacity = new_allocation_size;
        String ret = ckit_str_create_custom(str, header.capacity);
        
        return ret;
    }

    String ckit_str_create_custom(const char* c_string, size_t capacity) {
        u32 c_str_length = ckg_cstr_length(c_string);
        size_t true_capacity = capacity != 0 ? capacity : sizeof(char) * (c_str_length + 1);
        CKIT_StringHeader* header = MACRO_ckit_arena_push(string_arena, sizeof(CKIT_StringHeader) + true_capacity);
        header->length = c_str_length;
        header->capacity = true_capacity;
        header->magic = CKIT_STR_MAGIC;

        String ret = (String)((u8*)header + sizeof(CKIT_StringHeader));

        ckg_cstr_copy(ret, header->capacity, c_string);
        return ret;
    }

    Boolean ckit_str_equal(const char* str1, const char* str2) {
        return ckg_cstr_equal(str1, str2);
    }

    String MACRO_ckit_str_insert(String str, const char* to_insert, const u32 index) {
        u32 source_capacity = ckit_cstr_length(to_insert) + 1; 
        CKIT_StringHeader* header = ckit_str_header(str);
        if (header->length + source_capacity >= header->capacity) {
            str = ckit_str_grow(str, (header->length + source_capacity) * 2);
            header = ckit_str_header(str);
        }

        ckg_cstr_insert(str, header->capacity, to_insert, index);
        header->length += source_capacity - 1;
        return str;
    }

    String MACRO_ckit_str_insert_char(String str, const char to_insert, const u32 index) {
        u32 source_size = 1;
        CKIT_StringHeader* header = ckit_str_header(str);
        if (header->length + source_size >= header->capacity) {
            str = ckit_str_grow(str, (header->length + source_size) * 2);
            header = ckit_str_header(str);
        }

        ckg_cstr_insert_char(str, header->capacity, to_insert, index);
        header->length++;
        return str;
    }

    void ckit_str_clear(String str1) {
        ckg_cstr_clear(str1);
        ckit_str_recanonicalize_header_length(str1);
    }

    void ckit_cstr_clear(char* str1) {
        ckg_cstr_clear(str1);
    }

    void ckit_str_recanonicalize_header_length(String str) {
        u32 actual_length = ckit_cstr_length(str);
        ckit_str_header(str)->length = actual_length;
    }

    void ckit_str_copy(String str1, const char* source) {
        ckit_str_clear(str1);
        ckit_str_append(str1, source);
    }

    u32 ckit_cstr_length(const char* str) {
        return ckg_cstr_length(str);
    }

    u32 ckit_str_length(const String str) {
        ckit_str_check_magic(str);
        return ckit_str_header(str)->length;
    }

    String MACRO_ckit_str_append(String str, const char* source) {
        ckit_str_check_magic(str);
        ckit_assert_msg(str, "ckit_str_append: String passed is null\n");
        ckit_assert_msg(source, "ckit_str_append: Source passed is null\n");

        u32 source_capacity = ckit_cstr_length(source) + 1; 
        CKIT_StringHeader* header = ckit_str_header(str);
        if (header->length + source_capacity >= header->capacity) {
            str = ckit_str_grow(str, (header->length + source_capacity) * 2);
            header = ckit_str_header(str);
        }

        ckg_cstr_append(str, header->capacity, source);
        header->length += source_capacity - 1;
        return str;
    }

    String MACRO_ckit_str_append_char(String str, const char source) {
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

    String ckit_substring(const char* string_buffer, u32 start_range, u32 end_range) {
        ckit_assert(string_buffer);

        String ret_string = ckit_str_create_custom("", ((end_range + 1) - start_range) + 1);
        ckg_substring(string_buffer, ret_string, start_range, end_range);
        ckit_str_recanonicalize_header_length(ret_string);

        return ret_string;
    }

    internal String* ckit_str_split_helper(String* ret_buffer, const char* string_buffer, u32 offset_into_buffer, const char* delimitor) {
        s32 found_index = ckit_str_index_of(string_buffer + offset_into_buffer, delimitor);
        u32 offset_to_delimitor = found_index + offset_into_buffer;
        if (found_index == -1) {
            ckit_vector_push(ret_buffer, ckit_substring(string_buffer, offset_into_buffer, ckit_cstr_length(string_buffer) - 1));
            return ret_buffer;
        }

        String debug_test = ckit_substring(string_buffer, offset_into_buffer, offset_to_delimitor - 1);
        ckit_vector_push(ret_buffer, debug_test);
        return ckit_str_split_helper(ret_buffer, string_buffer, offset_to_delimitor + 1, delimitor);
    }

    String* ckit_str_split(const char* string_buffer, const char* delimitor) {
        ckit_assert(string_buffer);
        ckit_assert(delimitor);

        String* string_vector = NULLPTR;
        return ckit_str_split_helper(string_vector, string_buffer, 0, delimitor);
    }

    Boolean ckit_str_contains(const char* string_buffer, const char* contains) {
        ckit_assert(string_buffer);
        ckit_assert(contains);
        return ckg_cstr_contains(string_buffer, contains);
    }

    s32 ckit_str_index_of(const char* string_buffer, const char* sub_string) {
        ckit_assert(string_buffer);
        ckit_assert(sub_string);
        return ckg_cstr_index_of(string_buffer, sub_string);
    }

    s32 ckit_str_last_index_of(const char* string_buffer, const char* sub_string) {
        ckit_assert(string_buffer);
        ckit_assert(sub_string);
        return ckg_cstr_last_index_of(string_buffer, sub_string);
    }

    Boolean ckit_str_starts_with(const char* string_buffer, const char* starts_with) {
        ckit_assert(string_buffer);
        ckit_assert(starts_with);

        return ckg_cstr_starts_with(string_buffer, starts_with);
    }

    Boolean ckit_str_ends_with(const char* string_buffer, const char* ends_with) {
        ckit_assert(string_buffer);
        ckit_assert(ends_with);


        return ckg_cstr_ends_with(string_buffer, ends_with);
    }

    String ckit_str_reverse(const char* string_buffer) {
        ckit_assert(string_buffer);

        size_t reversed_string_buffer_capacity = ckit_cstr_length(string_buffer) + 1;
        String reversed_string_buffer = ckit_str_create_custom("", reversed_string_buffer_capacity);
        ckg_cstr_reverse(string_buffer, reversed_string_buffer, reversed_string_buffer_capacity);
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

    int ckit_str_to_int(const char* ascii_number) {
        ckit_assert(ascii_number);

        return atoi(ascii_number);
    }

    String ckit_str_between_delimiters(const char* str, const char* start_delimitor, const char* end_delimitor) {
        ckit_assert(str);
        ckit_assert(start_delimitor);
        ckit_assert(end_delimitor);
        ckit_assert(!ckit_str_equal(start_delimitor, end_delimitor));

        u32 start_delimitor_length = ckit_cstr_length(start_delimitor);
        s32 start_delimitor_index = ckit_str_index_of(str, start_delimitor); 
        s32 end_delimitor_index = ckit_str_index_of(str, end_delimitor);

        String ret = ckit_str_create("");

        if (start_delimitor_index == -1 || end_delimitor_index == -1) {
            return NULLPTR;
        }

        if (start_delimitor_index > end_delimitor_index) {
            return NULLPTR; // The start delimtor is after the end delimitor
        }

        u32 i = start_delimitor_index + start_delimitor_length;

        while (i < (u32)end_delimitor_index) {
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

    double lerp(double a, double b, double t) {
        return b + ((a - b) * t);
    }

    CKIT_Vector2 ckit_vector2_lerp(CKIT_Vector2 a, CKIT_Vector2 b, double t) {
        CKIT_Vector2 vec_ret = {0};
        vec_ret.x = lerp(a.x, b.x, t); 
        vec_ret.y = lerp(a.y, b.y, t); 
        return vec_ret;
    }

    CKIT_Vector3 ckit_vector3_lerp(CKIT_Vector3 a, CKIT_Vector3 b, double t) {
        CKIT_Vector3 vec_ret = {0};
        vec_ret.x = lerp(a.x, b.x, t);
        vec_ret.y = lerp(a.y, b.y, t);
        vec_ret.z = lerp(a.z, b.z, t);

        return vec_ret;
    }

    CKIT_Vector4 ckit_vector4_lerp(CKIT_Vector4 a, CKIT_Vector4 b, double t) {
        CKIT_Vector4 vec_ret = {0};
        vec_ret.x = lerp(a.x, b.x, t);
        vec_ret.y = lerp(a.y, b.y, t);
        vec_ret.z = lerp(a.z, b.z, t);
        vec_ret.w = lerp(a.w, b.w, t);

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

    Boolean ckit_rectangle_check_aabb_collision(CKIT_Rectangle2D rect1, CKIT_Rectangle2D rect2) {
        if (rect1.position.x < rect2.position.x + rect2.width && rect1.position.x + rect1.width > rect2.position.x &&
            rect1.position.y < rect2.position.y + rect2.height && rect1.position.y + rect1.height > rect2.position.y) {
            return TRUE;
        }

        return FALSE;
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
    internal String cached_directory = NULLPTR;

    // just asserts because I don't like handling errors

    void ckit_os_system(const char* command) {
        ckit_assert(system(command));
    }

    String ckit_os_path_join(char* path, const char* to_join) {
        String ret = ckit_str_create(path);
        ckit_str_append_char(ret, OS_DELIMITER);
        ckit_str_append(ret, to_join);
        return ret;
    }

    String* ckit_os_get_lines_from_file(const char* path) {
        size_t file_size = 0;
        u8* file_data = ckit_os_read_entire_file(path, &file_size);

        String* ret = NULLPTR;

        String current_line = ckit_str_create("");
        for (size_t i = 0; i <= file_size; i++) {
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
            cwd = ckit_str_create_custom("", PLATFORM_MAX_PATH);
        }

        String ckit_os_get_cwd() {
            TCHAR buffer[MAX_PATH];
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
        void ckit_os_chdir(const char* path) {
            ckit_os_path_exists(path);
            SetCurrentDirectory(path);
        }
        void ckit_os_mkdir();
        void ckit_os_create_file(const char* path) {
            HANDLE file_handle = CreateFileA(path, GENERIC_READ, 0, NULLPTR, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULLPTR);
            ckit_assert(CloseHandle(file_handle));
        }

        Boolean ckit_os_path_exists(const char* path) {
            return (GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES);
        }
        void ckit_os_run_subprocess(); // runs on seperate thread?
        void ckit_os_get_file_info();
        u8* ckit_os_read_entire_file(const char* path, size_t* returned_file_size) {
            ckit_assert(ckit_os_path_exists(path));

            HANDLE file_handle = CreateFileA(path, GENERIC_READ, 0, NULLPTR, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULLPTR);
            LARGE_INTEGER large_int = {0};
            BOOL success = GetFileSizeEx(file_handle, &large_int);
            ckit_assert(success);
            size_t file_size = large_int.QuadPart;
            DWORD bytes_read = 0;

            u8* file_data = ckit_alloc_custom(file_size, TAG_CKIT_EXPECTED_USER_FREE);
            success = ReadFile(file_handle, file_data, (DWORD)file_size, &bytes_read, NULLPTR);
            ckit_assert(success && CloseHandle(file_handle));

            *returned_file_size = file_size;

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
            cwd = ckit_str_create_custom("", PLATFORM_MAX_PATH);
        }

        String ckit_os_get_cwd() {
            // return cwd;
            return "";
        }

        void ckit_os_ls();
        void ckit_os_get_items();
        void ckit_os_chdir();
        void ckit_os_mkdir();
        void ckit_os_create_file(const char* path) {
            
        }

        Boolean ckit_os_path_exists(const char* path) {
            FILE *fptr = fopen(path, "r");

            if (fptr == NULL) {
                return FALSE;
            }

            fclose(fptr);

            return TRUE;
        }

        double ckit_os_query_performance_counter() {
            ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");
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

        u8* ckit_os_read_entire_file(const char* path, size_t* returned_file_size) {
            ckit_assert(ckit_os_path_exists(path));

            FILE* file_handle = fopen(path, "r");

            if (file_handle == NULL) {
                return FALSE;
            }

            fseek(file_handle, 0L, SEEK_END);
            size_t file_size = ftell(file_handle);
            rewind(file_handle);

            u8* file_data = ckit_alloc(file_size);
            ckg_assert_msg(fread(file_data, file_size, 1 , file_handle) != file_size, "Error reading file");
            rewind(file_handle);

            fclose(file_handle);
            *returned_file_size = file_size;

            return file_data;
        }

        void ckit_os_get_mouse_position(int* mouse_x, int* mouse_y) {
            ckit_assert_msg(FALSE, "NOT IMPLETMENTED FOR LINUX YET");
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

            ckit_assert(FALSE);
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
        
        void platform_console_write(const char* message, unsigned char color) {
            const char* color_strings[] = {COLOR_RESET, BLU, GRN, RED, MAG, WHT, BLKB, GRNB, REDB};
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
        ckit_assert_msg(ckit_str_equal(ckit_vector_base(vector)->magic, CKIT_VECTOR_MAGIC), "Vector has wrong magic: {%s} got: {%s} \n", CKIT_VECTOR_MAGIC, ckit_vector_base(vector)->magic);
    }

    void* ckit_vector_grow(void* vector, size_t element_size, Boolean force_grow, const char* file, const u32 line, const char* function) {
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

    void* MACRO_ckit_vector_reserve(size_t element_size, u32 inital_capacity, const char* file, const u32 line, const char* function) {
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
        ckit_assert_msg(vector_base, "WHAT"); 
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
        while (hashmap->entries[cannonical_hash_index].key && !ckit_str_equal(hashmap->entries[cannonical_hash_index].key, key)) {
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
        CKIT_HashMapEntry* new_entries = ckit_alloc_custom(sizeof(CKIT_HashMapEntry) * hashmap->capacity, TAG_CKIT_CORE_HASHMAP);
        
        // rehash
        for (u32 i = 0; i < old_capacity; i++) {
            if (hashmap->entries[i].key != NULLPTR) {
                u32 index =  ckit_hash_value(hashmap->entries[i].key) % hashmap->capacity;
                CKIT_HashMapEntry* cached_ptr = hashmap->entries;
                hashmap->entries = new_entries;
                u64 real_index = ckit_hashmap_resolve_collision(hashmap, cached_ptr[i].key, index);
                hashmap->entries = cached_ptr;

                new_entries[real_index] = hashmap->entries[i];
            }
        }

        ckit_free(hashmap->entries);
        hashmap->entries = new_entries;
    }

    CKIT_HashMap* MACRO_ckit_hashmap_create(u32 hashmap_capacity, size_t element_size, Boolean is_pointer_type) {
        CKIT_HashMap* ret = ckit_alloc_custom(sizeof(CKIT_HashMap), TAG_CKIT_CORE_HASHMAP);
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


    Boolean ckit_hashmap_entry_exists(CKIT_HashMap* hashmap, u32 index) {
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

    Boolean ckit_hashmap_has(CKIT_HashMap* hashmap, char* key) {
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
        while (hashset->entries[cannonical_hash_index] && !ckit_str_equal(hashset->entries[cannonical_hash_index], key)) {
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
        char** temp_entries = ckit_alloc_custom(sizeof(char*) * hashset->capacity, TAG_CKIT_TEMP);
        
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
        CKIT_HashSet* ret = ckit_alloc_custom(sizeof(CKIT_HashSet), TAG_CKIT_CORE_HASHSET);
        ret->capacity = 1;
        ret->entries = (char**)ckit_alloc_custom(sizeof(char*) * hashset_capacity, TAG_CKIT_CORE_HASHSET);
        ret->count = 0;
        ret->capacity = hashset_capacity;

        return ret;
    }

    CKIT_HashSet* MACRO_ckit_hashset_free(CKIT_HashSet* hashset) {
        ckit_free(hashset);

        return hashset;
    }


    Boolean ckit_hashset_entry_exists(CKIT_HashSet* hashset, u32 index) {
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

    Boolean ckit_hashset_has(CKIT_HashSet* hashset, char* key) {
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
    CKIT_LinkedList* MACRO_ckit_linked_list_create(size_t element_size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function) {
        CKIT_LinkedList* ret = MACRO_ckit_alloc(sizeof(CKIT_LinkedList), TAG_CKIT_CORE_LINKED_LIST, file, line, function);
        ret->count = 0;
        ret->element_size_in_bytes = element_size_in_bytes;
        ret->head = NULLPTR;
        ret->tail = NULLPTR;
        ret->is_pointer_type = is_pointer_type;
        return ret;
    }

    CKIT_Node* MACRO_ckit_node_create(CKIT_LinkedList* linked_list, void* data) {
        CKIT_Node* ret = ckit_alloc_custom(sizeof(CKIT_Node), TAG_CKIT_CORE_LINKED_LIST);
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

        ckit_assert(FALSE); // couldn't match a node to an address
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
    CKIT_Queue* MACRO_ckit_queue_create(u32 inital_capacity, size_t element_size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function) {
        CKIT_Queue* ret = MACRO_ckit_alloc(sizeof(CKIT_Queue), TAG_CKIT_CORE_QUEUE, file, line, function);
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
        queue->has_next_to_read = FALSE;

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
        queue->has_next_to_read = TRUE;

        queue->write_index = queue->write_index % queue->capacity;
        if (queue->is_pointer_type) {
            u8** temp_ptr = (u8**)((u8*)queue->data + (queue->element_size_in_bytes * queue->write_index++));
            *temp_ptr  = element;
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

    //
    // ========== Start CKIT_Stack ==========
    //
    typedef struct CKIT_Stack {
        CKIT_LinkedList* linked_list;
    } CKIT_Stack;

    CKIT_Stack* MACRO_ckit_stack_create(size_t size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function) {
        CKIT_Stack* stack = MACRO_ckit_alloc(sizeof(CKIT_Stack), TAG_CKIT_CORE_STACK, file, line, function);
        stack->linked_list = MACRO_ckit_linked_list_create(size_in_bytes, is_pointer_type, file, line, function);

        return stack;
    }

    void* ckit_stack_push(CKIT_Stack* stack, void* data) {
        return ckit_linked_list_push(stack->linked_list, data);
    }

    void* ckit_stack_pop(CKIT_Stack* stack) {
        return ckit_linked_list_pop(stack->linked_list).data;
    }

    void* MACRO_ckit_stack_free(CKIT_Stack* stack) {
        ckit_linked_list_free(stack->linked_list);
        ckit_free(stack);

        return stack;
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
            file_system->reachedEOF = TRUE;
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
        file_system.reachedEOF = FALSE;
        file_system.file_name = file_name;
        file_system.data = NULLPTR;
        return file_system;
    }

    // ======================================================================================

    u32 ckit_color_to_u32(CKIT_Color color) {
        const u32 alpha = ((int)(color.a) << 24);
        const u32 red = ((int)(color.r) << 16);
        const u32 green = ((int)(color.g) << 8);
        const u32 blue = ((int)(color.b) << 0);
                        
        const u32 rgba = alpha|red|green|blue;

        return rgba;
    }

    CKIT_Color ckit_color_from_u32(u32 color) {
        CKIT_Color ret = {0};

        ret.b = ((color >> 0) & 0xFF); 
        ret.g = ((color >> 8) & 0xFF); 
        ret.r = ((color >> 16) & 0xFF); 
        ret.a = ((color >> 24) & 0xFF); 
                    
        return ret;
    }

    CKIT_Color ckit_color_multiply(CKIT_Color color, float value) {
        CKIT_Color ret = {0};
        ret.r = color.r * value;
        ret.g = color.g * value;
        ret.b = color.b * value;
        ret.a = color.a * value;

        return ret;
    }

    u32 ckit_color_u32_multiply(u32 color, float value) {
        u8 b = (u8)(((color >> 0) & 0xFF)  * value);
        u8 g = (u8)(((color >> 8) & 0xFF)  * value);
        u8 r = (u8)(((color >> 16) & 0xFF) * value);
        u8 a = (u8)(((color >> 24) & 0xFF) * value);

        color = r|g|b|a;

        return color;
    }

    CKIT_Color ckit_color_alpha_blend(CKIT_Color front_color, CKIT_Color back_color) {
        CKIT_Color ret = {0};

        float normalized_back_alpha = (float)back_color.a / 255.0f;

        ret.a = back_color.a;
        ret.r = (back_color.r * normalized_back_alpha) + ((u32)front_color.r * (1 - normalized_back_alpha));
        ret.g = (back_color.g * normalized_back_alpha) + ((u32)front_color.g * (1 - normalized_back_alpha));
        ret.b = (back_color.b * normalized_back_alpha) + ((u32)front_color.b * (1 - normalized_back_alpha));

        return ret;
    }

    CKIT_Color ckit_color_u32_alpha_blend(u32 front_color_u32, u32 back_color_u32) {
        CKIT_Color front_color = {0};
        front_color.a = (u8)(((u32)front_color_u32 >> 24) & 0xFF);
        front_color.r = (u8)(((u32)front_color_u32 >> 16) & 0xFF);
        front_color.g = (u8)(((u32)front_color_u32 >> 8) & 0xFF);
        front_color.b = (u8)(((u32)front_color_u32 >> 0) & 0xFF);

        CKIT_Color back_color = {0};
        back_color.a = (u8)(((u32)back_color_u32 >> 24) & 0xFF);
        back_color.r = (u8)(((u32)back_color_u32 >> 16) & 0xFF);
        back_color.g = (u8)(((u32)back_color_u32 >> 8) & 0xFF);
        back_color.b = (u8)(((u32)back_color_u32 >> 0) & 0xFF);
                    
        return ckit_color_alpha_blend(front_color, back_color);
    }

	CKIT_Bitmap ckit_parser_load_bmp(u8* bmp_file_data, size_t file_size) {
		CKIT_Bitmap ret = {0};

		BmpHeader bmp_header;
		ckit_memory_copy(bmp_file_data, &bmp_header, sizeof(bmp_header), file_size);
		ret.bytes_per_pixel = (u8)(bmp_header.bits_per_pixel / 8);
		u8* bmp_data = bmp_file_data + bmp_header.data_offset;

		// size_t bmp_size = file_size - sizeof(BmpHeader);
		ret.width = bmp_header.width;
		ret.height = bmp_header.height;
		ret.memory = bmp_data;
		
		return ret;
	}
#endif