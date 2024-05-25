#include "../include/platform_services.h"
#include "../include/core_types.h"
#include "../include/core_string.h"
#include "../include/core_assert.h"

/*===========================================================
 * File: platform_services.cpp
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

//========================== Begin Types ==========================
//=========================== End Types ===========================

//************************* Begin Functions *************************
// DEFAULT IMPLEMENTATION
#ifndef CUSTOM_PLATFORM_IMPL
	#ifdef PLATFORM_WINDOWS
		#include <windows.h>
		void* platform_allocate(unsigned long long  number_of_bytes) {
			return VirtualAlloc(NULL, number_of_bytes, MEM_COMMIT, PAGE_READWRITE);
		}

		/**
		 * @brief Keep in mind that free only need a pointer that is pointign the the right data
		 * to free so it doesn't matter if its a copy or not because the copy is pointing the he same data
		 * 
		 * @param data 
		 */
		void* MACRO_platform_free(void* data) {
			VirtualFree(data, 0, MEM_RELEASE); // This is really interesting
			data = NULLPTR;
			return data;
			// Date: May 08, 2024
			// TODO(Jovanni): Look into VirtualProtect() this allows you to change memory access to NO_ACCESS
			// can help find use after free bugs interesting concept
		}

		void platform_console_init() {
			AllocConsole();
			freopen("CONIN$", "r", stdin); 
			freopen("CONOUT$", "w", stdout); 
			freopen("CONOUT$", "w", stderr);

			HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);   
			SetConsoleDisplayMode(console_handle, CONSOLE_FULLSCREEN_MODE, 0);
		}

		void platform_console_shutdown() {
			FreeConsole();
		}

		void platform_console_write(const char* message, unsigned char color) {
			// Date: May 01, 2024
			// TODO(Jovanni): This code is very flaky I would suggest fixing it
			DWORD num_written_bytes = 0;
			unsigned long long message_size_in_bytes = cstring_length(message);

			HANDLE console_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
			char out_message[PLATFORM_CHARACTER_LIMIT];
			for (int i = 0; i < PLATFORM_CHARACTER_LIMIT; i++) { // Zeroing out the buffer
				out_message[i] = '\0';
			}

			int count = 0;
			int new_line_required = 0;
			Boolean is_fatal = (color == BACK_RED);
			while (count < message_size_in_bytes) {
				char c = message[count];
				if (c == '\n' && message[count + 1] == '\0') {
					new_line_required = 1;
					break;
				}
				out_message[count++] = c;
			}

			SetConsoleTextAttribute(console_output_handle, color);
			WriteConsoleA(console_output_handle, out_message, message_size_in_bytes, &num_written_bytes, NULL);
			SetConsoleTextAttribute(console_output_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			if (new_line_required == 1) {
				WriteConsoleA(console_output_handle, "\n", 1, &num_written_bytes, NULL);
			}
		}
	#elif PLATFORM_LINUX
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

		// Date: May 24, 2024
		// NOTE(Jovanni): This doesn't clear to zero so its gonna be a problem
		void* _platform_allocate(size_t  number_of_bytes) {
			return malloc(number_of_bytes);
		}

		void MACRO_platform_free(void* data) {
			free(data);
			data = NULLPTR;
			return data;
		}
		
		// Date: April 13, 2024
		// TODO(Jovanni): Fix this to use linux's platfomr specific std console out
		void _platform_console_write(size_t message_size_in_bytes, const char* message, unsigned char color) {
			const char* color_strings[] = {COLOR_RESET, BLU, GRN, RED, MAG, WHT, BLKB, GRNB, REDB};
			printf("\033[%sm%s\033[0m", color_strings[translate_color(color)], message);
		}
	#elif PLATFORM_MACOS
	#endif
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++



