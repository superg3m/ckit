#pragma once

#include "../../ckg/ckg.h"
//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define CKIT_ASSERT_ENABLED TRUE

// Date: August 14, 2024
// TODO(Jovanni): This has to get fixed because I have my own logging logic for ckit now! ${} needs to be handled here
#if CKIT_ASSERT_ENABLED == TRUE
	#define ckit_assert_msg(expression, message, ...) ckg_assert_msg(expression, message, ##__VA_ARGS__)
	#define ckit_assert(expression) ckg_assert(expression)
#else
		#define ckit_assert_msg(expression, message, ...)
		#define ckit_assert(expression) 
#endif
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++