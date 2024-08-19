#pragma once
#include "./ckit_types.h"

#if defined(PLATFORM_WINDOWS)
	#include <windows.h>
	//========================== Begin Types ==========================
	typedef int (WINAPI StretchDIBitsFunc)(HDC, int, int, int, int, int, int, int, int, const VOID *, const BITMAPINFO *, UINT, DWORD);
	typedef BOOL (WINAPI TranslateMessageFunc)(const MSG *);
	typedef LRESULT (WINAPI DispatchMessageAFunc)(const MSG *);
	typedef BOOL (WINAPI PeekMessageAFunc)(LPMSG, HWND, UINT, UINT, UINT);
	typedef LRESULT (WINAPI DefWindowProcAFunc)(HWND, UINT, WPARAM, LPARAM);
	typedef void (WINAPI PostQuitMessageFunc)(int);
	typedef ATOM (WINAPI RegisterClassAFunc)(const WNDCLASSA *);
	typedef HWND (WINAPI CreateWindowExAFunc)(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
	typedef HDC (WINAPI GetDCFunc)(HWND);
	typedef int (WINAPI ReleaseDCFunc)(HWND, HDC);
	typedef BOOL (WINAPI GetClientRectFunc)(HWND, LPRECT);
	typedef BOOL (WINAPI GetWindowRectFunc)(HWND, LPRECT);
	typedef HANDLE (WINAPI LoadImageAFunc)(HINSTANCE, LPCSTR, UINT, int, int, UINT);
	// ----------------------------------------------------------------------------------------------------------------------------
	internal StretchDIBitsFunc* ckit_win32_StretchDIBits = NULL;
	internal TranslateMessageFunc* ckit_win32_TranslateMessage = NULL;
	internal DispatchMessageAFunc* ckit_win32_DispatchMessageA = NULL;
	internal PeekMessageAFunc* ckit_win32_PeekMessageA = NULL;
	internal DefWindowProcAFunc* ckit_win32_DefWindowProcA = NULL;
	internal PostQuitMessageFunc* ckit_win32_PostQuitMessage = NULL;
	internal RegisterClassAFunc* ckit_win32_RegisterClassA = NULL;
	internal CreateWindowExAFunc* ckit_win32_CreateWindowExA = NULL;
	internal GetDCFunc* ckit_win32_GetDC = NULL;
	internal ReleaseDCFunc* ckit_win32_ReleaseDC = NULL;
	internal GetClientRectFunc* ckit_win32_GetClientRect = NULL;
	internal GetWindowRectFunc* ckit_win32_GetWindowRect = NULL;
	internal LoadImageAFunc* ckit_win32_LoadImageA = NULL;
#endif
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	void ckit_platform_dll_init();
	void ckit_platform_dll_free();
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
#if defined(CKIT_IMPL)
	#if defined(PLATFORM_WINDOWS)
		internal HMODULE user32_handle = NULLPTR;
		internal HMODULE GDI32_handle = NULLPTR;

		void ckit_platform_dll_init() {
			user32_handle = LoadLibrary("user32.dll");
			if (user32_handle) {
				ckit_win32_TranslateMessage = (TranslateMessageFunc*)GetProcAddress(user32_handle, "TranslateMessage");
				ckit_win32_DispatchMessageA = (DispatchMessageAFunc*)GetProcAddress(user32_handle, "DispatchMessageA");
				ckit_win32_PeekMessageA = (PeekMessageAFunc*)GetProcAddress(user32_handle, "PeekMessageA");
				ckit_win32_DefWindowProcA = (DefWindowProcAFunc*)GetProcAddress(user32_handle, "DefWindowProcA");
				ckit_win32_PostQuitMessage = (PostQuitMessageFunc*)GetProcAddress(user32_handle, "PostQuitMessage");
				ckit_win32_RegisterClassA = (RegisterClassAFunc*)GetProcAddress(user32_handle, "RegisterClassA");
				ckit_win32_CreateWindowExA = (CreateWindowExAFunc*)GetProcAddress(user32_handle, "CreateWindowExA");
				ckit_win32_GetDC = (GetDCFunc*)GetProcAddress(user32_handle, "GetDC");
				ckit_win32_ReleaseDC = (ReleaseDCFunc*)GetProcAddress(user32_handle, "ReleaseDC");
				ckit_win32_GetClientRect = (GetClientRectFunc*)GetProcAddress(user32_handle, "GetClientRect");
				ckit_win32_GetWindowRect = (GetWindowRectFunc*)GetProcAddress(user32_handle, "GetWindowRect");
				ckit_win32_LoadImageA = (LoadImageAFunc*)GetProcAddress(user32_handle, "LoadImageA");
			}

			GDI32_handle = LoadLibrary("gdi32.dll");
			if (GDI32_handle) {
				ckit_win32_StretchDIBits = (StretchDIBitsFunc*)GetProcAddress(GDI32_handle, "StretchDIBits");
			}
		}

		void ckit_platform_dll_free() {
			FreeLibrary(user32_handle);
			FreeLibrary(GDI32_handle);
		}
	#elif defined(PLATFORM_LINUX)
	#endif
#endif // CKIT_IMPL

