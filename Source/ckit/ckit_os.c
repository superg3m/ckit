
#include "ckit_os.h"
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
