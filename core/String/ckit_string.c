
#include <setjmp.h>
#include <signal.h>

#include "./ckit_string.h"

#include "../Assert/ckit_assert.h"
#include "../Memory/ckit_memory.h"
#include "../../ckg/core/String/ckg_cstring.h"


typedef struct CKIT_StringHeader {
    u32 length;
    u32 capacity;
    char* magic; 
} CKIT_StringHeader;

#define ckit_str_header(string) ((CKIT_StringHeader*)(string - sizeof(CKIT_StringHeader)))
#define CKIT_STR_MAGIC "CKIT_MAGIC_APPLE"

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
    ckit_assert_msg(ckit_str_equal(ckit_str_header(str)->magic, CKIT_STR_MAGIC), "String: %s is null can't check magic: (%s) Likely not a CKIT_String\n", str, CKIT_STR_MAGIC);
}

internal void ckit_str_insert_header(char** str, CKIT_StringHeader header) {
  	ckg_memory_copy(&header, *str,  sizeof(header),  sizeof(header) + header.capacity);
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

void ckit_str_copy(); // Careful about the header
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

