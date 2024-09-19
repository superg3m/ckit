#pragma once 

#include "./Include/ckit_types.h"

// Basic
#include "./Include/Basic/ckit_assert.h"
#include "./Include/Basic/ckit_logger.h"
#include "./Include/Basic/ckit_memory.h"
#include "./Include/Basic/ckit_arena.h"
#include "./Include/Basic/ckit_char.h"
#include "./Include/Basic/ckit_string.h"
#include "./Include/Basic/ckit_math.h"
#include "./Include/Basic/ckit_os.h"
#include "./Include/Basic/ckit_platform_services.h"

// Collections
#include "./Include/Collections/ckit_vector.h"
#include "./Include/Collections/ckit_hashmap.h"
#include "./Include/Collections/ckit_hashset.h"
#include "./Include/Collections/ckit_linked_list.h"
#include "./Include/Collections/ckit_queue.h"
#include "./Include/Collections/ckit_stack.h"

// Utilities
#include "./Include/Utilities/ckit_file_system.h"

// Parser_and_Lexer
#include "./Include/Parser_and_Lexer/ckit_bmp_parser.h"

#ifdef __cplusplus
extern "C" {
#endif
  void ckit_init();
  void ckit_cleanup();
#ifdef __cplusplus
}
#endif