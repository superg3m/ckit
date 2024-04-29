#include "../include/deprecated/core_string.h"
#include "../include/core_memory.h"

struct StringHeader {
    u32 string_length;
};

u32 c_string_length(const char* c_string) {
    u32 length = 0;
    char* cursor = (char*)c_string;
    while(*cursor++ != '\0') {
        length++;
    }
    return length;
}

static inline void _string_insert_header(StringHeader header, String* string) {
    memory_copy(sizeof(header), &header, sizeof(header), *string);
    *string += sizeof(header);
}

static inline void _string_update_header(StringHeader newHeader, String* string) {
    memory_copy(sizeof(newHeader), &newHeader, sizeof(newHeader), (*string) - sizeof(newHeader));
}

String string_create(const char* c_string) {
    StringHeader header;
    u32 c_str_length = c_string_length(c_string);
    header.string_length = c_str_length;

    size_t string_allocation_size = sizeof(header) + (sizeof(char) * (c_str_length + 1));
    String ret = (String)memory_allocate(string_allocation_size, MEMORY_TAG_STRING);

    memory_zero(string_allocation_size,  ret);
    _string_insert_header(header, &ret);

    memory_copy(c_str_length, c_string, c_str_length, ret);
    return ret;
}

static inline StringHeader _string_extract_header(String string) {
    StringHeader header;
    memory_copy(sizeof(header), (string - sizeof(header)), sizeof(header), &header);
    return header;
}

/**
 * @brief Get the formated string buffer
 * Must free string buffer with respective string
 * 
 * @param message 
 * @param ... 
 * @return char* 
 */
char* formated_string_buffer(const char* message, ...) {
    //char out_message[LOG_CHARACTER_LIMIT];
    //memory_zero(sizeof(out_message), out_message);
    //
    //va_list args_list;
    //va_start(args_list, message);
    //vsnprintf(out_message, LOG_CHARACTER_LIMIT, message, args_list);
    //va_end(args_list);
    //return out_message;
    return NULL;
}

void string_copy(); // Careful about the header
void string_concat();
u64 string_length(String string) {
    StringHeader header = _string_extract_header(string);
    return header.string_length;
}

Boolean string_compare();
void string_destroy(String* string) {
    StringHeader header = _string_extract_header(*string);
    size_t string_allocation_size = sizeof(header) + (sizeof(u8) * (header.string_length + 1));
    *string -= sizeof(header);
    memory_free(string_allocation_size, string, MEMORY_TAG_STRING);
    string = NULL;
}