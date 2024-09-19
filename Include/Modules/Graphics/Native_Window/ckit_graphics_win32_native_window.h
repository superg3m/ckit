#pragma once

#include "../../../Core/Basic/ckit_types.h"
#include "../ckit_graphics_types.h"
#include "../ckit_graphics_shapes.h"
#include "../../../Core/Basic/ckit_math.h"
#include "../Rendering_Backends/ckit_graphics_software_backend.h"

#include <windows.h>

typedef enum CKIT_CursorState {
	DISABLED,
	ENABLED,
	POINTER,
	GRAB,
} CKIT_CursorState;

//========================== Begin Types ==========================
typedef struct CKIT_Window {
	HINSTANCE instance_handle;
	HWND handle;
	HDC	hdc;
	u16 height;
	u16 width;
	const char* name;
	BITMAPINFO bitmap_info;
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
	void ckit_window_draw_quad(CKIT_Window* window, CKIT_Rectangle2D quad, CKIT_Color color);
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
	#include "../../../Core/Basic/ckit_memory.h"
	#include "../../../Core/Basic/ckit_logger.h"
	#include "../../../Core/Basic/ckit_os.h"

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

		for (u32 i = 0; i < ckit_vector_count(registered_windows); i++) {
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

		if (window->hdc) {
			ReleaseDC(window->handle, window->hdc);
			window->hdc = NULLPTR;
		}
		window->hdc = GetDC(window->handle);

		RECT windowRect;
		GetWindowRect(window->handle, &windowRect);
		window->width = (u16)(windowRect.right - windowRect.left);
		window->height = (u16)(windowRect.bottom - windowRect.top);

		RECT client_rect;
		GetClientRect(window->handle, &client_rect);
		window->bitmap.width = (u16)(client_rect.right - client_rect.left);
		window->bitmap.height = (u16)(client_rect.bottom - client_rect.top);

		u32 bits_per_pixel = 32;
		u32 bytes_per_pixel = bits_per_pixel / 8;
		window->bitmap.bytes_per_pixel = bytes_per_pixel;

		BITMAPINFO bitmap_info;
		bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmap_info.bmiHeader.biWidth = window->bitmap.width;
		bitmap_info.bmiHeader.biHeight = -((s32)window->bitmap.height);
		bitmap_info.bmiHeader.biPlanes = 1;
		bitmap_info.bmiHeader.biBitCount = bits_per_pixel;
		bitmap_info.bmiHeader.biCompression = BI_RGB;
		bitmap_info.bmiHeader.biSizeImage = 0;
		bitmap_info.bmiHeader.biXPelsPerMeter = 0;
		bitmap_info.bmiHeader.biYPelsPerMeter = 0;
		bitmap_info.bmiHeader.biClrUsed = 0;
		bitmap_info.bmiHeader.biClrImportant = 0;

		window->bitmap_info = bitmap_info;

		size_t memory_size = window->bitmap.bytes_per_pixel * window->bitmap.width * window->bitmap.height;
		if (window->bitmap.memory && (memory_size != 0)) {
			ckit_free(window->bitmap.memory);
		}

		if (memory_size != 0) {
			window->bitmap.memory = ckit_alloc(memory_size);
		}
	}

	void ckit_window_swap_buffers(CKIT_Window* window) {
		if (window->bitmap.width == 0|| window->bitmap.height == 0) {
			return;
		}

		StretchDIBits(window->hdc, 
						0, 0, window->bitmap.width, window->bitmap.height, 
						0, 0, window->bitmap.width, window->bitmap.height,
						window->bitmap.memory, &window->bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}

	// Date: August 31, 2024
	// TODO(Jovanni): SIMD for optimizations

	// Date: September 10, 2024
	// TODO(Jovanni): Investigate the top left issue
	void ckit_window_draw_quad(CKIT_Window* window, CKIT_Rectangle2D quad, CKIT_Color color) {
		ckit_graphics_software_backend_draw_quad(window->bitmap.memory, window->bitmap.width, window->bitmap.height, quad, color);
	}

	void ckit_window_draw_line(CKIT_Window* window, CKIT_Vector3 p1, CKIT_Vector3 p2) {
		// Brensenhams line algorithm 
		// - https://www.youtube.com/watch?v=bfvmPa9eWew
		// - https://www.youtube.com/watch?v=IDFB5CDpLDE

		// - https://www.youtube.com/watch?v=CceepU1vIKo&t=12s
	}

	// Date: August 31, 2024
	// TODO(Jovanni): SIMD for optimizations
	void ckit_window_draw_bitmap(CKIT_Window* window, s32 start_x, s32 start_y, u32 scale_factor, CKIT_Bitmap bitmap) {
		ckit_graphics_software_backend_draw_bitmap(window->bitmap.memory, window->bitmap.width, window->bitmap.height, start_x, start_y, scale_factor, bitmap);
	}

	// Date: September 09, 2024
	// TODO(Jovanni): Switch to the midpoint circle algo because its just better
	// - https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
	// - https://noobtomaster.com/computer-graphics/circle-drawing-algorithms-midpoint-algorithm/
	// - https://www.thecrazyprogrammer.com/2016/12/bresenhams-midpoint-circle-algorithm-c-c.html
	// - https://www.youtube.com/watch?v=hpiILbMkF9w
	void ckit_window_draw_circle(CKIT_Window* window, s32 start_x, s32 start_y, s32 radius, Boolean is_filled, CKIT_Color color) {
		ckit_graphics_software_backend_draw_circle(window->bitmap.memory, window->bitmap.width, window->bitmap.height, start_x, start_y, radius, is_filled, color);
	}

	LRESULT CALLBACK custom_window_procedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
		LRESULT result = 0;
		switch (message) {
			case WM_CREATE: {
				LOG_SUCCESS("Window Created!\n");
			} break;

			case WM_SIZE: { // Resize
				RECT client_rect;
				GetClientRect(handle, &client_rect);
				u32 width = client_rect.right - client_rect.left;
				u32 height = client_rect.bottom - client_rect.top;

				CKIT_Window* window = find_ckit_window_by_handle(handle);
				ckit_window_resize(window); // should only do the area/region that need to be repainted ideally for optimization
			} break;

			case WM_CLOSE: {
				LOG_SUCCESS("Window is Closed!\n");
				PostQuitMessage(0);
			} break;

			case WM_DESTROY: {
				PostQuitMessage(0);
			} break;

			case WM_NCLBUTTONDBLCLK: {
				// Handle the double-click on the top-left icon
				if (wParam == HTSYSMENU) {
					interacting_with_left_menu = TRUE;
				}

				return DefWindowProcA(handle, message, wParam, lParam);
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

				return DefWindowProcA(handle, message, wParam, lParam);
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
				result = DefWindowProcA(handle, message, wParam, lParam);
			} break;
		}

		return result;
	}

	void ckit_window_bind_icon(const char* resource_path) {
		ckit_assert(ckit_os_path_exists(resource_path));
		icon_handle = (HICON)LoadImageA(GetModuleHandle(NULL), resource_path, IMAGE_ICON, 0, 0, LR_LOADFROMFILE|LR_DEFAULTSIZE);
	}

	void ckit_window_bind_cursor(const char* resource_path) {
		ckit_assert(ckit_os_path_exists(resource_path));
		cursor_handle = (HCURSOR)LoadImageA(GetModuleHandle(NULL), resource_path, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE|LR_DEFAULTSIZE);
	}

	void ckit_window_clear_color(CKIT_Window* window, CKIT_Color color) {
		ckit_graphics_software_backend_clear_color(window->bitmap.memory, window->bitmap.width, window->bitmap.height, color);
	}

	CKIT_Window* ckit_window_create(u32 width, u32 height, const char* name) {
		CKIT_Window* ret_window = ckit_alloc_custom(sizeof(CKIT_Window), TAG_CKIT_EXPECTED_USER_FREE);

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

		RegisterClassA(&window_class);

		// Date: May 04, 2024
		// TODO(Jovanni): Extended Window Styles (look into them you can do cool stuff)
		// WS_EX_ACCEPTFILES 0x00000010L (The window accepts drag-drop files.)
		DWORD dwStyle = WS_OVERLAPPEDWINDOW|WS_VISIBLE;
		ret_window->handle = CreateWindowExA(0, name, name, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULLPTR, NULLPTR, ret_window->instance_handle, NULLPTR);
		ret_window->hdc = GetDC(ret_window->handle);

		ckit_window_resize(ret_window);

		CKIT_WindowEntry window_entry = {0};
		window_entry.WINAPI_handle = ret_window->handle;
		window_entry.ckit_window = ret_window; 

		ckit_vector_push(registered_windows, window_entry);
		return ret_window;
	}

	void* MACRO_ckit_window_free(CKIT_Window* window) {
		for (u32 i = 0; i < ckit_vector_count(registered_windows); i++) {
			if (window == registered_windows[i].ckit_window) {
				ckit_vector_remove_at(registered_windows, i);
			}
		}

		if (ckit_vector_count(registered_windows) == 0) {
			ckit_vector_free(registered_windows);
		}

		ckit_free(window->bitmap.memory);
		ckit_free(window);

		return window;
	}

	Boolean ckit_window_should_quit(CKIT_Window* window) {
		MSG msg;
		while (PeekMessageA(&msg, NULLPTR, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				ReleaseDC(window->handle, window->hdc);
				window->hdc = NULLPTR;
				return TRUE;
			}

			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		return FALSE;
	}

	void ckit_window_get_mouse_position(CKIT_Window* window, s32* mouse_x, s32* mouse_y) {
		POINT point;
		ckit_os_get_mouse_position(mouse_x, mouse_y);
		point.x = *mouse_x;
		point.y = *mouse_y;
		ckit_assert(ScreenToClient(window->handle, &point));
		*mouse_x = point.x;
		*mouse_y = point.y;
	}

	void ckit_window_set_cursor_state(CKIT_Window* window, CKIT_CursorState cursor_state) {
		switch (cursor_state) {
			case ENABLED: {
				ShowCursor(TRUE);
			}

			case DISABLED: {
				ShowCursor(FALSE);
			}
		}
	}
#endif