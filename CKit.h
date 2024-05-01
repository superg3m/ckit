/*===========================================================
 * File: CKit.h
 * Date: April 29, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "include/core_types.h"
#include "include/core_logger.h"
#include "include/core_memory.h"
#include "include/core_arena.h"
#include "include/core_assert.h"
#include "include/core_event_system.h"

#ifdef DEPRECATED_STRING
	#include "include/deprecated/core_string.h"
#else 
	#include "include/core_string.h"
#endif

#ifdef DEPRECATED_DYNAMIC_ARRAY
	#include "include/deprecated/core_dynamic_array.h"
#else 
	#include "include/deprecated_two/core_dynamic_array.h"
#endif