#pragma once
/*===========================================================
 * File: core_assert.h
 * Date: April 23, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "./core_logger.h"

#define CRASH *((int*)0) = 0 

#define assert(expression, message) \
    if (!(expression)) { \
        LOG_FATAL("%s", message); \
        CRASH; \
    }


#define assert_in_function(expression, message, file, line, function) \
    if (!(expression)) { \
		char out_message[LOG_CHARACTER_LIMIT]; \
	    memory_zero(sizeof(out_message), out_message);  \
	    sprintf(out_message, "%s | File: %s:%d | function: %s", message, file, line, function);\
        LOG_FATAL("%s", out_message); \
        CRASH; \
    }
