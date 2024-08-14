#pragma once
/*===========================================================
 * File: ckit_logger.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "./ckit_types.h"
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
} CKIT_LogLevel;

//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    Boolean logger_init();
    // void logger_shutdown();
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

#if defined(CKIT_IMPL)
    #include "./ckit_memory.h"
    #include "./ckit_platform_services.h"
    #include "./ckit_assert.h"
    #include "./ckit_string.h"

    internal Boolean logging_is_initialized = FALSE;
    internal const char* start_delimitor = "${";
    internal const char* end_delimitor = "}";

    internal char log_level_strings[CKG_LOG_LEVEL_COUNT][CKG_LOG_LEVEL_CHARACTER_LIMIT] = {
        "[FATAL]  : ",
        "[ERROR]  : ",
        "[WARN]   : ",
        "[DEBUG]  : ",
        "[SUCCESS]: ",
        "",
    };

    internal char* log_level_format[CKG_LOG_LEVEL_COUNT] = {
        CKG_RED_BACKGROUND,
        CKG_RED,
        CKG_PURPLE,
        CKG_BLUE,
        CKG_GREEN,
        CKG_COLOR_RESET,
    };

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

    #if defined(PLATFORM_WINDOWS)
        #include <windows.h>
    #endif

    Boolean message_has_special_delmitor(const char* message) {
        s32 start_delimitor_index = ckit_str_index_of(message, start_delimitor);
        s32 end_delimitor_index = ckit_str_index_of(message, end_delimitor);
        return start_delimitor_index != -1 && end_delimitor_index != -1;
    }

    internal void special_print_helper(const char* message, CKIT_LogLevel log_level) {
        String middle_to_color = ckit_str_between_delimiters(message, start_delimitor, end_delimitor);
        if (!middle_to_color) {
            printf("%s", message);
            return;
        }

        u32 message_length = ckit_cstr_length(message);

        u32 start_delimitor_index = ckit_str_index_of(message, start_delimitor);
        u32 end_delimitor_index = ckit_str_index_of(message, end_delimitor);

        String left_side = ckit_substring(message, 0, start_delimitor_index - 1);
        String right_side = ckit_substring(message, end_delimitor_index + ckit_cstr_length(end_delimitor), ckit_cstr_length(message) - 1);

        printf("%s%s%s%s", left_side, log_level_format[log_level], middle_to_color, CKG_COLOR_RESET);

        special_print_helper(right_side, log_level);

        ckit_str_free(left_side);
        ckit_str_free(middle_to_color);
        ckit_str_free(right_side);

        return;
    }

    void MACRO_ckit_log_output(CKIT_LogLevel log_level, const char* message, ...) {


        Boolean is_fatal = (log_level == 0);

        char out_message[CKG_PLATFORM_CHARACTER_LIMIT];
        ckit_memory_zero(out_message, sizeof(out_message));
        
        va_list args_list;
        va_start(args_list, message);
        vsnprintf(out_message, CKG_PLATFORM_CHARACTER_LIMIT, message, args_list);
        va_end(args_list);



        #if (_WIN32)
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD dwMode = 0;
            GetConsoleMode(hOut, &dwMode);
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        #endif

        printf("%s%s%s", log_level_format[log_level], log_level_strings[log_level], CKG_COLOR_RESET);
        
        u32 out_message_length = ckit_cstr_length(out_message);

        if (message_has_special_delmitor(out_message)) {
            special_print_helper(out_message, log_level);
        } else {
            printf("%s%.*s%s", log_level_format[log_level], out_message_length - 1, out_message, CKG_COLOR_RESET);
        }

        if (out_message[out_message_length - 1] == '\n') {
            printf("\n");
        }           
    }
#endif