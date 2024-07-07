#include "./ckit_string.h"

#include "../Assert/ckit_assert.h"
#include "../Memory/ckit_memory.h"

#include "../../ckg/core/String/ckg_cstring.h"
typedef struct StringHeader {
    u32 length;
    u32 capacity;
} StringHeader;

internal void ckit_str_insert_header(char** str, StringHeader header) {
  	ckg_memory_copy(&header, *str,  sizeof(header),  sizeof(header) + header.capacity);
    *str = ((u8*)*str) + sizeof(header);
}

internal StringHeader* ckit_str_extract_header(String str) {
    return &((StringHeader*)str)[-1];
}
/*
internal inline String _ckit_str_grow(String str, u32 new_allocation_size) {
    StringHeader header = *_ckit_str_extract_header(str);
    header.capacity = new_allocation_size;
    String ret = ckit_str_create_custom(str, header.capacity);
    
    return ret;
}

String ckit_str_create_custom(const char* c_string, u32 capacity) {
  	u32 c_str_length = ckg_cstr_length(c_string);
  	StringHeader* header = arena_push(ckit_str_arena, StringHeader, MEMORY_TAG_STRING);
  	header->length = c_str_length;
  	header->capacity = capacity != 0 ? capacity : sizeof(char) * (c_str_length + 1);
	
  	String ret = arena_push_array(ckit_str_arena, u8, header->capacity, MEMORY_TAG_STRING);
	
  	ckg_memory_copy(c_string, ret, c_str_length, c_str_length);
  	return ret;
}

String MACRO_ckit_str_free(String str) {
    str = NULL;
    return str;
}
*/

String MACRO_ckit_str_free(String str) {
    str = str - sizeof(StringHeader);
    memory_free(str);
    return str;
}

internal inline String ckit_str_grow(String str, u32 new_allocation_size) {
    StringHeader header = *ckit_str_extract_header(str);
    header.capacity = new_allocation_size;
    String ret = ckit_str_create_custom(str, header.capacity);

    str = MACRO_ckit_str_free(str);
    
    return ret;
}

// Date: May 23, 2024
// TODO(Jovanni): I need to eventually go back to using an arena I liked the idea
// Of pushing the header then right after pushing the str data I think this worked really well.
String ckit_str_create_custom(const char* c_string, u32 capacity) {
  	u32 c_str_length = ckg_cstr_length(c_string);
  	StringHeader header;
  	header.length = c_str_length;
  	header.capacity = capacity != 0 ? capacity : sizeof(char) * (c_str_length + 1);
	
  	String ret = memory_allocate(sizeof(header) + header.capacity, MEMORY_TAG_STRING);
    ckit_str_insert_header(&ret, header);
	
  	ckg_cstr_copy(ret, header.capacity, c_string);
  	return ret;
}

void ckit_str_copy(); // Careful about the header
char* ckit_str_concat();
u32 ckit_str_length(const String str) {
    StringHeader* header = ckit_str_extract_header(str);
    return header->length;
}

String MACRO_ckit_str_append(String str, const char* source) {
    ckit_assert_msg(str, "ckit_str_append: String passed is null\n");
    ckit_assert_msg(source, "ckit_str_append: Source passed is null\n");

    u32 source_size = ckg_cstr_length(source) + 1; 

    StringHeader* header = ckit_str_extract_header(str);
    if (header->length + source_size >= header->capacity) {
        str = ckit_str_grow(str, (header->length + source_size) * 2);
        header = ckit_str_extract_header(str);
    }

	header->length += ckg_cstr_length(source);
    ckg_cstr_append(str, header->capacity, source);
    
    return str;
}

String MACRO_ckit_str_append_char(String str, const char source) {
    ckit_assert_msg(str, "ckit_str_append_char: String passed is null\n");
    ckit_assert_msg(source, "ckit_str_append_char: Source passed is null\n");

    u32 source_size = 1;
    StringHeader* header = ckit_str_extract_header(str);
    if (header->length + source_size >= header->capacity) {
        str = ckit_str_grow(str, (header->length + source_size) * 2);
        header = ckit_str_extract_header(str);
    }

    str[header->length] = source;
	header->length++;

    return str;
}

