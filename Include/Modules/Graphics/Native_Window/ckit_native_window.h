#pragma once

#if defined(PLATFORM_WINDOWS)
	#include "./ckit_graphics_win32_native_window.h"
#elif defined(PLATFORM_LINUX)
	#include "./ckit_graphics_linux_native_window.h"
#elif defined(PLATFORM_DARWIN)
	#include "./ckit_graphics_darwin_native_window.h"
#endif