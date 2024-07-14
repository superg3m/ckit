
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
	
  	String ret = ckit_alloc(sizeof(header) + header.capacity, MEMORY_TAG_STRING);
    ckit_str_insert_header(&ret, header);
	
  	ckg_cstr_copy(ret, header.capacity, c_string);
  	return ret;
}

Boolean ckit_str_equal(const String str1, const String str2) {
    return ckg_cstr_equal(str1, str2);
}

void ckit_str_clear(const String str1) {
    ckg_cstr_clear(str1);
}

void ckit_str_recanonicalize_header_state(String str) {
    ckit_str_header(str)->length = ckit_str_length(str);
}

void ckit_str_copy(String str1, const char* source) {
    ckit_str_clear(str1);
    ckit_str_append(str1, source);
}

u32 ckit_str_length(const String str) {
    return ckg_cstr_length(str);
}

String MACRO_ckit_str_append(String str, const char* source) {
    ckit_str_check_magic(str);
    ckit_assert_msg(str, "ckit_str_append: String passed is null\n");
    ckit_assert_msg(source, "ckit_str_append: Source passed is null\n");

    u32 source_size = ckg_cstr_length(source) + 1; 

    CKIT_StringHeader* header = ckit_str_header(str);
    if (header->length + source_size >= header->capacity) {
        str = ckit_str_grow(str, (header->length + source_size) * 2);
        header = ckit_str_header(str);
    }

	header->length += ckg_cstr_length(source);
    ckg_cstr_append(str, header->capacity, source);
    
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


String ckit_substring(const String string_buffer, u32 start_range, u32 end_range) {
    String ret_string = ckit_str_create_custom("", end_range - start_range + 1);
    ckg_substring(string_buffer, ret_string, start_range, end_range);
    ckit_str_recanonicalize_header_state(ret_string);

    return ret_string;
}

String* ckit_str_split(const String string_buffer, const char* delimitor) {
    ckit_assert_msg(FALSE, "NOT IMPLMENTED YET");
    ckit_str_check_magic(string_buffer);

	ckit_assert(string_buffer);
	ckit_assert(delimitor);

	size_t str_length = ckg_cstr_length(string_buffer); 
	size_t delmitor_length = ckg_cstr_length(delimitor); 

	if (delmitor_length <= 0) {
		return NULLPTR;
	}

	String* ret_string_array = NULLPTR;
	String temp_buffer = ckit_str_create("");
	String perma_buffer = ckit_str_create("");
	for (int i = 0; i < str_length; i++) {
		ckit_str_append_char(temp_buffer, string_buffer[i]);
		String temp_substring = ckit_substring(string_buffer, i, delmitor_length - 1);
		
		if (temp_substring[i] != delimitor[0]) {
			continue;
		}

		if (ckg_cstr_equal(temp_substring, delimitor)) {
            ckit_str_copy(perma_buffer, temp_buffer);
            ckit_vector_push(ret_string_array, perma_buffer);
			ckit_str_clear(temp_buffer);
		}
		ckit_str_free(temp_substring);
	}
	ckit_str_free(temp_buffer);

	return ret_string_array;
}

Boolean ckit_cstr_contains(const String string_buffer, const char* contains) {
    return ckg_cstr_contains(string_buffer, contains);
}

u32 ckit_cstr_index_of(const String string_buffer, const char* sub_string) {
    return ckg_cstr_index_of(string_buffer, sub_string);
}

u32 ckit_cstr_last_index_of(const String string_buffer, const char* sub_string) {
    return ckg_cstr_last_index_of(string_buffer, sub_string);
}

Boolean ckit_cstr_starts_with(const String string_buffer, const char* starts_with) {
    return ckg_cstr_starts_with(string_buffer, starts_with);
}

Boolean ckit_cstr_ends_with(const String string_buffer, const char* ends_with) {
    return ckg_cstr_ends_with(string_buffer, ends_with);
}

String ckit_cstr_reverse(const String string_buffer) {
    return ckg_cstr_reverse(string_buffer);
}
