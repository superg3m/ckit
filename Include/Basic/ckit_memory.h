#pragma once

#include "../ckit_types.h"
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

    void ckit_tracker_print_header(CKIT_MemoryHeader* header, CKIT_LogLevel log_level);
    void ckit_tracker_print_pool(CKIT_MemoryTagPool* pool, CKIT_LogLevel log_level);

    void ckit_tracker_print_all_pools(CKIT_LogLevel log_level);

    // CKIT_MemoryHeader** ckit_tracker_get_all_headers();
    // CKIT_MemoryTagPool** ckit_tracker_get_all_pools();

    //
    // Memory
    //
    void memory_init();
    void* MACRO_ckit_alloc(size_t number_of_bytes, CKIT_MemoryTagID tag_id, const char* file, const u32 line, const char* function);
    void* MACRO_ckit_realloc(void* data, u64 new_allocation_size, const char* file, const u32 line, const char* function);
    void* MACRO_ckit_free(void* data);

    void ckit_memory_report(CKIT_LogLevel log_level);

    Boolean ckit_memory_compare(const void* buffer_one, const void* buffer_two, u32 b1_allocation_size, u32 b2_allocation_size);
    void ckit_memory_copy(const void* source, void* destination, size_t source_size, size_t destination_capacity);
    void ckit_memory_move(const void* source, void* destination, size_t source_payload_size);
    void ckit_memory_zero(void* data, size_t data_size_in_bytes);

    void MACRO_ckit_memory_delete_index(void* data, u32 number_of_elements, u32 data_capacity, size_t element_size_in_bytes, u32 index);
    void MACRO_ckit_memory_insert_index(void* data, u32 number_of_elements, u32 data_capacity, size_t element_size_in_bytes, u32 index);


#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_alloc(number_of_bytes) MACRO_ckit_alloc(number_of_bytes, TAG_USER_UNKNOWN, __FILE__, __LINE__, __func__)
#define ckit_alloc_custom(number_of_bytes, tag_id) MACRO_ckit_alloc(number_of_bytes, tag_id, __FILE__, __LINE__, __func__)
#define ckit_realloc(data, new_allocation_size) MACRO_ckit_realloc(data, new_allocation_size, __FILE__, __LINE__, __func__)
#define ckit_free(data) data = MACRO_ckit_free(data)

#define ckit_memory_delete_index(data, number_of_elements, data_capacity, index) MACRO_ckit_memory_delete_index(data, number_of_elements, data_capacity, sizeof(data[0]), index)
#define ckit_memory_insert_index(data, number_of_elements, data_capacity, element, index) MACRO_ckit_memory_delete_index(data, number_of_elements, data_capacity, sizeof(data[0]), index); data[index] = element;

#define ckit_tracker_insert_header(data, header) data = MACRO_ckit_tracker_insert_header(data, header)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++