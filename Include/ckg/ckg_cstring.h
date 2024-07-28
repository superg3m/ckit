#pragma once

#include "ckg_types.h"
//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	// Modifies String Buffer

	// Does not modify String Buffer

	/**
	 * @brief returns a string buffer with nullterm
	 * must free with ckg_free()
	 * @param s1 
	 * @return char* 
	 */
	char* ckg_cstr_alloc(const char* s1);

	/**
	 * @brief Requires the string buffer to be cleared to zero terminated
	 * 
	 * @param string_buffer 
	 * @param string_buffer_size 
	 * @param source 
	 */
	void ckg_cstr_append(char* string_buffer, size_t string_buffer_capacity, const char* to_append);
	void ckg_cstr_append_char(char* string_buffer, size_t string_buffer_capacity, const char to_append);

	/**
	 * @brief Requires the string buffer to be cleared to zero terminated
	 * 
	 * @param string_buffer 
	 * @param string_buffer_size 
	 * @param index 
	 */
	void ckg_cstr_insert(char* string_buffer, size_t string_buffer_capacity, const char* to_insert, const u32 index);
	void ckg_cstr_insert_char(char* string_buffer, size_t string_buffer_capacity, const char to_insert, const u32 index);
	
	/**
	 * @brief Requires the string buffer to be cleared to zero, modifies string_buffer
	 * 
	 * 
	 * @param string_buffer 
	 * @param string_buffer_size 
	 */
	void ckg_cstr_copy(char* string_buffer, size_t string_buffer_capacity, const char* to_copy);

	/**
	 * @brief generate a random string and copy it to the dest pointer
	 * 
	 * @param dest 
	 * @param length 
	 */
	void ckg_cstr_random(char* dest, size_t length);
	
	/**
	 * @brief Tests each charater in the string for equaility
	 * returns TRUE(1) if equal and FALSE(0) if not equal
	 * => if (ckg_cstr_equal("hi", "hi"))
	 * 
	 * 
	 * @param s1 
	 * @param s2 
	 * @return Boolean 
	 */
	Boolean ckg_cstr_equal(const char* s1, const char* s2);
	u32 ckg_cstr_length(const char* c_string);
	void ckg_cstr_clear(char* string_buffer);
	// Inclusive start and end STR: SHOW | 0, 0 is: S | 0, 1 is: SH
	void ckg_substring(const char* string_buffer, char* returned_buffer, u32 start_range, u32 end_range);
	Boolean ckg_cstr_contains(const char* string_buffer, const char* contains);
	s32 ckg_cstr_index_of(const char* string_buffer, const char* sub_string);
	s32 ckg_cstr_last_index_of(const char* string_buffer, const char* sub_string);
	Boolean ckg_cstr_starts_with(const char* string_buffer, const char* starts_with);
	Boolean ckg_cstr_ends_with(const char* string_buffer, const char* ends_with);
	void ckg_cstr_reverse(const char* str, char* returned_reversed_string_buffer, size_t reversed_buffer_capacity);
	void ckg_cstr_int_to_cstr(char* string_buffer, size_t string_buffer_capacity, int number);
	// ------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************