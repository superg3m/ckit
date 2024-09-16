#pragma once

#include "../../Core/Basic/ckit_types.h"
#include "./ckit_graphics_types.h"
#include "./ckit_graphics_shapes.h"
#include "../../Core/Basic/ckit_math.h"

// #error "WHAT THE HECK?"
// https://www.youtube.com/watch?v=d2E7ryHCK08
// https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html
// https://www.youtube.com/watch?v=u2F_Lif4KGA&list=PLyxjkYF62ii8ZgaRBPlj9nVIoOeIaszw-
// https://www.youtube.com/watch?v=qZmJwk2xrJ0

#include <x11/xlib.h>

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
	Display* x11_display;
	Window* x11_window;
	XImage* memory;
	GC x11_gc;
} CKIT_Window;





	/*
	#include <X11/Xlib.h>
	#include <stdlib.h>

	int main() {
		Display *display = XOpenDisplay(NULL);
		if (!display) return 1;

		int screen = DefaultScreen(display);
		Window root = RootWindow(display, screen);
		Window win = XCreateSimpleWindow(display, root, 0, 0, 800, 600, 1, BlackPixel(display, screen), WhitePixel(display, screen));
		XMapWindow(display, win);

		// Bitmap memory setup
		int width = 800;
		int height = 600;
		XImage *ximage = XCreateImage(display, DefaultVisual(display, screen), DefaultDepth(display, screen),
									ZPixmap, 0, malloc(width * height * 4), width, height, 32, 0);

		// Write to the bitmap
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				XPutPixel(ximage, x, y, (x ^ y) & 0xff);  // Simple pattern
			}
		}

		// Display the image
		GC gc = XCreateGC(display, win, 0, NULL);
		XPutImage(display, win, gc, ximage, 0, 0, 0, 0, width, height);
		XFlush(display);

		// Event loop to keep the window open
		XEvent event;
		while (1) {
			XNextEvent(display, &event);
			if (event.type == Expose) {
				XPutImage(display, win, gc, ximage, 0, 0, 0, 0, width, height);
			}
		}

		XDestroyImage(ximage);
		XFreeGC(display, gc);
		XDestroyWindow(display, win);
		XCloseDisplay(display);
		return 0;
	}
	*/

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
		CKIT_Window* ret_window = ckit_alloc(sizeof(CKIT_Window));

		Display* display = XOpenDisplay(NULL);
		if (!display) {
			return 1;
		}

		int screen = DefaultScreen(display);
		Window root = RootWindow(display, screen);
		Window win = XCreateSimpleWindow(display, root, 0, 0, 800, 600, 1, BlackPixel(display, screen), WhitePixel(display, screen));
		XMapWindow(display, win);

		// Bitmap memory setup
		CKIT_Bitmap* bitmap = sizeof(CKIT_Bitmap);
		bitmap->bytes_per_pixel = 4;
		bitmap->memory = XCreateImage(display, DefaultVisual(display, screen), DefaultDepth(display, screen),
									ZPixmap, 0, ckit_alloc(width * height * bytes_per_pixel), width, height, 32, 0);

		// Write to the bitmap
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				XPutPixel(bitmap->memory, x, y, (x ^ y) & 0xff);  // Simple pattern
			}
		}

		// Display the image
		bitmap->gc = XCreateGC(display, win, 0, NULL);
		XPutImage(display, win, bitmap->gc, bitmap->memory, 0, 0, 0, 0, width, height);
		XFlush(display);

		ret_window->x11_display = display;
		ret_window->x11_window = win;
		ret_window->bitmap = bitmap;
	}

	void* MACRO_ckit_window_free(CKIT_Window* window) {
		//XDestroyImage(window->bitmap->memory);
		XDestroyImage(window->bitmap->memory);
		XFreeGC(window->x11_display, window->x11_gc);
		XDestroyWindow(window->x11_display, window->x11_window);
		XCloseDisplay(window->x11_display);
	}

	void ckit_window_bind_icon(const char* resource_path) {
		
	}

	void ckit_window_bind_cursor(const char* resource_path) {
		
	}

	Boolean ckit_window_should_quit(CKIT_Window* window) {
		XNextEvent(display, &event);
		if (event.type == Expose) {
			XPutImage(window->x11_display, window->x11_window, window->bitmap->x11_gc, window->bitmap->x11_ximage, 0, 0, 0, 0, window->bitmap->x11_width, window->bitmap->x11_height);
		}
	}

	void ckit_window_clear_color(CKIT_Window* window, CKIT_Color color) {
		// Write to the bitmap
		for (int y = 0; y < window->memory->bitmap->height; ++y) {
			for (int x = 0; x < window->memory->bitmap->width; ++x) {
				XPutPixel(bitmap->memory, x, y, ckit_color_to_u32(color));  // might have to endian swap the color
			}
		}
	}

	void ckit_window_draw_quad(CKIT_Window* window, s32 start_x, s32 start_y, u32 width, u32 height, CKIT_Color color) {
		const u32 VIEWPORT_WIDTH = window->bitmap->width;
		const u32 VIEWPORT_HEIGHT = window->bitmap->height;

		u32 left = CLAMP(start_x, 0, VIEWPORT_WIDTH);
		u32 right = CLAMP(start_x + width, 0, VIEWPORT_WIDTH);

		u32 bottom = CLAMP(start_y, 0, VIEWPORT_HEIGHT);
		u32 top = CLAMP(start_y + height, 0, VIEWPORT_HEIGHT);

		u32 true_quad_width = right - left;
		u32 true_quad_height = top - bottom;

		Boolean should_draw = (true_quad_width != 0) && (true_quad_height != 0);
		if (!should_draw) {
			return;
		}

		for (u32 y = 0; y < true_quad_height; y++) {
			for (u32 x = 0; x < true_quad_width; x++) {
				XPutPixel(bitmap->memory, x, y, ckit_color_to_u32(color));
			}
		}
	}

	void ckit_window_draw_bitmap(CKIT_Window* window) {
		
	}

	void ckit_window_swap_buffers(CKIT_Window* window) {
		
	}
#endif