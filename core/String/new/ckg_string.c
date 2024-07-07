/*===========================================================
 * File: ckit_string.cpp
 * Date: May 09, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

#include "./ckg_string.h"

#include "../Assert/ckg_assert.h"
#include "../Memory/ckg_memory.h"
#include "../Memory/ckg_arena.h"

typedef struct CKG_StringHeader {
    u32 length;
    size_t capacity;
    char* magic;
} CKG_StringHeader;

#define ckg_str_header(string) ((CKG_StringHeader*)(string - sizeof(CKG_StringHeader)))
#define CKG_STR_MAGIC "CKG_MAGIC_APPLE"

void ckg_str_check_magic(CKG_String str) {
    ckg_assert_msg(str && ckg_str_equal(ckg_str_header(str)->magic, CKG_STR_MAGIC), "String: %s contains the wrong magic: (%s) Likely not a CKG_String\n", str, ckg_str_header(str)->magic);
}

internal void ckg_string_insert_header(char** string, CKG_StringHeader header) {
  	ckg_memory_copy(&header, *string,  sizeof(header),  sizeof(header) + header.capacity);
    *string = ((u8*)*string) + sizeof(header);
}

size_t ckg_str_capacity(CKG_String string) {
    return (*ckg_str_header(string)).capacity;
}

// Date: May 23, 2024
// TODO(Jovanni): I need to eventually go back to using an arena I liked the idea
// Of pushing the header then right after pushing the string data I think this worked really well.
CKG_String ckg_str_create_custom(const char* c_string, size_t capacity) {
  	u32 c_str_length = ckg_cstr_length(c_string);
  	CKG_StringHeader header;
  	header.length = c_str_length;
  	header.capacity = capacity != 0 ? capacity : sizeof(char) * (c_str_length + 1);
    header.magic = CKG_STR_MAGIC;
	
  	CKG_String ret = ckg_alloc(sizeof(header) + header.capacity);
    ckg_string_insert_header(&ret, header);
	
  	ckg_cstr_copy(ret, header.capacity, c_string);
  	return ret;
}

/**
 * @brief if the string needs to grow return the new address otherwise return the input string address
 * 
 * @param string 
 * @param additional_allocation_size 
 * @return internal 
 */
internal CKG_String ckg_str_grow(CKG_String string, size_t additional_allocation_size) {
    CKG_StringHeader old_header = *ckg_str_header(string); 
    if (old_header.length + additional_allocation_size < old_header.capacity) {
        return string;
    }

    CKG_String ret = ckg_str_create_custom(string, 2 * (old_header.length + additional_allocation_size));
    ckg_str_free(string);
    
    return ret;
}

void ckg_str_copy(CKG_String string, const char* to_copy) {
    ckg_str_check_magic(string);
    u32 copy_length = ckg_str_length(to_copy);
    string = ckg_str_grow(string, copy_length);
    ckg_cstr_copy(string, ckg_str_capacity(string), to_copy);
}


CKG_String MACRO_ckg_str_free(CKG_String string) {
    string = string - sizeof(CKG_StringHeader);
    memory_free(string);
    return string;
}

CKG_String MACRO_ckg_str_append(CKG_String string, const char* to_append) {
    ckit_assert_msg(string, "ckg_str_append: String passed is null\n");
    ckit_assert_msg(to_append, "ckg_str_append: Source passed is null\n");

    u32 to_append_length = ckg_cstr_length(to_append); 

    string = ckg_str_grow(string, to_append_length);
	ckg_str_header(string)->length += to_append_length;
    ckg_cstr_append(string, ckg_str_capacity(string), to_append);
    
    return string;
}

CKG_String MACRO_ckg_str_append_char(CKG_String string, const char to_append) {
    ckit_assert_msg(string, "ckg_str_append_char: String passed is null\n");
    ckit_assert_msg(to_append, "ckg_str_append_char: Source passed is null\n");

    u32 to_append_length = 1;
    string = ckg_str_grow(string, to_append_length);
    ckg_str_header(string)->length += to_append_length;
    string[ckg_str_length(string)] = to_append;

    return string;
}