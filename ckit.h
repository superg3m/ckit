#pragma once 
/*===========================================================
 * File: CKit.h
 * Date: April 29, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

// CORE CKIT
#include "./Include/ckit_types.h"
#include "./Include/ckit_logger.h"
#include "./Include/ckit_memory.h"
#include "./Include/ckit_arena.h"
#include "./Include/ckit_memory_tracker.h"
#include "./Include/ckit_assert.h"
#include "./Include/ckit_event_system.h"
#include "./Include/ckit_string.h"
#include "./Include/ckit_char.h"
#include "./Include/ckit_file_system.h"
#include "./Include/ckit_math.h"
#include "./Include/ckit_vector.h"
#include "./Include/ckit_hashing.h"
#include "./Include/ckit_hashmap.h"
#include "./Include/ckit_hashset.h"
#include "./Include/ckit_linked_list.h"
#include "./Include/ckit_queue.h"
#include "./Include/ckit_stack.h"

// Modules
#include "./Include/bmp_parser.h"
#include "./Include/ckit_lexer.h"


#ifdef __cplusplus
extern "C" {
#endif
  void ckit_init();
  void ckit_cleanup();
#ifdef __cplusplus
}
#endif