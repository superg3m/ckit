#pragma once
/*===========================================================
 * File: core_memory_tracker.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_types.h"

typedef enum LogLevel LogLevel;
//========================== Begin Structs ==========================
typedef enum MemoryTag {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_TEMPORARY,
    MEMORY_TAG_INTERNAL,
    MEMORY_TAG_STRING,
    MEMORY_TAG_VECTOR,
    MEMORY_TAG_ARENA,
    MEMORY_TAG_COUNT
} MemoryTag;
//=========================== End Structs ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
  Boolean memory_tag_is_valid(MemoryTag memory_tag);
  Boolean memory_tag_is_unknown(MemoryTag memory_tag);
  void memory_tag_output(LogLevel log_level);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++

//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++




