#pragma once
/*===========================================================
 * File: ckit_logger.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../ckit_types.h"
#include "../../ckg/core/Logger/ckg_logger.h"
//========================== Begin Types ==========================
#define LOGGING_ENABLED TRUE
#define LOG_PRINT_ENABLED TRUE
#define LOG_INFO_ENABLED TRUE
#define LOG_DEBUG_ENABLED TRUE
#define LOG_WARN_ENABLED TRUE

#define LOG_LEVEL_CHARACTER_LIMIT 11

/*
typedef enum LogLevel {
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_PRINT,
    LOG_LEVEL_COUNT
} LogLevel;
*/
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    Boolean logger_init();
    // void logger_shutdown();
    // void MACRO_log_output(LogLevel log_level, const char* message, ...);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
// #define log_output(log_level, message, ...) MACRO_log_output(log_level, message, ##__VA_ARGS__)
#define log_output(log_level, message, ...) MACRO_ckg_log_output(log_level, message, ##__VA_ARGS__)
#define LOG_PRINT(message, ...) CKG_LOG_PRINT(message, ##__VA_ARGS__)
#define LOG_INFO(message, ...) CKG_LOG_SUCCESS(message, ##__VA_ARGS__)
#define LOG_DEBUG(message, ...) CKG_LOG_DEBUG(message, ##__VA_ARGS__)
#define LOG_WARN(message, ...) CKG_LOG_WARN(message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...) CKG_LOG_ERROR(message, ##__VA_ARGS__)
#define LOG_FATAL(message, ...) CKG_LOG_FATAL(message, ##__VA_ARGS__)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++







