#pragma once
/*===========================================================
 * File: platform_services.h
 * Date: May 10, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

#include "./ckit_types.h"
//========================== Begin Types ==========================
#define TEXT_CLEAR     0x0000 // text color default clear
#define TEXT_BLUE      0x0001 // text color contains blue.
#define TEXT_GREEN     0x0002 // text color contains green.
#define TEXT_RED       0x0004 // text color contains red.
#define TEXT_CYAN      TEXT_BLUE|TEXT_GREEN // text color contains cyan.
#define TEXT_PURPLE    TEXT_RED|TEXT_BLUE // text color contains purple.
#define TEXT_WHITE     TEXT_RED|TEXT_GREEN|TEXT_BLUE // text color contains white.

#define BACK_BLUE      0x0010 // background color contains blue.
#define BACK_GREEN     0x0020 // background color contains green.
#define BACK_RED       0x0040 // background color contains red.
//=========================== End Types ===========================

//************************* Begin Functions *************************
// Supported Platform Operations

#ifdef __cplusplus
extern "C" {
#endif
  void* platform_allocate(size_t number_of_bytes);
  void platform_free(void* data);
  void platform_console_write(const char* message, unsigned char color);
  void platform_console_init();
  void platform_console_shutdown();
  // void _platform_open_window();
  // void _platform_open_file();
  // void _platform_close_file();
  // void _platform_sleep();
#ifdef __cplusplus
}
#endif

//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

#if defined(CKIT_IMPL)
  #include "./ckit_types.h"
  #include "./ckit_string.h"
  #include "./ckit_assert.h"

  //************************* Begin Functions *************************
  // DEFAULT IMPLEMENTATION
  #if !defined(CUSTOM_PLATFORM_IMPL)
		#if defined(PLATFORM_WINDOWS)
			#include <windows.h>
			void* platform_allocate(size_t number_of_bytes) {
				return VirtualAlloc(NULL, number_of_bytes, MEM_COMMIT, PAGE_READWRITE); 
			}

			/**
			 * @brief Keep in mind that free only need a pointer that is pointign the the right data
			 * to free so it doesn't matter if its a copy or not because the copy is pointing the he same data
			 * 
			 * @param data 
			 */
			void platform_free(void* data) {
				VirtualFree(data, 0, MEM_RELEASE); // This is really interesting
				// Date: May 08, 2024
				// TODO(Jovanni): Look into VirtualProtect() this allows you to change memory access to NO_ACCESS
				// can help find use after free bugs interesting concept
			}

			void platform_console_init() {
				AllocConsole();

				HANDLE console_handle = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (console_handle == INVALID_HANDLE_VALUE) {
					LOG_ERROR("platform_console_init: ${FAILED}");
					platform_console_shutdown();
					return;
				}

				DWORD dwMode = 0;
				GetConsoleMode(console_handle, &dwMode);
				dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				SetConsoleMode(console_handle, dwMode);

				ckit_assert(CloseHandle(console_handle));

				freopen("CONOUT$", "w", stdout);
				freopen("CONIN$", "r", stdin);
				freopen("CONOUT$", "w", stderr);
			}

			void platform_console_shutdown() {
				FreeConsole();
			}

		#elif defined(PLATFORM_LINUX)
			#define COLOR_RESET "\033[0"
			#define BLU "\033[34"
			#define GRN "\033[32"
			#define RED "\033[31"
			#define MAG "\033[35"
			#define WHT "\033[37"
			#define BLKB "\033[40"
			#define GRNB "\033[42"
			#define REDB "\033[41"

			int translate_color(unsigned char color) {
				switch (color) {
				case 0x0000: { // text color default clear.
					return 0;
				} break;

				case 0x0001: { // text color contains blue.
					return 1;
				} break;

				case 0x0002: { // text color contains green.
					return 2;
				} break;

				case 0x0004: { // text color contains red.
					return 3;
				} break;

				case (0x0001|0x0004): { // background color contains purple.
					return 4;
				} break;

				case (0x0001|0x0002|0x0004): { // background color contains red.
					return 5;
				} break;

				case 0x0010: { // background color contains blue.
					return 6;
				} break;

				case 0x0020: { // background color contains green.
					return 7;
				} break;

				case 0x0040: { // background color contains red.
					return 8;
				} break;
				}
			}

			void* platform_allocate(size_t  number_of_bytes) {
				return malloc(number_of_bytes);
			}

			void platform_free(void* data) {
				free(data);
			}

			void platform_console_init() {

			}
			
			void platform_console_write(const char* message, unsigned char color) {
				const char* color_strings[] = {COLOR_RESET, BLU, GRN, RED, MAG, WHT, BLKB, GRNB, REDB};
				printf("\033[%sm%s\033[0m", color_strings[translate_color(color)], message);
			}
		#elif defined(PLATFORM_MACOS)
		#endif
	#endif
#endif // CKIT_IMPL