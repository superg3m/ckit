#include "../include/core_logger.h"
#include "../include/core_memory.h"
#include "../include/platform_services.h"
#include "../include/core_assert.h"

Boolean __logger_init(const char* func, u32 line, const char* file) {
    if (!LOGGING_ENABLED) {
        LOG_ERROR("The logging system is disabled!");
        return 1;
    }

    if (logging_is_initialized == FALSE) {
        logging_is_initialized = TRUE;
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @brief Includes a newline. This has a imposed limit of (PLATFORM_COMMON_CHARACTER_LIMIT = 3200)
 * Also another note this might want to allow the user of the log call to specify if its a new line or not instead of forcing it upon them.
 * @param log_level 
 * @param message 
 * @param ... 
 */
void _log_output(LogLevel log_level,  const char* message,  const char* func, u32 line,  const char* file ...) {
    char log_level_strings[LOG_LEVEL_COUNT][LOG_LEVEL_CHARACTER_LIMIT] = {
        "[FATAL]: ",
        "[ERROR]: ",
        "[WARN] : ",
        "[DEBUG]: ",
        "[INFO] : ",
        "",
    };

    u8 log_level_format[LOG_LEVEL_COUNT] = {
        BACK_RED,
        TEXT_RED,
        TEXT_PURPLE,
        TEXT_BLUE,
        TEXT_GREEN,
        TEXT_WHITE,
    };

    Boolean is_fatal = log_level == 0;

    char out_message[PLATFORM_COMMON_CHARACTER_LIMIT];
    memory_zero(sizeof(out_message), out_message);

    va_list args_list;
    va_start(args_list, file);
    vsnprintf(out_message, PLATFORM_COMMON_CHARACTER_LIMIT, message, args_list);
    va_end(args_list);

    char out_message2[PLATFORM_COMMON_CHARACTER_LIMIT]; 
    memory_zero(sizeof(out_message2), out_message2);  
    sprintf(out_message2, "%s%s", log_level_strings[log_level], out_message);
    _platform_console_write(sizeof(char) * PLATFORM_COMMON_CHARACTER_LIMIT, out_message2, log_level_format[log_level]);
}