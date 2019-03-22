#pragma once

#include <shlobj.h>

#define Windows_Main 	\
	APIENTRY			\
	WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_text, int nCmdShow)

#define Window_IsCreated(window) (window->hWnd != 0)

#define Window_ReadMessage(_msg, _running, _ptr_window, _is_zooming)				\
	while (PeekMessage(&_msg, _ptr_window->hWnd, 0, 0, PM_REMOVE)) {				\
		if (Mouse_Update(_ptr_window->mouse, _ptr_window, &_msg))	continue;		\
		if (Keyboard_Update(_ptr_window->keyboard, &_msg))          continue;		\
																		\
		switch (_msg.message) {											\
			case WINDOW_CLOSE: {										\
				_msg.wParam = 0;										\
				_running = false;										\
			} break;													\
																		\
			case WINDOW_RESIZE: {										\
				OpenGL_AdjustScaleViewport(_ptr_window, _is_zooming);	\
			} break;													\
																		\
			default: {													\
				TranslateMessage(&_msg);								\
				DispatchMessage(&_msg);									\
			}															\
		}																\
	}

instant bool
Dialog_OpenFile(
	String *s_file_out
) {
	Assert(s_file_out);

	bool result = false;

	static char filename[MAX_PATH + 1] = {};
	OPENFILENAME ofn = {};

    ofn.lStructSize  = sizeof(OPENFILENAME);
    /// If there is a window to center over, put hWND here
    ofn.hwndOwner    = 0;
    ofn.lpstrFilter  = "Any File\0*.*\0";
    ofn.lpstrFile    = filename;
    ofn.nMaxFile     = MAX_PATH;
    ofn.lpstrTitle   = "Open file...!";
    ofn.Flags        = 0x02000000 | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn)) {
		To_StringBuffer(s_file_out, filename);
		result = true;
    }

    return result;
}

instant bool
Dialog_OpenDirectory(
	String *s_directory_out
) {
	Assert(s_directory_out);

	bool result = false;

	BROWSEINFO bi = {};

	bi.lpszTitle = "Choose folder...";
	bi.ulFlags |= BIF_EDITBOX | BIF_NEWDIALOGSTYLE;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl) {
		static char directory[MAX_PATH] = {};

		result = SHGetPathFromIDList(pidl, directory);

		Memory_Free(pidl);

		if (result)
			To_StringBuffer(s_directory_out, directory);
	}

    return result;
}

static const char *class_name = "OpenGL";

struct Mouse;
struct Keyboard;

/// "external" event pass-through
#define WINDOW_CLOSE	WM_USER+0001
#define WINDOW_RESIZE	WM_USER+0002

struct Window {
	const char  *title	 	  = 0;
	HWND   		 hWnd         = 0;
	HDC    		 hDC          = 0;
	HGLRC  		 hRC          = 0;
	float		 x_viewport   = 0;
	float		 y_viewport   = 0;
	s32    		 width        = 0;
	s32    		 height       = 0;
	bool   		 isFullscreen = false;
	bool   		 useVSync     = false;
	Keyboard    *keyboard     = 0;
	Mouse       *mouse        = 0;
	float        scale_x      = 1;
	float        scale_y      = 1;
};

LONG WINAPI WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	switch (uMessage) {
		case WM_ACTIVATE: {
		} break;

		case WM_CLOSE: {
			PostMessage(hWnd, WINDOW_CLOSE, wParam, lParam);
			return 0;
		} break;

		case WM_SIZE: {
			PostMessage(hWnd, WINDOW_RESIZE, wParam, lParam);
			return 0;
		} break;

		case WM_SYSCOMMAND: {
			switch (wParam) {
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
		 } break;
	}

	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

instant void
Window_Destroy(
	Window *window_io
) {
	if (window_io->isFullscreen) {
		ChangeDisplaySettings(0, 0);
	}

	if (Window_IsCreated(window_io)) {
		if (window_io->hDC)
			ReleaseDC(window_io->hWnd, window_io->hDC);

		DestroyWindow(window_io->hWnd);
	}

	if (!UnregisterClass(class_name, GetModuleHandle(0))) {
		LOG_ERROR("UnregisterClass() failed.");
	}

	*window_io = {};
}

instant bool
Window_Create(
	Window *window_out,
	const char *title,
	s32 width,
	s32 height,
	s32 bits = 32,
	Keyboard *keyboard = 0,
	Mouse *mouse = 0
) {
	Assert(window_out);
	Assert(!Window_IsCreated(window_out));

	*window_out = {};

	/// register window class
	/// -------------------------------------
	HINSTANCE hInstance = GetModuleHandle(0);

	WNDCLASS wc 	 = {};
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = (WNDPROC)WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(0, IDI_WINLOGO);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName  = 0;
	wc.lpszClassName = class_name;

	if (!RegisterClass(&wc)) {
		LOG_ERROR("RegisterClass() failed: Cannot register window_out class.");
		return false;
	}

	/// default position for fullscreen
	s32 x = 0;
	s32 y = 0;

	RECT rect_window = {};
	GetClientRect(GetDesktopWindow(), &rect_window);

	/// get desktop window center
	/// -------------------------------------
	x = (rect_window.right  - width)  >> 1;
	y = (rect_window.bottom - height) >> 1;

	rect_window.right  = width;
	rect_window.bottom = height;

	u32 dwExStyle = WS_EX_APPWINDOW;
	u32 dwStyle   = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (window_out->isFullscreen) {
		dwStyle  |= WS_POPUP;
	}
	else {
		dwExStyle |= WS_EX_WINDOWEDGE;
		dwStyle   |= WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&rect_window, dwStyle, false, dwExStyle);

	/// create window
	/// -------------------------------------
	HWND hWnd = CreateWindowEx(	dwExStyle,
								class_name, title,
								dwStyle,
								x, y,
								rect_window.right - rect_window.left,
								rect_window.bottom - rect_window.top,
								0, 0, hInstance, 0);

	if (!hWnd) {
		LOG_ERROR("CreateWindow() failed: Cannot create a window_out.");
		Window_Destroy(window_out);
		return false;
	}

	window_out->hWnd = hWnd;

	/// set pixel format
	/// -------------------------------------
	HDC hDC = GetDC(hWnd);

	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize        = sizeof(pfd);
	pfd.nVersion     = 1;
	pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType   = PFD_TYPE_RGBA;
	pfd.cColorBits   = bits;

	s32 pf = ChoosePixelFormat(hDC, &pfd);

	if (!pf) {
		LOG_ERROR("ChoosePixelFormat() failed: Cannot find a suitable pixel format.");
		Window_Destroy(window_out);
		return false;
	}

	if (!SetPixelFormat(hDC, pf, &pfd)) {
		LOG_ERROR("SetPixelFormat() failed: Cannot set format specified.");
		Window_Destroy(window_out);
		return false;
	}

	if (!DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd)) {
		LOG_ERROR("DescribePixelFormat() failed: " << GetLastError());
		Window_Destroy(window_out);
		return false;
	}

	window_out->hDC = hDC;

	window_out->title  = title;
	window_out->width  = width;
	window_out->height = height;

	if (mouse)
		window_out->mouse = mouse;

	if (keyboard)
		window_out->keyboard = keyboard;

	return true;
}

/// nCmdShow = 10 => Normal window size (not min/max)
instant void
Window_Show(
	Window *window,
	int nCmdShow = 10
) {
	Assert(window);

	ShowWindow(window->hWnd, nCmdShow);

	SetForegroundWindow(window->hWnd);
	SetFocus(window->hWnd);
}

instant void Mouse_Reset(Mouse *mouse);
instant void Keyboard_ResetLastKey(Keyboard *keyboard);
instant void Keyboard_Reset(Keyboard *keyboard, bool full_reset);

instant void
Window_UpdateAndResetInput(
	Window *window
) {
	Assert(window);

	SwapBuffers(window->hDC);

	Mouse_Reset(window->mouse);
	Keyboard_Reset(window->keyboard, false);
}

instant void
Window_AlwaysOnTop(
	Window *window
) {
	Assert(window);

	RECT rctClient;
	GetWindowRect(window->hWnd, &rctClient);

	SetWindowPos(
		window->hWnd,
		HWND_TOPMOST,
		rctClient.left,
		rctClient.top,
		rctClient.right - rctClient.left,
		rctClient.bottom - rctClient.top,
		SWP_SHOWWINDOW
	);
}

/// Has to be used after Window_Show
/// or that function will turn fullscreen off but keeps
/// the missing borders
instant void
Window_ToggleFullscreen(
	Window *window_io
) {
	Assert(window_io);

	if (!window_io->isFullscreen) {
		DWORD dwStyle = (DWORD)GetWindowLong(window_io->hWnd, GWL_STYLE);

		dwStyle = dwStyle | WS_POPUP;
		dwStyle = dwStyle & ~WS_OVERLAPPEDWINDOW;

		SetWindowLong(window_io->hWnd, GWL_STYLE, dwStyle);

		ShowWindow(window_io->hWnd, SW_RESTORE);
		ShowWindow(window_io->hWnd, SW_MAXIMIZE);

		window_io->isFullscreen = true;
	}
	else {
		DWORD dwStyle = (DWORD)GetWindowLong(window_io->hWnd, GWL_STYLE);

		dwStyle = dwStyle & ~WS_POPUP;
		dwStyle = dwStyle |  WS_OVERLAPPEDWINDOW;

		SetWindowLong(window_io->hWnd, GWL_STYLE, dwStyle);

		ShowWindow(window_io->hWnd, SW_RESTORE);

		window_io->isFullscreen = false;
	}
}

instant void
Window_SetSize(
	Window *window_io,
	s32 width,
	s32 height
) {
	Assert(window_io);

	if (window_io->isFullscreen)
		Window_ToggleFullscreen(window_io);

	RECT rect = {};
	DWORD dwStyle   = (DWORD)GetWindowLong(window_io->hWnd, GWL_STYLE);
	DWORD dwExStyle = (DWORD)GetWindowLong(window_io->hWnd, GWL_EXSTYLE);

	bool success = AdjustWindowRectEx(&rect, dwStyle, 0, dwExStyle);

	if (success) {
		width  += (rect.right  - rect.left);
		height += (rect.bottom - rect.top);

		rect = {};
		GetWindowRect(window_io->hWnd, &rect);

		s32 x = rect.left + (rect.right  - rect.left - width)  / 2;
		s32 y = rect.top  + (rect.bottom - rect.top  - height) / 2;

		SetWindowPos(window_io->hWnd, HWND_TOP, x, y, width, height, 0);
	}
}

instant void
Window_SetTitle(
	Window *window_io,
	const char *title
) {
	Assert(window_io);
	Assert(title);

	window_io->title = title;

	SetWindowText(window_io->hWnd, window_io->title);
}

instant bool
Window_UnAdjustRect(
	HWND hWnd,
	RECT *rect_io
) {
	Assert(rect_io);

	RECT t_rect = {};

	DWORD dwStyle   = (DWORD)GetWindowLong(hWnd, GWL_STYLE);
	DWORD dwExStyle = (DWORD)GetWindowLong(hWnd, GWL_EXSTYLE);

	bool success = AdjustWindowRectEx(&t_rect, dwStyle, 0, dwExStyle);

	if (success) {
		rect_io->left   -= t_rect.left;
		rect_io->top    -= t_rect.top;
		rect_io->right  -= t_rect.right;
		rect_io->bottom -= t_rect.bottom;
	}
	return success;
}
