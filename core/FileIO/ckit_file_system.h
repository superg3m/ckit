#pragma once
/*===========================================================
 * File: ckit_file_system.h
 * Date: May 14, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include <stdio.h>
#include "../Assert/ckit_assert.h"
#include "../String/ckit_string.h"

typedef struct FileSystem {
	String file_name;
	FILE* handle;
	u8* data;
	size_t file_size;
	Boolean reachedEOF;
} FileSystem;

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	FileSystem file_system_create(String file_name);
	void file_open(FileSystem* file_system);
	void file_close(FileSystem* file_system);
	size_t file_size(FileSystem* file_system);
	String file_get_next_line(FileSystem* file_system);
	char file_get_next_char(FileSystem* file_system);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

