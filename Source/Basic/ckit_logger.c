#pragma once

#include "../../Include/Basic/ckit_logger.h"

#include "../../Include/Basic/ckit_memory.h"
#include "../../Include/Basic/ckit_platform_services.h"
#include "../../Include/Basic/ckit_assert.h"
#include "../../Include/Basic/ckit_string.h"

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
        u32 message_length = ckit_cstr_length(message);
        Boolean found = message[message_length - 1] == '\n';
        printf("%.*s", message_length - found, message);
        return;
    }

    u32 message_length = ckit_cstr_length(message);

    u32 start_delimitor_index = ckit_str_index_of(message, start_delimitor);
    u32 end_delimitor_index = ckit_str_index_of(message, end_delimitor);

    String left_side = ckit_substring(message, 0, start_delimitor_index - 1);
    String right_side = ckit_substring(message, end_delimitor_index + ckit_cstr_length(end_delimitor), ckit_cstr_length(message) - 1);

    printf("%s%s%s%s", left_side, log_level_format[log_level], middle_to_color, CKG_COLOR_RESET);

    special_print_helper(right_side, log_level);

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

    printf("%s%s%s", log_level_format[log_level], log_level_strings[log_level], CKG_COLOR_RESET);
    
    u32 out_message_length = ckit_cstr_length(out_message);

    if (message_has_special_delmitor(out_message)) {
        special_print_helper(out_message, log_level);
    } else {
        Boolean found = out_message[out_message_length - 1] == '\n';
        printf("%s%.*s%s", log_level_format[log_level], out_message_length - found, out_message, CKG_COLOR_RESET);
    } 

    if (out_message[out_message_length - 1] == '\n') {
        printf("\n");
    }           
}