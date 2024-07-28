#pragma once

#include "ckg_types.h"
#include "ckg_logger.h"
#include "ckg_stack_trace.h"

//========================== Begin Types ==========================
#define CKG_ASSERT_ENABLED TRUE
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
#if CKG_ASSERT_ENABLED == TRUE	   
    #define ckg_assert(expression)                            \
    do {                                                      \
        if (!(expression)) {                                  \
            ckg_stack_trace_dump();                           \
            char msg[] = "Func: %s, File: %s:%d\n";           \
            CKG_LOG_FATAL(msg, __func__, __FILE__, __LINE__); \
            CRASH;                                            \
        }                                                     \
    } while (FALSE)

    #define ckg_assert_msg(expression, message, ...)	\
        do {                                            \
            if (!(expression)) {                        \
                ckg_stack_trace_dump();                 \
                CKG_LOG_FATAL(message, ##__VA_ARGS__);  \
                CRASH;                                  \
            }                                           \
        } while (FALSE)
#else
        #define ckg_assert(expression)
        #define ckg_assert_msg(expression, message, ...)
#endif
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++