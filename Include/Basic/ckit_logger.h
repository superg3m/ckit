#pragma once
/*===========================================================
 * File: ckit_logger.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../ckit_types.h"
//========================== Begin Types ==========================
#define LOGGING_ENABLED TRUE
#define LOG_LEVEL_CHARACTER_LIMIT 12

// Log Levels
#define LOG_LEVEL_FATAL 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_SUCCESS 4
#define LOG_LEVEL_PRINT 5
#define LOG_LEVEL_COUNT 6
typedef u8 CKIT_LogLevel;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    void MACRO_ckit_log_output(CKIT_LogLevel log_level, const char* message, ...);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
// #define log_output(log_level, message, ...) MACRO_log_output(log_level, message, ##__VA_ARGS__)
#define ckit_log_output(log_level, message, ...) MACRO_ckit_log_output(log_level, message, ##__VA_ARGS__)
#define LOG_PRINT(message, ...) ckit_log_output(LOG_LEVEL_PRINT, message, ##__VA_ARGS__)
#define LOG_SUCCESS(message, ...) ckit_log_output(LOG_LEVEL_SUCCESS, message, ##__VA_ARGS__)
#define LOG_DEBUG(message, ...) ckit_log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#define LOG_WARN(message, ...) ckit_log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...) ckit_log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#define LOG_FATAL(message, ...) ckit_log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++