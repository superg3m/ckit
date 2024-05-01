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

#if CORE_ASSERT == TRUE
#define assert_info(expression, message, file, line, function)                       \
	if (!(expression))                                                               \
	{                                                                                \
		LOG_FATAL("%s", message);                                                    \
		char message_buffer[PLATFORM_COMMON_CHARACTER_LIMIT];                                    \
		memory_zero(PLATFORM_COMMON_CHARACTER_LIMIT, message_buffer);                            \
		sprintf(message_buffer, "file: %s:%d | Function: %s", file, line, function); \
		LOG_FATAL(" | %s", message);                                                 \
		CRASH;                                                                       \
	}

#define assert(expression, message) assert_info(expression, message, __FILE__, __LINE__, __func__)

#define assert_in_function(expression, message, file, line, function) \
	if (!(expression))                                                \
	{                                                                 \
		_assert_in_function(message, file, line, function)            \
	}

#else
#define assert(expression, message)
#define assert_in_function(expression, message, file, line, function)
#endif
