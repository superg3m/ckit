#pragma once
/*===========================================================
 * File: ckit_file_system.h
 * Date: May 14, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include <stdio.h>
#include "ckit_assert.h"
#include "ckit_string.h"


typedef struct FileSystem {
	Boolean reachedEOF;
	const char* fileName;
	FILE* file;
} FileSystem;

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	FileSystem file_system_create(const char* fileName);
	void file_open(FileSystem* file_system);
	void file_close(FileSystem* file_system);
	String file_get_next_line(FileSystem* file_system);
	char file_get_next_char(FileSystem* file_system);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

