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
	void ckit_window_bind_icon(const char* resource_path);
	void ckit_window_bind_cursor(const char* resource_path);
	Boolean ckit_window_should_quit(CKIT_Window* window);
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
		typedef struct CKIT_Window {
			HINSTANCE instance_handle;
			HWND handle;
			HDC	dc_handle;

			u16 width;
			u16 height;
			const char* name;

			void* bitmap_memory;
		} CKIT_Window;

		internal HICON icon_handle = NULLPTR;
		internal HCURSOR cursor_handle = NULLPTR;
		internal Boolean interacting_with_left_menu = FALSE; 
		
		LRESULT CALLBACK custom_window_procedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
			LRESULT result = 0;
			switch (message) {
				case WM_CREATE: {
					LOG_SUCCESS("Window Created!\n");
				} break;

				case WM_CLOSE: {
					LOG_SUCCESS("Window is Closed!\n");
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
				case WM_SYSCOMMAND:
					// Prevent system menu from appearing
					if (((wParam & 0xFFF0) == SC_MOUSEMENU) || ((wParam & 0xFFF0) == SC_TASKLIST) || ((wParam & 0xFFF0) == SC_KEYMENU)) {
						LOG_SUCCESS("Window menu created (NOPE)!\n");
						return 0;
					}

					return DefWindowProc(handle, message, wParam, lParam);
				break;
				*/

				/*
				case WM_SIZE: { // Resize
					RECT client_rect;
					GetClientRect(handle, &client_rect);
					u32 width = client_rect.right - client_rect.left;
					u32 height = client_rect.bottom - client_rect.top;

					// win32_resize_bitmap(&bitmap, width, height);
				} break;

				case WM_DESTROY: {
					// window_is_running = FALSE;
				} break;

				case WM_PAINT: { // Repaint window when its dirty
				PAINTSTRUCT paint;
				HDC hdc = BeginPaint(handle, &paint);
				u32 x = paint.rcPaint.left;
				u32 y = paint.rcPaint.top;

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
			ret_window->dc_handle = GetDC(ret_window->handle);

			return ret_window;
		}

		Boolean ckit_window_should_quit(CKIT_Window* window) {
			MSG msg;
			while (PeekMessageA(&msg, NULLPTR, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					return TRUE;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

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
