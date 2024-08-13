#pragma once 


// CORE CKIT
#include "./Include/Core/Basic/ckit_types.h"
#include "./Include/Core/Basic/ckit_assert.h"
#include "./Include/Core/Basic/ckit_logger.h"
#include "./Include/Core/Basic/ckit_memory.h"
#include "./Include/Core/Basic/ckit_arena.h"
#include "./Include/Core/Basic/ckit_char.h"
#include "./Include/Core/Basic/ckit_string.h"
#include "./Include/Core/Basic/ckit_math.h"
#include "./Include/Core/Basic/ckit_hashing.h"

#include "./Include/Core/Collections/ckit_vector.h"
#include "./Include/Core/Collections/ckit_hashmap.h"
#include "./Include/Core/Collections/ckit_hashset.h"
#include "./Include/Core/Collections/ckit_linked_list.h"
#include "./Include/Core/Collections/ckit_queue.h"
#include "./Include/Core/Collections/ckit_stack.h"

// Modules
#include "./Include/Modules/Structures/ckit_file_system.h"
#include "./Include/Modules/Parser_and_Lexer/bmp_parser.h"
// #include "./Include/Modules/Parser_and_Lexer/ckit_lexer.h"
//#include "./Include/Modules/Structures/ckit_event_system.h"



#ifdef __cplusplus
extern "C" {
#endif
  void ckit_init();
  void ckit_cleanup();
#ifdef __cplusplus
}
#endif