#pragma once

#define WINDOW_CLOSE	 		WM_USER+0001
#define WINDOW_RESIZE	 		WM_USER+0002
#define WINDOW_TRAY_ICON 		WM_USER+0003
#define WINDOW_TRAY_ICON_ID 	WM_USER+0004
#define WINDOW_TRAY_ICON_SHOW	WM_USER+0005
#define WINDOW_TRAY_ICON_HIDE	WM_USER+0006
#define WINDOW_TRAY_ICON_CREATE WM_USER+0007

#define APPLICATION_MAIN 	\
	APIENTRY				\
	WinMain(HINSTANCE _instance, HINSTANCE _prev_instance, LPSTR _cmd_text, int _cmd_show)

#define Window_IsCreated(window) (window->hWnd != 0)

struct Mouse;
struct Keyboard;

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

enum WINDOW_SCALE_TYPE {
	WINDOW_SCALE_EXPAND,
	WINDOW_SCALE_ASPECT_RATIO,
};

static const char *class_name = "OpenGL";

struct Window {
	const char        *title	 	  = 0;
	HWND   		       hWnd           = 0;
	HDC    		       hDC            = 0;
	HGLRC  		       hRC            = 0;
	float		       x_viewport     = 0;
	float		       y_viewport     = 0;
	s32    		       width          = 0;
	s32    		       height         = 0;
	bool   		       is_fullscreen  = false;
	bool   		       use_VSync      = false;
	bool               uses_opengl    = false;
	bool               is_running     = false;
	Keyboard          *keyboard       = 0;
	Mouse             *mouse          = 0;
	float              scale_x        = 1;
	float              scale_y        = 1;
	WINDOW_SCALE_TYPE  scale_type     = WINDOW_SCALE_EXPAND;

	s32                default_width  = 0;
	s32                default_height = 0;

	// f.e. to reset window / widget / aso. event flags during every frame
	Array<MemorySegment> a_segments_reset;

	struct Window_Events {
		bool on_size_changed = false;
	} events;

	struct Icon {
		bool enable                     = true;
		bool minimize_to                = true;
		bool always_visible             = false;
		NOTIFYICONDATA notify_icon_data = {};
	} icon;

	bool hotkey_triggered[KEYBOARD_HOTKEY_ID_COUNT];
};

/// forward declarations
/// ===========================================================================
instant void OpenGL_Init(Window *);
instant void OpenGL_Destroy(Window *);
instant bool OpenGL_AdjustScaleViewport(Window *, WINDOW_SCALE_TYPE);

instant void Window_ResetEvents(Window *);

instant bool Keyboard_Update(Keyboard *, MSG *);
instant void Keyboard_Reset(Keyboard *, bool);
instant void Keyboard_ResetLastKey(Keyboard *);

instant bool Mouse_Update(Mouse *, Window *, MSG *);
instant void Mouse_Reset(Mouse *);

LONG WINAPI WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	switch (uMessage) {
		case WM_CREATE: {
			PostMessage(hWnd, WINDOW_TRAY_ICON_CREATE, wParam, lParam);
			return 0;
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
				case SC_MONITORPOWER: {
					return 0;
				} break;

				case SC_MINIMIZE: {
					PostMessage(hWnd, WINDOW_TRAY_ICON_SHOW, wParam, lParam);
					return 0;
				} break;
			}
		 } break;

		 case WINDOW_TRAY_ICON: {
		 	switch (wParam) {
		 		case WINDOW_TRAY_ICON_ID: {
		 			if (   lParam == WM_LBUTTONUP
						OR lParam == WM_MBUTTONUP
						OR lParam == WM_RBUTTONUP
					) {
		 				PostMessage(hWnd, WINDOW_TRAY_ICON_HIDE, wParam, lParam);
						return 0;
		 			}

		 		} break;
		 	}
		 } break;
	}

	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

instant void
Window_Destroy(
	Window *window_io
) {
	if (window_io->uses_opengl)
		OpenGL_Destroy(window_io);

	if (window_io->icon.notify_icon_data.hWnd) {
		Shell_NotifyIcon(	NIM_DELETE,
							&window_io->icon.notify_icon_data);
	}

	if (window_io->is_fullscreen) {
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

instant void
Window_ToCenterPosition(
	Window *window
) {
	Assert(window);

	RECT rect_desktop;
	GetWindowRect(GetDesktopWindow(), &rect_desktop);

	/// includes window style size
	RECT rect_window;
	GetWindowRect(window->hWnd, &rect_window);

	s32 width  = rect_window.right  - rect_window.left;
	s32 height = rect_window.bottom - rect_window.top;

	s32 x = ((rect_desktop.right - rect_desktop.left) - width)  / 2 + rect_desktop.left;
	s32 y = ((rect_desktop.bottom - rect_desktop.top) - height) / 2 + rect_desktop.top;

	MoveWindow(window->hWnd, x, y, width, height, false);
}

/// nCmdShow = 10 => Normal window size (not min/max)
/// returns true, if tray icon changed
instant bool
Window_Show(
	Window *window,
	int nCmdShow = 10
) {
	Assert(window);

	ShowWindow(window->hWnd, nCmdShow);

	SetForegroundWindow(window->hWnd);
	SetFocus(window->hWnd);

	if (!window->icon.enable)                 return false;
	if (!window->icon.notify_icon_data.hWnd)  return false;
	if ( window->icon.always_visible)         return false;
	if (!window->icon.minimize_to)            return false;

	Shell_NotifyIcon(	NIM_DELETE,
						&window->icon.notify_icon_data);

	return true;
}

instant Window
Window_Create(
	const char *title,
	s32 width,
	s32 height,
	bool use_opengl,
	bool show,
	Keyboard *keyboard_opt,
	Mouse *mouse_opt,
	s32 bits = 32
) {
	Window window = {};

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
		return window;
	}

	RECT rect_window = {};
	GetClientRect(GetDesktopWindow(), &rect_window);

	rect_window.right  = width;
	rect_window.bottom = height;

	u32 dwExStyle = WS_EX_APPWINDOW;
	u32 dwStyle   = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	dwExStyle |= WS_EX_WINDOWEDGE;
	dwStyle   |= WS_OVERLAPPEDWINDOW;

	AdjustWindowRectEx(&rect_window, dwStyle, false, dwExStyle);

	/// create window
	/// -------------------------------------
	HWND hWnd = CreateWindowEx(	dwExStyle,
								class_name, title,
								dwStyle,
								CW_USEDEFAULT, CW_USEDEFAULT,
								rect_window.right - rect_window.left,
								rect_window.bottom - rect_window.top,
								0, 0, hInstance, 0);

	if (!hWnd) {
		LOG_ERROR("CreateWindow() failed: Cannot create a window_out.");
		Window_Destroy(&window);
		return window;
	}

	window.hWnd = hWnd;

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
		Window_Destroy(&window);
		return window;
	}

	if (!SetPixelFormat(hDC, pf, &pfd)) {
		LOG_ERROR("SetPixelFormat() failed: Cannot set format specified.");
		Window_Destroy(&window);
		return window;
	}

	if (!DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd)) {
		LOG_ERROR("DescribePixelFormat() failed: " << GetLastError());
		Window_Destroy(&window);
		return window;
	}

	window.hDC = hDC;

	window.title  = title;
	window.width  = width;
	window.height = height;

	if (mouse_opt)
		window.mouse = mouse_opt;

	if (keyboard_opt)
		window.keyboard = keyboard_opt;

	if (File_Exists(S("tray.ico"))) {
		NOTIFYICONDATA *nid   = &window.icon.notify_icon_data;

		nid->cbSize           = sizeof(NOTIFYICONDATA);
		nid->hWnd             = window.hWnd;
		nid->uID              = WINDOW_TRAY_ICON_ID;
		nid->uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid->uCallbackMessage = WINDOW_TRAY_ICON;
		nid->hIcon            = (HICON)LoadImage(0, TEXT("tray.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

		String_CopyBuffer(nid->szTip, S(window.title), 63);
	}

	Window_ToCenterPosition(&window);

	window.is_running = true;

	/// for scaling factor
	window.default_width  = window.width;
	window.default_height = window.height;

	if (use_opengl)
		OpenGL_Init(&window);

	if (show)
		Window_Show(&window);

	return window;
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

	if (!window_io->is_fullscreen) {
		DWORD dwStyle = (DWORD)GetWindowLong(window_io->hWnd, GWL_STYLE);

		dwStyle = dwStyle | WS_POPUP;
		dwStyle = dwStyle & ~WS_OVERLAPPEDWINDOW;

		SetWindowLong(window_io->hWnd, GWL_STYLE, dwStyle);

		ShowWindow(window_io->hWnd, SW_RESTORE);
		ShowWindow(window_io->hWnd, SW_MAXIMIZE);

		window_io->is_fullscreen = true;
	}
	else {
		DWORD dwStyle = (DWORD)GetWindowLong(window_io->hWnd, GWL_STYLE);

		dwStyle = dwStyle & ~WS_POPUP;
		dwStyle = dwStyle |  WS_OVERLAPPEDWINDOW;

		SetWindowLong(window_io->hWnd, GWL_STYLE, dwStyle);

		ShowWindow(window_io->hWnd, SW_RESTORE);

		window_io->is_fullscreen = false;
	}
}

instant void
Window_SetSize(
	Window *window_io,
	s32 width,
	s32 height
) {
	Assert(window_io);

	if (window_io->is_fullscreen)
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

	NOTIFYICONDATA *nid = &window_io->icon.notify_icon_data;
	String_CopyBuffer(nid->szTip, S(window_io->title), 63);
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

/// return false as decider to trigger DefWindowProc instead
instant bool
Window_Hide(
	Window *window
) {
	if (!window->icon.enable)                 return false;
	if (!window->icon.notify_icon_data.hWnd)  return false;

	Shell_NotifyIcon(	NIM_ADD,
						&window->icon.notify_icon_data);

	if (window->icon.minimize_to) {
		ShowWindow(window->hWnd, SW_HIDE);
		return true;
	}

	return false;
}

instant void
Window_ReadMessage(
	Window *window_io
) {
	Assert(window_io);
	Assert(window_io->hDC);

	MSG msg;

	/// so resetting event data will not be forgotten during development
	AssertMessage(window_io->a_segments_reset.count > 0,
				  "Missing segment reset data for (at least) window events.");

	MemorySegment_Reset(&window_io->a_segments_reset);

	Mouse_Reset(window_io->mouse);
	Keyboard_Reset(window_io->keyboard, false);

	/// vsync does not seem to work, when this does not execute,
	/// so the cpu ends up doing alot more work, because of the
	/// increased speed in the loop cicle, if this would be disabled
	/// for a hidden window
	SwapBuffers(window_io->hDC);

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		Memory_Set(window_io->hotkey_triggered, false, KEYBOARD_HOTKEY_ID_COUNT);

		if (Mouse_Update(window_io->mouse, window_io, &msg))  continue;
		if (Keyboard_Update(window_io->keyboard, &msg))       continue;

		switch (msg.message) {
			case WM_HOTKEY: {
				window_io->hotkey_triggered[msg.wParam] = true;
			} break;

			case WINDOW_CLOSE: {
				msg.wParam = 0;
				window_io->is_running = false;
			} break;

			case WINDOW_RESIZE: {
				OpenGL_AdjustScaleViewport(window_io, window_io->scale_type);
			} break;

			case WINDOW_TRAY_ICON_CREATE: {
				if (    window_io->icon.enable
					AND window_io->icon.notify_icon_data.hWnd
				) {
					/// if icon is available, enabled and app is not
					/// supposed to mimimize to tray, the tray icon
					/// behaves as if it would always be visible
					if (    window_io->icon.always_visible
						OR !window_io->icon.minimize_to
					) {
						Shell_NotifyIcon(	NIM_ADD,
											&window_io->icon.notify_icon_data);
					}
				}
			} break;

			case WINDOW_TRAY_ICON_SHOW: {
				if (!Window_Hide(window_io)) {
					DefWindowProc(	msg.hwnd,
									WM_SYSCOMMAND,
									SC_MINIMIZE,
									msg.lParam);
				}
			} break;

			case WINDOW_TRAY_ICON_HIDE: {
				Window_Show(window_io);
			} break;

			default: {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
}

instant bool
Window_IsVisible(
	Window *window
) {
	Assert(window);

	return IsWindowVisible(window->hWnd);
}

instant void
Window_ResetEvents(
	Window *window
) {
	Assert(window);

	window->events = {};
}

instant bool
Window_IsRunning(
	Window *window
) {
	Assert(window);

	return window->is_running;
}

instant void
Window_Close(
	Window *window
) {
	Assert(window);

	window->is_running = false;
}
