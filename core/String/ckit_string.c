
#include <setjmp.h>
#include <signal.h>

#include "./ckit_string.h"
#include "../Assert/ckit_assert.h"
#include "../Memory/ckit_memory.h"
#include "../Collection/Vector/ckit_vector.h"
#include "../../ckg/core/String/ckg_cstring.h"
typedef struct CKIT_StringHeader {
    u32 length; 
    u32 capacity;
    char* magic; 
} CKIT_StringHeader;

#define ckit_str_header(string) ((CKIT_StringHeader*)(string - sizeof(CKIT_StringHeader)))
#define CKIT_STR_MAGIC "CKIT_MAGIC_STRING"

jmp_buf env;

void handle_signal(int sig) {
    longjmp(env, 1);
}

Boolean is_string_literal(const char *str) {
    signal(SIGSEGV, handle_signal);
    if (setjmp(env) == 0) {
        // Attempt to modify the string
        char *ptr = (char *)str;
        char old = ptr[0];
        ptr[0] = 'X';
        ptr[0] = old;  // Restore original value
        return FALSE;  // No crash, likely not a string literal
    } else {
        return TRUE;  // Crash, it's likely a string literal
    }
}

internal void ckit_str_check_magic(String str) {
    ckit_assert_msg(str, "String: %s is null can't check magic: (%s) Likely not a CKIT_String\n", str, CKIT_STR_MAGIC);
    if (is_string_literal(ckit_str_header(str)->magic)) {
        ckit_assert_msg(FALSE, "String: %s contains the wrong magic: (%s) Likely not a CKIT_String\n", str, CKIT_STR_MAGIC);
    }
    ckit_assert_msg(ckit_str_equal(ckit_str_header(str)->magic, CKIT_STR_MAGIC), "String: %s has the wrong magic: {%s} got: {%s} \n", str, CKIT_STR_MAGIC, ckit_str_header(str)->magic);
}

internal void ckit_str_insert_header(char** str, CKIT_StringHeader header) {
  	ckit_memory_copy(&header, *str,  sizeof(header),  sizeof(header) + header.capacity);
    *str = ((u8*)*str) + sizeof(header);
}

String MACRO_ckit_str_free(String str) {
    ckit_str_check_magic(str);
    str = str - sizeof(CKIT_StringHeader);
    ckit_free(str);
    return str;
}

internal inline String ckit_str_grow(String str, u32 new_allocation_size) {
    ckit_str_check_magic(str);
    CKIT_StringHeader header = *ckit_str_header(str);
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
  	CKIT_StringHeader header;
  	header.length = c_str_length;
  	header.capacity = capacity != 0 ? capacity : sizeof(char) * (c_str_length + 1);
    header.magic = CKIT_STR_MAGIC;
	
  	String ret = ckit_alloc_custom(sizeof(header) + header.capacity, TAG_CKIT_CORE_STRING);
    ckit_str_insert_header(&ret, header);
	
  	ckg_cstr_copy(ret, header.capacity, c_string);
  	return ret;
}

Boolean ckit_str_equal(const char* str1, const char* str2) {
    return ckg_cstr_equal(str1, str2);
}

void ckit_str_clear(char* str1) {
    ckg_cstr_clear(str1);
}

void ckit_str_recanonicalize_header_length(String str) {
    ckit_str_header(str)->length = ckit_cstr_length(str);
}

void ckit_str_copy(String str1, const char* source) {
    ckit_str_clear(str1);
    ckit_str_append(str1, source);
}

u32 ckit_cstr_length(const char* str) {
    return ckg_cstr_length(str);
}

u32 ckit_str_length(const String str) {
    ckit_str_check_magic(str);
    return ckit_str_header(str)->length;
}

String MACRO_ckit_str_append(String str, const char* source) {
    ckit_str_check_magic(str);
    ckit_assert_msg(str, "ckit_str_append: String passed is null\n");
    ckit_assert_msg(source, "ckit_str_append: Source passed is null\n");

    u32 source_capacity = ckit_cstr_length(source) + 1; 
    CKIT_StringHeader* header = ckit_str_header(str);
    if (header->length + source_capacity >= header->capacity) {
        str = ckit_str_grow(str, (header->length + source_capacity) * 2);
        header = ckit_str_header(str);
    }

    ckg_cstr_append(str, header->capacity, source);
    header->length += source_capacity - 1;
    return str;
}

String MACRO_ckit_str_append_char(String str, const char source) {
    ckit_str_check_magic(str);
    ckit_assert_msg(str, "ckit_str_append_char: String passed is null\n");
    ckit_assert_msg(source, "ckit_str_append_char: Source passed is null\n");

    u32 source_size = 1;
    CKIT_StringHeader* header = ckit_str_header(str);
    if (header->length + source_size >= header->capacity) {
        str = ckit_str_grow(str, (header->length + source_size) * 2);
        header = ckit_str_header(str);
    }

    str[header->length] = source;
    header->length++;

    return str;
}


String ckit_substring(const char* string_buffer, u32 start_range, u32 end_range) {
    String ret_string = ckit_str_create_custom("", (end_range - start_range) + 1);
    ckg_substring(string_buffer, ret_string, start_range, end_range);
    ckit_str_recanonicalize_header_length(ret_string);

    return ret_string;
}

internal String* ckit_str_split_helper(String* ret_buffer, const char* string_buffer, u32 offset_into_buffer, const char* delimitor) {
    s32 found_index = ckit_str_index_of(string_buffer + offset_into_buffer, delimitor);
    u32 offset_to_space = found_index + offset_into_buffer;
    if (found_index == -1) {
        ckit_vector_push(ret_buffer, ckit_substring(string_buffer, offset_into_buffer, ckit_cstr_length(string_buffer) - 1));
        return ret_buffer;
    }
    
    ckit_vector_push(ret_buffer, ckit_substring(string_buffer, offset_into_buffer, offset_to_space - 1));
    return ckit_str_split_helper(ret_buffer, string_buffer, offset_to_space + 1, delimitor);
}

String* ckit_str_split(const char* string_buffer, const char* delimitor) {
    ckit_assert(string_buffer);
    ckit_assert(delimitor);

    String* string_vector = NULLPTR;
    return ckit_str_split_helper(string_vector, string_buffer, 0, delimitor);
}

Boolean kit_cstr_contains(const char* string_buffer, const char* contains) {
    return ckg_cstr_contains(string_buffer, contains);
}

s32 ckit_str_index_of(const char* string_buffer, const char* sub_string) {
    return ckg_cstr_index_of(string_buffer, sub_string);
}

s32 ckit_str_last_index_of(const char* string_buffer, const char* sub_string) {
    return ckg_cstr_last_index_of(string_buffer, sub_string);
}

Boolean ckit_str_starts_with(const char* string_buffer, const char* starts_with) {
    return ckg_cstr_starts_with(string_buffer, starts_with);
}

Boolean ckit_str_ends_with(const char* string_buffer, const char* ends_with) {
    return ckg_cstr_ends_with(string_buffer, ends_with);
}

String ckit_str_reverse(const char* string_buffer) {
    size_t reversed_string_buffer_capacity = ckit_cstr_length(string_buffer) + 1;
    String reversed_string_buffer = ckit_str_create_custom("", reversed_string_buffer_capacity);
    ckg_cstr_reverse(string_buffer, reversed_string_buffer, reversed_string_buffer_capacity);
    return reversed_string_buffer;
}
