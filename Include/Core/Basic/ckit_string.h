#pragma once

#include "./ckit_types.h"
//========================== Begin Types ==========================
typedef char* String;

// Date: July 13, 2024
// TODO(Jovanni): ONE THGING THATS IMPORTANT I DON"T WANT TO HAVE TO FREE ANY OF THESE IT SHOULD JUST BE AND ARENA
// TODO(Jovanni): ONE THGING THATS IMPORTANT I DON"T WANT TO HAVE TO FREE ANY OF THESE IT SHOULD JUST BE AND ARENA
// TODO(Jovanni): ONE THGING THATS IMPORTANT I DON"T WANT TO HAVE TO FREE ANY OF THESE IT SHOULD JUST BE AND ARENA

// Date: August 11, 2024
// NOTE(Jovanni): Just so we are clear this is fucking dogshit right now it takes an unholy amount of memory syscalls for this to be ok
// The arena idea might make this plausible tho! Maybe?

//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	String ckit_str_create_custom(const char* c_str, size_t capacity);
	u32 ckit_cstr_length(const char* str);
	u32 ckit_str_length(const String str);
	Boolean ckit_str_equal(const char* str1, const char* str2);
	void ckit_str_copy(String str, const char* source);

	String MACRO_ckit_str_append(String str, const char* source);
	String MACRO_ckit_str_append_char(String str, const char source);

	String MACRO_ckit_str_insert(String str, const char* to_insert, const u32 index);
	String MACRO_ckit_str_insert_char(String str, const char to_insert, const u32 index);
	void ckit_str_clear(char* str1);

	// If you are copying data to the string and need to update the header state specifically for length
	void ckit_str_recanonicalize_header_length(String str);

	String ckit_substring(const char* string_buffer, u32 start_range, u32 end_range);

	// Little bit tricky. This method returns a vector of strings so 
	// ckit_vector_count: to get the number of strings it returned 
	// Caller is required to loop through the vector and free all the strings
	//
	//  for (u32 i = 0; i < ckit_vector_count(string_vector); i++) {
	//     LOG_PRINT("%s\n", string_vector[i]);
	//  }
	//  ckit_vector_free(string_vector);
	String* ckit_str_split(const char* string_buffer, const char* delimitor);
	// String* ckit_str_split_with_char(const char* string_buffer, const char delimitor);
	Boolean ckit_str_contains(const char* string_buffer, const char* contains);
	s32 ckit_str_index_of(const char* string_buffer, const char* sub_string);
	s32 ckit_str_last_index_of(const char* string_buffer, const char* sub_string);
	Boolean ckit_str_starts_with(const char* string_buffer, const char* starts_with);
	Boolean ckit_str_ends_with(const char* string_buffer, const char* ends_with);
	String ckit_str_reverse(const char* string_buffer);
	String ckit_str_int_to_str(int number);
	int ckit_str_to_int(const char* ascii_number);
	String ckit_str_between_delimiters(const char* str, const char* start_delimitor, const char* end_delimitor);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_str_create(str) ckit_str_create_custom(str, 0)

#define ckit_str_insert(str, source, index) str = MACRO_ckit_str_insert(str, source, index);
#define ckit_str_insert_char(str, source, index) str = MACRO_ckit_str_insert_char(str, source, index);

#define ckit_str_append(str, source) str = MACRO_ckit_str_append(str, source);
#define ckit_str_append_char(str, source) str = MACRO_ckit_str_append_char(str, source);
//++++++++++++++++++++++++++++ End Macros ++++++++++++++++++++++++++

#if defined(CKIT_IMPL)
	#include "../../../ckg/Include/ckg_cstring.h"

	#include "./ckit_assert.h"
	#include "./ckit_memory.h"
	#include "../Collections/ckit_vector.h"
	typedef struct CKIT_StringHeader {
		u32 length; 
		size_t capacity;
		char* magic; 
	} CKIT_StringHeader;

	#define ckit_str_header(string) ((CKIT_StringHeader*)(string - sizeof(CKIT_StringHeader)))
	#define CKIT_STR_MAGIC "CKIT_MAGIC_STRING"

	#include "./ckit_arena.h"
	CKIT_Arena* string_arena;

	void ckit_str_register_arena() {
		#define STRING_ARENA_DEFAULT_CAPACITY MegaBytes(2)
		string_arena = ckit_arena_create_custom(STRING_ARENA_DEFAULT_CAPACITY, "String Arena", CKIT_ARENA_FLAG_EXTENDABLE_PAGES, sizeof(char));
	}

	internal void ckit_str_check_magic(String str) {
		ckit_assert_msg(str, "String: %s is null can't check magic: (%s) Likely not a CKIT_String\n", str, CKIT_STR_MAGIC);
		ckit_assert_msg(ckit_str_equal(ckit_str_header(str)->magic, CKIT_STR_MAGIC), "String: %s has the wrong magic: {%s} got: {%s} \n", str, CKIT_STR_MAGIC, ckit_str_header(str)->magic);
	}

	internal void ckit_str_insert_header(char** str, CKIT_StringHeader header) {
		ckit_memory_copy(&header, *str,  sizeof(header),  sizeof(header) + header.capacity);
		*str = (char*)(((u8*)(*str)) + sizeof(header));
	}

	internal inline String ckit_str_grow(String str, size_t new_allocation_size) {
		ckit_str_check_magic(str);
		CKIT_StringHeader header = *ckit_str_header(str);
		header.capacity = new_allocation_size;
		String ret = ckit_str_create_custom(str, header.capacity);
		
		return ret;
	}

	// Date: May 23, 2024
	// TODO(Jovanni): I need to eventually go back to using an arena I liked the idea
	// Of pushing the header then right after pushing the str data I think this worked really well.
	String ckit_str_create_custom(const char* c_string, size_t capacity) {
		u32 c_str_length = ckg_cstr_length(c_string);
		size_t true_capacity = capacity != 0 ? capacity : sizeof(char) * (c_str_length + 1);
		CKIT_StringHeader* header = MACRO_ckit_arena_push(string_arena, sizeof(CKIT_StringHeader) + true_capacity);
		header->length = c_str_length;
		header->capacity = true_capacity;
		header->magic = CKIT_STR_MAGIC;

		String ret = (String)((u8*)header + sizeof(CKIT_StringHeader));

		ckg_cstr_copy(ret, header->capacity, c_string);
		return ret;
	}

	Boolean ckit_str_equal(const char* str1, const char* str2) {
		return ckg_cstr_equal(str1, str2);
	}

	String MACRO_ckit_str_insert(String str, const char* to_insert, const u32 index) {
		u32 source_capacity = ckit_cstr_length(to_insert) + 1; 
		CKIT_StringHeader* header = ckit_str_header(str);
		if (header->length + source_capacity >= header->capacity) {
			str = ckit_str_grow(str, (header->length + source_capacity) * 2);
			header = ckit_str_header(str);
		}

		ckg_cstr_insert(str, header->capacity, to_insert, index);
		header->length += source_capacity - 1;
		return str;
	}

	String MACRO_ckit_str_insert_char(String str, const char to_insert, const u32 index) {
		u32 source_size = 1;
		CKIT_StringHeader* header = ckit_str_header(str);
		if (header->length + source_size >= header->capacity) {
			str = ckit_str_grow(str, (header->length + source_size) * 2);
			header = ckit_str_header(str);
		}

		ckg_cstr_insert_char(str, header->capacity, to_insert, index);
		header->length++;
		return str;
	}

	void ckit_str_clear(char* str1) {
		ckg_cstr_clear(str1);
	}

	void ckit_str_recanonicalize_header_length(String str) {
		u32 actual_length = ckit_cstr_length(str);
		ckit_str_header(str)->length = actual_length;
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
		String ret_string = ckit_str_create_custom("", ((end_range + 1) - start_range) + 1);
		ckg_substring(string_buffer, ret_string, start_range, end_range);
		ckit_str_recanonicalize_header_length(ret_string);

		return ret_string;
	}

	internal String* ckit_str_split_helper(String* ret_buffer, const char* string_buffer, u32 offset_into_buffer, const char* delimitor) {
		s32 found_index = ckit_str_index_of(string_buffer + offset_into_buffer, delimitor);
		u32 offset_to_delimitor = found_index + offset_into_buffer;
		if (found_index == -1) {
			ckit_vector_push(ret_buffer, ckit_substring(string_buffer, offset_into_buffer, ckit_cstr_length(string_buffer) - 1));
			return ret_buffer;
		}

		String debug_test = ckit_substring(string_buffer, offset_into_buffer, offset_to_delimitor - 1);
		ckit_vector_push(ret_buffer, debug_test);
		return ckit_str_split_helper(ret_buffer, string_buffer, offset_to_delimitor + 1, delimitor);
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

	String ckit_str_int_to_str(int number) {
		int length_of_number = 0;
		String ret = ckit_str_create("");

		while (number != 0) {
			char c = '0' + FIRST_DIGIT(number);
			ckit_str_insert_char(ret, c, 0);
			number /= (int)10;
		}

		return ret;
	}

	int ckit_str_to_int(const char* ascii_number) {
		return atoi(ascii_number);
	}

	String ckit_str_between_delimiters(const char* str, const char* start_delimitor, const char* end_delimitor) {
		ckit_assert(str);
		ckit_assert(start_delimitor);
		ckit_assert(end_delimitor);
		ckit_assert(!ckit_str_equal(start_delimitor, end_delimitor));

		u32 str_length = ckit_cstr_length(str);
		u32 start_delimitor_length = ckit_cstr_length(start_delimitor);
		u32 end_delimitor_length = ckit_cstr_length(end_delimitor);

		s32 start_delimitor_index = ckit_str_index_of(str, start_delimitor); 
		s32 end_delimitor_index = ckit_str_index_of(str, end_delimitor);

		String ret = ckit_str_create("");

		if (start_delimitor_index == -1 || end_delimitor_index == -1) {
			return NULLPTR;
		}

		if (start_delimitor_index > end_delimitor_index) {
			return NULLPTR; // The start delimtor is after the end delimitor
		}

		u32 i = start_delimitor_index + start_delimitor_length;

		while (i < (u32)end_delimitor_index) {
			ckit_str_append_char(ret, str[i++]);
		}

		return ret;
	}
#endif