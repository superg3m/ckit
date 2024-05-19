#pragma once
/*===========================================================
 * File: ckg.h
 * Date: May 19, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

#pragma region TYPES
	#include <stdint.h>
	#include <stdio.h>
	#include <stdarg.h>

	//========================== Begin Types ==========================
	typedef int8_t  s8;
	typedef int16_t s16;
	typedef int32_t s32;
	typedef int64_t s64;

	typedef uint8_t  u8;
	typedef uint16_t u16;
	typedef uint32_t u32;
	typedef uint64_t u64;

	typedef u8 Boolean;
	//=========================== End Types ===========================


	//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
	#define TRUE 1
	#define FALSE 0
	#define NULLPTR 0

	#define stringify(entry) #entry
	#define glue(a, b) a##b

	#define KiloBytes(value) ((u64)(value) * 1024L)
	#define MegaBytes(value) ((u64)KiloBytes(value) * 1024L)
	#define GigaBytes(value) ((u64)MegaBytes(value) * 1024L)

	#define local_persist static
	#define internal static

	/**
	 * @brief This only works on static arrays not buffers
	 * 
	 */
	#define ArrayCount(array) (sizeof(array) / sizeof(array[0]))
	//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
#pragma endregion

#pragma region MEMORY
	//************************* Begin Functions *************************
	#ifdef __cplusplus
	extern "C" {
	#endif

		Boolean memory_byte_compare(const void* buffer_one, const void* buffer_two, u32 buffer_one_size, u32 buffer_two_size);
		void memory_copy(const void* source, void* destination, u32 source_size, u32 destination_size);
		void memory_zero(void* data, u32 data_size_in_bytes);
		void memory_set(u8* data, u32 data_size_in_bytes, u8 element);

		void memory_buffer_delete_index(const void* data, u32 size_in_bytes, u32 buffer_count, u32 index);


		u8* memory_advance_new_ptr(const void* data, u32 size_in_bytes);
		u8* memory_retreat_new_ptr(const void* data, u32 size_in_bytes);
		void* MACRO_memory_byte_advance(const void* data, u32 size_in_bytes);
		void* MACRO_memory_byte_retreat(const void* data, u32 size_in_bytes);

	#ifdef __cplusplus
	}
	#endif
	//************************** End Functions **************************

	#define memory_fill(buffer, buffer_count, fill_element) \
	{														\
		for (int i = 0; i < buffer_count; i++) { 			\
			buffer[i] = fill_element;                       \
		}                                                  	\
	}

	/**
	 * @brief Modifies the data pointer, if you just want a new pointer consider using 
	 * memory_advance_new_ptr()
	 * 
	 */
	#define memory_byte_advance(data, size_in_bytes) data = MACRO_memory_byte_advance(data, size_in_bytes)
	#define memory_byte_retreat(data, size_in_bytes) data = MACRO_memory_byte_retreat(data, size_in_bytes)
#pragma endregion

#pragma region ASSERT
	//========================== Begin Types ==========================
	#define ASSERT_ENABLED TRUE
	#define CRASH *((int *)0) = 0
	//=========================== End Types ===========================

	//************************* Begin Functions *************************
	#ifdef __cplusplus
	extern "C" {
	#endif

	#ifdef __cplusplus
	}
	#endif
	//************************** End Functions **************************

	//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
	#if ASSERT_ENABLED == TRUE	

		#define ckg_assert_in_macro(buffer, buffer_size, expression, message, ...)								\
			do {																								\
				if (!(expression))                                                       		               	\
				{                                                                        		               	\
					memory_zero(buffer, buffer_size);                                             				\
					sprintf(buffer, "file: %s:%d | Function: %s | %s", __FILE__, __LINE__, __func__, message); 	\
					fprintf(stderr, buffer, ##__VA_ARGS__);                                             \
					CRASH;                                                                                     	\
				}																								\
			} while (FALSE)

		#define ckg_assert_in_function(expression, message, ...)	\
			do { 													\
				if (!(expression))                          		\
				{                                           		\
					fprintf(stderr, message, ##__VA_ARGS__); 		\
					CRASH;                                  		\
				}													\
			} while (FALSE)

	#else
			#define ckg_assert_in_function(expression, message)
	#endif
	//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
#pragma endregion

#pragma region STRING
	//************************* Begin Functions *************************
	#ifdef __cplusplus
	extern "C" {
	#endif

		u32 ckg_cstring_length(const char* c_string);
		Boolean ckg_string_compare(const char* s1, const char* s2);

		void ckg_string_append(char* string_buffer, u32 string_buffer_size, const char* source);
		void ckg_string_append_char(char* string_buffer, u32 string_buffer_size, const char source);

		/**
		 * @brief Requires the string buffer to be cleared to zero
		 * 
		 * @param string_buffer 
		 * @param string_buffer_size 
		 * @param index 
		 */
		void ckg_string_insert(char* string_buffer, u32 string_buffer_size, const u32 index, const char* source);

		/**
		 * @brief Requires the string buffer to be cleared to zero
		 * 
		 * @param string_buffer 
		 * @param string_buffer_size 
		 */
		void ckg_string_clear(char* string_buffer, u32 string_buffer_size);

		
		/**
		 * @brief Requires the string buffer to be cleared to zero
		 * 
		 * @param string_buffer 
		 * @param string_buffer_size 
		 */
		void ckg_string_copy(char* string_buffer, u32 string_buffer_size, const char* source);
		
	#ifdef __cplusplus
	}
	#endif
	//************************** End Functions **************************
#pragma endregion