#include "../include/platform_services.h"

// DEFAULT IMPLEMENTATION
#ifndef CUSTOM_PLATFORM_IMPL
	#ifdef PLATFORM_WINDOWS
		#include <windows.h>
		void* _platform_allocate(size_t  number_of_bytes) {
			return VirtualAlloc(NULL, number_of_bytes, MEM_COMMIT, PAGE_READWRITE);
		}

		void _platform_free(size_t number_of_bytes, void* data) {
			VirtualFree(data, number_of_bytes, MEM_DECOMMIT);
		}

		void _platform_console_write(size_t message_size_in_bytes, const char* message, unsigned char color) {
			DWORD num_written_bytes = 0;
			HANDLE console_output_handle = GetStdHandle(STD_OUTPUT_HANDLE );
			SetConsoleTextAttribute(console_output_handle, color);
			WriteConsoleA(console_output_handle, message, message_size_in_bytes, &num_written_bytes, NULL);
			SetConsoleTextAttribute(console_output_handle, 0);
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

		void _platform_free(size_t number_of_bytes, void* data) {
			free(data);
		}
		
		// TODO(Jovanni): Fix this to use linux's platfomr specific std console out
		// Date: April 13, 2024
		void _platform_console_write(size_t message_size_in_bytes, const char* message, unsigned char color) {
			const char* color_strings[] = {COLOR_RESET, BLU, GRN, RED, MAG, WHT, BLKB, GRNB, REDB};
			printf("\033[%sm%s\033[0m", color_strings[translate_color(color)], message);
		}
	#elif PLATFORM_MACOS
	#endif
#endif
