#pragma once
/*===========================================================
 * File: core_assert.h
 * Date: April 23, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "./core_logger.h"

#define CORE_ASSERT TRUE;

#define CRASH *((int*)0) = 0




#if CORE_ASSERT == TRUE
	
	#define assert_info(expression, message, file, line, name) \
		if (!(expression)) {                       			   \
			LOG_FATAL("%s\n", message);            			   \
			CRASH;                                 			   \
		}                                          			   \
		
    #define assert(expression, message) assert_info(expression, message, __FILE__, __LINE__, _func_)
    #define assert_in_function(expression, message, file, line, function) assert_info(expression, message, file, line, function)
#else 
    #define assert(expression, message) 
    #define assert_in_function(expression, message, file, line, function)
#endif
