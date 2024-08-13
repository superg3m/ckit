#pragma once

// This is going to be windows only for a bit
#include "ckit_types.h"

//========================== Begin Types ==========================
typedef struct CKIT_SystemObjectInfo {
	// last modified
	// created
	int a;
} CKIT_SystemObjectInfo;

typedef struct CKIT_File {
	char* name;
	size_t size;
} CKIT_File;

typedef struct CKIT_Directory {
	struct CKIT_Directory* parent_directory;
	struct CKIT_Directory** sub_directories;
	CKIT_File* files;
	size_t size;
} CKIT_Directory;


//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	const char* ckit_os_get_cwd();
	void ckit_os_ls();
	void ckit_os_get_items();
	void ckit_os_chdir();
	void ckit_os_mkdir();
	void ckit_os_create_file(const char* path); // prob just assert if path doesn't exists
	Boolean ckit_os_path_exists(const char* path);
	void ckit_os_run_subprocess();
	void ckit_os_get_file_info();
	void ckit_os_path_join();

	u8* ckit_os_read_entire_file(const char* path);

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
	#include "ckit_memory.h"
	#include "ckit_string.h"


	internal String cwd = NULLPTR;
	internal String cached_directory = NULLPTR;

	// just asserts because I don't like handling errors

	#if defined(PLATFORM_WINDOWS)
		#include <windows.h>

		void ckit_os_init() {
			cwd = ckit_str_create_custom("", PLATFORM_MAX_PATH);
		}

		const char* ckit_os_get_cwd() {
			return cwd;
		}

		void ckit_os_ls();
		void ckit_os_get_items();
		void ckit_os_chdir();
		void ckit_os_mkdir();
		void ckit_os_create_file(const char* path) {
			// ckit_assert(ckit_str_length(cwd + path) > PLATFORM_MAX_PATH);
		}

		Boolean ckit_os_path_exists(const char* path) {
			return PathFileExistsA(path);
		}
		void ckit_os_run_subprocess(); // runs on seperate thread?
		void ckit_os_get_file_info();
		void ckit_os_system();
		void ckit_os_path_join();

		u8* ckit_os_read_entire_file(const char* path) {
			// ckit_assert(ckit_os_path_exists(path));

			// THIS IS WINDOWS EXCLUSIVE FOR RIGHT NOW!!!
			HANDLE file_handle = CreateFileA(path, GENERIC_READ, 0, NULLPTR, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULLPTR);
			LARGE_INTEGER large_int = {};
			BOOL succuess = GetFileSizeEx(file_handle, &large_int);
			u64 file_size = large_int.QuadPart;
			DWORD bytes_read = 0;

			u8* file_data = ckit_alloc_custom(file_size, TAG_CKIT_EXPECTED_USER_FREE);
			succuess = ReadFile(file_handle, file_data, file_size, &bytes_read, NULLPTR);

			return file_data;
		}

		void ckit_os_push(char* path);
		void ckit_os_pop();

		void ckit_os_dir_append_subdir(CKIT_Directory* directory, CKIT_Directory* sub_directory);
		void ckit_os_dir_append_file(CKIT_Directory* directory, CKIT_File* file);
	#else
		void ckit_os_init() {
			cwd = ckit_str_create_custom("", PLATFORM_MAX_PATH);
		}

		const char* ckit_os_get_cwd() {
			return cwd;
		}

		void ckit_os_ls();
		void ckit_os_get_items();
		void ckit_os_chdir();
		void ckit_os_mkdir();
		void ckit_os_create_file(const char* path) {
			// ckit_assert(ckit_str_length(cwd + path) > PLATFORM_MAX_PATH);
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
		void ckit_os_system();
		void ckit_os_path_join();

		u8* ckit_os_read_entire_file(const char* path) {
			// ckit_assert(ckit_os_path_exists(path));

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

			return file_data;
		}

		void ckit_os_push(char* path);
		void ckit_os_pop();

		void ckit_os_dir_append_subdir(CKIT_Directory* directory, CKIT_Directory* sub_directory);
		void ckit_os_dir_append_file(CKIT_Directory* directory, CKIT_File* file);
	#endif
#endif // CKIT_IMPL
