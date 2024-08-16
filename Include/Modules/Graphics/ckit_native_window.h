#pragma once

#include "../../Core/Basic/ckit_types.h"
//========================== Begin Types ==========================
#if defined(PLATFORM_WINDOWS)
	#include <windows.h>
	typedef struct CKIT_Window CKIT_Window;
#elif defined(PLATFORM_LINUX)

#endif
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Window* ckit_window_create(u32 width, u32 height, const char* name);
	void ckit_window_set_icon(CKIT_Window* window, u32 icon_width_in_pixels, u32 icon_height_in_pixels, const char* resource_path);
	void ckit_window_set_cursor(CKIT_Window* window, u32 cursor_width_in_pixels, u32 cursor_height_in_pixels, const char* resource_path);
	Boolean ckit_window_should_quit(CKIT_Window* window);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
#if defined(CKIT_IMPL)
	#include "../../Core/Basic/ckit_memory.h"
	
	#if defined(PLATFORM_WINDOWS)
		typedef struct CKIT_Window {
			HINSTANCE instance_handle;
			HWND window_handle;
			HICON icon_handle;
			HCURSOR	cursor_handle;
			HDC	dc_handle;

			u16 width;
			u16 height;
			const char* name;

			void* bitmap_memory;

			MSG messages;
		} CKIT_Window;
		
		internal LRESULT custom_window_procedure(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam) {
			LRESULT result = 0;
			switch (message) {
				/*
				case WM_CREATE: {
					// Initialize the window.  
				} break;

				case WM_PAINT: {
					// Paint the window's client area.  
				} break;

				case WM_SIZE: {
					// Set the size and position of the window.  
				} break;

				case WM_DESTROY: {
					// Clean up window-specific data objects.  
				} break;
				*/

				default: {
					result = DefWindowProc(window_handle, message, wParam, lParam); 
				} break;
			}

			return result;
		}

		void ckit_window_set_icon(CKIT_Window* window, u32 icon_width_in_pixels, u32 icon_height_in_pixels, const char* resource_path) {
			if (!window->icon_handle) {
				// probably suppose to destroy the previous icon resource
			}

			window->icon_handle = (HICON)LoadImageA(window->instance_handle, resource_path, IMAGE_ICON, icon_width_in_pixels, icon_height_in_pixels, LR_DEFAULTCOLOR|LR_DEFAULTSIZE|LR_LOADFROMFILE);
		}

		void ckit_window_set_cursor(CKIT_Window* window, u32 cursor_width_in_pixels, u32 cursor_height_in_pixels, const char* resource_path) {
			if (!window->cursor_handle) {
				// probably suppose to destroy the previous cursor resource
			}

			window->cursor_handle = (HCURSOR)LoadImageA(window->instance_handle, resource_path, IMAGE_CURSOR, cursor_width_in_pixels, cursor_height_in_pixels, LR_DEFAULTCOLOR|LR_DEFAULTSIZE|LR_LOADFROMFILE);
		}

		void ckit_window_update_bitmap(CKIT_Window* window) {
			/*
			typedef struct tagBITMAPINFO {
				BITMAPINFOHEADER bmiHeader;
				RGBQUAD          bmiColors[1];
			} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;

			int StretchDIBits(
				[in] HDC              hdc,
				[in] int              xDest,
				[in] int              yDest,
				[in] int              DestWidth,
				[in] int              DestHeight,
				[in] int              xSrc,
				[in] int              ySrc,
				[in] int              SrcWidth,
				[in] int              SrcHeight,
				[in] const VOID       *lpBits,
				[in] const BITMAPINFO *lpbmi,
				[in] UINT             iUsage,
				[in] DWORD            rop
			);
			*/
		}

		void ckit_window_present_bitmap(CKIT_Window* window) {

		}

		CKIT_Window* ckit_window_create(u32 width, u32 height, const char* name) {
			CKIT_Window* ret_window = ckit_alloc_custom(sizeof(CKIT_Window), TAG_CKIT_EXPECTED_USER_FREE);
			ret_window->instance_handle = GetModuleHandle(NULL);
			ret_window->width = width;
			ret_window->height = height;
			ret_window->name = name;

			WNDCLASSA window_class = {0};
			window_class.style = CS_HREDRAW|CS_VREDRAW;
			window_class.lpfnWndProc = &custom_window_procedure;
			window_class.cbClsExtra = 0;
			window_class.cbWndExtra = 0;
			window_class.hInstance = GetModuleHandle(NULL);
			window_class.hIcon = ret_window->icon_handle;
			window_class.hCursor = ret_window->cursor_handle;
			window_class.hbrBackground = NULLPTR;
			window_class.lpszMenuName = NULLPTR;
			window_class.lpszClassName = name;

			RegisterClassA(&window_class);

			DWORD dwStyle = WS_VISIBLE;
			ret_window->window_handle = CreateWindowA(name, name, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULLPTR, NULLPTR, NULLPTR, NULLPTR);
			ret_window->dc_handle = GetDC(ret_window->window_handle);

			return ret_window;
		}

		Boolean ckit_window_should_quit(CKIT_Window* window) {
			Boolean should_quit = (GetMessage(&window->messages, NULL, 0, 0) <= 0);
			if (should_quit) {
				return TRUE;
			}

			TranslateMessage(&window->messages);
			DispatchMessage(&window->messages);

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
