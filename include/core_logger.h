#pragma once
/*===========================================================
 * File: core_logger.h
 * Date: April 30, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_types.h"
#include "core_assert.h"

#define LOGGING_ENABLED TRUE
#define LOG_PRINT_ENABLED TRUE
#define LOG_INFO_ENABLED TRUE
#define LOG_DEBUG_ENABLED TRUE
#define LOG_WARN_ENABLED TRUE

#define LOG_LEVEL_CHARACTER_LIMIT 11

#define LOG_CHARACTER_LIMIT 3200

enum LogLevel {
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_PRINT,
    LOG_LEVEL_COUNT
};

static Boolean logging_is_initialized = FALSE;

Boolean __logger_init(const char* file, u32 line, const char* func);
#define _logger_init(func, line, file) __logger_init(file, line, func)
#define logger_init() assert(_logger_init(__FILE__, __LINE__, __func__) == TRUE, "The logger system is already initalized"); LOG_INFO("The logger has been successfully initalized")

void __logger_shutdown(const char* func, u32 line, const char* file);
#define _logger_shutdown(func, line, file) __logger_shutdown(file, line, func)
#define logger_shutdown() _logger_shutdown(__FILE__, __LINE__, __func__)

void _log_output(LogLevel log_level, const char* message, const char* func, u32 line, const char* file ...);
#define log_output(log_level, message, func, line, file, ...) _log_output(log_level, message, func, line, file, ##__VA_ARGS__)

#if (LOG_PRINT_ENABLED)
    #define LOG_PRINT(message, ...) log_output(LOG_LEVEL_PRINT, message, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
    #define LOG_PRINT(message, ...)
#endif

#if (LOG_INFO_ENABLED)
    #define LOG_INFO(message, ...) log_output(LOG_LEVEL_INFO, message, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
    #define LOG_INFO(message, ...)
#endif

#if (LOG_DEBUG_ENABLED)
    #define LOG_DEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
    #define LOG_DEBUG(message, ...)
#endif

#if (LOG_WARN_ENABLED)
    #define LOG_WARN(message, ...) log_output(LOG_LEVEL_WARN, message, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
    #define LOG_WARN(message, ...)
#endif

/**
 * @brief Fix this the function and line should carry from here but it should also be pushed into the assert that way you can change it if you are 
 * in a function
 * 
 */
#define LOG_ERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_FATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, __FILE__, __LINE__, __func__, ##__VA_ARGS__)