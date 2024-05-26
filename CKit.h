#pragma once 
/*===========================================================
 * File: CKit.h
 * Date: April 29, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "include/core/core_types.h"
#include "include/core/core_logger.h"
#include "include/core/core_memory.h"
#include "include/core/core_arena.h"
#include "include/core/core_assert.h"
#include "include/core/core_event_system.h"
#include "include/core/core_vector.h"
#include "include/core/core_string.h"
#include "include/core/core_file_system.h"

#ifdef __cplusplus
extern "C" {
#endif
  void CKit_init();
  void CKit_cleanup();
#ifdef __cplusplus
}
#endif
