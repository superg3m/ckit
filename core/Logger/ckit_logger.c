#include "./ckit_logger.h"
#include "../Memory/ckit_memory.h"
#include "../Platform/ckit_platform_services.h"
#include "../Assert/ckit_assert.h"

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

/**
 * @brief Includes a newline. This has a imposed limit of (PLATFORM_CHARACTER_LIMIT = 3200)
 * Also another note this might want to allow the user of the log call to specify if its a new line or not instead of forcing it upon them.
 * @param log_level 
 * @param message 
 * @param ... 
 */
/*
void MACRO_log_output(LogLevel log_level, const char* message, ...) {
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

    Boolean is_fatal = (log_level == 0);

    char out_message[PLATFORM_CHARACTER_LIMIT];
    ckg_memory_zero(out_message, sizeof(out_message));

    char out_message2[PLATFORM_CHARACTER_LIMIT];
    ckg_memory_zero(out_message2, sizeof(out_message2));  
    
    va_list args_list;
    va_start(args_list, message);
    vsnprintf(out_message, PLATFORM_CHARACTER_LIMIT, message, args_list);
    va_end(args_list);

    sprintf(out_message2, "%s%s", log_level_strings[log_level], out_message);
    platform_console_write(out_message2, log_level_format[log_level]);
}
*/