/*===========================================================
 * File: core_string.cpp
 * Date: May 09, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../include/core_string.h"
#include "../include/core_memory.h"
#include "../include/core_assert.h"
#include "../include/core_arena.h"

typedef struct StringHeader {
    u32 length;
    u32 capacity;
    Arena* arena;
} StringHeader;

u32 c_string_length(const char* c_string) {
    u32 length = 0;
    char* cursor = (char*)c_string;
    while(*cursor++ != '\0') {
        length++;
    }
    return length;
}

internal StringHeader* _string_extract_header(String string) {
    return &((StringHeader*)string)[-1];
}

internal inline String _string_grow(String string, u32 new_allocation_size) {
    StringHeader header = *_string_extract_header(string);
    header.capacity = new_allocation_size;
    String ret = string_create_custom(header.arena, string, header.capacity);
    
    return ret;
}

String string_create_custom(Arena* arena, const char* c_string, u32 capacity) {
  u32 c_str_length = c_string_length(c_string);
  StringHeader* header = arena_push(arena, StringHeader, MEMORY_TAG_STRING);
  header->length = c_str_length;
  header->arena = arena;
  header->capacity = capacity != 0 ? capacity : sizeof(char) * (c_str_length + 1);

  String ret = arena_push_array(arena, u8, header->capacity, MEMORY_TAG_STRING);

  memory_copy(c_string, ret, c_str_length, c_str_length);
  return ret;
}

void string_copy(); // Careful about the header
void string_concat();
u32 string_length(String string) {
    StringHeader* header = _string_extract_header(string);
    return header->length;
}

/**
 * @brief Returns TRUE(1) if strings are equal and returns FALSE(0) if they are not
 * 
 * @param s1 
 * @param s2 
 * @return Boolean 
 */
Boolean string_compare(const char* s1, const char* s2) {
    assert_in_function(s1, "string_compare first argument is not valid | null\n");
    assert_in_function(s2, "string_compare second argument is not valid | null\n");

	u32 s1_length = c_string_length(s1);
	u32 s2_length = c_string_length(s2);

	return memory_byte_compare(s1, s2, s1_length, s2_length);
}

String MACRO_string_free(String string) {
    string = NULL;
    return string;
}

String MACRO_string_append(String string, const char* source) {
    assert_in_function(string, "string_append: String passed is null\n");
    assert_in_function(source, "string_append: Source passed is null\n");

    u32 source_size = c_string_length(source) + 1; 

    StringHeader* header = _string_extract_header(string);
    if (header->length + source_size >= header->capacity) {
        string = _string_grow(string, (header->length + source_size) * 2);
        header = _string_extract_header(string);
    }

    u8* dest_ptr = memory_advance_new_ptr(string, header->length);
	header->length++;
    memory_copy(source, dest_ptr, source_size, source_size);
    
    return string;
}

String MACRO_string_append_char(String string, const char source) {
    assert_in_function(string, "string_append_char: String passed is null\n");
    assert_in_function(source, "string_append_char: Source passed is null\n");

    u32 source_size = 1;
    StringHeader* header = _string_extract_header(string);
    if (header->length + source_size >= header->capacity) {
        string = _string_grow(string, (header->length + source_size) * 2);
        header = _string_extract_header(string);
    }

    string[header->length] = source;
	header->length++;

    return string;
}