#pragma once

#include "../../Core/Basic/ckit_types.h"
//========================== Begin Types ==========================
typedef struct CKIT_Color {
	u8 r;
	u8 g;
	u8 b;
	u8 a;
} CKIT_Color;

#if defined(PLATFORM_WINDOWS)
	#include <windows.h>

	typedef struct CKIT_Bitmap {
		BITMAPINFO info;
		u16 height;
		u16 width;
		u16 bytes_per_pixel;
		// u32 bitmap_memory_size = width * height * bytes_per_pixel
		u8* memory;
	} CKIT_Bitmap;

	typedef struct CKIT_Window {
		HINSTANCE instance_handle;
		HWND handle;
		HDC	hdc;
		u16 height;
		u16 width;
		const char* name;
		CKIT_Bitmap* bitmap;
	} CKIT_Window;
#elif defined(PLATFORM_LINUX)
	// https://www.youtube.com/watch?v=d2E7ryHCK08
	// https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html
	// https://www.youtube.com/watch?v=u2F_Lif4KGA&list=PLyxjkYF62ii8ZgaRBPlj9nVIoOeIaszw-
	// https://www.youtube.com/watch?v=qZmJwk2xrJ0
	#include <x11/xlib.h>

	typedef struct CKIT_Bitmap {
		u16 height;
		u16 width;
		u16 bytes_per_pixel;
		XImage* memory;
		GC x11_gc;
	} CKIT_Bitmap;

	typedef struct CKIT_Window {
		Display* x11_display;
		Window* x11_window;
		CKIT_Bitmap* bitmap;
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
	void ckit_window_draw_quad(CKIT_Window* window, s32 start_x, s32 start_y, u32 width, u32 height, CKIT_Color color);
	void ckit_window_draw_bitmap(CKIT_Window* window);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_window_free(window) window = MACRO_ckit_window_free(window);
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
#if defined(CKIT_IMPL)
	#include "../../Core/Basic/ckit_memory.h"
	#include "../../Core/Basic/ckit_logger.h"
	#include "../../Core/Basic/ckit_os.h"
	#include "../../Core/Basic/ckit_platform_function_bindings.h"

	u32 ckit_color_to_u32(CKIT_Color color) {
		const u32 red = ((color.r) << 16);
		const u32 green = ((color.g) << 8);
		const u32 blue = ((color.b) << 0);
						
		const u32 rgb = red|green|blue;

		return rgb;
	}

	#if defined(PLATFORM_WINDOWS)
		typedef struct CKIT_WindowEntry {
			HWND WINAPI_handle;
			CKIT_Window* ckit_window;
		} CKIT_WindowEntry;

		internal HICON icon_handle = NULLPTR;
		internal HCURSOR cursor_handle = NULLPTR;
		internal Boolean interacting_with_left_menu = FALSE;
		internal CKIT_WindowEntry* registered_windows = NULLPTR;

		internal CKIT_Window* find_ckit_window_by_handle(HWND handle) {
			if (!registered_windows) {
				return NULLPTR;
			}

			for (int i = 0; i < ckit_vector_count(registered_windows); i++) {
				if (registered_windows[i].WINAPI_handle == handle) {
					return registered_windows[i].ckit_window;
				}
			}

			ckit_assert(FALSE); // SHOULD NEVER GET HERE!
			return NULLPTR;
		}

		internal void ckit_window_resize(CKIT_Window* window) {
			if (!window) {
				return;
			}

			RECT windowRect;
			ckit_win32_GetWindowRect(window->handle, &windowRect);
			window->width = windowRect.right - windowRect.left;
			window->height = windowRect.bottom - windowRect.top;

			RECT client_rect;
			ckit_win32_GetClientRect(window->handle, &client_rect);
			window->bitmap->width = client_rect.right - client_rect.left;
			window->bitmap->height = client_rect.bottom - client_rect.top;

			u32 bits_per_pixel = 32;
			u32 bytes_per_pixel = bits_per_pixel / 8;
			window->bitmap->bytes_per_pixel = bytes_per_pixel;

			BITMAPINFO bitmap_info;
			bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        	bitmap_info.bmiHeader.biWidth = window->bitmap->width;
        	bitmap_info.bmiHeader.biHeight = -window->bitmap->height;
        	bitmap_info.bmiHeader.biPlanes = 1;
        	bitmap_info.bmiHeader.biBitCount = bits_per_pixel;
        	bitmap_info.bmiHeader.biCompression = BI_RGB;
        	bitmap_info.bmiHeader.biSizeImage = 0;
        	bitmap_info.bmiHeader.biXPelsPerMeter = 0;
        	bitmap_info.bmiHeader.biYPelsPerMeter = 0;
        	bitmap_info.bmiHeader.biClrUsed = 0;
        	bitmap_info.bmiHeader.biClrImportant = 0;

			window->bitmap->info = bitmap_info;

			size_t memory_size = window->bitmap->bytes_per_pixel * window->bitmap->width * window->bitmap->height;
			if (window->bitmap->memory && (memory_size != 0)) {
				ckit_free(window->bitmap->memory);
			}

			if (memory_size != 0) {
    			window->bitmap->memory = ckit_alloc(memory_size);
			}
		}

		void ckit_window_draw_bitmap(CKIT_Window* window) {
			if (window->hdc) {
				ckit_win32_ReleaseDC(window->handle, window->hdc);
			}

			// Date: August 17, 2024
			// NOTE(Jovanni): I'm not sure at all if I have to get a new dc each frame?
			window->hdc = ckit_win32_GetDC(window->handle);
			ckit_win32_StretchDIBits(window->hdc, 
						  0, 0, window->bitmap->width, window->bitmap->height, 
			 			  0, 0, window->bitmap->width, window->bitmap->height,
						  window->bitmap->memory, &window->bitmap->info, DIB_RGB_COLORS, SRCCOPY);
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

			size_t start_index = left + (VIEWPORT_WIDTH * bottom);
			u32* dest = &((u32*)window->bitmap->memory)[start_index];

			for (u32 y = 0; y < true_quad_height; y++) {
				for (u32 x = 0; x < true_quad_width; x++) {
					size_t final_pixel_index = x + (y * VIEWPORT_WIDTH);
					dest[final_pixel_index] = ckit_color_to_u32(color);
				}
			}
		}
		
		LRESULT CALLBACK custom_window_procedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
			LRESULT result = 0;
			switch (message) {
				case WM_CREATE: {
					LOG_SUCCESS("Window Created!\n");
				} break;

				case WM_SIZE: { // Resize
					RECT client_rect;
					ckit_win32_GetClientRect(handle, &client_rect);
					u32 width = client_rect.right - client_rect.left;
					u32 height = client_rect.bottom - client_rect.top;

					CKIT_Window* window = find_ckit_window_by_handle(handle);
					ckit_window_resize(window); // should only do the area/region that need to be repainted ideally for optimization
				} break;

				case WM_CLOSE: {
					LOG_SUCCESS("Window is Closed!\n");
					ckit_win32_PostQuitMessage(0);
				} break;

				case WM_DESTROY: {
					ckit_win32_PostQuitMessage(0);
				} break;

				case WM_NCLBUTTONDBLCLK: {
					// Handle the double-click on the top-left icon
					if (wParam == HTSYSMENU) {
						interacting_with_left_menu = TRUE;
					}

					return ckit_win32_DefWindowProcA(handle, message, wParam, lParam);
				} break;

				case WM_SYSCOMMAND: {
					// Handle the double-click on the top-left icon
					if ((wParam & 0xFFF0) == SC_MOUSEMENU) {
						// you can full screen and minimize here like normal behaviour
						return 0;
					}
					
					if (((wParam & 0xFFF0) == SC_CLOSE) && interacting_with_left_menu) {
						interacting_with_left_menu = FALSE;
						return 0;
					}

					return ckit_win32_DefWindowProcA(handle, message, wParam, lParam);
				} break;

				/*
				case WM_PAINT: { // Repaint window when its dirty
					PAINTSTRUCT paint;
					HDC hdc = BeginPaint(handle, &paint);
					u32 x = paint.rcPaint.left;
					u32 y = paint.rcPaint.top;

					// Maybe you will need this but for right now i'm saying you don't need this

					RECT ClientRect;
					GetClientRect(handle, &ClientRect);

					win32_draw_bitmap(hdc, &bitmap, &ClientRect, x, y);
					EndPaint(handle, &paint);
				} break;
				*/
				
				default: {
					result = ckit_win32_DefWindowProcA(handle, message, wParam, lParam);
				} break;
			}

			return result;
		}

		void ckit_window_bind_icon(const char* resource_path) {
			ckit_assert(ckit_os_path_exists(resource_path));
			icon_handle = (HICON)ckit_win32_LoadImageA(GetModuleHandle(NULL), resource_path, IMAGE_ICON, 0, 0, LR_LOADFROMFILE|LR_DEFAULTSIZE);
		}

		void ckit_window_bind_cursor(const char* resource_path) {
			ckit_assert(ckit_os_path_exists(resource_path));
			cursor_handle = (HCURSOR)ckit_win32_LoadImageA(GetModuleHandle(NULL), resource_path, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE|LR_DEFAULTSIZE);
		}

		void ckit_window_clear_color(CKIT_Window* window, CKIT_Color color) {
			int stride = window->bitmap->width * window->bitmap->bytes_per_pixel;
			u8* row = window->bitmap->memory;    
			for(u32 y = 0; y < window->bitmap->height; y++)
			{
				u32* pixel = (u32*)row;
				for(u32 x = 0; x < window->bitmap->width; x++)
				{
					*pixel++ = ckit_color_to_u32(color);
				}
				row += stride;
			}
		}

		CKIT_Window* ckit_window_create(u32 width, u32 height, const char* name) {
			CKIT_Window* ret_window = ckit_alloc_custom(sizeof(CKIT_Window), TAG_CKIT_EXPECTED_USER_FREE);
			ret_window->bitmap = ckit_alloc_custom(sizeof(CKIT_Bitmap), TAG_CKIT_EXPECTED_USER_FREE);

			ret_window->instance_handle = GetModuleHandle(NULL);
			ret_window->width = width;
			ret_window->height = height;
			ret_window->name = name;

			WNDCLASSA window_class = {0};
			window_class.style = CS_HREDRAW|CS_VREDRAW;
			window_class.lpfnWndProc = custom_window_procedure;
			window_class.cbClsExtra = 0;
			window_class.cbWndExtra = 0;
			window_class.hInstance = ret_window->instance_handle;
			window_class.hIcon = icon_handle;
			window_class.hCursor = cursor_handle;
			window_class.hbrBackground = NULLPTR;
			window_class.lpszMenuName = NULLPTR;
			window_class.lpszClassName = name;

			ckit_win32_RegisterClassA(&window_class);

			// Date: May 04, 2024
			// TODO(Jovanni): Extended Window Styles (look into them you can do cool stuff)
			// WS_EX_ACCEPTFILES 0x00000010L (The window accepts drag-drop files.)
			DWORD dwStyle = WS_OVERLAPPEDWINDOW|WS_VISIBLE;
			ret_window->handle = ckit_win32_CreateWindowExA(0, name, name, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULLPTR, NULLPTR, ret_window->instance_handle, NULLPTR);
			ret_window->hdc = ckit_win32_GetDC(ret_window->handle);

			ckit_window_resize(ret_window);

			CKIT_WindowEntry window_entry = {0};
			window_entry.WINAPI_handle = ret_window->handle;
			window_entry.ckit_window = ret_window; 

			ckit_vector_push(registered_windows, window_entry);
			return ret_window;
		}

		void* MACRO_ckit_window_free(CKIT_Window* window) {
			for (int i = 0; i < ckit_vector_count(registered_windows); i++) {
				if (window == registered_windows[i].ckit_window) {
 					ckit_vector_remove_at(registered_windows, i);
				}
			}

			if (ckit_vector_count(registered_windows) == 0) {
				ckit_vector_free(registered_windows);
			}

			ckit_free(window->bitmap->memory);
			ckit_free(window->bitmap);
			ckit_free(window);

			return window;
		}

		Boolean ckit_window_should_quit(CKIT_Window* window) {
			MSG msg;
			while (ckit_win32_PeekMessageA(&msg, NULLPTR, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					ckit_win32_ReleaseDC(window->handle, window->hdc);
					return TRUE;
				}

				ckit_win32_TranslateMessage(&msg);
				ckit_win32_DispatchMessageA(&msg);
			}

			// ckit_window_draw_bitmap(window);
			return FALSE;
		}
	#elif defined(PLATFORM_LINUX)
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
	#endif
#endif // CKG_IMPL


/*

The usage code

int main() {

	u32 width = 800;
	u32 height = 600;
	CKIT_Window window = ckit_window_create(width, height, "GameEngine");

	while (!ckit_window_should_quit()) {

	}

}

*/