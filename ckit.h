#pragma once 
/*===========================================================
 * File: CKit.h
 * Date: April 29, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

// CORE CKIT
#include "ckit_types.h"
#include "ckit_logger.h"
#include "ckit_memory.h"
#include "ckit_arena.h"
#include "ckit_memory_tracker.h"
#include "ckit_assert.h"
#include "ckit_event_system.h"
#include "ckit_string.h"
#include "ckit_char.h"
#include "ckit_file_system.h"
#include "ckit_math.h"
#include "ckit_vector.h"
#include "ckit_hashing.h"
#include "ckit_hashmap.h"
#include "ckit_hashset.h"
#include "ckit_linked_list.h"
#include "ckit_queue.h"
#include "ckit_stack.h"

// Modules
#include "./bmp_parser.h"
#include "./ckit_lexer.h"


#ifdef __cplusplus
extern "C" {
#endif
  void ckit_init();
  void ckit_cleanup();
#ifdef __cplusplus
}
#endif