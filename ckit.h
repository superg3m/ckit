#pragma once 


// CORE CKIT
#include "./Include/Core/ckit_types.h"
#include "./Include/Core/ckit_logger.h"
#include "./Include/Core/ckit_memory_tracker.h"
#include "./Include/Core/ckit_memory.h"
#include "./Include/Core/ckit_arena.h"
#include "./Include/Core/ckit_assert.h"
#include "./Include/Core/ckit_event_system.h"
#include "./Include/Core/ckit_string.h"
#include "./Include/Core/ckit_char.h"
#include "./Include/Core/ckit_file_system.h"
#include "./Include/Core/ckit_math.h"
#include "./Include/Core/ckit_vector.h"
#include "./Include/Core/ckit_hashing.h"
#include "./Include/Core/ckit_hashmap.h"
#include "./Include/Core/ckit_hashset.h"
#include "./Include/Core/ckit_linked_list.h"
#include "./Include/Core/ckit_queue.h"
#include "./Include/Core/ckit_stack.h"

// Modules
#include "./Include/Modules/bmp_parser.h"
#include "./Include/Modules/ckit_lexer.h"


#ifdef __cplusplus
extern "C" {
#endif
  void ckit_init();
  void ckit_cleanup();
#ifdef __cplusplus
}
#endif