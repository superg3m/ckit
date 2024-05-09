#include "../include/core_string.h"
#include "../include/core_memory.h"

struct StringHeader {
    u32 length;
    u32 capacity;
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

static inline StringHeader _string_extract_header(String string) {
    StringHeader header;
    memory_copy(sizeof(header), (string - sizeof(header)), sizeof(header), &header);
    return header;
}

String string_create(const char* c_string) {
    StringHeader header;
    u32 c_str_length = c_string_length(c_string);
    header.length = c_str_length;

    size_t string_allocation_size = sizeof(header) + (sizeof(char) * (c_str_length + 1));
    String ret = (String)memory_allocate(string_allocation_size, MEMORY_TAG_STRING);

    memory_zero(string_allocation_size,  ret);
    _string_insert_header(header, &ret);

    memory_copy(c_str_length, c_string, c_str_length, ret);
    return ret;
}

static inline void _string_grow(String* string) {
    StringHeader header = _string_extract_header(*string);
	header.capacity *= 2;
	memory_reallocate(sizeof(char) * header.capacity, (void**)string);
}

void string_copy(); // Careful about the header
void string_concat();
u64 string_length(String string) {
    StringHeader header = _string_extract_header(string);
    return header.length;
}

Boolean string_compare(const char* s1, const char* s2) {
	u32 s1_length = c_string_length(s1);
	u32 s2_length = c_string_length(s2);

	return memory_byte_compare(s1_length, s1, s2_length, s2);
}

Boolean string_compare(const String s1, const char* s2) {
	u32 s1_length = string_length(s1);
	u32 s2_length = c_string_length(s2);

	return memory_byte_compare(s1_length, s1, s2_length, s2);
}

Boolean string_compare(const String s1, const String s2) {
	u32 s1_length = string_length(s1);
	u32 s2_length = string_length(s2);

	return memory_byte_compare(s1_length, s1, s2_length, s2);
}

void string_free(String* string) {
    memory_byte_retreat(sizeof(StringHeader), (void**)string);
    memory_free((void**)string);
    string = NULL;
}