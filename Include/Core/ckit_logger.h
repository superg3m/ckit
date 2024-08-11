#pragma once
/*===========================================================
 * File: ckit_logger.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "ckit_types.h"
#include "../../ckg/Include/ckg_logger.h"
//========================== Begin Types ==========================
#define LOGGING_ENABLED TRUE
#define LOG_LEVEL_CHARACTER_LIMIT 11


typedef enum CKIT_LogLevel {
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_SUCCESS,
    LOG_LEVEL_PRINT,
    LOG_LEVEL_COUNT
} LogLevel;

//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    Boolean logger_init();
    // void logger_shutdown();
    void MACRO_ckit_log_output(CKG_LogLevel log_level, const char* message, ...);
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

#if defined(CKIT_IMPL)
    #include "ckit_memory.h"
    #include "ckit_platform_services.h"
    #include "ckit_assert.h"
    #include "ckit_string.h"

    static Boolean logging_is_initialized = FALSE;
    Boolean logger_init() {
        if (!LOGGING_ENABLED) {
            // The logging system is disabled!
            return 1;
        }

        if (logging_is_initialized == FALSE) {
            logging_is_initialized = TRUE;
            platform_console_init();
            return TRUE;
        } else {
            LOG_FATAL("The logger system is already initalized!\n");
            return FALSE;
        }
    }

    #if (_WIN32)
        #include <windows.h>
    #endif

    void MACRO_ckit_log_output(CKG_LogLevel log_level, const char* message, ...) {
        char log_level_strings[CKG_LOG_LEVEL_COUNT][CKG_LOG_LEVEL_CHARACTER_LIMIT] = {
            "[FATAL]  : ",
            "[ERROR]  : ",
            "[WARN]   : ",
            "[DEBUG]  : ",
            "[SUCCESS]: ",
            "",
        };

        char* log_level_format[CKG_LOG_LEVEL_COUNT] = {
            CKG_RED_BACKGROUND,
            CKG_RED,
            CKG_PURPLE,
            CKG_BLUE,
            CKG_GREEN,
            CKG_COLOR_RESET,
        };

        Boolean is_fatal = (log_level == 0);

        char out_message[CKG_PLATFORM_CHARACTER_LIMIT];
        ckit_memory_zero(out_message, sizeof(out_message));
        
        va_list args_list;
        va_start(args_list, message);
        vsnprintf(out_message, CKG_PLATFORM_CHARACTER_LIMIT, message, args_list);
        va_end(args_list);

        u32 out_message_length = ckit_cstr_length(out_message);

        #if (_WIN32)
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD dwMode = 0;
            GetConsoleMode(hOut, &dwMode);
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        #endif


        const char* start_delimitor = "${";
        const char* end_delimitor = "}";
        String middle_to_color = ckit_str_between_delimiters(out_message, start_delimitor, end_delimitor);
        if (middle_to_color != NULLPTR) {
            u32 start_delimitor_index = ckit_str_index_of(out_message, start_delimitor);
            u32 end_delimitor_index = ckit_str_index_of(out_message, end_delimitor);

            String left_side = ckit_substring(out_message, 0, start_delimitor_index - 1);
            String right_side = ckit_substring(out_message, end_delimitor_index + ckit_cstr_length(end_delimitor), ckit_cstr_length(out_message) - 1);

            if (middle_to_color != NULLPTR && out_message[out_message_length - 1] == '\n') {
                printf("%s%s%s%s%s%.*s%s%s\n", log_level_format[log_level], log_level_strings[log_level], CKG_COLOR_RESET, left_side, log_level_format[log_level], ckit_str_length(middle_to_color) - 1, middle_to_color, CKG_COLOR_RESET, right_side);
            } else if(middle_to_color != NULLPTR && out_message[out_message_length - 1] != '\n') {
                printf("%s%s%s%s%s%.*s%s%s", log_level_format[log_level], log_level_strings[log_level], CKG_COLOR_RESET, left_side, log_level_format[log_level], ckit_str_length(middle_to_color) - 1, middle_to_color, CKG_COLOR_RESET, right_side);
            }

            ckit_str_free(left_side);
            ckit_str_free(middle_to_color);
            ckit_str_free(right_side);

            return;
        }

        if (out_message[out_message_length - 1] == '\n') {
            printf("%s%s%.*s%s\n", log_level_format[log_level], log_level_strings[log_level], out_message_length - 1, out_message, CKG_COLOR_RESET);
        } else {
            printf("%s%s%.*s%s", log_level_format[log_level], log_level_strings[log_level], out_message_length, out_message, CKG_COLOR_RESET);
        }
                    
    }
#endif