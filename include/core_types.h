#pragma once
/*===========================================================
 * File: core_types.h
 * Date: April 23, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u8 Boolean;

#define TRUE 1
#define FALSE 0

#define stringify(entry) #entry
#define glue(a, b) a##b

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

#define KiloBytes(value) ((u64)(value) * 1024L)
#define MegaBytes(value) ((u64)KiloBytes(value) * 1024L)
#define GigaBytes(value) ((u64)MegaBytes(value) * 1024L)

#define CORE_RELEASE TRUE
#define CORE_DEBUG FALSE

#define global_variable static
#define local_persist static
#define internal static

// Date: May 01, 2024
// TODO(Jovanni): This shouldn't be here it doesn't make sense 
#define MUTABLE_VOID_POINTER(buffer) (void**)(&buffer)

// Date: May 04, 2024
// TODO(Jovanni): This is gonna be a problem no doubt
#define PLATFORM_COMMON_CHARACTER_LIMIT 160