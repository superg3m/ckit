/*===========================================================
 * File: core_string.cpp
 * Date: May 09, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../include/core_string.h"
#include "../include/core_memory.h"
#include "../include/core_assert.h"

typedef struct StringHeader {
    u32 length;
    u32 capacity;
} StringHeader;

u32 c_string_length(const char* c_string) {
    u32 length = 0;
    char* cursor = (char*)c_string;
    while(*cursor++ != '\0') {
        length++;
    }
    return length;
}

#pragma region Private Functions
internal inline void _string_insert_header(StringHeader header, String* string) {
    memory_copy(sizeof(header), &header, sizeof(header), *string);
    *string += sizeof(header);
}

internal inline void _string_update_header(StringHeader newHeader, String* string) {
    memory_copy(sizeof(newHeader), &newHeader, sizeof(newHeader), (*string) - sizeof(newHeader));
}

internal inline StringHeader _string_extract_header(String string) {
    StringHeader header;
    memory_copy(sizeof(header), (string - sizeof(header)), sizeof(header), &header);
    return header;
}

internal inline String _string_grow(u32 new_allocation_size, String* string) {
    StringHeader header = _string_extract_header(*string);
    header.capacity = new_allocation_size;
    memory_byte_retreat(sizeof(header), (void**)string);
    String ret = (String)memory_reallocate(sizeof(header) + new_allocation_size, (void**)string);
    _string_insert_header(header, &ret);
    
    return ret;
}


#pragma endregion

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

void string_copy(); // Careful about the header
void string_concat();
u32 string_length(String string) {
    StringHeader header = _string_extract_header(string);
    return header.length;
}

/**
 * @brief Returns TRUE(1) if strings are equal and returns FALSE(0) if they are not
 * 
 * @param s1 
 * @param s2 
 * @return Boolean 
 */
Boolean string_compare(const char* s1, const char* s2) {
    assert_in_function(s1, "string_compare first argument is not valid | null");
    assert_in_function(s2, "string_compare second argument is not valid | null");

	u32 s1_length = c_string_length(s1);
	u32 s2_length = c_string_length(s2);

	return memory_byte_compare(s1_length, s1, s2_length, s2);
}

void string_free(String* string) {
    memory_byte_retreat(sizeof(StringHeader), (void**)string);
    memory_free((void**)string);
    string = NULL;
}

void string_append_char(String* string, const char source) {
    u32 source_size = 1;

    StringHeader header = _string_extract_header(*string);
    if (header.length + source_size >= header.capacity) {
        *string = _string_grow((header.length + source_size) * 2, string);
        header = _string_extract_header(*string);
    }

    (*string)[header.length] = source;
	header.length++;
    _string_update_header(header, string);
}

void string_append(String* string, const char* source) {
    u32 source_size = c_string_length(source); 

    StringHeader header = _string_extract_header(*string);
    if (header.length + source_size >= header.capacity) {
        *string = _string_grow((header.length + source_size) * 2, string);
        header = _string_extract_header(*string);
    }

    u8* dest_ptr = memory_advance_new_ptr(header.length, *string);
	header.length++;
    memory_copy(source_size, source, source_size, dest_ptr);
    
    _string_update_header(header, string);
}