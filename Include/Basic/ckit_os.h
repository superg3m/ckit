#pragma once

// This is going to be windows only for a bit
#include "../ckit_types.h"

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

	void ckit_os_push(char* path);
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