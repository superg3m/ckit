#include "../include/core_string.h"
#include "../include/core_memory.h"
#include "../include/core_assert.h"

u32 c_string_length(const char* c_string) {
    u32 length = 0;
    char* cursor = (char*)c_string;
    while(*cursor++ != '\0') {
        length++;
    }
    return length;
}

void _string_grow(String* string) {
	u32 old_allocation_size = string->capacity;
	string->capacity *= 2;
	u32 new_allocation_size = string->capacity;
	memory_free(old_allocation_size, (void**)(&string->data), MEMORY_TAG_STRING);
	string->data = (char*)memory_allocate(new_allocation_size, MEMORY_TAG_STRING);
}

Boolean _string_validate(const String* string) {
	return string != NULL;
}

String string_create(const char* c_string) {
	String str;
	memory_zero(sizeof(str), &str);
    u32 c_str_length = c_string_length(c_string);
    str.length = c_str_length;

    size_t string_data_allocation_size = (sizeof(char) * (c_str_length + 1));
    str.data = (char*)memory_allocate(string_data_allocation_size, MEMORY_TAG_STRING); // Zeros the memory to account for null term
    memory_copy(c_str_length, c_string, c_str_length, str.data);
    return str;
}


/**
 * @brief Make these all macros so it works with assert
 * 
 * @param string 
 * @param source 
 */
void string_copy(String* string, const char* source) {
	assert(_string_validate(string), "The string copy failed because the string is not valid it has been freed before this call!");
	u32 source_size = c_string_length(source);
	if (source_size > string->capacity) {
		_string_grow(string);
	}
	memory_copy(source_size, source, string->capacity, string->data);
}
void string_append(String* string, const char* source) {
	assert(_string_validate(string), "The string append failed because the string is not valid it has been freed before this call!");
}

Boolean string_compare(const char* s1, const char* s2) {
	return FALSE;
}
Boolean string_compare(const String* s1, const char* s2) {
	return FALSE;
}
Boolean string_compare(const String* s1, const String* s2) {
	return FALSE;
}

void string_free(String* string) {
	assert(_string_validate(string), "The string free failed because the string is not valid it has been freed before this call!");
    size_t string_allocation_size = (sizeof(u8) * (string->length + 1));
    memory_free(string_allocation_size, (void**)(&string->data), MEMORY_TAG_STRING);
    string->data = NULL;
}