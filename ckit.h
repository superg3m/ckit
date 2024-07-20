#pragma once 
/*===========================================================
 * File: CKit.h
 * Date: April 29, 2024
 * Creator: Jovanni Djonaj
===========================================================*/


// CORE CKIT
#include "./core/ckit_types.h"
#include "./core/Logger/ckit_logger.h"
#include "./core/Memory/ckit_memory.h"
#include "./core/Memory/ckit_arena.h"
#include "./core/Assert/ckit_assert.h"
#include "./core/EventSystem/ckit_event_system.h"
#include "./core/String/ckit_string.h"
#include "./core/String/ckit_char.h"
#include "./core/FileIO/ckit_file_system.h"
#include "./core/Math/ckit_math.h"
#include "./core/Collection/Vector/ckit_vector.h"
#include "./core/Collection/HashMap/ckit_hashing.h"
#include "./core/Collection/HashMap/ckit_hashmap.h"
#include "./core/Collection/Queue/ckit_queue.h"

// Modules
#include "./modules/File_Format_Parser/Asset/BMP/bmp_parser.h"
#include "./modules/Lexer/ckit_lexer.h"


#ifdef __cplusplus
extern "C" {
#endif
  void ckit_init();
  void ckit_cleanup();
#ifdef __cplusplus
}
#endif
