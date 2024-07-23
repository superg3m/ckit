#pragma once

#include "../ckit_types.h"

#include "../Collection/LinkedList/ckit_linked_list.h"
#include "../../ckg/core/LinkedList/ckg_linked_list.h"

#include "../Logger/ckit_logger.h"

// Date: July 23, 2024
// TODO(Jovanni): SWITCH THIS TO BE INSTANCE BASED SO I CAN USE IT FOR ARENAS TOO!!!
// TODO(Jovanni): SWITCH THIS TO BE INSTANCE BASED SO I CAN USE IT FOR ARENAS TOO!!!
// TODO(Jovanni): SWITCH THIS TO BE INSTANCE BASED SO I CAN USE IT FOR ARENAS TOO!!!
// TODO(Jovanni): SWITCH THIS TO BE INSTANCE BASED SO I CAN USE IT FOR ARENAS TOO!!!
// TODO(Jovanni): SWITCH THIS TO BE INSTANCE BASED SO I CAN USE IT FOR ARENAS TOO!!!
// TODO(Jovanni): SWITCH THIS TO BE INSTANCE BASED SO I CAN USE IT FOR ARENAS TOO!!!
// TODO(Jovanni): SWITCH THIS TO BE INSTANCE BASED SO I CAN USE IT FOR ARENAS TOO!!!
// TODO(Jovanni): SWITCH THIS TO BE INSTANCE BASED SO I CAN USE IT FOR ARENAS TOO!!!
// TODO(Jovanni): SWITCH THIS TO BE INSTANCE BASED SO I CAN USE IT FOR ARENAS TOO!!!
// TODO(Jovanni): SWITCH THIS TO BE INSTANCE BASED SO I CAN USE IT FOR ARENAS TOO!!!
// TODO(Jovanni): SWITCH THIS TO BE INSTANCE BASED SO I CAN USE IT FOR ARENAS TOO!!!
// TODO(Jovanni): SWITCH THIS TO BE INSTANCE BASED SO I CAN USE IT FOR ARENAS TOO!!!
// TODO(Jovanni): SWITCH THIS TO BE INSTANCE BASED SO I CAN USE IT FOR ARENAS TOO!!!

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
    TAG_CKIT_INTERNAL,
    TAG_CKIT_RESERVED_COUNT
} CKIT_MemoryTagID;

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
	void ckit_tracker_init();
	void ckit_tracker_register_tag_pool(CKIT_MemoryTagID tag_id, const char* name);
    void* MACRO_ckit_tracker_insert_header(void* data, CKIT_MemoryHeader header);

    CKIT_MemoryHeader ckit_tracker_header_create(CKIT_MemoryTagID tag_id, size_t allocation_size, char* file_name, u64 line, char* function_name);
	void* memory_insert_header(void* data, CKIT_MemoryHeader header);
    void ckit_tracker_add(CKIT_MemoryHeader* header);
    void ckit_tracker_remove(CKIT_MemoryHeader* header);
    CKIT_MemoryHeader* ckit_tracker_get_header(void* data);

    void ckit_tracker_print_header(CKIT_MemoryHeader* header, CKG_LogLevel log_level);
    void ckit_tracker_print_pool(CKIT_MemoryTagPool* pool, CKG_LogLevel log_level);

    CKIT_MemoryHeader** ckit_tracker_get_all_headers();
    CKIT_MemoryTagPool** ckit_tracker_get_all_pools();
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_tracker_insert_header(data, header) data = MACRO_ckit_tracker_insert_header(data, header)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++