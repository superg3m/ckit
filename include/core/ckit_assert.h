#pragma once
/*===========================================================
 * File: ckg_assert.h
 * Date: May 18, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "ckit_logger.h"
#include "ckit_memory.h"

//========================== Begin Types ==========================
#define ASSERT_ENABLED TRUE

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
#if ASSERT_ENABLED == TRUE

	#define assert_in_macro(expression, message, ...)														\
		do {																								\
			if (!(expression))                                                       		               	\
			{  																								\
				char buffer[PLATFORM_CHARACTER_LIMIT];                                        		        \
				ckg_memory_zero(buffer, PLATFORM_CHARACTER_LIMIT);                                             	\
				sprintf(buffer, "file: %s:%d | Function: %s | %s", __FILE__, __LINE__, __func__, message); 	\
				LOG_FATAL(buffer, ##__VA_ARGS__);                                             				\
				CRASH;                                                                                     	\
			}																								\
		} while (FALSE)

	#define assert_in_function(expression, message, ...) \
		do { 											 \
			if (!(expression))                           \
			{                                            \
				LOG_FATAL(message, ##__VA_ARGS__); 		 \
				CRASH;                                   \
			}											 \
		} while (FALSE)

#else
		#define assert_in_function(expression, message)
		#define assert_in_macro(expression, message) 
#endif
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

