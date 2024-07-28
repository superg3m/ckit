#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

//========================== Begin Types ==========================
typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u8 Boolean;

//=========================== End Types ===========================

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define FALSE 0
#define TRUE 1
#define NULLPTR 0

#define stringify(entry) #entry
#define glue(a, b) a##b

#define KiloBytes(value) ((u64)(value) * 1024L)
#define MegaBytes(value) ((u64)KiloBytes(value) * 1024L)
#define GigaBytes(value) ((u64)MegaBytes(value) * 1024L)

#define MIN(value, min_value) (value > min_value ? value : min_value)
#define MAX(value, max_value) (value < max_value ? value : max_value)
#define CLAMP(value, min_value, max_value) (MAX(MIN(value, min_value), min_value))

#define local_persist static
#define internal static

// Date: July 12, 2024
// TODO(Jovanni): Test this to make sure its actually works but it makes sense to me
#define OFFSET_OF(type, member) ((u64)(&(((type*)0)->member)))

#define FIRST_DIGIT(number) (((int)number % 10) / 10); 

/**
 * @brief This only works on static arrays not buffers
 * 
 */
#define ArrayCount(array) (sizeof(array) / sizeof(array[0]))

#define PLATFORM_MAX_PATH 256
#ifdef _WIN32
    #define PLATFORM_WINDOWS
    #define OS_DELIMITER '\\'
#elif defined(__linux__)
    #define PLATFORM_LINUX
    #define OS_DELIMITER '/'
#elif defined(__APPLE__) && defined(__MACH__)
    #define PLATFORM_MACOS
    #define OS_DELIMITER '/'
#else
    #define PLATFORM_UNKNOWN
    #define OS_DELIMITER '/'
#endif
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++