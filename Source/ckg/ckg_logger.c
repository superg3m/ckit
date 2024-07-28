#include "ckg_logger.h"

#include "Memory/ckg_memory.h"
#include "String/ckg_cstring.h"
#if (_WIN32)
    #include <windows.h>
#endif

void MACRO_ckg_log_output(CKG_LogLevel log_level, const char* message, ...) {
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
    ckg_memory_zero(out_message, sizeof(out_message));

    char out_message2[CKG_PLATFORM_CHARACTER_LIMIT];
    ckg_memory_zero(out_message2, sizeof(out_message2));  
    
    va_list args_list;
    va_start(args_list, message);
    vsnprintf(out_message, CKG_PLATFORM_CHARACTER_LIMIT, message, args_list);
    va_end(args_list);

    sprintf(out_message2, "%s%s", log_level_strings[log_level], out_message);

    int out_message2_length = ckg_cstr_length(out_message2);

    #if (_WIN32)
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    #endif

    if (out_message2[out_message2_length - 1] == '\n') {
        printf("%s%.*s%s\n", log_level_format[log_level], out_message2_length - 1, out_message2, CKG_COLOR_RESET);
    } else {
        printf("%s%.*s%s", log_level_format[log_level], out_message2_length, out_message2, CKG_COLOR_RESET);
    }
}