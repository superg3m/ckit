#pragma once
/*===========================================================
 * File: core_assert.h
 * Date: April 23, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_logger.h"
#include "core_memory.h"

#define CORE_ASSERT TRUE

#define CRASH *((int *)0) = 0

void _assert_in_function(char* message, char* file, int line, char* function);

#define assert_info() {}

#if CORE_ASSERT == TRUE	
	#define assert_in_macro(expression, message) 										                       \
		if (!(expression))                                                       		                       \
		{                                                                        		                       \
			char message_buffer[PLATFORM_CHARACTER_LIMIT];                                              \
			memory_zero(PLATFORM_CHARACTER_LIMIT, message_buffer);                                      \
			sprintf(message_buffer, "%s | file: %s:%d | Function: %s", message, __FILE__, __LINE__, __func__); \
			LOG_FATAL("%s\n", message_buffer);                                                        \
			CRASH;                                                                                             \
		}                                                                                                      \

	#define assert_in_function(expression, message) \
		if (!(expression))                          \
		{                                           \
			LOG_FATAL("%s", message);               \
			CRASH;                                  \
		}                                           \

#else
		#define assert_in_function(expression, message)
		#define assert_in_macro(expression, message) 
#endif
