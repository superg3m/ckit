#pragma once
/*===========================================================
 * File: core_assert.h
 * Date: April 23, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_logger.h"

#define CORE_ASSERT TRUE;

#define CRASH *((int*)0) = 0




#if CORE_ASSERT == TRUE
	
	#define assert_info(expression, message, file, line, name) 		 \
		if (!(expression)) {                       			   		 \
			LOG_FATAL("%s", message);            			   		 \
			char message_buffer[LOG_CHARACTER_LIMIT];                \
        	memory_zero(LOG_CHARACTER_LIMIT, message_buffer);        \
        	sprintf("file: %s:%d | Function: %s", file, line, func); \
			LOG_FATAL(" | %s", message);                             \
			CRASH;                                 			   		 \
		}                                          			   		 \
	
	void _assert_in_function(char* message, char* file, int line, char* function) {
		assert_info(0, message, file, line, function);
	}

    #define assert(expression, message) assert_info(expression, message, __FILE__, __LINE__, __func__)
    #define assert_in_function(expression, message, file, line, function) assert_info(expression, message, file, line, function)
#else 
    #define assert(expression, message) 
    #define assert_in_function(expression, message, file, line, function)
#endif
