#pragma once
/*===========================================================
 * File: platform_services.h
 * Date: May 10, 2024
 * Creator: Jovanni Djonaj
===========================================================*/


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

// Supported Platforms
#if (_WIN32)
  #define PLATFORM_WINDOWS
#elif (__linux__)
  #define PLATFORM_LINUX
#elif (__APPLE__)
  #define PLATFORM_MACOS
#endif
//=========================== End Types ===========================





//************************* Begin Functions *************************
// Supported Platform Operations

#ifdef __cplusplus
extern "C" {
#endif
  void* platform_allocate(unsigned long long number_of_bytes);
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