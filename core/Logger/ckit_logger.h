#pragma once
/*===========================================================
 * File: ckit_logger.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../ckit_types.h"

//========================== Begin Types ==========================
#define LOGGING_ENABLED TRUE
#define LOG_PRINT_ENABLED TRUE
#define LOG_INFO_ENABLED TRUE
#define LOG_DEBUG_ENABLED TRUE
#define LOG_WARN_ENABLED TRUE

#define LOG_LEVEL_CHARACTER_LIMIT 11

typedef enum LogLevel {
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_PRINT,
    LOG_LEVEL_COUNT
} LogLevel;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    Boolean logger_init();
    void logger_shutdown();
    void MACRO_log_output(LogLevel log_level, const char* message, ...);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define log_output(log_level, message, ...) MACRO_log_output(log_level, message, ##__VA_ARGS__)

#if (LOG_PRINT_ENABLED)
    #define LOG_PRINT(message, ...) log_output(LOG_LEVEL_PRINT, message, ##__VA_ARGS__)
#else
    #define LOG_PRINT(message, ...)
#endif

#if (LOG_INFO_ENABLED)
    #define LOG_INFO(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
    #define LOG_INFO(message, ...)
#endif

#if (LOG_DEBUG_ENABLED)
    #define LOG_DEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
    #define LOG_DEBUG(message, ...)
#endif

#if (LOG_WARN_ENABLED)
    #define LOG_WARN(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#else
    #define LOG_WARN(message, ...)
#endif

#define LOG_ERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#define LOG_FATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++







