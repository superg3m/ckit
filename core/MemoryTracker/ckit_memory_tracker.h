#pragma once

#include "../ckit_types.h"
//========================== Begin Types ==========================
typedef enum CKIT_MemoryTagID { // Reserved tags
    TAG_CKIT_CORE_STRING,
    TAG_CKIT_CORE_ARENA,
    TAG_CKIT_CORE_VECTOR,
    TAG_CKIT_CORE_STACK,
    TAG_CKIT_CORE_LINKED_LIST,
    TAG_CKIT_CORE_QUEUE,
    TAG_CKIT_CORE_FILE_SYSTEM,

    TAG_CKIT_MODULE_LEXER,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_BMP,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_OBJ,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_PNG,
    TAG_CKIT_MODULE_FILE_FORMAT_PARSER_JSON,

    TAG_CKIT_EXPECTED_USER_FREE,
    TAG_CKIT_INTERNAL,
    TAG_CKIT_RESERVED_COUNT
} CKIT_MemoryTagID;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	void ckit_tracker_init();
	void ckit_tracker_register_tag_pool(u32 tag_id, const char* name);

	void* memory_insert_header(void* data, CKIT_MemoryHeader header);
	void ckit_tracker_add(CKIT_MemoryTagID tag_id, CKIT_MemoryHeader header);
	void ckit_tracker_remove(CKIT_MemoryTagID tag_id, CKIT_MemoryHeader header);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++