#pragma once

// This is going to be windows only for a bit
#include "ckit_types.h"

//========================== Begin Types ==========================
typedef struct CKIT_SystemObjectInfo {
	// last modified
	// created
	int a;
} CKIT_SystemObjectInfo;

typedef struct CKIT_Directory {
	CKIT_Directory* parent_directory;
	CKIT_Directory* sub_directories;
	CKIT_File* files;
	size_t size;
} CKIT_Directory;

typedef struct CKIT_File {
	CKIT_Directory* parent_directory;
	char* name;
	size_t size;
} CKIT_File;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	void ckit_os_get_cwd();
	void ckit_os_ls();
	void ckit_os_get_items();
	void ckit_os_chdir();
	void ckit_os_mkdir();
	// Boolean ckit_os_create_file(); // can fail if name is not valid path
	// Boolean ckit_os_exists();
	void ckit_os_run_subprocess();
	void ckit_os_get_file_info();
	void ckit_os_system();
	void ckit_os_path_join();

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
	#include "ckit_string.h"

	String cwd = NULLPTR;
	String cached_directory = NULLPTR;

	// just asserts because I don't like handling errors

	void ckit_os_init() {
		cwd = ckit_str_create_custom("", PLATFORM_MAX_PATH);
	}

	char* ckit_os_get_cwd() {
		return cwd;
	}

	void ckit_os_ls();
	void ckit_os_get_items();
	void ckit_os_chdir();
	void ckit_os_mkdir();
	void ckit_os_create_file() {
		if (ckit_str_length(cwd) > PLATFORM_MAX_PATH) {
			return FALSE;
		}
	}

	Boolean ckit_os_exists();
	void ckit_os_run_subprocess(); // runs on seperate thread?
	void ckit_os_get_file_info();
	void ckit_os_system();
	void ckit_os_path_join();

	void ckit_os_push(char* path);
	void ckit_os_pop();

	void ckit_os_dir_append_subdir(CKIT_Directory* directory, CKIT_Directory* sub_directory);
	void ckit_os_dir_append_file(CKIT_Directory* directory, CKIT_File* file);
#endif // CKIT_IMPL
