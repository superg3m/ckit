#include "../include/platform_services.h"
#include "../include/core_types.h"

// DEFAULT IMPLEMENTATION
#ifndef CUSTOM_PLATFORM_IMPL
	#ifdef PLATFORM_WINDOWS
		#include <windows.h>
		void* _platform_allocate(size_t  number_of_bytes) {
			return VirtualAlloc(NULL, number_of_bytes, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
		}

		void _platform_free(size_t number_of_bytes, void** data) {
			VirtualFree(*data, number_of_bytes, MEM_DECOMMIT);
		}

		void set_console_screen_size(int x, int y){

			COORD coord;
			coord.X = x;
			coord.Y = y;

			_SMALL_RECT Rect;
			Rect.Top = 0;
			Rect.Left = 0;
			Rect.Bottom = x - 1;
			Rect.Right = y - 1;

			// Adjust buffer size:
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);    
			SetConsoleScreenBufferSize(handle, coord);

			// display as a maximized window
			ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
		}

		void _platform_console_init() {
			AllocConsole();
			freopen("CONIN$", "r", stdin); 
			freopen("CONOUT$", "w", stdout); 
			freopen("CONOUT$", "w", stderr);

			set_console_screen_size(100, 100);
			set_console_screen_size(800, 600);
		}

		void _platform_console_shutdown() {
			FreeConsole();
		}

		void _platform_console_write(size_t message_size_in_bytes, const char* message, unsigned char color) {
			// Date: May 01, 2024
			// TODO(Jovanni): This code is very flaky I would suggest fixing it
			DWORD num_written_bytes = 0;
			HANDLE console_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
			char out_message[PLATFORM_COMMON_CHARACTER_LIMIT];
			for (int i = 0; i < PLATFORM_COMMON_CHARACTER_LIMIT; i++) { // Zeroing out the buffer
				out_message[i] = '\0';
			}

			int count = 0;
			int new_line_required = 0; 
			while (count < message_size_in_bytes) {
				char c = message[count];
				if (c == '\n') {
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

		void* _platform_allocate(size_t  number_of_bytes) {
			return malloc(number_of_bytes);
		}

		void _platform_free(size_t number_of_bytes, void** data) {
			free(*data);
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
