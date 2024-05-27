#pragma once 
/*===========================================================
 * File: CKit.h
 * Date: April 29, 2024
 * Creator: Jovanni Djonaj
===========================================================*/


// CORE CKIT
#include "include/core/ckit_types.h"
#include "include/core/ckit_logger.h"
#include "include/core/ckit_memory.h"
#include "include/core/ckit_arena.h"
#include "include/core/ckit_assert.h"
#include "include/core/ckit_event_system.h"
#include "include/core/ckit_vector.h"
#include "include/core/ckit_string.h"
#include "include/core/ckit_file_system.h"
#include "include/core/ckit_math.h"

// ASSET LOADINGs


#ifdef __cplusplus
extern "C" {
#endif
  void CKit_init();
  void CKit_cleanup();
#ifdef __cplusplus
}
#endif
