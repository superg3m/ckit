#pragma once

// This is going to be windows only for a bit
#include "./ckit_types.h"

//========================== Begin Types ==========================
typedef struct CKIT_SystemObjectInfo {
	// last modified
	// created
	int a;
} CKIT_SystemObjectInfo;

typedef struct CKIT_File {
	char* name;
	size_t size;
	size_t cursor;
} CKIT_File;

typedef struct CKIT_Directory {
	struct CKIT_Directory* parent_directory;
	struct CKIT_Directory** sub_directories;
	CKIT_File* files;
	size_t size;
} CKIT_Directory;

typedef char* String;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	String ckit_os_get_cwd();
	void ckit_os_ls();
	void ckit_os_get_items();
	void ckit_os_chdir();
	void ckit_os_mkdir();
	void ckit_os_create_file(const char* path);
	Boolean ckit_os_path_exists(const char* path);
	void ckit_os_run_subprocess();
	void ckit_os_get_file_info();
	String ckit_os_path_join(char* path, const char* to_join);
	void ckit_os_system(const char* command);

	/**
	 * @brief returns the number of counts in a millisecond
	 * double start_counter = ckit_os_query_performance_counter();
	 * 
	 * ... do some work
	 * 
	 * double end_counter = ckit_os_query_performance_counter();
	 * double milliseconds_per_frame = end_counter - start_counter
	 * double seconds_per_frame = milliseconds_per_frame / 1000.0
	 * u64 fps = 1.0 / seconds_per_frame;
	 * 
	 * @return double 
	 */
	double ckit_os_query_performance_counter();

	CKIT_File* ckit_os_file_open(const char* path);
	String ckit_os_file_read_next_line(CKIT_File* file);
	String ckit_os_file_read_next_integer(CKIT_File* file);
	String ckit_os_file_read_next_float(CKIT_File* file);

	CKIT_File* ckit_os_file_close(CKIT_File* file);

	CKIT_File* MACRO_ckit_os_close_file(CKIT_File* file);
	u8* ckit_os_read_entire_file(const char* path, size_t* returned_file_size);

	void ckit_os_get_mouse_position(int* mouse_x, int* mouse_y);

	void ckit_os_push();
	void ckit_os_pop();

	void ckit_os_dir_append_subdir(CKIT_Directory* directory, CKIT_Directory* sub_directory);
	void ckit_os_dir_append_file(CKIT_Directory* directory, CKIT_File* file);

	// CKIT_Time ckit_os_timer_start()
	// CKIT_Time ckit_os_timer_end()
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

#if defined(CKIT_IMPL)
	#include "./ckit_memory.h"
	#include "./ckit_string.h"

	internal String cwd = NULLPTR;
	internal String cached_directory = NULLPTR;

	// just asserts because I don't like handling errors

	void ckit_os_system(const char* command) {
		ckit_assert(system(command));
	}

	double ckit_os_query_performance_counter() {
		LARGE_INTEGER performance_counter_frequency;
		QueryPerformanceFrequency(&performance_counter_frequency);

		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		double milliseconds_elapsed = ((double)(counter.QuadPart * 1000.0) / (double)performance_counter_frequency.QuadPart);

		return milliseconds_elapsed;
	}

	String ckit_os_path_join(char* path, const char* to_join) {
		String ret = ckit_str_create(path);
		ckit_str_append_char(ret, OS_DELIMITER);
		ckit_str_append(ret, to_join);
		return ret;
	}

	#if defined(PLATFORM_WINDOWS)
		#include <windows.h>

		void ckit_os_init() {
			cwd = ckit_str_create_custom("", PLATFORM_MAX_PATH);
		}

		String ckit_os_get_cwd() {
			TCHAR buffer[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, buffer);

			String ret = ckit_str_create(buffer);

			return ret;
		}

		void ckit_os_ls();
		void ckit_os_get_items();
		void ckit_os_chdir(const char* path) {
			ckit_os_path_exists(path);
			SetCurrentDirectory(path);
		}
		void ckit_os_mkdir();
		void ckit_os_create_file(const char* path) {
			HANDLE file_handle = CreateFileA(path, GENERIC_READ, 0, NULLPTR, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULLPTR);
			ckit_assert(CloseHandle(file_handle));
		}

		Boolean ckit_os_path_exists(const char* path) {
			return (GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES);
		}
		void ckit_os_run_subprocess(); // runs on seperate thread?
		void ckit_os_get_file_info();
		u8* ckit_os_read_entire_file(const char* path, size_t* returned_file_size) {
			ckit_assert(ckit_os_path_exists(path));

			HANDLE file_handle = CreateFileA(path, GENERIC_READ, 0, NULLPTR, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULLPTR);
			LARGE_INTEGER large_int = {0};
			BOOL success = GetFileSizeEx(file_handle, &large_int);
			ckit_assert(success);
			size_t file_size = large_int.QuadPart;
			DWORD bytes_read = 0;

			u8* file_data = ckit_alloc_custom(file_size, TAG_CKIT_EXPECTED_USER_FREE);
			success = ReadFile(file_handle, file_data, (DWORD)file_size, &bytes_read, NULLPTR);
			ckit_assert(success && CloseHandle(file_handle));

			*returned_file_size = file_size;

			return file_data;
		}

		void ckit_os_get_mouse_position(int* mouse_x, int* mouse_y) {
			POINT point;
			ckit_assert(GetCursorPos(&point));
			*mouse_x = point.x;
			*mouse_y = point.y;
		}

		void ckit_os_push(char* path);
		void ckit_os_pop();

		void ckit_os_dir_append_subdir(CKIT_Directory* directory, CKIT_Directory* sub_directory);
		void ckit_os_dir_append_file(CKIT_Directory* directory, CKIT_File* file);
	#else
		void ckit_os_init() {
			cwd = ckit_str_create_custom("", PLATFORM_MAX_PATH);
		}

		String ckit_os_get_cwd() {
			// return cwd;
		}

		void ckit_os_ls();
		void ckit_os_get_items();
		void ckit_os_chdir();
		void ckit_os_mkdir();
		void ckit_os_create_file(const char* path) {
			
		}

		Boolean ckit_os_path_exists(const char* path) {
			FILE *fptr = fopen(path, "r");

			if (fptr == NULL) {
				return FALSE;
			}

			fclose(fptr);

			return TRUE;
		}
		
		void ckit_os_run_subprocess(); // runs on seperate thread?
		void ckit_os_get_file_info();
		void ckit_os_system(const char* command);
		String ckit_os_path_join(char* path, const char* to_join);

		u8* ckit_os_read_entire_file(const char* path, size_t* returned_file_size) {
			ckit_assert(ckit_os_path_exists(path));

			FILE* file_handle = fopen(path, "r");

			if (file_handle == NULL) {
				return FALSE;
			}

			fseek(file_handle, 0L, SEEK_END);
			size_t file_size = ftell(file_handle);
			rewind(file_handle);

			u8* file_data = ckit_alloc(file_size);
			ckg_assert_msg(fread(file_data, file_size, 1 , file_handle) != file_size, "Error reading file");
			rewind(file_handle);

			fclose(file_handle);
			*returned_file_size = file_size;

			return file_data;
		}

		void ckit_os_get_mouse_position(int* mouse_x, int* mouse_y) {
			ckit_assert_msg(FALSE, "NOT IMPLETMENTED FOR LINUX YET");
		}
		
		void ckit_os_push(char* path);
		void ckit_os_pop();

		void ckit_os_dir_append_subdir(CKIT_Directory* directory, CKIT_Directory* sub_directory);
		void ckit_os_dir_append_file(CKIT_Directory* directory, CKIT_File* file);
	#endif
#endif // CKIT_IMPL
