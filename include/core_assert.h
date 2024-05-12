#pragma once
/*===========================================================
 * File: core_assert.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_logger.h"
#include "core_memory.h"



//========================== Begin Types ==========================
#define CORE_ASSERT TRUE

#define CRASH *((int *)0) = 0
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#if CORE_ASSERT == TRUE	
	#define assert_in_macro(expression, message) 										                                       \
		if (!(expression))                                                       		                         \
		{                                                                        		                         \
			char message_buffer[PLATFORM_CHARACTER_LIMIT];                                                     \
			memory_zero(message_buffer, PLATFORM_CHARACTER_LIMIT);                                             \
			sprintf(message_buffer, "%s | file: %s:%d | Function: %s", message, __FILE__, __LINE__, __func__); \
			LOG_FATAL("%s\n", message_buffer);                                                                 \
			CRASH;                                                                                             \
		}

	#define assert_in_function(expression, message, ...) \
		if (!(expression))                                 \
		{                                                  \
			LOG_FATAL(message, ##__VA_ARGS__);               \
			CRASH;                                           \
		}

#else
		#define assert_in_function(expression, message)
		#define assert_in_macro(expression, message) 
#endif
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

