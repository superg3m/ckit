/*===========================================================
 * File: ckit_string.cpp
 * Date: May 09, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../../include/core/ckit_string.h"
#include "../../include/core/ckit_memory.h"
#include "../../include/core/ckit_assert.h"
#include "../../include/core/ckit_arena.h"

typedef struct StringHeader {
    u32 length;
    u32 capacity;
} StringHeader;

internal void _string_insert_header(String* string, StringHeader header) {
  	memory_copy(&header, *string,  sizeof(header),  sizeof(header) + header.capacity);
    memory_byte_advance(*string, sizeof(header));
}

internal StringHeader* _string_extract_header(String string) {
    return &((StringHeader*)string)[-1];
}
/*
internal inline String _string_grow(String string, u32 new_allocation_size) {
    StringHeader header = *_string_extract_header(string);
    header.capacity = new_allocation_size;
    String ret = string_create_custom(string, header.capacity);
    
    return ret;
}

String string_create_custom(const char* c_string, u32 capacity) {
  	u32 c_str_length = cstring_length(c_string);
  	StringHeader* header = arena_push(string_arena, StringHeader, MEMORY_TAG_STRING);
  	header->length = c_str_length;
  	header->capacity = capacity != 0 ? capacity : sizeof(char) * (c_str_length + 1);
	
  	String ret = arena_push_array(string_arena, u8, header->capacity, MEMORY_TAG_STRING);
	
  	memory_copy(c_string, ret, c_str_length, c_str_length);
  	return ret;
}

String MACRO_string_free(String string) {
    string = NULL;
    return string;
}
*/

internal inline String _string_grow(String string, u32 new_allocation_size) {
    StringHeader header = *_string_extract_header(string);
    header.capacity = new_allocation_size;
    String ret = string_create_custom(string, header.capacity);

    string_free(string);
    
    return ret;
}

// Date: May 23, 2024
// TODO(Jovanni): I need to eventually go back to using an arena I liked the idea
// Of pushing the header then right after pushing the string data I think this worked really well.
String string_create_custom(const char* c_string, u32 capacity) {
  	u32 c_str_length = cstring_length(c_string);
  	StringHeader header;
  	header.length = c_str_length;
  	header.capacity = capacity != 0 ? capacity : sizeof(char) * (c_str_length + 1);
	
  	String ret = memory_allocate(sizeof(header) + header.capacity, MEMORY_TAG_STRING);
    _string_insert_header(&ret, header);
	
  	ckg_string_copy(ret, header.capacity, c_string);
  	return ret;
}

void string_copy(); // Careful about the header
char* string_concat();
u32 string_length(String string) {
    StringHeader* header = _string_extract_header(string);
    return header->length;
}

String MACRO_string_free(String string) {
    memory_byte_retreat(string, sizeof(StringHeader));
    memory_free(string);
    return string;
}

String MACRO_string_append(String string, const char* source) {
    assert_in_function(string, "string_append: String passed is null\n");
    assert_in_function(source, "string_append: Source passed is null\n");

    u32 source_size = cstring_length(source) + 1; 

    StringHeader* header = _string_extract_header(string);
    if (header->length + source_size >= header->capacity) {
        string = _string_grow(string, (header->length + source_size) * 2);
        header = _string_extract_header(string);
    }

	header->length += cstring_length(source);
    ckg_string_append(string, header->capacity, source);
    
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