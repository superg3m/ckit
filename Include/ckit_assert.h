#pragma once
/*===========================================================
 * File: ckg_assert.h
 * Date: May 18, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "ckg_assert.h"
//========================== Begin Types ==========================
#define CKIT_ASSERT_ENABLED TRUE
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
#if CKIT_ASSERT_ENABLED == TRUE
	#define ckit_assert_msg(expression, message, ...) ckg_assert_msg(expression, message, ##__VA_ARGS__)
	#define ckit_assert(expression) ckg_assert(expression)
#else
		#define ckit_assert_msg(expression, message, ...)
		#define ckit_assert(expression) 
#endif
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++