#pragma once

#include "../../Core/Basic/ckit_types.h"
#include "./ckit_graphics_types.h"
#include "./ckit_graphics_shapes.h"
#include "../../Core/Basic/ckit_math.h"

typedef enum CKIT_CursorState {
	DISABLED,
	ENABLED,
	POINTER,
	GRAB,
} CKIT_CursorState;

//========================== Begin Types ==========================
typedef struct CKIT_Window {
	u16 height;
	u16 width;
	const char* name;
	CKIT_Bitmap bitmap;
} CKIT_Window;
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
	#include "../../Core/Basic/ckit_memory.h"
	#include "../../Core/Basic/ckit_logger.h"
	#include "../../Core/Basic/ckit_os.h"

	CKIT_Window* ckit_window_create(u32 width, u32 height, const char* name) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");	
		return NULLPTR;
	}

	void* MACRO_ckit_window_free(CKIT_Window* window) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");	
	}

	void ckit_window_bind_icon(const char* resource_path) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");	
	}

	void ckit_window_bind_cursor(const char* resource_path) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");	
	}

	Boolean ckit_window_should_quit(CKIT_Window* window) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");	
		return FALSE;
	}

	void ckit_window_clear_color(CKIT_Window* window, CKIT_Color color) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");	
	}

	void ckit_window_draw_quad(CKIT_Window* window, CKIT_Rectangle2D rectangle, CKIT_Color color) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");	
	}

	void ckit_window_draw_line(CKIT_Window* window, CKIT_Vector3 p1, CKIT_Vector3 p2) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");	
	}

	void ckit_window_draw_circle(CKIT_Window* window, s32 start_x, s32 start_y, s32 radius, Boolean is_filled, CKIT_Color color) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");	
	}

	void ckit_window_draw_bitmap(CKIT_Window* window, s32 start_x, s32 start_y, u32 scale_factor, CKIT_Bitmap bitmap) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");	
	}

	void ckit_window_swap_buffers(CKIT_Window* window) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");	
	}

	void ckit_window_get_mouse_position(CKIT_Window* window, s32* mouse_x, s32* mouse_y) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");	
	}

	void ckit_window_set_cursor_state(CKIT_Window* window, CKIT_CursorState cursor_state) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET!");	
	}
#endif