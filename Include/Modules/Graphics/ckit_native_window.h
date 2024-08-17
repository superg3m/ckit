#pragma once

#include "../../Core/Basic/ckit_types.h"
//========================== Begin Types ==========================
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

#endif
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Window* ckit_window_create(u32 width, u32 height, const char* name);
	void ckit_window_bind_icon(const char* resource_path);
	void ckit_window_bind_cursor(const char* resource_path);
	Boolean ckit_window_should_quit(CKIT_Window* window);
	void ckit_window_clear_color(u8 r, u8 g, u8 b);
	void ckit_window_draw_bitmap(CKIT_Window* window);
	void ckit_window_draw_quad(CKIT_Window* window, u32 start_x, u32 start_y, u32 width, u32 height);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
#if defined(CKIT_IMPL)
	#include "../../Core/Basic/ckit_memory.h"
	#include "../../Core/Basic/ckit_logger.h"
	#include "../../Core/Basic/ckit_os.h"

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
			GetWindowRect(window->handle, &windowRect);
			window->width = windowRect.right - windowRect.left;
			window->height = windowRect.bottom - windowRect.top;

			RECT client_rect;
			GetClientRect(window->handle, &client_rect);
			window->bitmap->width = client_rect.right - client_rect.left;
			window->bitmap->height = client_rect.bottom - client_rect.top;

			u32 bits_per_pixel = 32;
			u32 bytes_per_pixel = bits_per_pixel / 8;
			window->bitmap->bytes_per_pixel = bytes_per_pixel;

			BITMAPINFO bitmap_info;
			bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        	bitmap_info.bmiHeader.biWidth = window->bitmap->width;
        	bitmap_info.bmiHeader.biHeight = window->bitmap->height;
        	bitmap_info.bmiHeader.biPlanes = 1;
        	bitmap_info.bmiHeader.biBitCount = bits_per_pixel;
        	bitmap_info.bmiHeader.biCompression = BI_RGB;
        	bitmap_info.bmiHeader.biSizeImage = 0;
        	bitmap_info.bmiHeader.biXPelsPerMeter = 0;
        	bitmap_info.bmiHeader.biYPelsPerMeter = 0;
        	bitmap_info.bmiHeader.biClrUsed = 0;
        	bitmap_info.bmiHeader.biClrImportant = 0;

			window->bitmap->info = bitmap_info;

			if (window->bitmap->memory) {
				ckit_free(window->bitmap->memory);
			}

			size_t memory_size = window->bitmap->bytes_per_pixel * window->bitmap->width * window->bitmap->height;
    		window->bitmap->memory = ckit_alloc(memory_size);
		}

		void ckit_window_draw_bitmap(CKIT_Window* window) {
			if (window->hdc) {
				ReleaseDC(window->handle, window->hdc);
			}

			// Date: August 17, 2024
			// NOTE(Jovanni): I'm not sure at all if I have to get a new dc each frame?
			window->hdc = GetDC(window->handle);
			StretchDIBits(window->hdc, 
						  0, 0, window->bitmap->width, window->bitmap->height, 
			 			  0, 0, window->bitmap->width, window->bitmap->height,
						  window->bitmap->memory, &window->bitmap->info, DIB_RGB_COLORS, SRCCOPY);
		}

		void ckit_window_draw_quad(CKIT_Window* window, u32 start_x, u32 start_y, u32 width, u32 height) {

			const u32 VIEWPORT_WIDTH = window->bitmap->width;
			const u32 VIEWPORT_HEIGHT = window->bitmap->width;

			u32 left = CLAMP(start_x, 0, VIEWPORT_WIDTH);
			u32 top = CLAMP(start_y, 0, VIEWPORT_HEIGHT);
			u32 right = CLAMP(start_x + width, 0, VIEWPORT_WIDTH);
			u32 bottom = CLAMP(start_y + height, 0, VIEWPORT_HEIGHT);

			u32 true_quad_width = right - left;
			u32 true_quad_height = bottom - top;

			Boolean should_draw = (true_quad_width != 0) && (true_quad_height != 0);
			if (!should_draw) {
				return;
			}

			const u32 stride = window->bitmap->width * window->bitmap->bytes_per_pixel;
			u8* row = window->bitmap->memory + top;
			for (u32 y = top; y < bottom; y++) {
				u32* pixel = (u32*)row;
				for (u32 x = left; x < right; x++) {
					const u32 red = ((0) << 16);
					const u32 green = ((0) << 8);
					const u32 blue = ((0) << 0);
					
					const u32 rgb = red|green|blue;

					*pixel++ = rgb;
				}
				row += stride;
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

					return DefWindowProc(handle, message, wParam, lParam);
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

					return DefWindowProc(handle, message, wParam, lParam);
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
					result = DefWindowProc(handle, message, wParam, lParam);
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

		void ckit_window_clear_color(u8 r, u8 g, u8 b) {

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

			RegisterClassA(&window_class);

			// Date: May 04, 2024
			// TODO(Jovanni): Extended Window Styles (look into them you can do cool stuff)
			// WS_EX_ACCEPTFILES 0x00000010L (The window accepts drag-drop files.)
			DWORD dwStyle = WS_OVERLAPPEDWINDOW|WS_VISIBLE;
			ret_window->handle = CreateWindowA(name, name, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULLPTR, NULLPTR, ret_window->instance_handle, NULLPTR);
			ret_window->hdc = GetDC(ret_window->handle);

			ckit_window_resize(ret_window);

			CKIT_WindowEntry window_entry = {0};
			window_entry.WINAPI_handle = ret_window->handle;
			window_entry.ckit_window = ret_window; 

			ckit_vector_push(registered_windows, window_entry);
			return ret_window;
		}

		Boolean ckit_window_should_quit(CKIT_Window* window) {
			MSG msg;
			while (PeekMessageA(&msg, NULLPTR, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					ReleaseDC(window->handle, window->hdc);
					ckit_free(window->bitmap->memory);
					ckit_free(window->bitmap);
					return TRUE;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// ckit_window_draw_bitmap(window);
			return FALSE;
		}
	#elif defined(PLATFORM_LINUX)

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
