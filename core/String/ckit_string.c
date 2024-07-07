#include "./ckit_string.h"

#include "../Assert/ckit_assert.h"
#include "../Memory/ckit_memory.h"

typedef struct StringHeader {
    u32 length;
    u32 capacity;
} StringHeader;

internal void ckit_str_insert_header(char** string, StringHeader header) {
  	ckg_memory_copy(&header, *string,  sizeof(header),  sizeof(header) + header.capacity);
    *string = ((u8*)*string) + sizeof(header);
}

internal StringHeader* ckit_str_extract_header(String string) {
    return &((StringHeader*)string)[-1];
}
/*
internal inline String _ckit_str_grow(String string, u32 new_allocation_size) {
    StringHeader header = *_ckit_str_extract_header(string);
    header.capacity = new_allocation_size;
    String ret = ckit_str_create_custom(string, header.capacity);
    
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

String MACRO_ckit_str_free(String string) {
    string = NULL;
    return string;
}
*/

String MACRO_ckit_str_free(String string) {
    string = string - sizeof(StringHeader);
    memory_free(string);
    return string;
}

internal inline String ckit_str_grow(String string, u32 new_allocation_size) {
    StringHeader header = *ckit_str_extract_header(string);
    header.capacity = new_allocation_size;
    String ret = ckit_str_create_custom(string, header.capacity);

    string = MACRO_ckit_str_free(string);
    
    return ret;
}

// Date: May 23, 2024
// TODO(Jovanni): I need to eventually go back to using an arena I liked the idea
// Of pushing the header then right after pushing the string data I think this worked really well.
String ckit_str_create_custom(const char* c_string, u32 capacity) {
  	u32 c_str_length = ckg_cstr_length(c_string);
  	StringHeader header;
  	header.length = c_str_length;
  	header.capacity = capacity != 0 ? capacity : sizeof(char) * (c_str_length + 1);
	
  	String ret = memory_allocate(sizeof(header) + header.capacity, MEMORY_TAG_STRING);
    _ckit_str_insert_header(&ret, header);
	
  	ckg_str_copy(ret, header.capacity, c_string);
  	return ret;
}

void ckit_str_copy(); // Careful about the header
char* ckit_str_concat();
u32 ckit_str_length(String string) {
    StringHeader* header = ckit_str_extract_header(string);
    return header->length;
}

String MACRO_ckit_str_append(String string, const char* source) {
    ckit_assert_msg(string, "ckit_str_append: String passed is null\n");
    ckit_assert_msg(source, "ckit_str_append: Source passed is null\n");

    u32 source_size = ckg_cstr_length(source) + 1; 

    StringHeader* header = ckit_str_extract_header(string);
    if (header->length + source_size >= header->capacity) {
        string = ckit_str_grow(string, (header->length + source_size) * 2);
        header = ckit_str_extract_header(string);
    }

	header->length += ckg_cstr_length(source);
    ckg_str_append(string, header->capacity, source);
    
    return string;
}

String MACRO_ckit_str_append_char(String string, const char source) {
    ckit_assert_msg(string, "ckit_str_append_char: String passed is null\n");
    ckit_assert_msg(source, "ckit_str_append_char: Source passed is null\n");

    u32 source_size = 1;
    StringHeader* header = ckit_str_extract_header(string);
    if (header->length + source_size >= header->capacity) {
        string = ckit_str_grow(string, (header->length + source_size) * 2);
        header = ckit_str_extract_header(string);
    }

    string[header->length] = source;
	header->length++;

    return string;
}

