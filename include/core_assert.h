#pragma once
/*===========================================================
 * File: ckg_assert.h
 * Date: May 18, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_logger.h"
#include "core_memory.h"

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
	#define assert_in_macro(expression, message) 																	\
		do { 																										\
		 	if (!(expression))                                                       		                       	\
			{                                                                        		                       	\
				char message_buffer[PLATFORM_CHARACTER_LIMIT];                                                     	\
				memory_zero(message_buffer, PLATFORM_CHARACTER_LIMIT);                                             	\
				sprintf(message_buffer, "%s | file: %s:%d | Function: %s", message, __FILE__, __LINE__, __func__); 	\
				LOG_FATAL("%s\n", message_buffer);                                                                 	\
				CRASH;                                                                                             	\
			}																									 	\
		} while (FALSE)


	#define assert_in_function(expression, message, ...) 	\
		do { 												\
			if (!(expression))                              \
			{                                               \
				LOG_FATAL(message, ##__VA_ARGS__);          \
				CRASH;                                      \
			}												\
		} while (FALSE)


#else
		#define assert_in_function(expression, message)
		#define assert_in_macro(expression, message) 
#endif
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

