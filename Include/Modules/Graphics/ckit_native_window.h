#pragma once

// Date: September 06, 2024
// TODO(Jovanni): In need to make the drawing from the center not top left

#include "../../Core/Basic/ckit_types.h"
#include "./ckit_graphics_types.h"
#include "./ckit_graphics_shapes.h"
#include "../../Core/Basic/ckit_math.h"
//========================== Begin Types ==========================

#if defined(PLATFORM_WINDOWS)
	#include "./ckit_graphics_win32_native_window.h"
#elif defined(PLATFORM_LINUX)
	#include "./ckit_graphics_linux_native_window.h"
#elif defined(PLATFORM_DARWIN)
	#include "./ckit_graphics_darwin_native_window.h"
#endif
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	/**
	 * @brief Must be freed with ckit_window_free()
	 * 
	 * @param width 
	 * @param height 
	 * @param name 
	 * @return CKIT_Window* 
	 */
	CKIT_Window* ckit_window_create(u32 width, u32 height, const char* name);
	void* MACRO_ckit_window_free(CKIT_Window* window);
	void ckit_window_bind_icon(const char* resource_path);
	void ckit_window_bind_cursor(const char* resource_path);
	Boolean ckit_window_should_quit(CKIT_Window* window);
	void ckit_window_clear_color(CKIT_Window* window, CKIT_Color color);
	void ckit_window_draw_quad(CKIT_Window* window, CKIT_Rectangle2D rectangle, CKIT_Color color);
	void ckit_window_draw_line(CKIT_Window* window, CKIT_Vector3 p1, CKIT_Vector3 p2);
	void ckit_window_draw_circle(CKIT_Window* window, s32 start_x, s32 start_y, s32 radius, Boolean is_filled, CKIT_Color color);
	void ckit_window_draw_bitmap(CKIT_Window* window, s32 start_x, s32 start_y, u32 scale_factor, CKIT_Bitmap bitmap);
	void ckit_window_swap_buffers(CKIT_Window* window);
	void ckit_window_get_mouse_position(CKIT_Window* window, s32* mouse_x, s32* mouse_y);
	void ckit_window_set_cursor_state(CKIT_Window* window, CKIT_CursorState cursor_state);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_window_draw_quad_custom(window, start_x, start_y, width, height, color) ckit_window_draw_quad(window, ckit_rectangle_create(start_x, start_y, width, height), color)
#define ckit_window_free(window) window = MACRO_ckit_window_free(window);

#define ckit_window_free(window) window = MACRO_ckit_window_free(window);
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
#if defined(CKIT_IMPL)
	#if defined(PLATFORM_WINDOWS)
		#include "./ckit_graphics_win32_native_window.h"
	#elif defined(PLATFORM_LINUX)
		#include "./ckit_graphics_linux_native_window.h"
	#elif defined(PLATFORM_APPLE)
		#include "./ckit_graphics_darwin_native_window.h"
	#endif
#endif // CKIT_IMPL
