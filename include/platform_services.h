#pragma once

// Supported Platforms
#if (_WIN32)
  #define PLATFORM_WINDOWS
#elif (__linux__)
  #define PLATFORM_LINUX
#elif (__APPLE__)
  #define PLATFORM_MACOS
#endif


// Supported Platform Operations
void* _platform_allocate(size_t  number_of_bytes);
void _platform_free(size_t number_of_bytes, void** data);
void _platform_console_write(size_t message_size_in_bytes, const char* message, unsigned char color);
void _platform_console_init();
void _platform_console_shutdown();
// void _platform_open_window();
// void _platform_open_file();
// void _platform_close_file();
// void _platform_sleep();
