#pragma once

#include "ckg_types.h"
//========================== Begin Types ==========================
//Regular text
#define CKG_BLACK "\033[0;30m"
#define CKG_RED "\033[0;31m"
#define CKG_GREEN "\033[0;32m"
#define CKG_YELLOW "\033[0;33m"
#define CKG_BLUE "\033[0;34m"
#define CKG_PURPLE "\033[0;35m"
#define CKG_CYAN "\033[0;36m"

//Regular background
#define CKG_BLACK_BACKGROUND "\033[40m"
#define CKG_RED_BACKGROUND "\033[41m"
#define CKG_GREENN_BACKGROUND "\033[42m"
#define CKG_YELLOW_BACKGROUND "\033[43m"
#define CKG_BLUE_BACKGROUND "\033[44m"
#define CKG_PURPLE_BACKGROUND "\033[45m"
#define CKG_CYAN_BACKGROUND "\033[46m"

#define CKG_COLOR_RESET "\033[0m"

#define CKG_LOG_LEVEL_CHARACTER_LIMIT 15
#define CKG_PLATFORM_CHARACTER_LIMIT 512

typedef enum CKG_LogLevel {
    CKG_LOG_LEVEL_FATAL,
    CKG_LOG_LEVEL_ERROR,
    CKG_LOG_LEVEL_WARN,
    CKG_LOG_LEVEL_DEBUG,
    CKG_LOG_LEVEL_SUCCESS,
    CKG_LOG_LEVEL_PRINT,
    CKG_LOG_LEVEL_COUNT
} CKG_LogLevel;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    void MACRO_ckg_log_output(CKG_LogLevel log_level, const char* message, ...);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckg_log_output(log_level, message, ...) MACRO_ckg_log_output(log_level, message, ##__VA_ARGS__)
#define CKG_LOG_PRINT(message, ...) ckg_log_output(CKG_LOG_LEVEL_PRINT, message, ##__VA_ARGS__)
#define CKG_LOG_SUCCESS(message, ...) ckg_log_output(CKG_LOG_LEVEL_SUCCESS, message, ##__VA_ARGS__)
#define CKG_LOG_DEBUG(message, ...) ckg_log_output(CKG_LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#define CKG_LOG_WARN(message, ...) ckg_log_output(CKG_LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#define CKG_LOG_ERROR(message, ...) ckg_log_output(CKG_LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#define CKG_LOG_FATAL(message, ...) ckg_log_output(CKG_LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++







