#pragma once

/// Compiler: g++ (6.3.0)
///
/// Linker flags:
///		-lcomdlg32
///		-lopengl32
///		-lgdi32
///
/// Compiler options:
///		-fexceptions
///		-Wno-unknown-pragmas
///		-Wno-unused-function
///		-Wno-unused-but-set-variable
///		-Wno-comment
///		-Wno-unused-variable
///
/// Convention:
/// 	a_ -> Array
///     s_ -> String
///     c_ -> C-String (when String with the same name might also be used)
///    as_ -> Array<String> ...
///     t_ -> temporary local variable (which might overlap in name with
///           a parameter)
///
///     Function names that ends with "Static" have a byRef return parameter
///     which needs manual freeing, in case it is not a static object / variable
///
/// Return types: if a function returns a struct, it's memory needs to
///               be free'd to prevent memory leaks
///
/// Usage: Window Event Handler
/// ===========================================================================
//instant void
//Window_HandleEvents(Window *window) {
//	MSG msg;
//	bool running = true;
//	bool ui_zoom_enabled = false;
//
//	while(running) {
//		msg = {};
//
//		/// Events
//		/// ===================================================================
//		Window_ReadMessage(msg, running, window);
//		Window_AdjustScale(window, ui_zoom_enabled);
//
//		/// Render
//		/// ===================================================================
//		OpenGL_ClearScreen();
//
//		Window_Update(window);
//	}
//}
/// ===========================================================================

/// Example: display a texture
/// ===========================================================================
//instant void
//Window_HandleEvents(
//	Window *window
//) {
//	MSG msg;
//	bool running = true;
//	bool ui_zoom_enabled = true;
//
//	Timer timer_fps;
//	Time_Reset(&timer_fps);
//
//	Timer timer_fps_log;
//	Time_Reset(&timer_fps_log);
//
//	Texture texture;
//
//#if 0
//	String s_file_image;
//	String_Append(&s_file_image, "test/32_bit_(rgba)_bitmap.bmp");
//
//	Image image = Image_LoadBMP32(s_file_image.value, s_file_image.length);
//
//	if (!image.data) {
//		AssertMessage(0, "Image not found or failed to load.");
//	}
//
//	String_Destroy(&s_file_image);
//
//	texture = Texture_Load(image.data, image.width, image.height, GL_ABGR_EXT, true);
//	texture.flip_h = image.flip_h;
//
//	Image_Destroy(&image);
//#else
//	Texture_Reload(&texture, "test/32_bit_(rgba)_bitmap.bmp");
//#endif // 0
//
//	ShaderSet shader_set = ShaderSet_Create(window);
//	ShaderSet_Use(&shader_set, SHADER_PROG_TEXTURE_FULL);
//
//	Vertex vertex = Vertex_Create(&shader_set, &texture);
//	Vertex_AddTexturePosition(&vertex,   0,   0);
//	Vertex_AddTexturePosition(&vertex, 150, 150);
//
//	Vertex_BindAttributes(&shader_set, &vertex);
//
//	while(running) {
//		msg = {};
//
//		/// Events
//		/// ===================================================================
//		Window_ReadMessage(msg, running, window);
//		OpenGL_AdjustScaleViewport(window, ui_zoom_enabled);
//
//		/// Render
//		/// ===================================================================
//		OpenGL_ClearScreen();
//
//		Vertex_Render(&shader_set, &vertex);
//
//		u32 fps = Time_GetFPS(&timer_fps);
//
//		if (Time_HasElapsed(&timer_fps_log, 1000)) {
//			LOG_DEBUG(fps << " fps");
//		}
//
//		Window_Update(window);
//	}
//
//	Texture_Destroy(&texture);
//	Vertex_Destroy(&vertex);
//	ShaderSet_Destroy(&shader_set);
//}
//
//int main() {
//	Window window;
//
//	Window_Create(&window, "Hello, World!", 800, 480);
//	Window_Show(&window);
//
//	OpenGL_Init(&window);
//
//	Window_HandleEvents(&window);
//
//	OpenGL_Destroy(&window);
//	Window_Destroy(&window);
//
//	return 0;
//}
/// ===========================================================================

/// Example: show mouse position
/// ===========================================================================
//instant void
//Window_HandleEvents(
//	Window *window
//) {
//	MSG msg;
//	bool running = true;
//
//	Timer timer_mouse_move;
//	Time_Reset(&timer_mouse_move);
//
//	while(running) {
//		msg = {};
//
//		/// Events
//		/// ===================================================================
//		Window_ReadMessage(msg, running, window);
//
//		/// Render
//		/// ===================================================================
//		OpenGL_ClearScreen();
//
//		if (Time_HasElapsed(&timer_mouse_move, 100)) {
//			Point t_point;
//
//			if (window->mouse)
//				t_point = window->mouse->point;
//
//			LOG_DEBUG("Mouse pos: x = " << t_point.x << " - y: " << t_point.y);
//		}
//
//		Window_Update(window);
//	}
//}
//
//int main() {
//	Mouse mouse;
//	Window window;
//
//	Window_Create(&window, "Hello, World!", 800, 480, 32, 0, &mouse);
//	Window_Show(&window);
//
//	OpenGL_Init(&window);
//
//	Window_HandleEvents(&window);
//
//	OpenGL_Destroy(&window);
//	Window_Destroy(&window);
//
//	return 0;
//}
/// ===========================================================================

/// Example: reading keyboard input
/// ===========================================================================
//instant void
//Window_HandleEvents(Window *window) {
//	MSG msg;
//	bool running = true;
//	bool ui_zoom_enabled = true;
//
//	Keyboard *keyboard = window->keyboard;
//	String s_keyboard;
//
//	while(running) {
//		msg = {};
//
//		/// Events
//		/// ===================================================================
//		Window_ReadMessage(msg, running, window);
//		OpenGL_AdjustScaleViewport(window, ui_zoom_enabled);
//
//		/// Render
//		/// ===================================================================
//		OpenGL_ClearScreen();
//
//		if (keyboard) {
//			if (0) {}
//			else if (keyboard->is_up) {
//				if (keyboard->up[VK_ESCAPE])
//					running = false;
//			}
//			else if (keyboard->is_down) {
//				Keyboard_Insert(keyboard, &s_keyboard);
//				char *c_buffer = String_CreateCBufferCopy(&s_keyboard);
//				LOG_DEBUG(c_buffer);
//				Memory_Free(c_buffer);
//			}
//		}
//
//		Window_Update(window);
//	}
//}
//
//int main() {
//	Keyboard keyboard;
//	Window window;
//
//	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard);
//	Window_Show(&window);
//
//	OpenGL_Init(&window);
//
//	Window_HandleEvents(&window);
//
//	OpenGL_Destroy(&window);
//	Window_Destroy(&window);
//
//	return 0;
//}
/// ===========================================================================

/// Example: read left thumb x/y from xbox 360 (xinput) joypad
/// ===========================================================================
//instant void
//Window_HandleEvents(Window *window) {
//	MSG msg;
//	bool running = true;
//	bool ui_zoom_enabled = true;
//
//	Joypad joypad;
//	Joypad_Init_XInput(&joypad);
//
//	while(running) {
//		msg = {};
//
//		/// Events
//		/// ===================================================================
//		Window_ReadMessage(msg, running, window);
//		OpenGL_AdjustScaleViewport(window, ui_zoom_enabled);
//
//		Joypad_GetInput(&joypad, 0);
//
//		/// Render
//		/// ===================================================================
//		OpenGL_ClearScreen();
//
//		LOG_DEBUG("X-Axis Section: " << Joypad_GetSection(joypad.state.thumb_left_x, 3000, 10));
//		LOG_DEBUG("Y-Axis Section: " << Joypad_GetSection(joypad.state.thumb_left_y, 3000, 10));
//
//		Window_Update(window);
//	}
//}
//
//int main() {
//	Window window;
//
//	Window_Create(&window, "Hello, World!", 800, 480);
//	Window_Show(&window);
//
//	OpenGL_Init(&window);
//
//	Window_HandleEvents(&window);
//
//	OpenGL_Destroy(&window);
//	Window_Destroy(&window);
//
//	return 0;
//}
/// ===========================================================================

/// Example: draw text from a file with ttf in opengl window + word wrap
/// ===========================================================================
//instant void
//Window_HandleEvents(
//	Window *window
//) {
//	MSG msg;
//	bool running = true;
//
//	Timer timer_fps;
//	Time_Reset(&timer_fps);
//
//	Timer timer_fps_log;
//	Time_Reset(&timer_fps_log);
//
//	String s_font;
//	String_Append(&s_font, "test/AutourOne-Regular.ttf");
//
//	Font font = Font_Load(&s_font, 20);
//
//	ShaderSet shader_set = ShaderSet_Create(window);
//
//	Keyboard *keyboard = window->keyboard;
//
//	String s_file;
//	String_Append(&s_file, "test.txt");
//
//	String s_data = File_ReadAll(s_file.value, s_file.length);
//	String_Destroy(&s_file);
//
//	String_Replace(&s_data, "\r\n", "\n");
//
//	Text text = Text_Create(&shader_set, &font, &s_data, {10, 10, window->width - 20, window->height}, TEXT_ALIGN_X_LEFT);
//	ShaderSet_Use(&shader_set, SHADER_PROG_TEXT);
//
//	while(running) {
//		msg = {};
//
//		/// Events
//		/// ===================================================================
//		Window_ReadMessage(msg, running, window);
//		OpenGL_AdjustScaleViewport(window);
//
//		if (keyboard->up[VK_ESCAPE])
//			running = false;
//
//		/// Render
//		/// ===================================================================
//		OpenGL_ClearScreen();
//
//		Text_Render(&text);
//
//		Window_Update(window);
//
//		u32 fps = Time_GetFPS(&timer_fps);
//
//		if (Time_HasElapsed(&timer_fps_log, 1000)) {
//			LOG_DEBUG(fps << " fps");
//		}
//	}
//
//	Text_Destroy(&text);
//	Font_Destroy(&font);
//	ShaderSet_Destroy(&shader_set);
//
//	String_Destroy(&s_data);
//}
//
//int main() {
//	Window window;
//
//	Keyboard keyboard;
//	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard);
//	Window_Show(&window);
//
//	OpenGL_Init(&window);
//	OpenGL_SetVSync(&window, true);
//
//	Window_HandleEvents(&window);
//
//	OpenGL_Destroy(&window);
//	Window_Destroy(&window);
//
//	return 0;
//}
/// ===========================================================================

/// Example: draw text in multiple rectangles
/// ===========================================================================
//instant void
//Window_HandleEvents(
//	Window *window
//) {
//	MSG msg;
//	bool running = true;
//
//	ShaderSet shader_set = ShaderSet_Create(window);
//
//	OpenGL_SetBlending(true);
//
//	Keyboard *keyboard = window->keyboard;
//
//	String s_font;
//	String_Append(&s_font, "test/AutourOne-Regular.ttf");
//	Font font = Font_Load(&s_font, 20);
//
//	Rect rect_box = {10, 20, 300, 200};
//
//	Vertex vertex_rect = Vertex_Create();
//	Vertex_AddRect32(&vertex_rect, rect_box, Color_MakeGrey(1.0f));
//
//	Rect_Resize(&rect_box, -1);
//	Vertex_AddRect32(&vertex_rect, rect_box, Color_MakeGrey(0.0f));
//
//	Rect_Resize(&rect_box, -2);
//	Vertex_AddRect32(&vertex_rect, rect_box, Color_MakeGrey(1.0f));
//
//	Text text_box;
//	{
//		String s_box_data;
//		String_Append(&s_box_data, "Hello");
//
//		text_box.shader_set = &shader_set;
//		text_box.rect       = rect_box;
//		text_box.font       = &font;
//		text_box.color      = Color_MakeGrey(0.0f);
//
//		String_Append(&text_box.s_data, s_box_data.value, s_box_data.length);
//
//		String_Destroy(&s_box_data);
//	}
//
//	while(running) {
//		msg = {};
//
//		/// Events
//		/// ===================================================================
//		Window_ReadMessage(msg, running, window);
//		OpenGL_AdjustScaleViewport(window);
//
//		if (keyboard->up[VK_ESCAPE])
//			running = false;
//
//		/// Render
//		/// ===================================================================
//		OpenGL_ClearScreen();
//
//		ShaderSet_Use(&shader_set, SHADER_PROG_RECT);
//		Rect_Render(&shader_set, &vertex_rect);
//
//		ShaderSet_Use(&shader_set, SHADER_PROG_TEXT);
//		Text_Render(&text_box);
//
//		Window_Update(window);
//	}
//
//	Text_Destroy(&text_box);
//	Vertex_Destroy(&vertex_rect);
//	ShaderSet_Destroy(&shader_set);
//}
//
//int main() {
//	Window window;
//
//	Keyboard keyboard;
//	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard);
//	Window_Show(&window);
//
//	OpenGL_Init(&window);
//	OpenGL_SetVSync(&window, true);
//
//	Window_HandleEvents(&window);
//
//	OpenGL_Destroy(&window);
//	Window_Destroy(&window);
//
//	return 0;
//}
/// ===========================================================================

/// Example: show basic click-able / tab-able widgets
/// ===========================================================================
//instant void
//Window_HandleEvents(
//	Window *window
//) {
//	MSG msg;
//	bool running = true;
//
//	ShaderSet shader_set = ShaderSet_Create(window);
//
//	OpenGL_SetBlending(true);
//
//	Keyboard *keyboard = window->keyboard;
//
//	String s_font;
//	String_Append(&s_font, "test/AutourOne-Regular.ttf");
//	Font font_20 = Font_Load(&s_font, 20);
//
//	Widget widget_label    = Widget_CreateLabel( window, &font_20, {  10,  20, 300, 200}, "Label");
//	Widget widget_click_me = Widget_CreateButton(window, &font_20, { 320,  20, 100,  30}, "click me");
//	Widget widget_exit     = Widget_CreateButton(window, &font_20, { 320,  50, 100,  30}, "Exit");
//
//	Array<Widget *> ap_widgets;
//	Widget_AddRenderTabStop(&ap_widgets, &widget_label);
//	Widget_AddRenderTabStop(&ap_widgets, &widget_click_me);
//	Widget_AddRenderTabStop(&ap_widgets, &widget_exit);
//
//	while(running) {
//		msg = {};
//
//		/// Events
//		/// ===================================================================
//		Window_ReadMessage(msg, running, window);
//		OpenGL_AdjustScaleViewport(window);
//
//		/// hold shift-key to get reverse order
//		Widget_UpdateFocus(&ap_widgets, keyboard->pressing[VK_SHIFT]);
//
//		if (keyboard->up[VK_ESCAPE] OR Widget_OnClick(&widget_exit))
//			running = false;
//
//		if (Widget_OnClick(&widget_click_me)) {
//			std::cout << "clicked" << std::endl;
//		}
//
//		/// Render
//		/// ===================================================================
//		OpenGL_ClearScreen();
//
//		Widget_Render(&shader_set, &ap_widgets);
//
//		Window_Update(window);
//	}
//
//	Widget_Destroy(&ap_widgets);
//
//	ShaderSet_Destroy(&shader_set);
//}
//
//int main() {
//	Window window;
//
//	Keyboard keyboard;
//	Mouse    mouse;
//
//	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard, &mouse);
//	Window_Show(&window);
//
//	OpenGL_Init(&window);
//	OpenGL_SetVSync(&window, true);
//
//	Window_HandleEvents(&window);
//
//	OpenGL_Destroy(&window);
//	Window_Destroy(&window);
//
//	return 0;
//}
/// ===========================================================================

/// Example: create render and input data to a listbox widget
/// ===========================================================================
//instant void
//Window_HandleEvents(
//	Window *window
//) {
//	MSG msg;
//	bool running = true;
//
//	Timer timer_fps;
//	Time_Reset(&timer_fps);
//
//	Timer timer_fps_log;
//	Time_Reset(&timer_fps_log);
//
//	ShaderSet shader_set = ShaderSet_Create(window);
//
//	OpenGL_SetBlending(true);
//
//	Keyboard *keyboard = window->keyboard;
//
//	String s_font;
//	String_Append(&s_font, "test/AutourOne-Regular.ttf");
//	Font font_20 = Font_Load(&s_font, 20);
//
//	Widget widget_listbox  = Widget_CreateListbox(window, &font_20, { 10,  20, 300, 200});
//	Widget widget_click_me = Widget_CreateButton( window, &font_20, {320,  20, 100,  30}, "click me");
//	Widget widget_exit     = Widget_CreateButton( window, &font_20, {320,  50, 100,  30}, "Exit");
//
//	Array<Widget *> ap_widgets;
//	Widget_AddRenderTabStop(&ap_widgets, &widget_listbox);
//	Widget_AddRenderTabStop(&ap_widgets, &widget_click_me);
//	Widget_AddRenderTabStop(&ap_widgets, &widget_exit);
//
//	Widget_AddRow(&widget_listbox, "1");
//	Widget_AddRow(&widget_listbox, "2");
//	Widget_AddRow(&widget_listbox, "3");
//	Widget_AddRow(&widget_listbox, "4");
//	Widget_AddRow(&widget_listbox, "5");
//	Widget_AddRow(&widget_listbox, "6");
//	Widget_AddRow(&widget_listbox, "7");
//	Widget_AddRow(&widget_listbox, "8");
//	Widget_AddRow(&widget_listbox, "9");
//	Widget_AddRow(&widget_listbox, "10");
//	Widget_AddRow(&widget_listbox, "11");
//	Widget_AddRow(&widget_listbox, "12");
//	Widget_AddRow(&widget_listbox, "13");
//	Widget_AddRow(&widget_listbox, "14");
//	Widget_AddRow(&widget_listbox, "15");
//
//	while(running) {
//		msg = {};
//
//		/// Events
//		/// ===================================================================
//		Window_ReadMessage(msg, running, window);
//		OpenGL_AdjustScaleViewport(window);
//
//		/// hold shift-key to get reverse tab order
//		Widget_UpdateInput(&ap_widgets);
//		Widget_UpdateFocus(&ap_widgets, keyboard->pressing[VK_SHIFT]);
//
//		if (keyboard->up[VK_ESCAPE] OR Widget_OnClick(&widget_exit))
//			running = false;
//
//		if (Widget_OnClick(&widget_click_me)) {
//			std::cout << "clicked" << std::endl;
//		}
//
//		if (Widget_OnClick(&widget_listbox)) {
//			String s_row_data;
//			Widget_GetSelectedRow(&widget_listbox, &s_row_data);
//
//			LOG_DEBUG(s_row_data.value);
//		}
//
//		/// Render
//		/// ===================================================================
//		OpenGL_ClearScreen();
//
//		Widget_Render(&shader_set, &ap_widgets);
//
//		Window_Update(window);
//
//		u32 fps = Time_GetFPS(&timer_fps);
//
//		if (Time_HasElapsed(&timer_fps_log, 1000)) {
//			LOG_DEBUG(fps << " fps");
//		}
//	}
//
//	Widget_Destroy(&ap_widgets);
//
//	ShaderSet_Destroy(&shader_set);
//}
//
//int main() {
//	Window window;
//
//	Keyboard keyboard;
//	Mouse    mouse;
//
//	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard, &mouse);
//	Window_Show(&window);
//
//	OpenGL_Init(&window);
//	OpenGL_SetVSync(&window, true);
//
//	Window_HandleEvents(&window);
//
//	OpenGL_Destroy(&window);
//	Window_Destroy(&window);
//
//	return 0;
//}
/// ===========================================================================

/// Example: layout widgets
/// ===========================================================================
//instant void
//Window_HandleEvents(
//	Window *window
//) {
//	MSG msg;
//	bool running = true;
//
//	ShaderSet shader_set = ShaderSet_Create(window);
//
//	OpenGL_SetBlending(true);
//
//	Keyboard *keyboard = window->keyboard;
//
//	String s_font;
//	String_Append(&s_font, "test/AutourOne-Regular.ttf");
//	Font font_20 = Font_Load(&s_font, 20);
//
//	Widget w_button_menu_1   = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, "menu_1");
//	Widget w_button_menu_2   = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, "menu_2");
//	Widget w_button_menu_3   = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, "menu_3");
//	Widget w_button_top_1    = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, "top_1");
//	Widget w_button_top_2    = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, "top_2");
//	Widget w_button_top_3    = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, "top_3");
//	Widget w_button_center_1 = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, "center_1");
//	Widget w_spreader        = Widget_CreateSpreader(window);
//	Widget w_numpic          = Widget_CreateNumberPicker(window, &font_20, {0, 0, 100, 30}, {0, 5, 3, 1});
//
//	w_button_top_2.layout_data.auto_width = false;
//
//	Layout layout;
//	Layout_Create(&layout, {0, 0, window->width, window->height}, true);
//
//	Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT);
//	Layout_Add(&layout, &w_numpic);
//	Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT, -1);
//	Layout_Add(&layout, &w_button_top_1);
//	Layout_Add(&layout, &w_button_top_2);
//	Layout_Add(&layout, &w_button_top_3);
//	Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT,  -1);
//	Layout_Add(&layout, &w_button_menu_1);
//	Layout_Add(&layout, &w_button_menu_2);
//	Layout_Add(&layout, &w_spreader);
//	Layout_Add(&layout, &w_button_menu_3);
//	Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT);
//	Layout_Add(&layout, &w_button_center_1);
//	Layout_Arrange(&layout);
//
//	Array<Widget *> ap_widgets;
//	Widget_AddRenderTabStop(&ap_widgets, &w_button_top_1);
//	Widget_AddRenderTabStop(&ap_widgets, &w_button_top_2);
//	Widget_AddRenderTabStop(&ap_widgets, &w_button_top_3);
//	Widget_AddRenderTabStop(&ap_widgets, &w_button_menu_1);
//	Widget_AddRenderTabStop(&ap_widgets, &w_button_menu_2);
//	Widget_AddRenderTabStop(&ap_widgets, &w_button_menu_3);
//	Widget_AddRenderTabStop(&ap_widgets, &w_button_center_1);
//	Widget_AddRenderTabStop(&ap_widgets, &w_numpic);
//
//	while(running) {
//		msg = {};
//
//		/// Events
//		/// ===================================================================
//		Window_ReadMessage(msg, running, window);
//		OpenGL_AdjustScaleViewport(window);
//
//		/// hold shift-key to get reverse tab order
//		Widget_UpdateInput(&ap_widgets);
//		Widget_UpdateFocus(&ap_widgets, keyboard->pressing[VK_SHIFT]);
//
//		if (keyboard->up[VK_ESCAPE])
//			running = false;
//
//		/// Render
//		/// ===================================================================
//		OpenGL_ClearScreen();
//
//		Widget_Render(&shader_set, &ap_widgets);
//
//		Window_Update(window);
//	}
//
//	Widget_Destroy(&ap_widgets);
//
//	ShaderSet_Destroy(&shader_set);
//}
//
//int main() {
//	Window window;
//
//	Keyboard keyboard;
//	Mouse    mouse;
//
//	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard, &mouse);
//	Window_Show(&window);
//
//	OpenGL_Init(&window);
//	OpenGL_SetVSync(&window, true);
//	Window_HandleEvents(&window);
//
//	OpenGL_Destroy(&window);
//	Window_Destroy(&window);
//
//	return 0;
//}
/// ===========================================================================

#include <iostream>
#include <math.h>
#include <windows.h>
#include <shlobj.h>
#include <GL/gl.h>
#include "SimpleLibrary_OpenGLExt.h"

__attribute__((gnu_inline, always_inline))
__inline__ static void debug_break(void)
{
	__asm__ volatile("int $0x03");
}

#define s8	char
#define s16	short
#define s32	int
#define s64	long

#define u8	unsigned char
#define u16	unsigned short
#define u32	unsigned int
#define u64	unsigned long

#define AND &&
#define OR  ||

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define instant static inline

/// ::: Iterators
/// ===========================================================================
#define FOR(_max, _it)				\
	for(u64 _it = 0;				\
	it < (_max); 					\
	++_it)

#define FOR_START(_start, _max, _it)	\
	for(u64 _it = _start; _it < _max; ++_it)

/// ::: Assert
/// ===========================================================================
#define	Assert(EX) \
	(void)((EX) OR (_Assert(#EX, __FILE__, __LINE__), 0))

#define AssertMessage(EX, INFO) \
	(void)((EX) OR (_AssertMessage(INFO, __FILE__, __LINE__), 0))

instant void
_Assert(
	const char *ex,
	const char *file,
	s32 line
) {
	std::cerr <<
		"Assertion error: \"" << ex << "\" "
		"in: " << file << " "
	    "[Line: " << line << "]" << std::endl;

	debug_break();
}

instant void
_AssertMessage(
	const char *ex,
	const char *file,
	s32 line
) {
	std::cerr <<
		"[Line: " << line <<
		" - " << file << ""
		"]\n    " << ex << "\n"
		;

	debug_break();
}

/// ::: Debug
/// ===========================================================================
#define LOG_DEBUG(text) std::cout << text << std::endl;

/// ::: Utilities
/// ===========================================================================
#define MIN(val_1, val_2) ((val_1 < val_2) ? val_1 : val_2)
#define MAX(val_1, val_2) ((val_1 < val_2) ? val_2 : val_1)

//#define LOBYTE(x) ((x) & 0xFF)
//#define HIBYTE(x) LOBYTE((x) >> 8)

#define GETBYTE(x, bit_start) LOBYTE((x) >> (bit_start))
#define GETBIT(x, bit_start) (((x) >> (bit_start)) & 0x1)

#define IF_USE(pointer) \
	(pointer) AND (*pointer)

#define IF_SET(pointer) \
	if (pointer) (*pointer)

struct Rect {
	float x = 0.0f;
	float y = 0.0f;
	s32 w = 0;
	s32 h = 0;
};

struct RectF {
	float x = 0.0f;
	float y = 0.0f;
	float w = 0.0f;
	float h = 0.0f;
};

struct RectI {
	s32 x = 0;
	s32 y = 0;
	s32 w = 0;
	s32 h = 0;
};

struct Color32 {
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;
};

struct Point {
	float x = 0.0f;
	float y = 0.0f;
};

template<typename K, typename L>
struct Tuple {
	K first;
	L second;
};

bool
operator != (
	Point p1,
	Point p2
) {
	if (p1.x != p2.x OR p1.y != p2.y)
		return true;

	return false;
}

bool
operator < (
	Point p1,
	Point p2
) {
	if (p1.x < p2.x AND p1.y <= p2.y)
		return true;

	if (p1.x <= p2.x AND p1.y < p2.y)
		return true;

	return false;
}

template<typename K, typename L>
bool
operator == (
	Tuple<K, L> &data1,
	Tuple<K, L> &data2
) {
	if (data1.first == data2.first){
		return true;
	}

	return false;
}

template <typename T>
instant void
Swap(
	T *first,
	T *second
) {
	Assert(first);
	Assert(second);

	T temp = *first;
	*first = *second;
	*second = temp;
}

instant void
Rect_GetAspect(
	s32 width_src,
	s32 height_src,
	s32 *width_dst,
	s32 *height_dst,
	float *off_x,
	float *off_y,
	float *ratio_x = 0,
	float *ratio_y = 0,
	bool scale_to_dest = false
) {
	Assert(width_dst);
	Assert(height_dst);

	float aspect_src = (float) width_src /  height_src;
    float aspect_dst = (float)*width_dst / *height_dst;

	if (0) {}
    else if (aspect_dst > aspect_src) {
		float ratio = (aspect_dst / aspect_src);
		float offset = *width_dst / ratio;

		if (off_x)  *off_x += floor((*width_dst - offset) / 2);
		if (ratio_x)  *ratio_x = ratio;
		if (ratio_y)  *ratio_y = 1;

		*width_dst = floor(offset);
    }
    else if (aspect_dst < aspect_src) {
		float ratio = (aspect_src / aspect_dst);
		float offset = *height_dst / ratio;

		if (off_y)  *off_y += floor((*height_dst - offset) / 2);
		if (ratio_x)  *ratio_x = 1;
		if (ratio_y)  *ratio_y = ratio;

		*height_dst = floor(offset);
    }

    if (scale_to_dest) {
		if (ratio_x)  *ratio_x = (float)*width_dst  / width_src;
		if (ratio_y)  *ratio_y = (float)*height_dst / height_src;
    }
}

instant void
Rect_GetAspect(
	Rect *rect_convert_to_dest,
	s32 width_src,
	s32 height_src,
	float *ratio_x = 0,
	float *ratio_y = 0,
	bool scale_to_dest = false
) {
	Assert(rect_convert_to_dest);

	Rect_GetAspect(	 width_src,
					 height_src,
					&rect_convert_to_dest->w,
					&rect_convert_to_dest->h,
					&rect_convert_to_dest->x,
					&rect_convert_to_dest->y,
					ratio_x,
					ratio_y,
					scale_to_dest);
}

instant bool
Rect_IsIntersecting(
	Rect *rect_inner,
	Rect *rect_outer,
	Point pt_inner_offset
) {
	Assert(rect_inner);
	Assert(rect_outer);

	if (rect_inner->x + pt_inner_offset.x + rect_inner->w < rect_outer->x)  return false;
	if (rect_inner->x + pt_inner_offset.x > rect_outer->x + rect_outer->w)  return false;

	if (rect_inner->y + pt_inner_offset.y + rect_inner->h < rect_outer->y)  return false;
	if (rect_inner->y + pt_inner_offset.y > rect_outer->y + rect_outer->h)  return false;

	return true;
}

instant bool
Rect_IsIntersecting(
	Rect *rect_inner,
	Rect *rect_outer
) {
	Assert(rect_inner);
	Assert(rect_outer);

	return Rect_IsIntersecting(rect_inner, rect_outer, {0, 0});
}

instant bool
Rect_IsIntersecting(
	Point *point,
	Rect  *rect
) {
	Assert(point);
	Assert(rect);

	if (point->x < rect->x)  return false;
	if (point->y < rect->y)  return false;

	if (point->x > rect->x + rect->w)  return false;
	if (point->y > rect->y + rect->h)  return false;

	return true;
}

instant bool
Rect_IsVisibleFully(
	Rect *rect_inner,
	Rect *rect_outer
) {
	Assert(rect_inner);
	Assert(rect_outer);


	if (rect_inner->x < rect_outer->x)  return false;
	if (rect_inner->y < rect_outer->y)  return false;

	if (rect_inner->x + rect_inner->w > rect_outer->x + rect_outer->w)  return false;
	if (rect_inner->y + rect_inner->h > rect_outer->y + rect_outer->h)  return false;

	return true;
}

instant void
Rect_Resize(
	Rect *rect,
	s32 pixel_offset
) {
	Assert(rect);

	rect->x -= pixel_offset;
	rect->y -= pixel_offset;
	rect->w += (pixel_offset << 1);
	rect->h += (pixel_offset << 1);
}

instant void
Rect_AddPadding(
	Rect *rect,
	Rect rect_padding
) {
	Assert(rect);

	rect->x += rect_padding.x;
	rect->y += rect_padding.y;
	rect->w -= (rect_padding.x + rect_padding.w);
	rect->h -= (rect_padding.y + rect_padding.h);
}

instant Color32
Color_MakeGrey(
	float value,
	float alpha = 1.0f
) {
	return {value, value, value, alpha};
}

instant void
Clamp(
	s64 *value,
	s64 min,
	s64 max
) {
	Assert(value);

	if (*value < min)  *value = min;
	if (*value > max)  *value = max;
}

instant void
Rect_Clamp(
	Rect *rect,
	Rect  rect_limit
) {
	Assert(rect);

	/// do not show negative space below the last visible
	if (rect->y + rect->h < rect_limit.h)
		rect->y = rect_limit.h - rect->h;

	/// do not show negative space above the first visible
	if (rect->y > 0)
		rect->y = 0;

	/// avoid scrolling, when all content is visible
	if (rect->h <= rect_limit.h)
		rect->y = 0;
}

/// ::: Memory
/// ===========================================================================
#define Memory_Create(type, length) \
		(type *)_Memory_Alloc_Empty(sizeof(type) * length)

#define Memory_Resize(buffer, type, length) \
		(buffer \
			? (type *) _Memory_Resize(buffer, sizeof(type) * length) \
			: (type *) Memory_Create(type, length));

#define MEMORY_SIGNATURE 123456

struct Memory_Header {
	u32 sig;
};

instant void
Memory_GetHeader(
	Memory_Header *header,
	void *mem
) {
	Assert(header);
	Assert(mem);

	mem = (char *)mem - sizeof(Memory_Header);

	*header = (*(Memory_Header *)mem);
}

instant void *
_Memory_Alloc_Empty(
	u64 size
) {
	if (size == 0)  return 0;

	void *mem = calloc(1, size + sizeof(Memory_Header));
	((Memory_Header *)mem)->sig = MEMORY_SIGNATURE;
	mem = (char *)mem + sizeof(Memory_Header);

	return mem;
}

template <typename T>
struct Array;

template <typename T>
instant void *
_Memory_Resize(
	void *mem,
	u64 size
) {
	if (!mem)  return _Memory_Alloc_Empty(size);

	Memory_Header mem_header;
	Memory_GetHeader(&mem_header, mem);

	if (mem_header.sig != MEMORY_SIGNATURE) {
		return _Memory_Alloc_Empty(size);
	}

	mem = (char *)mem - sizeof(Memory_Header);

	///@Info: will NOT keep the same (virtual) memory address!!!
	mem = realloc(mem, size + sizeof(Memory_Header));
	((Memory_Header *)mem)->sig = MEMORY_SIGNATURE;

	mem = (char *)mem + sizeof(Memory_Header);

	return mem;
}

instant void *
_Memory_Resize(
	void *mem,
	u64 size
) {
	if (!mem)  return _Memory_Alloc_Empty(size);

	Memory_Header mem_header;
	Memory_GetHeader(&mem_header, mem);

	if (mem_header.sig != MEMORY_SIGNATURE) {
		return _Memory_Alloc_Empty(size);
	}

	mem = (char *)mem - sizeof(Memory_Header);

	mem = realloc(mem, size + sizeof(Memory_Header));
	((Memory_Header *)mem)->sig = MEMORY_SIGNATURE;

	mem = (char *)mem + sizeof(Memory_Header);

	return mem;
}

instant void
Memory_Free(
	void *data
) {
	if (data == 0)  return;

	Memory_Header mem_header;
	Memory_GetHeader(&mem_header, data);

	if (mem_header.sig == MEMORY_SIGNATURE) {
		void *mem = (char *)data - sizeof(Memory_Header);
		free(mem);

		Assert(mem != data);
	}
	else {
		LOG_DEBUG("Trying to free heap pointer(?).")
	}
}

instant void
Memory_Copy(
	const void *dest,
	const void *src,
	u64 length
) {
	if (!dest OR !src OR !length)  return;
	if (dest == src)            return;

    char *c_dest = (char *)dest;
    char *c_src  = (char *)src;

    if (dest > src) {
		while(length-- > 0)
			c_dest[length] = c_src[length];
    }
    else {
		FOR(length, it) {
			c_dest[it] = c_src[it];
		}
    }
}

instant void
Memory_Set(
	void *dest,
	int data,
	u64 length
) {
	if (dest == 0) return;

	u8 *cDest = (u8*)dest;
	u8  cData = data;

	while (length-- > 0)
		*cDest++ = cData;
}

instant bool
Memory_Compare(
	void *data_1,
	void *data_2,
	u64   length
) {
	FOR(length, it) {
		if (((char *)data_1)[it] != ((char *)data_2)[it])
			return false;
	}

	return true;
}

/// ::: Convert
/// ===========================================================================
instant char *
ToCString(
	s64 value,
	u32 len = 0
) {
    u64 tmpValue   = value;
	u8  digitCount = 0;
	u8  is_signed  = 0;

	/// make space for '-' sign
	if (value < 0) {
		++digitCount;
		is_signed = 1;
	}

	/// calc number of digits
	do {
		value /= 10;
		++digitCount;
	} while (value);

    /// include null-terminate char
	char *buffer = Memory_Create(char, digitCount + 1);

	/// display negative sign and stay in ascii number
	/// range by removing the sign from the value
	value = tmpValue;
	if (value < 0) {
		*buffer = '-';
		value *= -1;
	}

	if (len AND len > digitCount)
		digitCount = len;

	/// convert digits to char array
	do {
		int digit = value % 10;
		buffer[--digitCount] = digit + '0';
		value /= 10;
	} while (digitCount - is_signed);

	if (len)
		buffer[len] = 0;

	return buffer;
}

struct String;
instant char* String_CreateCBufferCopy(String *s_data);


instant s32
ToInt(
	String *s_data
) {
	char *c_data = String_CreateCBufferCopy(s_data);
	s32 result = atoi(c_data);
	Memory_Free(c_data);
	return result;
}

/// ::: Time
/// ===========================================================================
struct Timer {
	u32 lo_timer  = 0;
	u64 hi_timer  = 0;
	u32 counter   = 0;
	u32 fps       = 0;
	u32 fps_worst = 0;
};

instant void
Time_Reset(
	Timer *timer
) {
	if (!timer)
		return;

	timer->lo_timer = GetTickCount();

	LARGE_INTEGER largeCounter;
	QueryPerformanceCounter(&largeCounter);

	timer->hi_timer = largeCounter.QuadPart;
	timer->counter = 0;
}

instant u32
Time_Get() {
	return GetTickCount();
}

instant bool
Time_HasElapsed(
	Timer *timer,
	u32 interval_in_ms,
	bool run_once = false
) {
	Assert(timer);

	if (!timer->lo_timer) {
		Time_Reset(timer);
		return false;
	}

	bool result = true;

	if (Time_Get() - timer->lo_timer < interval_in_ms) {
		result = false;
	}
	else {
		if (!run_once) {
			timer->lo_timer = Time_Get();
		}
	}

	return result;
}

/// milliseconds
instant double
Time_Measure(
	Timer *timer
) {
	if (!timer)
		return 0.f;

	LARGE_INTEGER largeCounter;
	QueryPerformanceCounter(&largeCounter);

	/// init timer
	if (timer->hi_timer == 0) {
		timer->hi_timer = largeCounter.QuadPart;
		return 0.f;
	}

	u64 current_time = largeCounter.QuadPart;

	LARGE_INTEGER largeFreq;
	QueryPerformanceFrequency(&largeFreq);

	double diff = ((double)( (current_time - timer->hi_timer)
							* 1000.0)
							/ largeFreq.QuadPart);

	timer->hi_timer = current_time;

	return diff;
}

/// has to be used every frame or the calculation will be wrong
instant u32
Time_GetFPS(
	Timer *timer,
	bool get_worst = false
) {
	if (!timer)
		return 0;

	u32 diff = Time_Get() - timer->lo_timer;

	if (timer->lo_timer > 0 AND diff < 1000) {
		++timer->counter;
	}
	else {
		timer->fps_worst = MIN(timer->fps_worst, timer->counter);

		if (timer->fps_worst == 0)
			timer->fps_worst = timer->counter;

		timer->fps = timer->counter;
		timer->counter = 1;
		timer->lo_timer = Time_Get();
	}

	return (get_worst ? timer->fps_worst : timer->fps);
}

/// Reach the end (from the start) in the timespan of the duration
/// Returns: step increase
instant float
Time_Move(
	Timer *timer,
	u32 timespan_in_ms,
	float distance
) {
	Assert(timer);

	if (!timespan_in_ms)  return 0.0f;

	double step_size = Time_Measure(timer) / timespan_in_ms;
	return distance * step_size;
}


/// ::: String
/// ===========================================================================
#define String_Split Array_Split

struct String {
	bool  changed = false;
	u64   length  = 0;
	char *value   = 0;
};

instant void
String_Print(
	String *s_data
) {
	Assert(s_data);

	FOR(s_data->length, it) {
		std::cout << s_data->value[it];
	}
}

/// does NOT support UTF-8
instant u64
String_Length(
	const char *c_data
) {
	u64 length = 0;

	if (c_data == 0)
		return length;

	while (*c_data++)
		++length;

	return length;
}

instant void
String_Destroy(
	String *s_data
) {
	Assert(s_data);

	Memory_Free(s_data->value);
	*s_data = {};

	s_data->changed = true;
}

instant void
String_Resize(
	String *s_data,
	s64 length_delta
) {
	Assert(s_data);

	s_data->value = Memory_Resize(s_data->value, char, s_data->length + length_delta);
}

instant bool
String_Append(
	String *s_data,
	const char *c_data,
	u64 length_append = 0
) {
    if (!s_data)
		return false;

    if (length_append == 0)  length_append = String_Length(c_data);
    if (length_append == 0)  return false;

	String_Resize(s_data, length_append);
	Memory_Copy(s_data->value + s_data->length, (char *)c_data, length_append);
	s_data->length += length_append;

	s_data->changed = true;

	return true;
}

instant u64
String_Insert(
	String *s_data,
	u64 index_start,
	const char *c_data,
	u64 c_length = 0
) {
	if (!s_data)  return 0;
	if (!c_data)  return 0;

	if (!c_length)
		c_length = String_Length(c_data);

    s_data->value = Memory_Resize(s_data->value, char, s_data->length + c_length);
	Memory_Copy(s_data->value + index_start + c_length, s_data->value + index_start, s_data->length - index_start);

	FOR_START(index_start, index_start + c_length, it) {
		s_data->value[index_start + (it - index_start)] = *c_data++;
	}

	s_data->length += c_length;

	s_data->changed = true;

	return c_length;
}

instant void
To_String(
	String *s_data,
	const char *c_data,
	u64 c_length = 0
) {
	Assert(s_data);
	Assert(c_data);

	*s_data = {};
	String_Append(s_data, c_data, c_length);
}

/// does NOT add memory for '\0'
/// make sure you add an additional byte
/// in the buffer so you don't truncate the string
instant void
To_CString(
	char *c_buffer,
	u64 c_length,
	String *s_data
) {
	Assert(s_data);

	if (!c_length)  return;

	if (!s_data->length) {
		c_buffer[0] = '\0';
		return;
	}

	/// stop copying memory after '\0'
	if (c_length > s_data->length + 1) {
		c_length = s_data->length + 1;
	}

	/// stop before '\0'
	FOR(c_length - 1, it) {
		c_buffer[it] = s_data->value[it];
	}

	/// set final '\0'
	c_buffer[c_length - 1] = '\0';
}

instant char *
String_CreateCBufferCopy(
	String *s_data
) {
	Assert(s_data);

	char *c_buffer = Memory_Create(char, s_data->length + 1);
	To_CString(c_buffer, s_data->length + 1, s_data);

	return c_buffer;
}

instant bool
String_IsEqual(
	const char *c_text1,
	const char *c_text2,
	u64 length = 0
) {
	bool result = true;

	/// check if both have equally nothing
	if (!c_text1 AND !c_text2)
		return true;

	/// in case one has more than nothing
	if (!c_text1 OR !c_text2)
		return false;

	while(true) {
		/// both have the same length
		if (*c_text1 == 0 AND *c_text2 == 0)
			break;

		/// check everything in between
		/// (before the end was reached)
		if (*c_text1++ != *c_text2++) {
			return false;
		}

		/// abort checking, if
		/// length is specified
		if (length)
			if (--length <= 0) break;
	}

	return result;
}

instant bool
String_IsEqual(
	String     *s_data,
	const char *c_data,
	u64 length = 0
) {
	/// check if both have equally nothing
	if (!s_data AND !c_data)
		return true;

	/// in case one has more than nothing
	if (!s_data OR !c_data)
		return false;

	/// in case one nothing
	if (!s_data->value OR !s_data->length)
		return false;

	u64 len_c = String_Length(c_data);

	if (!length) {
		/// different lengths
		if (s_data->length != len_c)
			return false;
	}

	/// check min length needed to check
	u64 len_min = 0;
	u64 len_max = (len_c < s_data->length)
						? len_c
						: s_data->length;

	if (length > 0 AND length > len_max)  len_max = length;

	while(len_min < len_max) {
		if (s_data->value[len_min] != c_data[len_min])
			return false;

		++len_min;
	}

	return true;
}

instant void
String_Copy(
	String *s_dest,
	const char *c_source,
	u32 length = 0
) {
	Assert(s_dest);
	Assert(c_source);

	/// overwrite protection check
	Assert(!s_dest->value);

	if (length == 0)
		length = String_Length(c_source);

	s_dest->value = Memory_Create(char, length);
	Memory_Copy(s_dest->value, c_source, length);
	s_dest->length = length;

	s_dest->changed = true;
}

instant void
String_Copy(
	char *dest,
	const char *src,
	u32 length = 0
) {
	if (!src)
		return;

	char *result = dest;

	if (!length)
		length = String_Length(src);

	/// for '0'-terminator
	++length;

	while (*src AND --length > 0)
		*dest++ = *src++;

	*dest = '\0';

	dest = result;
}

instant char *
String_CreateCBufferCopy(
	const char *c_data,
	u64 c_length = 0
) {
	Assert(c_data);

	if (!c_length)
		c_length = String_Length(c_data);

	char *c_buffer = Memory_Create(char, c_length + 1);
	String_Copy(c_buffer, c_data, c_length);

	return c_buffer;
}

instant s64
String_IndexOf(
	String *s_data,
	const char *key,
	s64 index_start = 0
) {
	int result = -1;

	if (!s_data OR !s_data->length)
		return result;

	int length_key  = String_Length(key);

	if (length_key == 0)
		return result;

	u64 length_data = s_data->length;

	if (index_start < 0) index_start = 0;

	FOR_START(index_start, length_data, index) {
		if (String_IsEqual(s_data->value + index, key, length_key)) {
			return index;
		}
	}

	return result;
}

instant s64
String_IndexOfRev(
	String *s_data,
	const char *key,
	s64 index_start = -1
) {
	int result = -1;

	if (!s_data OR !s_data->length)
		return result;

	int len_key  = String_Length(key);

	if (len_key == 0)
		return result;

	if (index_start > (s64)s_data->length OR index_start < 0)
		index_start = (s64)s_data->length;

	for(s64 it = index_start; it >= 0; --it) {
		if (String_IsEqual(s_data->value + it, key, len_key)) {
			return it;
		}
	}

	return result;
}

instant bool
String_StartWith(
	String *s_data,
	const char* start_with
) {
	if (!start_with OR !s_data OR !s_data->length)
		return false;

	return String_IsEqual(s_data->value, start_with, String_Length(start_with));
}

instant bool
String_Find(
	String *s_data,
	const char *find,
	s64 *pos_found = 0,
	s64 pos_start = 0,
	bool pos_after_find = false
) {
	Assert(s_data);

	s64 t_pos_found = String_IndexOf(s_data, find, pos_after_find);

	if (t_pos_found < 0) {
		if (pos_found) *pos_found = t_pos_found;
		return false;
	}

	if (pos_after_find)
		t_pos_found += String_Length(find);

	if (pos_found) *pos_found = t_pos_found;

	return true;
}

instant bool
String_FindRev(
	String *s_data,
	const char *find,
	s64 *pos_found = 0,
	s64  pos_start = 0,
	bool pos_after_find = false
) {
	Assert(s_data);

	s64 t_pos_found = String_IndexOfRev(s_data, find);

	if (t_pos_found < 0) {
		if (pos_found) *pos_found = t_pos_found;
		return false;
	}

	if (pos_after_find)
		t_pos_found += String_Length(find);

	if (pos_found) *pos_found = t_pos_found;

	return true;
}

instant void
String_Cut(
	String *s_data,
	u32 length
) {
	if (s_data AND length < s_data->length) {
		s_data->length  = length;
		s_data->changed = true;
	}
}

instant bool
String_EndWith(
	String *s_data,
	const char *endwith,
	u64 length = 0
) {
	if (!length)  length = String_Length(endwith);

	if (length > s_data->length)
		return false;

	String ts_data = *s_data;
	ts_data.value = ts_data.value + (ts_data.length - length);
	ts_data.length = length;

	return String_IsEqual(&ts_data, endwith, length);
}

instant bool
String_EndWith(
	const char *c_data,
	const char *endwith,
	u64 c_length = 0
) {
	if (!c_length)
		c_length = String_Length(c_data);

	String ts_data;
	ts_data.value  = (char *)c_data;
	ts_data.length = c_length;

	u64 length_endwith = String_Length(endwith);

	ts_data.value  = ts_data.value + (ts_data.length - length_endwith);
	ts_data.length = length_endwith;

	return String_IsEqual(&ts_data, endwith, length_endwith);
}

instant void
String_ToLower(
	String *s_data
) {
	if (!s_data)
		return;

	u64 index = 0;
	u64 len_max = s_data->length + 1;

	while(index < len_max) {
		if (s_data->value[index] >= 'A' AND s_data->value[index] <= 'Z') {
			s_data->value[index] = s_data->value[index] + 32;
		}
		++index;
	}

	s_data->changed = true;
}

instant void
String_ToUpper(
	String *s_data
) {
	if (!s_data)
		return;

	u64 index = 0;
	u64 len_max = s_data->length + 1;

	while(index < len_max) {
		if (s_data->value[index] >= 'a' AND s_data->value[index] <= 'z') {
			s_data->value[index] = s_data->value[index] - 32;
		}
		++index;
	}

	s_data->changed = true;
}

instant void
String_Reverse(
	String *s_data
) {
	if (!s_data)
		return;

	u64 length = s_data->length;

	for(u64 it = 0; it < (length >> 1); ++it) {
		char temp = s_data->value[it];
		/// don't reverse the '\0'!
		s_data->value[it] = s_data->value[length - it - 1];
		s_data->value[length - it - 1] = temp;
	}

	s_data->changed = true;
}

instant void
String_TrimLeft(
	String *s_data
) {
	Assert(s_data);

	u64 length = 0;
	u64 len_max = s_data->length;

	/// does not skip '\0'
    while(length <= len_max) {
		if (s_data->value[length] <= 32 AND s_data->value[length] != 127) ++length;
		else												   break;
    }

    Memory_Copy(s_data->value, s_data->value + length, s_data->length - length);
    s_data->length -= length;

    s_data->changed = true;
}

instant void
String_TrimRight(
	String *s_data
) {
	Assert(s_data);

	u64 length = s_data->length;

    while(length > 0) {
		if (s_data->value[length - 1] <= 32 AND s_data->value[length - 1] != 127)
			--length;
		else
			break;
    }

    s_data->length = length;

    s_data->changed = true;
}

instant u64
String_Remove(
	String *s_data,
	u64 index_start,
	u64 index_end
) {
	Assert(s_data);

	if (index_start == index_end)
		return 0;

	if (index_start > index_end)
		Swap(&index_start, &index_end);

	if (index_end > s_data->length)
		index_end = s_data->length;

	u64 length = s_data->length - index_end;
	u64 rm_count = (index_end - index_start);

	Memory_Copy(s_data->value + index_start, s_data->value + index_end, length);
	s_data->length -= rm_count;

	s_data->changed = true;

	return rm_count;
}

instant void
String_Clear(
	String *s_data
) {
	Assert(s_data);

	s_data->length = 0;

	s_data->changed = true;
}

instant void
String_Replace(
	String     *s_data,
	const char *find,
	const char *replace
) {
	Assert(s_data);
	Assert(find);
	Assert(replace);

	s64 pos_found = 0;
	s64 pos_start = 0;
	s64 find_length     = String_Length(find);
	s64 replace_length  = String_Length(replace);

	while(String_Find(s_data, find, &pos_found, pos_start)) {
		String_Remove(s_data, pos_found, pos_found + find_length);
		String_Insert(s_data, pos_found, replace);
		pos_start += pos_found + replace_length;
	}

	s_data->changed = true;
}

instant void
String_Replace(
	String     *s_data,
	const char *find,
	String     *s_replace
) {
	Assert(s_data);
	Assert(find);
	Assert(s_replace);

	char *c_replace = String_CreateCBufferCopy(s_replace);
	String_Replace(s_data, find, c_replace);
	Memory_Free(c_replace);

	s_data->changed = true;
}

instant s64
String_Insert(
	String *s_data,
	u64 index_start,
	const char c_data
) {
	Assert(s_data);

	s64 length = 0;

	if (c_data == '\b') {
		length = 0;
		if (s_data->value[index_start + length - 1] == '\n')  --length;
		if (s_data->value[index_start + length - 1] == '\r')  --length;
		if (!length) --length;

		String_Remove(s_data, index_start + length, index_start);
	}
	else if (c_data == '\r' OR c_data == '\n') {
		length = 1;
		String_Insert(s_data, index_start, "\n", length);
	}
	else {
		length = 1;
		String_Insert(s_data, index_start, &c_data, length);
	}

	s_data->changed = true;

	return length;
}

instant void
String_Cut(
	String *s_data,
	const char *c_start,
	const char *c_end
) {
	Assert(s_data);

	s64 start = 0, end = 0;
	u64 len_end = String_Length(c_end);

	while (true) {
		start = String_IndexOf(s_data, c_start);

		if (start < 0)
			break;

		end = String_IndexOf(s_data, c_end, start);

		if (end > start)
			String_Remove(s_data, start, end + len_end);
		else
			break;
	}
}

instant char *
String_Encode64(
	const char *text
) {
	const char encodingTable[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int length = String_Length(text);
	char *encoded = Memory_Create(char, (length + 2) / 3 * 4 + 1);
	int enc_index = -1;

	for (int index = 0; index < length; index += 3) {
		u32 buffer =  (text[index + 0] << 16)
					| (text[index + 1] <<  8)
					| (text[index + 2]);

		/// 6 bits -> 0-63 * position in buffer & trunkated by 63 (0x3F)
		encoded[++enc_index] = encodingTable[(buffer >> (6 * 3) & 0x3F)];
		encoded[++enc_index] = encodingTable[(buffer >> (6 * 2) & 0x3F)];

		if (text[index + 1] == '\0') {
			encoded[++enc_index] = '=';
			encoded[++enc_index] = '=';
			break;
		}

		encoded[++enc_index] = encodingTable[(buffer >> (6 * 1) & 0x3F)];

		if (text[index + 2] == '\0') {
			encoded[++enc_index] = '=';
			break;
		}

		encoded[++enc_index] = encodingTable[(buffer >> (6 * 0) & 0x3F)];
	}

	return encoded;
}

bool
operator == (
	String &s_data1,
	String &s_data2
) {
	return String_IsEqual(&s_data1, s_data2.value, s_data2.length);
}

bool
operator < (
	String &s_data1,
	String &s_data2
) {
	if (s_data1.length < s_data2.length)  return true;
	if (s_data1.length > s_data2.length)  return false;

	FOR(s_data1.length, it) {
		if (s_data1.value[it] < s_data2.value[it]) return true;
		if (s_data1.value[it] > s_data2.value[it]) return false;
	}

	return false;
}

bool
operator > (
	String &s_data1,
	String &s_data2
) {
	if (s_data1.length > s_data2.length)  return true;
	if (s_data1.length < s_data2.length)  return false;

	FOR(s_data1.length, it) {
		if (s_data1.value[it] > s_data2.value[it]) return true;
		if (s_data1.value[it] < s_data2.value[it]) return false;
	}

	return false;
}

/// ::: Array
/// ===========================================================================
#define ARRAY_IT(_array, _it) \
	((_array).memory)[_it]

#define FOR_ARRAY(_array, _it) 		\
	for(u64 _it = 0;        		\
		_it < (_array).count;       	\
		++_it)

#define FOR_ARRAY_REV(_array, _it)	\
	for(s64 _it = (_array).count - 1;	\
		_it >= 0;						\
		--_it)

#define FOR_ARRAY_START(_array, _start, _it) 	\
	for(u64 _it = (_start);   					\
		_it < (_array).count;       				\
		++_it)

template <typename T>
struct Array {
	T    *memory   = 0;
	u64   size     = 0;
	u64   limit    = 0; /// in bytes
	u64   count    = 0;
	bool  by_reference = false;
};

///@Info: will copy the struct element pointers only
///
///       if the array should take ownership of all data
///       make sure you copy every data that is connected
///       to a pointer (clone) or don't free the
///       passed / connected data
template <typename T>
instant void
Array_Add(
	Array<T> *array,
	T element
) {
	Assert(array);

	constexpr u64 length = 1;

	if (array->size + sizeof(T) * length > array->limit) {
		array->limit += sizeof(T) * length;
		array->memory = (T *)_Memory_Resize(array->memory, array->limit);;
	}

	u64 target = array->size / sizeof(T);

	array->memory[target] = element;
	array->size += sizeof(T) * length;

	++array->count;
}

template <typename T>
instant u64
Array_AddEmpty(
	Array<T> *array,
	T **element_empty
) {
	Assert(array);
	Assert(element_empty);

	T t_element_empty = {};
	Array_Add(array, t_element_empty);
	*element_empty = &ARRAY_IT(*array, array->count - 1);

	return array->count - 1;
}

///@Info: does NOT have ownership and clears/destroys
///       dynamic data inside structs and such.
///       the same thing applies for inserting data
///       into empty array slots (created with addempty)
///       into dynamic pointers inside a struct element
///    -> non-generic overlap functions might exist that
///       do that
template<typename T>
instant void
Array_ClearContainer(
	Array<T> *array
) {
	Assert(array);

	array->size = 0;
	array->count = 0;
}

template<typename T>
instant void
Array_DestroyContainer(
	Array<T> *array
) {
	Assert(array);

	Memory_Free(array->memory);
	*array = {};
}

/// Will add memory slots on top of existing ones and add to that count
template <typename T>
instant void
Array_Reserve(
	Array<T> *array,
	u64 count_delta,
	bool clear_zero = false
) {
	Assert(array);

	u64 old_limit = array->limit;

	if (array->size + sizeof(T) * count_delta > array->limit) {
		array->limit += sizeof(T) * count_delta;
		array->memory = (T *)_Memory_Resize(array->memory, array->limit);
	}

	if (clear_zero) {
		/// only clear new reserved data
		Memory_Set(array->memory + array->count, 0, array->limit - old_limit);
	}
}

template <typename T>
instant bool
Array_Find(
	Array<T> *array,
	T find,
	u64 *index = 0
) {
	Assert(array);

	FOR_ARRAY(*array, it) {
		if (ARRAY_IT(*array, it) == find) {
			if (index) *index = it;
			return true;
		}
	}

	return false;
}

template <typename T>
instant bool
Array_FindOrAddEmpty(
	Array<T> *array,
	T find,
	T **entry
) {
	Assert(array);
	Assert(entry);

	u64 t_index_find;
	bool found_element = Array_Find(array, find, &t_index_find);

	if (found_element) {
		*entry = &ARRAY_IT(*array, t_index_find);
	}
	else {
        Array_AddEmpty(array, entry);

        /// store what you want to find, if it does not exists,
        /// so it does not have to be assigned manually all the time
        **entry = find;
	}

	return found_element;
}

/// Returns T, so dynamic memory can still be free'd
template <typename T>
instant T
Array_Remove(
	Array<T> *array,
	u64 index
) {
	Assert(array);
	Assert(index < array->count);

	T result = ARRAY_IT(*array, index);

	FOR_ARRAY_START(*array, index, it) {
		if (it + 1 >= array->count)
			break;

		ARRAY_IT(*array, it) = ARRAY_IT(*array, it + 1);
	}

	array->count -= 1;
	array->size  -= sizeof(T);

	return result;
}

instant void
Array_Destroy(
	Array<String> *array
) {
	Assert(array);

	if (!array->by_reference) {
		while(array->count) {
			String s_data_it = Array_Remove(array, 0);
			String_Destroy(&s_data_it);
		}
	}

    Array_DestroyContainer(array);
}

enum DELIMITER_TYPE {
	DELIMITER_IGNORE,
	/// Insert delimiter at the beginning of the next token
	DELIMITER_ADD_FRONT,
	/// Insert delimiter at the end if the current token
	DELIMITER_ADD_BACK
};

instant Array<String>
Array_SplitRef(
	String *s_data,
	const char *delimiter,
	DELIMITER_TYPE type = DELIMITER_IGNORE

) {
	Assert(s_data);

	Array<String> as_result;
	as_result.by_reference = true;

	String s_data_it = *s_data;
	u64 len_delim = String_Length(delimiter);

	s64 pos_found;
	while(String_Find(&s_data_it, delimiter, &pos_found)) {
		if (pos_found) {
			String *s_element;
			Array_AddEmpty(&as_result, &s_element);

			s_element->value  = s_data_it.value;
			s_element->length = pos_found;

			if (type == DELIMITER_ADD_FRONT AND as_result.count) {
				s_element->value  -= len_delim;
				s_element->length += len_delim;
			}
			else if (type == DELIMITER_ADD_BACK) {
				s_element->length += len_delim;
			}
		}
		else {
			/// in case of f.e: "\n\n\n" with "\n" as delimiter
			String *s_element;
			Array_AddEmpty(&as_result, &s_element);

			if (type == DELIMITER_ADD_BACK) {
				s_element->value  = s_data_it.value;
				s_element->length = len_delim;
			}
		}

		s_data_it.value  += pos_found + len_delim;
		s_data_it.length -= pos_found + len_delim;
	}

	if (s_data_it.length > 0) {
		String *s_element;
		Array_AddEmpty(&as_result, &s_element);

		if (type == DELIMITER_ADD_FRONT AND as_result.count) {
			s_element->value  -= len_delim;
			s_element->length += len_delim;
		}

		s_element->value  = s_data_it.value;
		s_element->length = s_data_it.length;
	}

	return as_result;
}

/// Will copy string values, so array content has to be free'd
instant Array<String>
Array_Split(
	String *s_data,
	const char *delimiter,
	DELIMITER_TYPE type = DELIMITER_IGNORE

) {
	Assert(s_data);

	Array<String> as_result;

	String s_data_it = *s_data;
	u64 len_delim = String_Length(delimiter);

	s64 pos_found;
	while(String_Find(&s_data_it, delimiter, &pos_found)) {
		if (pos_found) {
			String s_element;

			if (type == DELIMITER_ADD_FRONT AND as_result.count) {
				String_Append(&s_element, delimiter, len_delim);
			}

			String_Append(&s_element, s_data_it.value, pos_found);

			if (type == DELIMITER_ADD_BACK) {
				String_Append(&s_element, delimiter, len_delim);
			}

			Array_Add(&as_result, s_element);
		}
		else {
			/// in case of f.e: "\n\n\n" with "\n" as delimiter
			String *s_element;
			Array_AddEmpty(&as_result, &s_element);

			if (type == DELIMITER_ADD_BACK) {
				String_Append(s_element, delimiter, len_delim);
			}
		}

		s_data_it.value  += pos_found + len_delim;
		s_data_it.length -= pos_found + len_delim;
	}

	if (s_data_it.length > 0) {
		String s_element;

		if (type == DELIMITER_ADD_FRONT AND as_result.count)
			String_Append(&s_element, delimiter, len_delim);

		String_Append(&s_element, s_data_it.value, s_data_it.length);
		Array_Add(&as_result, s_element);
	}

	return as_result;
}

instant void
Array_Clear(
	Array<String> *array
) {
	Assert(array);

	if (!array->by_reference) {
		FOR_ARRAY(*array, it) {
			String *ts_data = &ARRAY_IT(*array, it);
			String_Destroy(ts_data);
		}
	}

    Array_ClearContainer(array);
}

instant void
String_SplitWordsStatic(
	String *s_data,
	Array<String> *as_words
) {
	Assert(s_data);
	Assert(as_words);

	Array_Clear(as_words);

	Array<String> as_lines = Array_Split(s_data, "\n", DELIMITER_ADD_BACK);

	FOR_ARRAY(as_lines, it_lines) {
		Array<String> tas_words = Array_Split(&ARRAY_IT(as_lines, it_lines), " ");

		FOR_ARRAY(tas_words, it_words) {
			Array_Add(as_words, ARRAY_IT(tas_words, it_words));
		}

		Array_DestroyContainer(&tas_words);
	}

	Array_Destroy(&as_lines);
}

template <typename T>
instant void
Array_Sort_Quick_Ascending(
	T *begin,
	T *end
) {
	Assert(begin);
	Assert(end);

	if (begin == end)
		return;

    T *pivot = begin;
    T *next  = begin;
	++next;

	while(next <= end) {
		if (*next < *pivot) {
			Swap(next, pivot);

			/// next will be past pivot in next loop
			/// pivot will follow next, but never catches up
			if (pivot < next) {
                ++pivot;
                continue;
			}
		}
		++next;
    }

    if (begin < pivot)  Array_Sort_Quick_Ascending(begin    , pivot - 1);
	if (end   > pivot)  Array_Sort_Quick_Ascending(pivot + 1, end      );
}

template <typename T>
instant void
Array_Sort_Quick_Descending(
	T *begin,
	T *end
) {
	Assert(begin);
	Assert(end);

	if (begin == end)
		return;

    T *pivot = begin;
    T *next  = begin;
	++next;

	while(next <= end) {
		if (*next > *pivot) {
			Swap(next, pivot);

			/// next will be past pivot in next loop
			/// pivot will follow next, but never catches up
			if (pivot < next) {
                ++pivot;
                continue;
			}
		}
		++next;
    }

    if (begin < pivot)  Array_Sort_Quick_Descending(begin    , pivot - 1);
	if (end   > pivot)  Array_Sort_Quick_Descending(pivot + 1, end      );
}

template <typename T>
instant void
Array_Sort_Ascending(
	Array<T> *array
) {
	Assert(array);
	Assert(array->count);

	Array_Sort_Quick_Ascending( &array->memory[0], &array->memory[array->count - 1]);
}

template <typename T>
instant void
Array_Sort_Descending(
	Array<T> *array
) {
	Assert(array);
	Assert(array->count);

	Array_Sort_Quick_Descending(&array->memory[0], &array->memory[array->count - 1]);
}

instant String
String_GetDelimiterSection(
	String *s_data,
	const char *c_delimiter,
	u64 index,
	bool auto_trim = true
) {
	String s_result;

	static Array<String> as_section;

	Array_Clear(&as_section);
	as_section = String_Split(s_data, c_delimiter);

	FOR_ARRAY(as_section, it) {
		if (it == index) {
			String *ts_entry = &ARRAY_IT(as_section, it);

			String_Append(&s_result, ts_entry->value, ts_entry->length);

			break;
		}
	}

	String_TrimLeft(&s_result);
	String_TrimRight(&s_result);

	return s_result;
}

/// ::: CPU
/// ===========================================================================
struct CPU_ID {
	u32 EAX;
	u32 EBX;
	u32 ECX;
	u32 EDX;
};

struct CPU_Type {
	u16 familty_ext;
	u16 model_ext;
	u16 type;
	u16 family;
	u16 model;
	u16 stepping;
};

struct CPU_Features {
	bool mmx;
	bool mmx_ext;
	bool sse;
	bool sse2;
	bool sse3;
	bool sse4_1;
	bool sse4_2;
	bool _3dnow;
	bool _3dnow_ext;
};

instant void
CPU_GetID(
	u32 id,
	CPU_ID *cpu_id
) {
	Assert(cpu_id);

	asm volatile
		("cpuid" :
		 "=a" (cpu_id->EAX), "=b" (cpu_id->EBX),
		 "=c" (cpu_id->ECX), "=d" (cpu_id->EDX)
		: "a" (id), "c" (0));
}

instant char *
CPU_GetVendor() {
	CPU_ID cpu_id;
	CPU_GetID(0, &cpu_id);

	char *vendor = Memory_Create(char, 13);
	String_Copy(vendor + 0, (char*)&cpu_id.EBX, 4);
	String_Copy(vendor + 4, (char*)&cpu_id.EDX, 4);
	String_Copy(vendor + 8, (char*)&cpu_id.ECX, 4);

	return vendor;
}

instant CPU_Type
CPU_GetInfo() {
	CPU_ID cpu_id;
	CPU_GetID(1, &cpu_id);

	CPU_Type cpu_type = {0};

	cpu_type.stepping    = (cpu_id.EAX >>  0) & 0xF;
	cpu_type.model       = (cpu_id.EAX >>  4) & 0xF;
	cpu_type.family      = (cpu_id.EAX >>  8) & 0xF;
	cpu_type.type        = (cpu_id.EAX >> 12) & 0x3;
	cpu_type.model_ext   = (cpu_id.EAX >> 16) & 0xF;
	cpu_type.familty_ext = (cpu_id.EAX >> 20) & 0xFF;

    return (cpu_type);
}

instant CPU_Features
CPU_GetFeatures() {
	char *vendor_name = CPU_GetVendor();

	bool is_Intel 	= String_IsEqual(vendor_name, "GenuineIntel");
	bool is_AMD 	= String_IsEqual(vendor_name, "AuthenticAMD");

	CPU_ID cpu_id;
	CPU_GetID(1, &cpu_id);

	CPU_Features cpu_features = {0};

	if (is_Intel OR is_AMD) {
		cpu_features.mmx_ext    = (cpu_id.EDX >> 22) & 0x1;
		cpu_features.mmx        = (cpu_id.EDX >> 23) & 0x1;
		cpu_features._3dnow_ext = (cpu_id.EDX >> 30) & 0x1;
		cpu_features._3dnow     = (cpu_id.EDX >> 31) & 0x1;

		if (is_Intel) {
			cpu_features.sse    = (cpu_id.EDX >> 25) & 0x1;
			cpu_features.sse2   = (cpu_id.EDX >> 26) & 0x1;
			cpu_features.sse3   = (cpu_id.ECX >>  0) & 0x1;
			cpu_features.sse4_1 = (cpu_id.ECX >> 19) & 0x1;
			cpu_features.sse4_2 = (cpu_id.ECX >> 20) & 0x1;
		}
	}

	Memory_Free(vendor_name);

	return (cpu_features);
}

instant void
CPU_GetFeatures(
	Array<const char *> *a_features
) {
	Assert(a_features);

	CPU_Features cpu_features = CPU_GetFeatures();

	if (cpu_features.mmx)     Array_Add(a_features, "MMX");
	if (cpu_features.mmx_ext) Array_Add(a_features, "MMX Ext");

	if (cpu_features.sse)     Array_Add(a_features, "SSE");
	if (cpu_features.sse2)    Array_Add(a_features, "SSE2");
	if (cpu_features.sse3)    Array_Add(a_features, "SSE3");
	if (cpu_features.sse4_1)  Array_Add(a_features, "SSE4.1");
	if (cpu_features.sse4_2)  Array_Add(a_features, "SSE4.2");

	if (cpu_features._3dnow)     Array_Add(a_features, "3DNow");
	if (cpu_features._3dnow_ext) Array_Add(a_features, "3DNow Ext");
}

/// ::: Files
/// ===========================================================================
struct File {
	FILE *fp;
};

instant bool
File_HasExtension(
	String *s_filename,
	String *s_extension
) {
	Assert(s_filename);
	Assert(s_extension);

	bool result = false;

	if (!s_extension->length)  return true;

	Array<String> as_extentions = Array_Split(s_extension, "|");

    FOR_ARRAY(as_extentions, it) {
    	String s_data_it = ARRAY_IT(as_extentions, it);

		if (String_EndWith(s_filename, s_data_it.value, s_data_it.length)) {
			result = true;
			break;
		}
    }

    Array_Destroy(&as_extentions);

	return result;
}

instant bool
File_HasExtension(
	const char *c_filename,
	const char *c_extension,
	u64 c_length_filename = 0,
	u64 c_length_extension = 0
) {
	Assert(c_filename);
	Assert(c_extension);

	bool result = false;

	if (!c_length_filename)
		c_length_filename = String_Length(c_filename);

	if (!c_length_extension)
		c_length_extension = String_Length(c_extension);

	String s_extension;
	s_extension.value  = (char *)c_extension;
	s_extension.length = c_length_extension;

	String s_filename;
	s_filename.value  = (char *)c_filename;
	s_filename.length = c_length_filename;

	Array<String> as_extentions = Array_Split(&s_extension, "|");

    FOR_ARRAY(as_extentions, it) {
    	String s_data_it = ARRAY_IT(as_extentions, it);

		if (String_EndWith(&s_filename, s_data_it.value, s_data_it.length)) {
			result = true;
			break;
		}
    }

    Array_Destroy(&as_extentions);

	return result;
}

instant bool
File_Exists(
	const char *c_filename,
	u64 c_length = 0
) {
	Assert(c_filename);

	WIN32_FIND_DATA file_data;

	char *c_search_file = String_CreateCBufferCopy(c_filename, c_length);

	bool result = false;

	if (FindFirstFile(c_search_file, &file_data) != INVALID_HANDLE_VALUE)
		result = true;

	Memory_Free(c_search_file);

	return result;
}

instant bool
File_Exists(
	String *s_path,
	String *s_filename
) {
	Assert(s_path);
	Assert(s_filename);

	WIN32_FIND_DATA file_data;

	String ts_filename;
	String_Append(&ts_filename, s_path->value, s_path->length);
	String_Append(&ts_filename, "/", 1);
	String_Append(&ts_filename, s_filename->value, s_filename->length);

	bool result = File_Exists(ts_filename.value, ts_filename.length);

	String_Destroy(&ts_filename);

	return result;
}

instant bool
File_CreateDirectory(
	String *s_directory,
	bool make_path_relative = true
) {
	Assert(s_directory);

	if (!s_directory->length)  return false;

	String s_path_relative;

	if (make_path_relative)
		String_Append(&s_path_relative, "./", 2);

	String_Append(&s_path_relative, s_directory->value, s_directory->length);

	char *c_path_relative = String_CreateCBufferCopy(&s_path_relative);

	bool success = (CreateDirectory(c_path_relative, 0) != 0);

	if (!success) {
		/// you wanted it, you got it -> success
		if (GetLastError() == ERROR_ALREADY_EXISTS)
			success = true;
	}

	Memory_Free(c_path_relative);
	String_Destroy(&s_path_relative);

	return success;
}

instant File
File_Open(
	const char *c_filename,
	const char *mode,
	u64 c_length = 0
) {
	Assert(c_filename);
	Assert(mode);

	File file = {};

	char *tc_filename = String_CreateCBufferCopy(c_filename, c_length);

	file.fp = fopen(tc_filename, mode);

	Memory_Free(tc_filename);

	return file;
}

instant bool
File_Close(
	File *file
) {
	Assert(file);

	/// Returns 0 on success
	return (fclose(file->fp) == 0);
}

instant u64
File_Size(
	File *file
) {
	Assert(file);

	if (!file->fp)
		return 0;

	fseek (file->fp, 0 ,SEEK_END);
	u64 size = ftell(file->fp);
	rewind(file->fp);

	return size;
}

instant void
File_Execute(
	String *s_filename
) {
	Assert(s_filename);

	char *c_filename = String_CreateCBufferCopy(s_filename);

	ShellExecute(GetDesktopWindow(), "open", c_filename, 0, 0, SW_SHOWNORMAL);

	Memory_Free(c_filename);
}

instant void
File_Write(
	File *file,
	const char *data,
	u64 length = 0
) {
	Assert(file);

	if (!length)
		length = String_Length(data);

    fwrite(data, sizeof(char), sizeof(char) * length, file->fp);
}

instant String
File_Read(
	File *file
) {
	Assert(file);

	u64 length = File_Size(file);

	String s_data;

	s_data.value  = Memory_Resize(s_data.value, char, length);
	s_data.length = length;
	fread(s_data.value, sizeof(char), sizeof(char) * length, file->fp);

	s_data.changed = true;

	return s_data;
}

instant String
File_ReadAll(
	const char *c_filename,
	u64 c_length = 0,
	bool as_binary = true
) {
    Assert(c_filename);

    File file;

    if (as_binary)
		file = File_Open(c_filename, "rb", c_length);
	else
		file = File_Open(c_filename, "r" , c_length);

	String s_data = File_Read(&file);

	File_Close(&file);

	s_data.changed = true;

	return s_data;
}

struct File_Watcher {
	HANDLE file = 0;
	bool exists = false;
	FILETIME lastWriteTime = {};
};

instant void
File_Watch(
	File_Watcher *file_watcher,
	const char *c_filename,
	u64 c_length = 0
) {
	Assert(file_watcher);

	*file_watcher = {};

	char *tc_filename = String_CreateCBufferCopy(c_filename, c_length);

	file_watcher->file = CreateFile(
			tc_filename,
			0,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL
		);

	if (file_watcher->file)
		file_watcher->exists = true;

	Memory_Free(tc_filename);
}

instant bool
File_HasChanged(
	File_Watcher *file_watcher
) {
	Assert(file_watcher);

	bool has_changed = false;

	if (!file_watcher->exists)  return false;

	FILETIME *lwt = &file_watcher->lastWriteTime;

	FILETIME prevWriteTime = *lwt;
	GetFileTime(file_watcher->file, 0, 0, lwt);

	if (0) {}
	else if (lwt->dwHighDateTime != prevWriteTime.dwHighDateTime)
		has_changed = true;
	else if (lwt->dwLowDateTime  != prevWriteTime.dwLowDateTime )
		has_changed = true;

	return has_changed;
}

enum DIR_ENTRY_TYPE {
	DIR_ENTRY_FILE,
	DIR_ENTRY_DIR
};

struct Directory_Entry {
	String s_name;
	DIR_ENTRY_TYPE type;
};

enum DIR_LIST_TYPE {
	DIR_LIST_ONLY_FILES,
	DIR_LIST_ONLY_DIR,
	DIR_LIST_ALL
};

/// does not list or includes subdirectories
instant void
File_ReadDirectory(
	Array<Directory_Entry> *a_entries,
	const char *c_path,
	const char *extension_filter = 0,
	u64 c_length = 0,
	bool prefix_path = true,
	const char *name_filter = 0,
	DIR_LIST_TYPE type = DIR_LIST_ONLY_FILES
) {
	Assert(a_entries);

	if (!c_length)
		c_length = String_Length(c_path);

	if (!c_length)   return;

	HANDLE id_directory;
	WIN32_FIND_DATA file_data;

	String s_search_path;
	String_Append(&s_search_path, c_path, c_length);
	String_Append(&s_search_path, "/*");

	String s_extension_filter;
	String_Append(&s_extension_filter, extension_filter);

	char *c_search_path = String_CreateCBufferCopy(&s_search_path);

	if ((id_directory = FindFirstFile(c_search_path, &file_data)) != INVALID_HANDLE_VALUE) {
		do {
			const bool is_directory = (file_data.dwFileAttributes &
									   FILE_ATTRIBUTE_DIRECTORY)  != 0;

			if (is_directory) {
				if (type == DIR_LIST_ONLY_FILES)
					continue;

				if (String_IsEqual(file_data.cFileName, ".", 1) AND String_Length(file_data.cFileName) == 1) {
					continue;
				}
			}
			else {
				 if (type == DIR_LIST_ONLY_DIR)
					continue;
			}

			String s_filename;
			String_Append(&s_filename, file_data.cFileName);

			bool has_extension = File_HasExtension(&s_filename, &s_extension_filter);

			bool found_name = true;

			/// exclude if filter does not match
			if (name_filter)
				found_name = String_Find(&s_filename, name_filter);

			if (has_extension AND found_name) {
				String ts_filename;

				if (prefix_path) {
					String_Append(&ts_filename, c_path, c_length);
					if (!String_EndWith(&ts_filename, "/") AND !String_EndWith(&ts_filename, "\\"))
						String_Append(&ts_filename, "\\");
				}

				String_Append(&ts_filename, file_data.cFileName);

				Directory_Entry dir_entry;
				String_Append(&dir_entry.s_name, ts_filename.value, ts_filename.length);

				if (is_directory)
					dir_entry.type = DIR_ENTRY_DIR;
				else
					dir_entry.type = DIR_ENTRY_FILE;

				Array_Add(a_entries, dir_entry);
			}

			String_Destroy(&s_filename);
		} while (FindNextFile(id_directory, &file_data));

		FindClose(id_directory);
	}

	String_Destroy(&s_extension_filter);

	Memory_Free(c_search_path);
	String_Destroy(&s_search_path);
}

instant void
File_Rename(
	const char *c_path,
	u64 c_length_path,
	const char *c_filename_old,
	u64 c_length_file_old,
	const char *c_filename_new,
	u64 c_length_file_new
) {
	if (!c_length_path)
		c_length_path = String_Length(c_path);

	if (!c_length_file_old)
		c_length_file_old = String_Length(c_filename_old);

	if (!c_length_file_new)
		c_length_file_new = String_Length(c_filename_new);

	String s_file_old;
	String_Append(&s_file_old, c_path, c_length_path);
	String_Append(&s_file_old, "/", 1);
	String_Append(&s_file_old, c_filename_old, c_length_file_old);

	String s_file_new;
	String_Append(&s_file_new, c_path, c_length_path);
	String_Append(&s_file_new, "/", 1);
	String_Append(&s_file_new, c_filename_new, c_length_file_new);

	String_Destroy(&s_file_old);
	String_Destroy(&s_file_new);

	char *tc_file_old = String_CreateCBufferCopy(&s_file_old);
	char *tc_file_new = String_CreateCBufferCopy(&s_file_new);

//	LOG_DEBUG(tc_file_new + c_length_path + 1 << " - " << tc_file_old + c_length_path + 1);
	MoveFile(tc_file_old, tc_file_new);

	Memory_Free(tc_file_old);
	Memory_Free(tc_file_new);
}

instant String
File_GetExtension(
	String *s_data
) {
	String s_result;

	s64 pos_ext = String_IndexOfRev(s_data, ".");

	if (!pos_ext) {
		return s_result;
	}

	String_Copy(&s_result, s_data->value + pos_ext, s_data->length - pos_ext);

	return s_result;
}

/// ::: Windows (OpenGL)
/// ===========================================================================
#define Windows_Main 	\
	APIENTRY			\
	WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_text, int nCmdShow)

#define LOG_ERROR(text) std::cerr << "[Error] " << text << std::endl

#define Window_IsCreated(window) (window->hWnd != 0)

#define Window_ReadMessage(_msg, _running, _ptr_window)             				\
	while (PeekMessage(&_msg, _ptr_window->hWnd, 0, 0, PM_REMOVE)) {				\
		if (Mouse_Update(_ptr_window->mouse, _ptr_window, &_msg))	continue;		\
		if (Keyboard_Update(_ptr_window->keyboard, &_msg))          continue;		\
																\
		switch (msg.message) {									\
			case WM_QUIT: {										\
				_msg.wParam = 0;								\
				_running = false;								\
			} break;											\
																\
			default: {											\
				TranslateMessage(&_msg);						\
				DispatchMessage(&_msg);							\
			}													\
		}														\
	}

instant bool
Dialog_OpenFile(
	String *s_file
) {
	Assert(s_file);

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
		To_String(s_file, filename);
		result = true;
    }

    return result;
}

instant bool
Dialog_OpenDirectory(
	String *s_directory
) {
	Assert(s_directory);

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
			To_String(s_directory, directory);
	}

    return result;
}

static const char *class_name = "OpenGL";

struct Mouse;
struct Keyboard;

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

LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_ACTIVATE: {

		} break;

		case WM_CLOSE: {
			PostQuitMessage(0);
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

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

instant void
Window_Destroy(
	Window *window
) {
	if (window->isFullscreen) {
		ChangeDisplaySettings(0, 0);
	}

	if (Window_IsCreated(window)) {
		if (window->hDC)
			ReleaseDC(window->hWnd, window->hDC);

		DestroyWindow(window->hWnd);
	}

	if (!UnregisterClass(class_name, GetModuleHandle(0))) {
		LOG_ERROR("UnregisterClass() failed.");
	}
}

instant bool
Window_Create(
	Window *window,
	const char *title,
	s32 width,
	s32 height,
	s32 bits = 32,
	Keyboard *keyboard = 0,
	Mouse *mouse = 0
) {
	Assert(window);
	*window = {};

	/// register window class
	/// -------------------------------------
	HINSTANCE hInstance = GetModuleHandle(0);

	WNDCLASS wc 	 = {};
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
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
		LOG_ERROR("RegisterClass() failed: Cannot register window class.");
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

	if (window->isFullscreen) {
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
		LOG_ERROR("CreateWindow() failed: Cannot create a window.");
		Window_Destroy(window);
		return false;
	}

	window->hWnd = hWnd;

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
		Window_Destroy(window);
		return false;
	}

	if (!SetPixelFormat(hDC, pf, &pfd)) {
		LOG_ERROR("SetPixelFormat() failed: Cannot set format specified.");
		Window_Destroy(window);
		return false;
	}

	if (!DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd)) {
		LOG_ERROR("DescribePixelFormat() failed: " << GetLastError());
		Window_Destroy(window);
		return false;
	}

	window->hDC = hDC;

	window->title  = title;
	window->width  = width;
	window->height = height;

	if (mouse)
		window->mouse = mouse;

	if (keyboard)
		window->keyboard = keyboard;

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

instant void
Window_UpdateAndResetInput(
	Window *window
) {
	Assert(window);

	SwapBuffers(window->hDC);

	Mouse_Reset(window->mouse);
	Keyboard_ResetLastKey(window->keyboard);
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
	Window *window
) {
	Assert(window);

	if (!window->isFullscreen) {
		DWORD dwStyle = (DWORD)GetWindowLong(window->hWnd, GWL_STYLE);

		dwStyle = dwStyle | WS_POPUP;
		dwStyle = dwStyle & ~WS_OVERLAPPEDWINDOW;

		SetWindowLong(window->hWnd, GWL_STYLE, dwStyle);

		ShowWindow(window->hWnd, SW_RESTORE);
		ShowWindow(window->hWnd, SW_MAXIMIZE);

		window->isFullscreen = true;
	}
	else {
		DWORD dwStyle = (DWORD)GetWindowLong(window->hWnd, GWL_STYLE);

		dwStyle = dwStyle & ~WS_POPUP;
		dwStyle = dwStyle |  WS_OVERLAPPEDWINDOW;

		SetWindowLong(window->hWnd, GWL_STYLE, dwStyle);

		ShowWindow(window->hWnd, SW_RESTORE);

		window->isFullscreen = false;
	}
}

instant void
Window_SetSize(
	Window *window,
	s32 width,
	s32 height
) {
	Assert(window);

	if (window->isFullscreen)
		Window_ToggleFullscreen(window);

	RECT rect = {};
	DWORD dwStyle   = (DWORD)GetWindowLong(window->hWnd, GWL_STYLE);
	DWORD dwExStyle = (DWORD)GetWindowLong(window->hWnd, GWL_EXSTYLE);

	bool success = AdjustWindowRectEx(&rect, dwStyle, 0, dwExStyle);

	if (success) {
		width  += (rect.right  - rect.left);
		height += (rect.bottom - rect.top);

		rect = {};
		GetWindowRect(window->hWnd, &rect);

		s32 x = rect.left + (rect.right  - rect.left - width)  / 2;
		s32 y = rect.top  + (rect.bottom - rect.top  - height) / 2;

		SetWindowPos(window->hWnd, HWND_TOP, x, y, width, height, 0);
	}
}

instant void
Window_SetTitle(
	Window *window,
	const char *title
) {
	Assert(window);
	Assert(title);

	window->title = title;

	SetWindowText(window->hWnd, window->title);
}

instant bool
Window_UnAdjustRect(
	HWND hWnd,
	RECT *rect
) {
	Assert(rect);

	RECT t_rect = {};

	DWORD dwStyle   = (DWORD)GetWindowLong(hWnd, GWL_STYLE);
	DWORD dwExStyle = (DWORD)GetWindowLong(hWnd, GWL_EXSTYLE);

	bool success = AdjustWindowRectEx(&t_rect, dwStyle, 0, dwExStyle);

	if (success) {
		rect->left   -= t_rect.left;
		rect->top    -= t_rect.top;
		rect->right  -= t_rect.right;
		rect->bottom -= t_rect.bottom;
	}
	return success;
}

/// ::: OpenGL
/// ===========================================================================
instant void
OpenGL_Init(
	Window *window
) {
	Assert(window);

	if (window->hDC) {
		window->hRC = wglCreateContext(window->hDC);
		wglMakeCurrent(window->hDC, window->hRC);

		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		Window_InitOpenGL_Ext();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
	}
}

/// call before Window_Destroy
instant void
OpenGL_Destroy(
	Window *window
) {
	Assert(window);

	if (window->hRC) {
		wglMakeCurrent(0, 0);
		wglDeleteContext(window->hRC);
	}
}

instant void
OpenGL_SetVSync(
	Window *window,
	bool enable
) {
	Assert(window);

	if(wglSwapIntervalEXT)
		wglSwapIntervalEXT(enable);

	window->useVSync = enable;

	LOG_DEBUG("VSync: " << (enable
							? "enabled."
							: "disabled."));
}

instant void
OpenGL_ClearScreen(
	float r = 0.f,
	float g = 0.f,
	float b = 0.f,
	float a = 0.f
) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor4f(1.f, 1.f, 1.f, 1.f);
}

/// keeps aspect ratio by changing the viewport,
/// or ignores aspect ratio and sets the viewport
/// to the current window size
instant bool
OpenGL_AdjustScaleViewport(
	Window *window,
	bool zooming = true
) {
	Assert(window);

	RECT rect_window;
	GetClientRect(window->hWnd, &rect_window);
	s32 new_width  = rect_window.right  - rect_window.left;
	s32 new_height = rect_window.bottom - rect_window.top;

	float x = 0.f;
	float y = 0.f;

	if (!zooming) {
		bool result =    window->width  != new_width
		              OR window->height != new_height;

		glViewport(x, y, new_width, new_height);
		window->scale_x = 1.0f;
		window->scale_y = 1.0f;
		window->width = new_width;
		window->height = new_height;

		return result;
	}

	Rect_GetAspect(
		window->width,
		window->height,
		&new_width,
		&new_height,
		&x,
		&y
	);

    window->x_viewport = x;
    window->y_viewport = y;

	float prev_scale_x = window->scale_x;
	float prev_scale_y = window->scale_y;

	if (window->width > new_width)
		window->scale_x = (float)new_width / window->width;
	else
		window->scale_x = (float)window->width / new_width;


	if (window->height > new_height)
		window->scale_y = (float)new_height / window->height;
	else
		window->scale_y = (float)window->height / new_height;

	glViewport(x, y, new_width, new_height);

    if (prev_scale_x != window->scale_x) return true;
    if (prev_scale_y != window->scale_y) return true;

    return false;
}

instant s32
OpenGL_GetTextureSizeMax() {
	s32 max = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
	return max;
}

instant void
OpenGL_Scissor(
	Window *window,
	float x,
	float y,
	s32 w,
	s32 h
) {
	Assert(window);

	/// convert to right-hand coordinate system
	float t_x = x / window->scale_x + window->x_viewport;
	float t_y = (window->height - y - h) / window->scale_y + window->y_viewport;
	s32   t_w = w / window->scale_x;
	s32   t_h = h / window->scale_y;

	glEnable(GL_SCISSOR_TEST);
	glScissor(t_x, t_y, t_w, t_h);
}

instant void
OpenGL_Scissor(
	Window *window,
	Rect rect
) {
	Assert(window);

	float x = rect.x;
	float y = rect.y;
	s32   w = rect.w;
	s32   h = rect.h;

	/// convert to right-hand coordinate system
	float t_x = x / window->scale_x + window->x_viewport;
	float t_y = (window->height - y - h) / window->scale_y + window->y_viewport;
	s32   t_w = w / window->scale_x;
	s32   t_h = h / window->scale_y;

	glEnable(GL_SCISSOR_TEST);
	glScissor(t_x, t_y, t_w, t_h);
}

instant void
OpenGL_Scissor_Disable() {
	glDisable(GL_SCISSOR_TEST);
}

instant void
OpenGL_SetBlending(
	bool enabled
) {
	if (enabled) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		glDisable(GL_BLEND);
	}
}

/// ::: Image
/// ===========================================================================
struct Image {
	s32   width  = 0;
	s32   height = 0;
	u32   bits   = 0;
	u8   *data   = 0;
	bool  flip_h = false;
};

/// 32-bit BMP only!
instant Image
Image_LoadBMP32(
	const char *c_filename,
	u64 c_length = 0
) {
	Assert(c_filename);

	Image result = {};

	if (!File_Exists(c_filename, c_length)) {
		return result;
	}

	File file = File_Open(c_filename, "rb", c_length);

	String s_data = File_Read(&file);

	File_Close(&file);

    String s_data_it = s_data;

    if (String_StartWith(&s_data_it, "BM")) {
		BITMAPFILEHEADER *bmp_header = (BITMAPFILEHEADER *)s_data_it.value;
		s_data_it.value  += sizeof(BITMAPFILEHEADER);
		s_data_it.length -= sizeof(BITMAPFILEHEADER);
		BITMAPINFOHEADER *bmp_info = (BITMAPINFOHEADER *)s_data_it.value;

		///@Info: make sure you free the memory after usage!
		result.data   = Memory_Create(u8, bmp_info->biSizeImage);
		result.width  = bmp_info->biWidth;
		result.height = bmp_info->biHeight;
		result.bits   = bmp_info->biBitCount / 8;

		result.flip_h = (bmp_info->biHeight > 0);

		s_data_it = s_data;
		s_data_it.value  += bmp_header->bfOffBits;
		s_data_it.length -= bmp_header->bfOffBits;

		Memory_Copy(result.data, s_data_it.value, bmp_info->biSizeImage);
	}

	String_Destroy(&s_data);

	return result;
}

instant void
Image_Destroy(
	Image *image
) {
	Assert(image);

	Memory_Free(image->data);
}

/// ::: Textures
/// ===========================================================================
struct Texture {
	u32 ID = 0;
	bool flip_h = false;
};

bool
operator == (
	Texture &texture_1,
	Texture &texture_2
) {
	if (texture_1.ID == texture_2.ID){
		return true;
	}

	return false;
}

instant void
Array_Destroy(
	Array<Tuple<Texture, Array<float>>> *array
) {
	Assert(array);

	FOR_ARRAY(*array, it) {
		Tuple<Texture, Array<float>> *t_tuple = &ARRAY_IT(*array, it);
		Array<float> *a_second = &t_tuple->second;

		Array_DestroyContainer(a_second);
	}

	Array_DestroyContainer(array);
}

instant bool
Texture_IsEmpty(
	Texture *texture
) {
	Assert(texture);

	return (texture->ID == 0);
}

instant void
Texture_GetSizeAndBind(
	Texture *texture,
	s32 *width,
	s32 *height
) {
    Assert(texture);

    glBindTexture(GL_TEXTURE_2D, texture->ID);
    if (width)  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH , width);
	if (height) glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height);
}

instant Texture
Texture_Load(
	const u8 *data,
	s32 width,
	s32 height,
	u32 format_input,
	bool linearFilter
) {
	Texture result = {};

    u32 id_texture;

    glGenTextures(1, &id_texture);
    glBindTexture(GL_TEXTURE_2D, id_texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA , width, height, 0,
					format_input, GL_UNSIGNED_BYTE, data);

	s32 param;

	if (linearFilter) param = GL_LINEAR;
	else              param = GL_NEAREST;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);

    result.ID = id_texture;

    return result;
}

instant void
Texture_Destroy(
	Texture *texture
) {
	Assert(texture);

	glDeleteTextures(1, &texture->ID);
	texture->ID = 0;
}

instant void
Texture_Reload(
	Texture *texture,
	const char *c_filename,
	u64 c_length = 0
) {
	Assert(texture);

	if (File_HasExtension(c_filename, ".bmp", c_length)) {
		u32 format_input;

		/// 32-bit bmp only
		Image image = Image_LoadBMP32(c_filename, c_length);

		if (image.flip_h)  format_input = GL_ABGR_EXT;
		else               format_input = GL_RGBA;

		Texture_Destroy(texture);
		*texture = Texture_Load(image.data, image.width, image.height, format_input, false);
		texture->flip_h = image.flip_h;

		Image_Destroy(&image);

		return;
	}

	if (File_HasExtension(c_filename, ".jpg|.png|.gif", c_length)) {
		char *tc_filename = String_CreateCBufferCopy(c_filename, c_length);

		s32 width, height, bits;
		u8 *c_data = stbi_load(tc_filename, &width, &height, &bits, 4);

		Texture_Destroy(texture);
		*texture = Texture_Load(c_data, width, height, GL_RGBA, true);

		Memory_Free(tc_filename);

		return;
	}
}

instant Texture
Texture_Load(
	const char *c_filename,
	u64 c_length = 0
) {
	Texture result;

	Texture_Reload(&result, c_filename, c_length);

	return result;
}

instant Texture
Texture_Create(
	s32 width,
	s32 height
) {
	return Texture_Load(0, width, height, GL_RGBA, false);
}

instant bool
Texture_IsEqual(
	Texture data1,
	Texture data2
) {
	return (data1.ID == data2.ID);
}

/// ::: GLSL
/// ===========================================================================
enum SHADER_PROG_TYPE {
	SHADER_PROG_RECT = 0,
	SHADER_PROG_TEXT,
	SHADER_PROG_TEXTURE_FULL,
	SHADER_PROG_TEXTURE_SIZE,
	SHADER_PROG_COUNT
};

struct Shader {
	SHADER_PROG_TYPE type;
	const char *code[3];
};

static const Shader shader_rect = {
	SHADER_PROG_RECT,
R"(
	#version 330 core

	uniform vec4 viewport = vec4(0, 0, 800, 480);
	uniform float scale_x = 1.0f;
	uniform float scale_y = 1.0f;

	in vec4 vertex_position;
	in vec4 rect_color;

	float left   = 0.0f;
	float right  = viewport.z;
	float top    = 0.0f;
	float bottom = viewport.w;

    mat4 proj_matrix = mat4(
		 2.0f / (right - left), 0                    ,  0,  0,
		 0                    , 2.0f / (top - bottom),  0,  0,
		 0                    , 0                    ,  1,  0,
		-(right + left)   / (right - left),
		-(top   + bottom) / (top   - bottom),
		 0,
		 1
	);

	mat4 scale_matrix = mat4(
		scale_x , 0      , 0, 0,
		0       , scale_y, 0, 0,
		0       , 0      , 1, 0,
		0       , 0      , 0, 1
	);

	out Vertex_Data {
		mat4 proj_matrix;
		mat4 scale_matrix;
		vec4 rect_color;
	} o_Vertex;

	void main() {
		gl_Position           = vertex_position;
		o_Vertex.proj_matrix  = proj_matrix;
		o_Vertex.scale_matrix = scale_matrix;
		o_Vertex.rect_color   = rect_color;
	}
)",

R"(
	#version 330 core

	layout(points) in;
	layout(triangle_strip, max_vertices = 4) out;

	in Vertex_Data {
		mat4 proj_matrix;
		mat4 scale_matrix;
		vec4 rect_color;
	} i_Vertex[];

	out vec4 rect_color;

	void main() {
		vec4 pt           = gl_in[0].gl_Position;
		mat4 proj_matrix  = i_Vertex[0].proj_matrix;
		mat4 scale_matrix = i_Vertex[0].scale_matrix;

		mat4 matrix_mod = proj_matrix;

		vec4 v_pos_1 = matrix_mod * vec4(pt.x, pt.y, 0, 1) * scale_matrix;
		vec4 v_pos_2 = matrix_mod * vec4(pt.x, pt.w, 0, 1) * scale_matrix;
		vec4 v_pos_3 = matrix_mod * vec4(pt.z, pt.y, 0, 1) * scale_matrix;
		vec4 v_pos_4 = matrix_mod * vec4(pt.z, pt.w, 0, 1) * scale_matrix;

		/// v1
		gl_Position = v_pos_1;
		rect_color = i_Vertex[0].rect_color;
		EmitVertex();

		/// v3
		gl_Position = v_pos_2;
		rect_color = i_Vertex[0].rect_color;
		EmitVertex();

		/// v2
		gl_Position = v_pos_3;
		rect_color = i_Vertex[0].rect_color;
		EmitVertex();

		/// v4
		gl_Position = v_pos_4;
		rect_color = i_Vertex[0].rect_color;
		EmitVertex();

		EndPrimitive();
	}
)",

R"(
	#version 330 core

	layout(origin_upper_left) in vec4 gl_FragCoord;

	in vec4 rect_color;

	void main() {
		gl_FragColor = rect_color;
	}
)"};

static const Shader shader_text = {
	SHADER_PROG_TEXT,
R"(
	#version 330 core

	uniform vec4  viewport = vec4(0, 0, 800, 480);
	uniform float scale_x  = 1.0f;
	uniform float scale_y  = 1.0f;

	in vec2 vertex_position;
	in vec3 text_color;

	float left   = 0.0f;
	float right  = viewport.z;
	float top    = 0.0f;
	float bottom = viewport.w;

    mat4 proj_matrix = mat4(
		 2.0f / (right - left), 0                    ,  0,  0,
		 0                    , 2.0f / (top - bottom),  0,  0,
		 0                    , 0                    ,  1,  0,
		-(right + left)   / (right - left),
		-(top   + bottom) / (top   - bottom),
		 0,
		 1
	);

	mat4 scale_matrix = mat4(
		scale_x , 0      , 0, 0,
		0       , scale_y, 0, 0,
		0       , 0      , 1, 0,
		0       , 0      , 0, 1
	);

	out Vertex_Data {
		mat4 proj_matrix;
		mat4 scale_matrix;
		vec3 text_color;
	} o_Vertex;

	void main() {
		gl_Position = vec4(vertex_position, 0, 1);
		o_Vertex.proj_matrix = proj_matrix;
		o_Vertex.scale_matrix = scale_matrix;
		o_Vertex.text_color  = text_color;
	}
)",

R"(
	#version 330 core

	layout(points) in;
	layout(triangle_strip, max_vertices = 4) out;

	uniform sampler2D fragment_texture;
	vec2 size = textureSize(fragment_texture, 0);

	in Vertex_Data {
		mat4 proj_matrix;
		mat4 scale_matrix;
		vec3 text_color;
	} i_Vertex[];

	out vec2 tex_coords;
	out vec3 text_color;

	void main() {
		vec4 point        = gl_in[0].gl_Position;
		mat4 proj_matrix  = i_Vertex[0].proj_matrix;
		mat4 scale_matrix = i_Vertex[0].scale_matrix;

		mat4 matrix_mod = proj_matrix;

		vec4 v_pos_1 = matrix_mod * (point + vec4(0     , 0     , 0, 0) * scale_matrix);
		vec4 v_pos_2 = matrix_mod * (point + vec4(size.x, 0     , 0, 0) * scale_matrix);
		vec4 v_pos_3 = matrix_mod * (point + vec4(0     , size.y, 0, 0) * scale_matrix);
		vec4 v_pos_4 = matrix_mod * (point + vec4(size.x, size.y, 0, 0) * scale_matrix);

		/// v1
		gl_Position = v_pos_1;
		tex_coords = vec2(0, 0);
		text_color = i_Vertex[0].text_color;
		EmitVertex();

		/// v3
		gl_Position = v_pos_2;
		tex_coords = vec2(1, 0);
		text_color = i_Vertex[0].text_color;
		EmitVertex();

		/// v2
		gl_Position = v_pos_3;
		tex_coords = vec2(0, 1);
		text_color = i_Vertex[0].text_color;
		EmitVertex();

		/// v4
		gl_Position = v_pos_4;
		tex_coords = vec2(1, 1);
		text_color = i_Vertex[0].text_color;
		EmitVertex();

		EndPrimitive();
	}
)",

R"(
	#version 330 core

	uniform sampler2D fragment_texture;

	layout(origin_upper_left) in vec4 gl_FragCoord;

	in vec2 tex_coords;
	in vec3 text_color;

	void main() {
		vec4 color_greyscale = texture2D(fragment_texture, tex_coords);
		gl_FragColor = vec4(text_color.xyz, color_greyscale.a);
	}
)"};

static const Shader shader_texture_full = {
	SHADER_PROG_TEXTURE_FULL,
R"(
	#version 330 core

	uniform vec4 viewport = vec4(0, 0, 800, 480);

	in vec2 vertex_position;

	float left   = 0.0f;
	float right  = viewport.z;
	float top    = 0.0f;
	float bottom = viewport.w;

    mat4 proj_matrix = mat4(
		 2.0f / (right - left), 0                    ,  0,  0,
		 0                    , 2.0f / (top - bottom),  0,  0,
		 0                    , 0                    ,  1,  0,
		-(right + left)   / (right - left),
		-(top   + bottom) / (top   - bottom),
		 0,
		 1
	);

	out Vertex_Data {
		mat4 proj_matrix;
	} o_Vertex;

	void main() {
		gl_Position = vec4(vertex_position, 0, 1);
		o_Vertex.proj_matrix = proj_matrix;
	}
)",

R"(
	#version 330 core

	layout(points) in;
	layout(triangle_strip, max_vertices = 4) out;

	uniform sampler2D fragment_texture;
	vec2 size = textureSize(fragment_texture, 0);

	uniform bool  flip_h  = false;
	uniform float scale_x = 1.0f;
	uniform float scale_y = 1.0f;

	in Vertex_Data {
		mat4 proj_matrix;
	} i_Vertex[];

	out Vertex_Data {
		vec2 tex_coords;
	} o_Vertex;

	mat4 scale_matrix = mat4(
		scale_x, 0      , 0, 0,
		0      , scale_y, 0, 0,
		0      , 0      , 1, 0,
		0      , 0      , 0, 1
	);

	void main() {
		vec4 point       = gl_in[0].gl_Position;
		mat4 proj_matrix = i_Vertex[0].proj_matrix;

		mat4 matrix_mod = proj_matrix;

		if (!flip_h) {
			/// v1
			gl_Position = matrix_mod * (point + vec4(0     , 0     , 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(0, 0);
			EmitVertex();

			/// v3
			gl_Position = matrix_mod * (point + vec4(size.x, 0     , 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(1, 0);
			EmitVertex();

			/// v2
			gl_Position = matrix_mod * (point + vec4(0     , size.y, 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(0, 1);
			EmitVertex();

			/// v4
			gl_Position = matrix_mod * (point + vec4(size.x, size.y, 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(1, 1);
			EmitVertex();
		}

		if (flip_h) {
			/// v1
			gl_Position = matrix_mod * (point + vec4(0     , 0     , 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(0, 1);
			EmitVertex();

			/// v3
			gl_Position = matrix_mod * (point + vec4(size.x, 0     , 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(1, 1);
			EmitVertex();

			/// v2
			gl_Position = matrix_mod * (point + vec4(0     , size.y, 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(0, 0);
			EmitVertex();

			/// v4
			gl_Position = matrix_mod * (point + vec4(size.x, size.y, 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(1, 0);
			EmitVertex();
		}

		EndPrimitive();
	}
)",

R"(
	#version 330 core

	uniform sampler2D fragment_texture;

	in Vertex_Data {
		vec2 tex_coords;
	} i_Vertex;

	void main() {
		gl_FragColor = texture2D(fragment_texture, i_Vertex.tex_coords);
	}
)"};

static const Shader shader_texture_size = {
	SHADER_PROG_TEXTURE_SIZE,
R"(
	#version 330 core

	uniform vec4 viewport = vec4(0, 0, 800, 480);
	uniform float scale_x = 1.0f;
	uniform float scale_y = 1.0f;

	in vec4 vertex_position;

	float left   = 0.0f;
	float right  = viewport.z;
	float top    = 0.0f;
	float bottom = viewport.w;

    mat4 proj_matrix = mat4(
		 2.0f / (right - left), 0                    ,  0,  0,
		 0                    , 2.0f / (top - bottom),  0,  0,
		 0                    , 0                    ,  1,  0,
		-(right + left)   / (right - left),
		-(top   + bottom) / (top   - bottom),
		 0,
		 1
	);

	mat4 scale_matrix = mat4(
		scale_x , 0      , 0, 0,
		0       , scale_y, 0, 0,
		0       , 0      , 1, 0,
		0       , 0      , 0, 1
	);

	out Vertex_Data {
		mat4 proj_matrix;
		mat4 scale_matrix;
	} o_Vertex;

	void main() {
		gl_Position           = vertex_position;
		o_Vertex.proj_matrix  = proj_matrix;
		o_Vertex.scale_matrix = scale_matrix;
	}
)",

R"(
	#version 330 core

	layout(points) in;
	layout(triangle_strip, max_vertices = 4) out;

	uniform bool flip_h = false;

	in Vertex_Data {
		mat4 proj_matrix;
		mat4 scale_matrix;
	} i_Vertex[];

	out vec2 tex_coords;

	void main() {
		vec4 pt           = gl_in[0].gl_Position;
		mat4 proj_matrix  = i_Vertex[0].proj_matrix;
		mat4 scale_matrix = i_Vertex[0].scale_matrix;

		mat4 matrix_mod = proj_matrix;

		vec4 pos_top_left     = matrix_mod * vec4(pt.x, pt.y, 0, 1) * scale_matrix;
		vec4 pos_top_right    = matrix_mod * vec4(pt.x, pt.w, 0, 1) * scale_matrix;
		vec4 pos_bottom_left  = matrix_mod * vec4(pt.z, pt.y, 0, 1) * scale_matrix;
		vec4 pos_bottom_right = matrix_mod * vec4(pt.z, pt.w, 0, 1) * scale_matrix;

		///@Info:
		/// position: right hand side
		/// u/v     : left  hand side
		///
		/// BOTH in triangle_strip order

		if (!flip_h) {
			gl_Position = pos_top_left;
			tex_coords = vec2(0, 0);
			EmitVertex();

			gl_Position = pos_bottom_left;
			tex_coords = vec2(1, 0);
			EmitVertex();

			gl_Position = pos_top_right;
			tex_coords = vec2(0, 1);
			EmitVertex();

			gl_Position = pos_bottom_right;
			tex_coords = vec2(1, 1);
			EmitVertex();
		}
		else {
			gl_Position = pos_top_left;
			tex_coords = vec2(0, 1);
			EmitVertex();

			gl_Position = pos_bottom_left;
			tex_coords = vec2(1, 1);
			EmitVertex();

			gl_Position = pos_top_right;
			tex_coords = vec2(0, 0);
			EmitVertex();

			gl_Position = pos_bottom_right;
			tex_coords = vec2(1, 0);
			EmitVertex();

		}

		EndPrimitive();
	}
)",

R"(
	#version 330 core

	layout(origin_upper_left) in vec4 gl_FragCoord;

	uniform sampler2D fragment_texture;

	in vec2 tex_coords;

	void main() {
		gl_FragColor = texture2D(fragment_texture, tex_coords);
	}
)"};


/// ::: Shader
/// ===========================================================================
struct ShaderProgram {
	SHADER_PROG_TYPE type;
	u32 id = 0;
	u32 vertex_id   = 0;
	u32 geometry_id = 0;
	u32 fragment_id = 0;
};

struct ShaderSet {
	Array<ShaderProgram> a_shaders;
	s64 active_id  = -1;
	Window *window =  0;
};

instant void
ShaderProgram_Add(
	ShaderProgram *shader_prog,
	u32 type,
	const char *code
) {
	Assert(shader_prog);

    u32 id_shader = glCreateShader(type);

	glShaderSource(id_shader, 1, &code, 0);
	glCompileShader(id_shader);

	GLint result = GL_FALSE;
	s32 length_info_log;

	glGetShaderiv(id_shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(id_shader, GL_INFO_LOG_LENGTH, &length_info_log);

	if (length_info_log) {
		/// make space for '\0'
		char *c_error_msg = Memory_Create(char, length_info_log + 1);
		glGetShaderInfoLog(id_shader, length_info_log, 0, c_error_msg);
		LOG_DEBUG(c_error_msg);
		Memory_Free(c_error_msg);
	}

	switch (type) {
		case GL_VERTEX_SHADER:   { shader_prog->vertex_id   = id_shader; } break;
		case GL_GEOMETRY_SHADER: { shader_prog->geometry_id = id_shader; } break;
		case GL_FRAGMENT_SHADER: { shader_prog->fragment_id = id_shader; } break;
		default: {
			LOG_DEBUG("Unimplemented shader type");
		}
	}
}

/// returns shader array index
instant u32
ShaderSet_Add(
	ShaderSet *shader_set,
	const Shader *shader
) {
	Assert(shader_set);
    Assert(shader);

	ShaderProgram *shader_prog;
	u64 array_id = Array_AddEmpty(&shader_set->a_shaders, &shader_prog);

	AssertMessage(	array_id == shader->type,
					"Shader added out of SHADER_PROG_TYPE order.");

	if (shader->code[0])  ShaderProgram_Add(shader_prog, GL_VERTEX_SHADER  , shader->code[0]);
	if (shader->code[1])  ShaderProgram_Add(shader_prog, GL_GEOMETRY_SHADER, shader->code[1]);
	if (shader->code[2])  ShaderProgram_Add(shader_prog, GL_FRAGMENT_SHADER, shader->code[2]);

	shader_prog->id = glCreateProgram();

	if (shader->code[0])  glAttachShader(shader_prog->id, shader_prog->vertex_id);
	if (shader->code[1])  glAttachShader(shader_prog->id, shader_prog->geometry_id);
	if (shader->code[2])  glAttachShader(shader_prog->id, shader_prog->fragment_id);
	glLinkProgram(shader_prog->id);

	GLint result = GL_FALSE;
	s32 length_info_log;
	glGetProgramiv(shader_prog->id, GL_LINK_STATUS, &result);
	glGetProgramiv(shader_prog->id, GL_INFO_LOG_LENGTH, &length_info_log);

	if (length_info_log) {
		/// make space for '\0'
		char *c_error_msg = Memory_Create(char, length_info_log + 1);
		glGetProgramInfoLog(shader_prog->id, length_info_log, 0, c_error_msg);
		LOG_DEBUG(c_error_msg);
		Memory_Free(c_error_msg);
	}

	shader_prog->type = shader->type;

	return array_id;
}

instant ShaderSet
ShaderSet_Create(
	Window *window
) {
	Assert(window);

	ShaderSet t_shader_set;
	t_shader_set.window = window;

	ShaderSet_Add(&t_shader_set, &shader_rect);
	ShaderSet_Add(&t_shader_set, &shader_text);
	ShaderSet_Add(&t_shader_set, &shader_texture_full);
	ShaderSet_Add(&t_shader_set, &shader_texture_size);

	AssertMessage(	t_shader_set.a_shaders.count == SHADER_PROG_COUNT,
					"Shader missing compared to SHADER_PROG_TYPE count.");

	return t_shader_set;
}

inline void
ShaderProgram_Destroy(
	ShaderProgram *shader_prog
) {
	Assert(shader_prog);

    if (shader_prog->id) {
		glDeleteProgram(shader_prog->id);
		glDeleteShader(shader_prog->vertex_id);
		glDeleteShader(shader_prog->geometry_id);
		glDeleteShader(shader_prog->fragment_id);

		*shader_prog = {};
    }
}

inline void
ShaderSet_Destroy(
	ShaderSet *shader_set
) {
    Assert(shader_set);

    FOR_ARRAY(shader_set->a_shaders, it) {
		ShaderProgram *t_shader_prog = &ARRAY_IT(shader_set->a_shaders, it);
		ShaderProgram_Destroy(t_shader_prog);
    }
}

inline void
Shader_SetValue(
	ShaderSet *shader_set,
	const char *name,
	const float buffer
) {
	Assert((u64)shader_set->active_id < shader_set->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set->a_shaders, shader_set->active_id);

	s32 loc_id = glGetUniformLocation(shader_prog->id, name);

	if (loc_id >= 0)
		glProgramUniform1f(shader_prog->id, loc_id, buffer);
}

inline void
Shader_SetValue(
	ShaderSet   *shader_set,
	const char  *name,
	const float *buffer,
	u32 count
) {
	Assert((u64)shader_set->active_id < shader_set->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set->a_shaders, shader_set->active_id);

	s32 loc_id = glGetUniformLocation(shader_prog->id, name);

	if (loc_id >= 0) {
		switch (count) {
			case 3: {
				glProgramUniform3fv(shader_prog->id, loc_id, 1, buffer);
			} break;

			case 4: {
				glProgramUniform4fv(shader_prog->id, loc_id, 1, buffer);
			} break;

			default: {
				Assert(!"Unknown uniform passing parameter count.");
			} break;
		}
	}
}

inline void
Shader_SetValue(
	ShaderSet *shader_set,
	const char *name,
	int value
) {
	Assert((u64)shader_set->active_id < shader_set->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set->a_shaders, shader_set->active_id);

	s32 loc_id = glGetUniformLocation(shader_prog->id, name);

	if (loc_id >= 0)
		glProgramUniform1i(shader_prog->id, loc_id, value);
}

inline void
Shader_SetValue(
	ShaderSet *shader_set,
	const char *name,
	int value_a,
	int value_b
) {
	Assert((u64)shader_set->active_id < shader_set->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set->a_shaders, shader_set->active_id);

	s32 loc_id = glGetUniformLocation(shader_prog->id, name);

	if (loc_id >= 0) {
		glProgramUniform2i(shader_prog->id, loc_id, value_a, value_b);
	}
}

inline void
Shader_SetValue(
	ShaderSet *shader_set,
	const char *name,
	float value_a,
	float value_b
) {
	Assert((u64)shader_set->active_id < shader_set->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set->a_shaders, shader_set->active_id);

	s32 loc_id = glGetUniformLocation(shader_prog->id, name);

	if (loc_id >= 0) {
		glProgramUniform2f(shader_prog->id, loc_id, value_a, value_b);
	}
}

inline void
Shader_BindAndUseIndex0(
	ShaderSet *shader_set,
	const char *name,
	Texture *texture
) {
	Assert(shader_set);
	Assert(name);
	Assert(texture);

	glBindTexture(GL_TEXTURE_2D, texture->ID);
	Shader_SetValue(shader_set, name, 0);
}

inline void
ShaderSet_Use(
	ShaderSet *shader_set,
	SHADER_PROG_TYPE type
) {
	Assert(shader_set);
	Assert(shader_set->window);

	s64 prev_active_id = shader_set->active_id;

	switch (type) {
		case SHADER_PROG_RECT:
		case SHADER_PROG_TEXT:
		case SHADER_PROG_TEXTURE_FULL:
		case SHADER_PROG_TEXTURE_SIZE: {
			shader_set->active_id = type;
		} break;

		default: {
			AssertMessage(	false,
							"Unhandled Shader-Program type.");
		} break;
	}

	if (prev_active_id == shader_set->active_id)
		return;

	Assert((u64)shader_set->active_id < shader_set->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set->a_shaders, shader_set->active_id);

	glUseProgram(shader_prog->id);

	if (shader_set->window) {
		RectF viewport;
		Window *t_window = shader_set->window;

		viewport.x = t_window->x_viewport;
		viewport.y = t_window->y_viewport;
		viewport.w = (float)t_window->width;
		viewport.h = (float)t_window->height;

		Shader_SetValue(shader_set, "viewport", (float *)&viewport, 4);
		OpenGL_SetBlending(true);
	}
}

/// ::: Vertex / Rendering
/// ===========================================================================
template <typename T>
struct Vertex_Buffer {
	u32 id = 0;
	const char *name = 0;
	u32 group_count = 0;
	Array<T> a_buffer;
};

struct Vertex_Settings {
	bool  flip_h = false;
	float scale_x = 1.0f;
	float scale_y = 1.0f;
};

///@Hint: does not take texture ownership
///       so it will not be free'd when
///       destroyed
struct Vertex {
	u32 array_id = 0;
	Texture texture;
	Array<Vertex_Buffer<float>> a_attributes;
	Vertex_Settings settings;
};

bool
operator == (
	Vertex &v1,
	Vertex &v2
) {
	if (v1.texture.ID OR v2.texture.ID) {
		if (v1.texture == v2.texture) {
			return true;
		}

		return false;
	}

	if (v1.array_id == v2.array_id) {
		return true;
	}

	return false;
}

template <typename T>
bool
operator == (
	Vertex_Buffer<T> &b1,
	Vertex_Buffer<T> &b2
) {
	return String_IsEqual(b1.name, b2.name);
}

///@Hint: texture has to be bound first
instant void
Vertex_GetTextureSize(
	Vertex *vertex,
	s32 *width,
	s32 *height
) {
	Assert(vertex);

    if (width)  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH , width);
	if (height) glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height);
}

inline void
Vertex_Destroy(
	Vertex *vertex
) {
	Assert(vertex);

	FOR_ARRAY(vertex->a_attributes, it) {
		Vertex_Buffer<float> *t_attribute = &ARRAY_IT(vertex->a_attributes, it);
		glDeleteBuffers(1, &t_attribute->id);
		Array_DestroyContainer(&t_attribute->a_buffer);
	}

	glDeleteVertexArrays(1, &vertex->array_id);
	Array_DestroyContainer(&vertex->a_attributes);

	*vertex = {};
}

inline void
Vertex_SetTexture(
	ShaderSet *shader_set,
	Vertex *vertex,
	Texture *texture
) {
	Assert(shader_set);
	Assert(vertex);
	Assert(texture);

	vertex->texture = *texture;
	vertex->settings.flip_h = texture->flip_h;

	/// use this uniform in a shader, if the texture index has to change
	Shader_BindAndUseIndex0(shader_set, "fragment_texture", &vertex->texture);
}

instant void
Vertex_CreateStatic(
	Vertex *vertex
) {
	Assert(vertex);

	glGenVertexArrays(1, &vertex->array_id);
}

instant Vertex
Vertex_Create(
) {
	Vertex vertex = {};

	Vertex_CreateStatic(&vertex);

	return vertex;
}

instant Vertex
Vertex_Create(
	ShaderSet *shader_set,
	Texture *texture
) {
	Assert(shader_set);
	Assert(texture);

	Vertex vertex = {};

	if (texture->ID) {
		Vertex_SetTexture(shader_set, &vertex, texture);
	}

	glGenVertexArrays(1, &vertex.array_id);

	return vertex;
}

instant void
Vertex_Load(
	ShaderSet *shader_set,
	Vertex *vertex
) {
	Assert(shader_set);
	Assert(vertex);

	Assert((u64)shader_set->active_id < shader_set->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set->a_shaders, shader_set->active_id);

	FOR_ARRAY(vertex->a_attributes, it) {
		Vertex_Buffer<float> *entry = &ARRAY_IT(vertex->a_attributes, it);
		s32 attrib_position = glGetAttribLocation(shader_prog->id, entry->name);

		if (attrib_position < 0) {
			String s_error;
			String_Append(&s_error, "Shader and attributes mismatch.\n    Missing: ");
			String_Append(&s_error, entry->name);

			char *c_error_msg = String_CreateCBufferCopy(&s_error);

			AssertMessage(false, c_error_msg);

			Memory_Free(c_error_msg);

			continue;
		}

		glEnableVertexAttribArray(attrib_position);
		glBindBuffer(GL_ARRAY_BUFFER, entry->id);
		glVertexAttribPointer(
			attrib_position,
			entry->group_count,
			GL_FLOAT,
			false,
			0,
			0
		);
	}

	if (vertex->texture.ID) {
		Vertex_SetTexture(shader_set, vertex, &vertex->texture);
	}

}

inline void
Vertex_BindAttributes(
	ShaderSet *shader_set,
	Vertex *vertex
) {
	Assert(shader_set);
	Assert(vertex);
	Assert(vertex->a_attributes.count);

	FOR_ARRAY(vertex->a_attributes, it_buffer) {
		Vertex_Buffer<float> *t_buffer = &ARRAY_IT(vertex->a_attributes, it_buffer);

		if (!t_buffer->id)
			glGenBuffers(1, &t_buffer->id);

		glBindBuffer(GL_ARRAY_BUFFER, t_buffer->id);
		glBufferData(GL_ARRAY_BUFFER, t_buffer->a_buffer.size, t_buffer->a_buffer.memory, GL_DYNAMIC_DRAW);
	}

	Vertex_Load(shader_set, vertex);
}

instant void
Vertex_ClearAttributes(
	Vertex *vertex
) {
	Assert(vertex);

	FOR_ARRAY(vertex->a_attributes, it) {
		auto *t_attribute = &ARRAY_IT(vertex->a_attributes, it);
		Array_ClearContainer(&t_attribute->a_buffer);
	}
}

instant void
Vertex_ClearAttributes(
	Array<Vertex> *a_vertex
) {
	Assert(a_vertex);

	FOR_ARRAY(*a_vertex, it_vertex) {
		Vertex *t_vertex = &ARRAY_IT(*a_vertex, it_vertex);

		FOR_ARRAY(t_vertex->a_attributes, it_attrib) {
			auto *t_attribute = &ARRAY_IT(t_vertex->a_attributes, it_attrib);
			Array_ClearContainer(&t_attribute->a_buffer);
		}
	}
}

inline void
Vertex_Render(
	ShaderSet *shader_set,
	Vertex *vertex
) {
	Assert(shader_set);
	Assert(vertex);
	Assert(vertex->a_attributes.count);

	///@Hint: vertex positions have to be the first entry in the array
	Vertex_Buffer<float> *a_positions = &ARRAY_IT(vertex->a_attributes, 0);

	Assert(String_IsEqual(a_positions->name, "vertex_position"));

	AssertMessage(	vertex->array_id,
					"Vertex has not been created. Forgot to call Vertex_Create?");

	AssertMessage(	a_positions->id,
					"No Attributes found.\n    Forgot to bind the attributes?");

	Assert(a_positions->group_count);

	Shader_SetValue(shader_set, "flip_h" , vertex->settings.flip_h);
	Shader_SetValue(shader_set, "scale_x", vertex->settings.scale_x);
	Shader_SetValue(shader_set, "scale_y", vertex->settings.scale_y);

	glDrawArrays(GL_POINTS, 0, a_positions->a_buffer.size / sizeof(GLfloat) / a_positions->group_count);
}

instant void
Vertex_Render(
	ShaderSet *shader_set,
	Array<Vertex> *a_vertex
) {
	Assert(shader_set);
	Assert(a_vertex);

	FOR_ARRAY(*a_vertex, it) {
		Vertex *t_vertex = &ARRAY_IT(*a_vertex, it);
		Vertex_BindAttributes(shader_set, t_vertex);

		Vertex_Render(shader_set, t_vertex);
	}
}

instant void
Vertex_FindOrAdd(
	Array<Vertex> *a_vertex,
	Texture *texture_find,
	Vertex **entry
) {
	Assert(a_vertex);
	Assert(texture_find);
	Assert(entry);

	Vertex *t_vertex_entry;
	Vertex t_vertex_find;
	t_vertex_find.texture = *texture_find;

	if (!Array_FindOrAddEmpty(a_vertex, t_vertex_find, &t_vertex_entry))
		*t_vertex_entry = t_vertex_find;

	if (!t_vertex_entry->array_id)
		Vertex_CreateStatic(t_vertex_entry);

	*entry = t_vertex_entry;
}

instant void
Vertex_FindOrAddAttribute(
	Vertex *vertex,
	u32 group_count,
	const char *c_attribute_name,
	Vertex_Buffer<float> **a_buffer
) {
	Assert(vertex);
	Assert(c_attribute_name);
	Assert(a_buffer);

	Vertex_Buffer<float> *t_attribute_entry;
	Vertex_Buffer<float> t_attribute_find;
	t_attribute_find.name = c_attribute_name;
	t_attribute_find.group_count = group_count;

	if (!Array_FindOrAddEmpty(&vertex->a_attributes, t_attribute_find, &t_attribute_entry))
		*t_attribute_entry = t_attribute_find;

	*a_buffer = t_attribute_entry;
}

instant void
Vertex_AddTexturePosition(
	Vertex *vertex,
	float x,
	float y
) {
	Assert(vertex);

	Vertex_Buffer<float> *t_attribute;

	Vertex_FindOrAddAttribute(vertex, 2, "vertex_position", &t_attribute);
	Array_Add(&t_attribute->a_buffer, x);
	Array_Add(&t_attribute->a_buffer, y);
}

instant void
Vertex_AddRect32(
	Vertex *vertex,
	Rect rect,
	Color32 color
) {
	Assert(vertex);

	Vertex_Buffer<float> *t_attribute;

	Vertex_FindOrAddAttribute(vertex, 4, "vertex_position", &t_attribute);
	Array_Add(&t_attribute->a_buffer, (float)rect.x);
	Array_Add(&t_attribute->a_buffer, (float)rect.y);
	Array_Add(&t_attribute->a_buffer, (float)rect.x + rect.w);
	Array_Add(&t_attribute->a_buffer, (float)rect.y + rect.h);

	Vertex_FindOrAddAttribute(vertex, 4, "rect_color", &t_attribute);
	Array_Add(&t_attribute->a_buffer, (float)color.r);
	Array_Add(&t_attribute->a_buffer, (float)color.g);
	Array_Add(&t_attribute->a_buffer, (float)color.b);
	Array_Add(&t_attribute->a_buffer, (float)color.a);
}

instant void
Vertex_AddRectTexture(
	Vertex *vertex,
	Rect rect
) {
	Assert(vertex);

	Vertex_Buffer<float> *t_attribute;

	Vertex_FindOrAddAttribute(vertex, 4, "vertex_position", &t_attribute);
	Array_Add(&t_attribute->a_buffer, (float)rect.x);
	Array_Add(&t_attribute->a_buffer, (float)rect.y);
	Array_Add(&t_attribute->a_buffer, (float)rect.x + rect.w);
	Array_Add(&t_attribute->a_buffer, (float)rect.y + rect.h);
}

instant void
Rect_Render(
	ShaderSet *shader_set,
	Vertex *vertex
) {
	Assert(shader_set);
	Assert(shader_set->window);
	Assert(vertex);

	Vertex_BindAttributes(shader_set, vertex);

	Vertex_Render(shader_set, vertex);
}

/// ::: Mouse
/// ===========================================================================
#define MOUSE_WHEEL_GET_DELTA(value) GET_WHEEL_DELTA_WPARAM(value) / 120

#define MOUSE_BUTTON_COUNT 3

struct Mouse {
	Point point          = {};
	Point point_relative = {};

	bool up[MOUSE_BUTTON_COUNT];
	bool down[MOUSE_BUTTON_COUNT];
	bool pressing[MOUSE_BUTTON_COUNT];
	s32  wheel = 0;
	bool double_click[MOUSE_BUTTON_COUNT];

	bool is_up        = false;
	bool is_down      = false;
	bool is_moving    = false;
};

instant void
Mouse_Show(
) {
	ShowCursor(true);
}

instant void
Mouse_AutoHide(
	u32 msg_message = 0,
	u32 millisec = 2000
) {
    static Timer timer_mouse_move;
    static bool is_cursor_visible = true;

    /// reset timer on mouse move window event
    if (msg_message == WM_MOUSEMOVE) {
		Time_Reset(&timer_mouse_move);
		if (!is_cursor_visible) {
			is_cursor_visible = true;
			Mouse_Show();
		}
		return;
    }

    /// auto-hide mouse cursor after x millisec. of no mouse movement
	if (is_cursor_visible) {
		if (Time_Get() - timer_mouse_move.lo_timer >= millisec) {
			is_cursor_visible = false;
			ShowCursor(false);
		}
	}
}

instant void
Mouse_Reset(
	Mouse *mouse
) {
	if (!mouse)
		return;

	mouse->wheel = 0;

	FOR(MOUSE_BUTTON_COUNT, it) {
		mouse->double_click[it] = false;
		mouse->down[it] = false;
		mouse->up[it]   = false;
	}

	mouse->is_up        = false;
	mouse->is_down      = false;
	mouse->is_moving    = false;

	mouse->point_relative = {};
}

instant void
Mouse_GetPosition(
	float *x,
	float *y,
	Window *window
) {
	POINT point;
	GetCursorPos(&point);

	RECT rect_active;

	if (window) {
		GetWindowRect(window->hWnd, &rect_active);
		Window_UnAdjustRect(window->hWnd, &rect_active);

		RectF rect_viewport;
		glGetFloatv(GL_VIEWPORT, (GLfloat *)&rect_viewport);

		float scale_x = window->width  / rect_viewport.w;
		float scale_y = window->height / rect_viewport.h;

		point.x = (point.x - (rect_active.left + rect_viewport.x)) * scale_x;
		point.y = (point.y - (rect_active.top  + rect_viewport.y)) * scale_y;
	}


	if (x) *x = point.x;
	if (y) *y = point.y;
}

instant void
Mouse_GetPosition(
	Mouse *mouse,
	Window *window
) {
	Assert(window);
	Assert(mouse);

	RECT rect_active;
	GetWindowRect(window->hWnd, &rect_active);
	Window_UnAdjustRect(window->hWnd, &rect_active);

	Mouse t_mouse = *mouse;

	Mouse_GetPosition(&mouse->point.x, &mouse->point.y, 0);

	RectF rect_viewport;

	glGetFloatv(GL_VIEWPORT, (GLfloat *)&rect_viewport);

	float scale_x = window->width  / rect_viewport.w;
	float scale_y = window->height / rect_viewport.h;

	mouse->point.x = (mouse->point.x - (rect_active.left + rect_viewport.x)) * scale_x;
	mouse->point.y = (mouse->point.y - (rect_active.top  + rect_viewport.y)) * scale_y;

	mouse->point_relative.x = mouse->point.x - t_mouse.point.x;
	mouse->point_relative.y = mouse->point.y - t_mouse.point.y;
}

instant bool
Mouse_Update(
	Mouse *mouse,
	Window *window,
	MSG *msg
) {
	if (!mouse)
		return false;

	if (!msg)
		return false;

	Mouse_Reset(mouse);

	if (window AND msg->message == WM_MOUSEMOVE) {
		mouse->is_moving = true;
		Mouse_GetPosition(mouse, window);
		return true;
	}

    switch(msg->message) {
		case WM_LBUTTONDOWN: {
			mouse->pressing[0] = true;
			mouse->down[0] = true;
			mouse->is_down = true;
		} break;

		case WM_MBUTTONDOWN: {
			mouse->pressing[1] = true;
			mouse->down[1] = true;
			mouse->is_down = true;
		} break;

		case WM_RBUTTONDOWN: {
			mouse->pressing[2] = true;
			mouse->down[2] = true;
			mouse->is_down = true;
		} break;

		case WM_LBUTTONUP:   {
			mouse->pressing[0] = false;
			mouse->up[0] = true;
			mouse->is_up = true;
		} break;

		case WM_MBUTTONUP:   {
			mouse->pressing[1] = false;
			mouse->up[1] = true;
			mouse->is_up = true;
		} break;

		case WM_RBUTTONUP:   {
			mouse->pressing[2] = false;
			mouse->up[2] = true;
			mouse->is_up = true;
		} break;

		case WM_LBUTTONDBLCLK: {
			mouse->double_click[0] = true;
			mouse->is_down = true;
			mouse->is_up = true;
		} break;

		case WM_MBUTTONDBLCLK: {
			mouse->double_click[1] = true;
			mouse->is_down = true;
			mouse->is_up = true;
		} break;

		case WM_RBUTTONDBLCLK: {
			mouse->double_click[2] = true;
			mouse->is_down = true;
			mouse->is_up = true;
		} break;

		case WM_MOUSEWHEEL: {
			mouse->wheel = MOUSE_WHEEL_GET_DELTA(msg->wParam) * 16;
		} break;

		default: { return false; } break;
    }

    return true;
}

instant void
Mouse_AutoClickLeft(
) {
	/// auto left mouse click on current mouse position
	mouse_event(0x02, 0, 0, 0, 1);
	mouse_event(0x04, 0, 0, 0, 1);
}

/// ::: Keyboard
/// ===========================================================================
#define KEYBOARD_KEYCOUNT 0xFFFF

struct Keyboard {
    bool up[KEYBOARD_KEYCOUNT] 			= {};
    bool down[KEYBOARD_KEYCOUNT] 		= {};
    bool pressing[KEYBOARD_KEYCOUNT] 	= {};
    bool repeating[KEYBOARD_KEYCOUNT] 	= {};

    /// last_key_virtual = msg.wParam = SL_KEYBOARD_KEYCOUNT (max.)
	u32  last_key_virtual	= 0;
	u16  key_sym			= 0;		/// translated key (incl. shift + alt)
	u32  key_scan			= 0;		/// scancode
    bool is_down			= false;	/// "any" key is down
    bool is_up				= false;	/// "any" key is up
    bool is_key_sym			= false;	/// key could be translated
    u8   key_states[256]	= {};
};

/// reset last key pressed
/// to prevent continuous input events
instant void
Keyboard_ResetLastKey(
	Keyboard *keyboard
) {
	if (!keyboard)
		return;

	u32 vkey = keyboard->last_key_virtual;

	keyboard->down[vkey] 		= false;
	keyboard->up[vkey] 			= false;

	keyboard->is_down 			= false;
	keyboard->is_up 			= false;

	keyboard->is_key_sym 		= false;
	keyboard->key_sym 			= 0;
	keyboard->key_scan			= 0;
	keyboard->last_key_virtual	= 0;
}

instant void
Keyboard_Reset(
	Keyboard *keyboard,
	bool full_reset = false
) {
	Assert(keyboard);

	bool pressing[KEYBOARD_KEYCOUNT] = {};

	if (!full_reset)
		Memory_Copy(&pressing, &keyboard->pressing, sizeof(bool) * KEYBOARD_KEYCOUNT);

    *keyboard = {};

    if (!full_reset)
		Memory_Copy(&keyboard->pressing, &pressing, sizeof(bool) * KEYBOARD_KEYCOUNT);
}

instant void
Keyboard_GetKeySym(
	Keyboard *keyboard,
	MSG *msg
) {
	Assert(keyboard);
	Assert(msg);

	u16 ch = 0;
	GetKeyboardState(keyboard->key_states);

	keyboard->is_key_sym	= false;
	keyboard->key_sym		= 0;

	if (ToAscii(msg->wParam,
				MapVirtualKey(msg->wParam, 0),
				keyboard->key_states, &ch, 0) > 0)
	{
		if (ch == VK_ESCAPE)  return;

		keyboard->is_key_sym 	= true;
		keyboard->key_sym 		= ch; // (char)LOBYTE(ch);
	}
}

instant void
Keyboard_SetDown(
	Keyboard *keyboard,
	MSG *msg
) {
	Assert(keyboard);
	Assert(msg);

	keyboard->key_scan = MapVirtualKey(msg->wParam, 0);

	keyboard->down[msg->wParam] 		= true;
	keyboard->up[msg->wParam] 			= false;
	keyboard->pressing[msg->wParam] 	= true;
	keyboard->repeating[msg->wParam] 	= GETBIT(msg->lParam, 30);

	Keyboard_GetKeySym(keyboard, msg);

	keyboard->last_key_virtual 	= msg->wParam;
	keyboard->is_down 			= true;
	keyboard->is_up				= false;
}

instant void
Keyboard_SetUp(
	Keyboard *keyboard,
	MSG *msg
) {
	Assert(keyboard);
	Assert(msg);

	keyboard->key_scan = MapVirtualKey(msg->wParam, 0);
	keyboard->last_key_virtual 	= msg->wParam;

	if (keyboard->pressing[msg->wParam]) {
		keyboard->down[msg->wParam] 		= false;
		keyboard->up[msg->wParam] 			= true;
		keyboard->pressing[msg->wParam] 	= false;
		keyboard->repeating[msg->wParam] 	= false;

		Keyboard_GetKeySym(keyboard, msg);

		keyboard->is_down 			= false;
		keyboard->is_up				= true;
	}
}

instant bool
Keyboard_Update(
	Keyboard *keyboard,
	MSG *msg
) {
	bool result = false;

	if (!keyboard)
		return result;

	Assert(msg);

	switch (msg->message) {
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN: {
			Keyboard_SetDown(keyboard, msg);
			result = true;
		} break;

		case WM_SYSKEYUP:
		case WM_KEYUP: {
			Keyboard_SetUp(keyboard, msg);
			result = true;
		} break;
	}

	return result;
}

instant void
Keyboard_Insert(
	Keyboard *keyboard,
	String *s_data
) {
	Assert(keyboard);
	Assert(s_data);

	String_Insert(s_data, s_data->length, keyboard->key_sym);
}

instant bool
Keyboard_IsPressingAnyKey(
	Keyboard *keyboard
) {
	Assert(keyboard);

	bool result = false;

	FOR(KEYBOARD_KEYCOUNT, it) {
		if (keyboard->pressing[it]) {
			result = true;
			break;
		}
	}

	return result;
}

instant void
Keyboard_WaitForIdle(
	Keyboard *keyboard,
	bool *pending_event,
	u32 delay_in_ms = 400
) {
	Assert(keyboard);
	Assert(pending_event);

	if (*pending_event) {
		static Timer timer_input_delay;

		if (Time_HasElapsed(&timer_input_delay, delay_in_ms, true)) {
			if (!Keyboard_IsPressingAnyKey(keyboard)) {
				*pending_event = false;
			}
		}

		Keyboard_Reset(keyboard);
	}
}

/// ::: Joypad
/// ===========================================================================
#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000

#define XINPUT_ERROR_DEVICE_NOT_CONNECTED 0x048F

struct Joypad_State {
	u64     packet_number = 0;

    u16		button = 0;
    u8		trigger_left = 0;
    u8		trigger_right = 0;
    s16		thumb_left_x = 0;
    s16		thumb_left_y = 0;
    s16		thumb_right_x = 0;
    s16		thumb_right_y = 0;
};

typedef DWORD(WINAPI* proc_XInputGetState)(u64 joy_index, Joypad_State *state);

struct Joypad {
	HINSTANCE dll = 0;
	Joypad_State state = {};
	Joypad_State prev_state = {};

	proc_XInputGetState GetState = 0;
};

instant bool
Joypad_Init_XInput(
	Joypad *joypad
) {
	Assert(joypad);

    char sys_dir[MAX_PATH];

    u64 sys_dir_len = GetSystemDirectory(sys_dir, sizeof(sys_dir));
    if (!sys_dir_len OR sys_dir_len > sizeof(sys_dir))  return false;

    strcat(sys_dir, "\\xinput1_3.dll");

    joypad->dll = LoadLibrary(sys_dir);
    if (!joypad->dll)  return false;

    joypad->GetState = (proc_XInputGetState)GetProcAddress(joypad->dll, "XInputGetState");

	return true;
}

instant bool
Joypad_GetInput(
	Joypad *joypad,
	u64 joy_index
) {
	Assert(joypad);

	if (!joypad->dll)  return false;

	joypad->prev_state = joypad->state;

	u64 result = joypad->GetState(joy_index, &joypad->state);

	return (result != XINPUT_ERROR_DEVICE_NOT_CONNECTED);
}

///@Hint: test for != 0
///@Hint: "pressing" states

/// DPad
#define Joypad_IsPadUp(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_DPAD_UP)
#define Joypad_IsPadDown(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_DPAD_DOWN)
#define Joypad_IsPadLeft(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_DPAD_LEFT)
#define Joypad_IsPadRight(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_DPAD_RIGHT)

/// Center buttons
#define Joypad_IsStart(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_START)
#define Joypad_IsSelect(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_BACK)

/// Thumb button
#define Joypad_IsThumbLeft(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_LEFT_THUMB)
#define Joypad_IsThumbRight(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_RIGHT_THUMB)

/// Shoulder buttons
#define Joypad_IsShoulderLeft(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_LEFT_SHOULDER)
#define Joypad_IsShoulderRight(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_RIGHT_SHOULDER)

/// Control buttons
#define Joypad_IsA(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_A)
#define Joypad_IsB(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_B)
#define Joypad_IsX(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_X)
#define Joypad_IsY(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_Y)

///@Hint: "button up" states

/// Center buttons
#define Joypad_IsStart_UP(ptr_joypad) \
	(   (  (*ptr_joypad).prev_state.button & XINPUT_GAMEPAD_START) \
	 && (!((*ptr_joypad).state.button      & XINPUT_GAMEPAD_START)))

#define Joypad_IsSelect_UP(ptr_joypad) \
	(   (  (*ptr_joypad).prev_state.button & XINPUT_GAMEPAD_BACK) \
	 && (!((*ptr_joypad).state.button      & XINPUT_GAMEPAD_BACK)))

/// Control buttons
#define Joypad_IsA_Up(ptr_joypad) \
	(   (  (*ptr_joypad).prev_state.button & XINPUT_GAMEPAD_A) \
	 && (!((*ptr_joypad).state.button      & XINPUT_GAMEPAD_A)))

#define Joypad_IsB_Up(ptr_joypad) \
	(   (  (*ptr_joypad).prev_state.button & XINPUT_GAMEPAD_B) \
	 && (!((*ptr_joypad).state.button      & XINPUT_GAMEPAD_B)))

#define Joypad_IsX_Up(ptr_joypad) \
	(   (  (*ptr_joypad).prev_state.button & XINPUT_GAMEPAD_X) \
	 && (!((*ptr_joypad).state.button      & XINPUT_GAMEPAD_X)))

#define Joypad_IsY_Up(ptr_joypad) \
	(   (  (*ptr_joypad).prev_state.button & XINPUT_GAMEPAD_Y) \
	 && (!((*ptr_joypad).state.button      & XINPUT_GAMEPAD_Y)))

/// for axis
/// Returns: current section

/// @Suggestion: deadzone = 8000
instant s32
Joypad_GetSection(
	s16 axis_value,
	u16 deadzone,
	u16 section_max
) {
	if (axis_value <= deadzone AND axis_value >= -deadzone)  return 0;

	if (section_max > 0) {
		constexpr s16 limit = 32767;

		if (axis_value < 0) axis_value += deadzone;
		else                axis_value -= deadzone;

		float steps = (float)((limit - deadzone) / section_max);

		return (axis_value / steps);
	}

	return 0;
}

/// for triggers
/// Returns: current section
instant u8
Joypad_GetSection(
	u8  axis_value,
	u16 deadzone,
	u16 section_max
) {
	if (axis_value <= deadzone AND axis_value >= -deadzone)  return 0;

	if (section_max > 0) {
		constexpr s64 limit = 255;

		if (axis_value < 0) axis_value += deadzone;
		else                axis_value -= deadzone;

		float steps = (float)((limit - deadzone) / section_max);
		return axis_value / steps;
	}

	return 0;
}

/// ::: Font (TrueType)
/// ===========================================================================
struct Font;

struct Codepoint {
	Font *font = 0;
	s32 codepoint = 0;
	s32 advance = 0;
	s32 left_side_bearing = 0;
	RectI rect_subpixel = {};
	Texture texture = {};
};

struct Font {
	stbtt_fontinfo info = {};
	String s_data;
	s32 size = 0;
	s32 ascent = 0;
	s32 descent = 0;
	s32 linegap = 0;
	bool filter_linear = false;
	Array<Codepoint> a_codepoint;
};

bool
operator == (
	Codepoint &cp_1,
	Codepoint &cp_2
) {
	if (cp_1.codepoint == cp_2.codepoint){
		return true;
	}

	return false;
}

instant Font
Font_Load(
	String *s_file,
	u32 size
) {
	Assert(s_file);

    Font font = {};
    String s_font_data = File_ReadAll(s_file->value, s_file->length, true);

    if (!s_font_data.length) {
		char *c_filename = String_CreateCBufferCopy(s_file);

		LOG_ERROR("Font \"" << c_filename << "\" does not exists.");
    }
    else {
		font.s_data = s_font_data;
		font.size = size;

		const u8 *c_data = (u8 *)font.s_data.value;
		stbtt_InitFont(&font.info, c_data, stbtt_GetFontOffsetForIndex(c_data, 0));

		float scale = stbtt_ScaleForPixelHeight(&font.info, font.size);

		stbtt_GetFontVMetrics(&font.info, &font.ascent, &font.descent, &font.linegap);
		font.ascent  *= scale;
		font.descent *= scale;
		font.linegap *= scale;
    }

    return font;
}

instant void Codepoint_Destroy(Codepoint *);

instant void
Font_Destroy(
	Font *font
) {
	Assert(font);

	FOR_ARRAY(font->a_codepoint, it) {
		Codepoint *t_codepoint = &ARRAY_IT(font->a_codepoint, it);
        Codepoint_Destroy(t_codepoint);
	}

	String_Destroy(&font->s_data);

	*font = {};
}

instant s32
Font_GetLineHeight(
	Font *font
) {
	Assert(font);

	return (font->ascent - font->descent + font->linegap + 1);
}

/// ::: Font (TrueType) ::: Codepoint
/// ===========================================================================
instant Texture
Codepoint_ToTexture(
	Font *font,
	s32 codepoint
) {
	Assert(font);

	Texture result = {};

	s32 w, h, x_off, y_off;

	float scale = stbtt_ScaleForPixelHeight(&font->info, font->size);
	unsigned char *bitmap = stbtt_GetCodepointBitmap(
			&font->info,
			0,
			scale,
			codepoint,
			&w,
			&h,
			&x_off,
			&y_off
		);

	if (bitmap) {
		result = Texture_Load(bitmap, w, h, GL_ALPHA, font->filter_linear);
		free(bitmap);
	}

	return result;
}

instant void
Codepoint_GetData(
	Font *font,
	s32 codepoint,
	Codepoint *entry
) {
	Assert(font);
	Assert(entry);

    float scale = stbtt_ScaleForPixelHeight(&font->info, font->size);

    u64 t_index_find;

    Codepoint t_codepoint_find;
    t_codepoint_find.font      = font;
    t_codepoint_find.codepoint = codepoint;

    Codepoint *t_entry;

    if (!Array_FindOrAddEmpty(&font->a_codepoint, t_codepoint_find, &t_entry)) {
		/// get texture
		t_entry->texture = Codepoint_ToTexture(font, codepoint);

		/// get advance / left side bearing
		stbtt_GetCodepointHMetrics(&font->info,
									codepoint,
									&t_entry->advance,
									&t_entry->left_side_bearing);

		t_entry->advance *= scale;
		t_entry->left_side_bearing *= scale;

		/// get subpixel
		stbtt_GetCodepointBitmapBoxSubpixel(&font->info,
											codepoint,
											scale,
											scale,
											0,
											0,
											&t_entry->rect_subpixel.x,
											&t_entry->rect_subpixel.y,
											&t_entry->rect_subpixel.w,
											&t_entry->rect_subpixel.h);
    }

	*entry = *t_entry;
}

instant void
Codepoint_GetPositionNext(
	Codepoint *codepoint,
	RectF *rect
) {
	Assert(codepoint);
	Assert(codepoint->font);
	Assert(rect);

	/// rect_x: starting position for each line
	///         to reset -> set 0 or x-offset
	/// rect_h: lower end baseline for text drawing
	///         increasing it will skip to the next line
	///         has to get the value from itself, or it
	///         will reset to 0
	/// rect_w: store advance of prev codepoint to
	///         set the correct start position for the
	///         next codepoint

	rect->x = rect->x + rect->w + codepoint->left_side_bearing;
	rect->y = rect->h + codepoint->rect_subpixel.y + codepoint->font->size + codepoint->font->descent;
	rect->w = (float)codepoint->advance - codepoint->left_side_bearing;
}

instant s32
Codepoint_GetAdvance(
	Font *font,
	s32 codepoint
) {
	Assert(font);

	float scale = stbtt_ScaleForPixelHeight(&font->info, font->size);

	s32 advance;
	stbtt_GetCodepointHMetrics(&font->info, codepoint, &advance, 0);
	advance *= scale;

	return advance;
}


instant u64
Codepoint_GetStringAdvance(
	Font *font,
	String *s_data
) {
	Assert(font);
	Assert(s_data);

	u64 advance_word = 0;

	FOR(s_data->length, it) {
		char ch = s_data->value[it];
		advance_word += Codepoint_GetAdvance(font, ch);
	}

	return advance_word;
}

instant void
Codepoint_SetNewline(
	Font *font,
	RectF *rect_position,
	float x_offset_start = 0
) {
	Assert(font);
	Assert(rect_position);

	rect_position->h += Font_GetLineHeight(font);
	rect_position->x = x_offset_start;
	rect_position->w = 0;
}

instant void
Codepoint_Destroy(
	Codepoint *codepoint
) {
	Assert(codepoint);
	Assert(codepoint->font);

	Texture_Destroy(&codepoint->texture);
}


/// ::: Text (OpenGL rendering)
/// ===========================================================================
enum TEXT_ALIGN_X_TYPE {
	TEXT_ALIGN_X_LEFT,
	TEXT_ALIGN_X_MIDDLE,
	TEXT_ALIGN_X_RIGHT
};

enum TEXT_ALIGN_Y_TYPE {
	TEXT_ALIGN_Y_TOP,
	TEXT_ALIGN_Y_CENTER,
	TEXT_ALIGN_Y_BOTTOM
};

struct Text_Line {
	u64 width_pixel;
	String s_data;
};

struct Text_Cursor {
	Point point_select_start = {};
	Point point_select_end   = {};

	Color32 color_cursor = {1.0f, 0.0f, 0.0f, 1.0f};
	Color32 color_select = {0.5f, 0.5f, 1.0f, 1.0f};

	u32 blink_inverval_ms = 500;
	Timer timer_blinking;
	bool is_blink_on = false;
	bool show_cursor = false;

	Vertex vertex_select;
	Vertex vertex_cursor;
};

struct Text_Data {
	Rect rect 			= {}; /// draw area
	Rect rect_padding 	= {};
	Rect rect_margin    = {};
	Color32 color 		= {1.0f, 0, 0, 1.0f};

	TEXT_ALIGN_X_TYPE align_x = TEXT_ALIGN_X_LEFT;
	TEXT_ALIGN_Y_TYPE align_y = TEXT_ALIGN_Y_TOP;

	Rect rect_content = {}; /// offset x/y, max width / height

	bool is_editable   = false;
	bool use_word_wrap = true;
};

struct Text {
	ShaderSet *shader_set = 0;
	Font *font = 0;
	String s_data = {};

	Text_Data data;
	Text_Data data_prev;

	Text_Cursor   cursor;

	Array<String>    as_words;
	Array<Text_Line> a_text_lines;
	Array<Vertex>    a_vertex_chars;
};

instant Text
Text_Create(
	ShaderSet *shader_set,
	Font *font,
	String *s_data,
	Rect rect,
	TEXT_ALIGN_X_TYPE align_x
) {
	Assert(shader_set);
	Assert(font);

	Text text;

	text.shader_set = shader_set;
	text.font       = font;

	text.data.rect		= rect;
	text.data.align_x	= align_x;

	if (s_data)
		String_Append(&text.s_data, s_data->value, s_data->length);

	return text;
}

instant void
Text_Destroy(
	Text *text
) {
	Assert(text);

	String_Destroy(&text->s_data);
}

instant bool
Text_HasChanged(
	Text *text
) {
	Assert(text);

	bool has_changed = text->s_data.changed;

	has_changed |= !Memory_Compare(&text->data, &text->data_prev, sizeof(text->data));

	return has_changed;
}

instant bool
Text_HasChanged(
	Array<String> *as_data
) {
	Assert(as_data);

	bool anything_changed = false;

	FOR_ARRAY(*as_data, it) {
		String *ts_data = &ARRAY_IT(*as_data, it);

		if (ts_data->changed) {
			anything_changed = true;
			break;
		}
	}

	return anything_changed;
}

instant s32
Text_BuildLinesStatic(
	Text *text,
	Array<String> *as_words,
	Array<Text_Line> *a_text_line
) {
	Assert(text);
	Assert(text->font);
	Assert(as_words);
	Assert(a_text_line);

	Font *font = text->font;
	Rect  rect = text->data.rect;

	Rect_AddPadding(&rect, text->data.rect_padding);

	FOR_ARRAY(*a_text_line, it_line) {
		Text_Line *t_text_line = &ARRAY_IT(*a_text_line, it_line);
		String_Destroy(&t_text_line->s_data);
		t_text_line->width_pixel = 0;
	}
	Array_ClearContainer(a_text_line);

	Assert(a_text_line->count == 0);

	s32 height_max  = 0;
	s32 height_line = Font_GetLineHeight(font);
	bool line_start = true;

	u64 advance_space = Codepoint_GetAdvance(font, ' ');

	if (as_words->count)
		height_max += height_line;

	Rect rect_line_current = {rect.x, rect.y, 0, 0};

	Text_Line *t_text_line;

	if (as_words->count)
		Array_AddEmpty(a_text_line, &t_text_line);

    FOR_ARRAY(*as_words, it_words) {
		String *ts_word = &ARRAY_IT(*as_words, it_words);

		u64 advance_word = Codepoint_GetStringAdvance(font, ts_word);

		if (!line_start) {
			rect_line_current.x      += advance_space;
			t_text_line->width_pixel += advance_space;
			String_Append(&t_text_line->s_data, " ", 1);
		}

		/// word wrap
		if (text->data.use_word_wrap AND !line_start AND rect.w > 0 AND (rect_line_current.x - rect.x) + advance_word > rect.w) {
			Array_AddEmpty(a_text_line, &t_text_line);
			line_start = true;

			rect_line_current.x  = rect.x;
			rect_line_current.y += height_line;

			height_max += height_line;
		}

		if (String_EndWith(ts_word, "\n")) {
			t_text_line->width_pixel += advance_word;
			String_Append(&t_text_line->s_data, ts_word->value, ts_word->length);

			Array_AddEmpty(a_text_line, &t_text_line);
			line_start = true;

			rect_line_current.x  = rect.x;
			rect_line_current.y += height_line;

			height_max += height_line;

			continue;
		}

		rect_line_current.x      += advance_word;
		t_text_line->width_pixel += advance_word;
		String_Append(&t_text_line->s_data, ts_word->value, ts_word->length);

		line_start = false;
    }

	return height_max;
}

instant void
Text_Cursor_SetSelection(
	Text *text,
	Point point_start,
	Point point_end
) {
	bool is_adding_selection = false;
	bool found_start  = false;
	bool found_end    = false;
	bool found_cursor = false;

	const s32 width_cursor = 2;

	if (!text->data.is_editable)
		return;

	Text_Cursor *cursor = &text->cursor;

	Rect rect = text->data.rect;
	Rect_AddPadding(&rect, text->data.rect_padding);

	u64 width_max = rect.w;

	if (!width_max) {
		FOR_ARRAY(text->a_text_lines, it_line) {
			Text_Line *t_text_line = &ARRAY_IT(text->a_text_lines, it_line);

			width_max = MAX(width_max, t_text_line->width_pixel);
		}
	}

	Rect rect_position_start = {
		rect.x + text->data.rect_content.x,
		rect.y + text->data.rect_content.y,
		0,
		Font_GetLineHeight(text->font)
	};

	Rect rect_position_it    = rect_position_start;
	Rect rect_position_end   = rect_position_it;

	point_start.x += text->data.rect_content.x;
	point_start.y += text->data.rect_content.y;
	point_end.x   += text->data.rect_content.x;
	point_end.y   += text->data.rect_content.y;

	if (!cursor->vertex_select.array_id)
		cursor->vertex_select = Vertex_Create();

	Vertex_ClearAttributes(&cursor->vertex_select);

	Codepoint codepoint_space;
	Codepoint_GetData(text->font, ' ', &codepoint_space);

	FOR_ARRAY(text->a_text_lines, it_line) {
		Text_Line *t_text_line = &ARRAY_IT(text->a_text_lines, it_line);

		u64 it_data = 0;
		u64 x_align_offset = 0;

		if (width_max > t_text_line->width_pixel) {
			if (     text->data.align_x == TEXT_ALIGN_X_MIDDLE)
				x_align_offset = (width_max - t_text_line->width_pixel) >> 1;
			else if (text->data.align_x == TEXT_ALIGN_X_RIGHT)
				x_align_offset = (width_max - t_text_line->width_pixel);
		}

		rect_position_it.x += x_align_offset;

		bool is_line_end    = false;
		bool added_line_end = false;

		while(it_data < t_text_line->s_data.length) {
			Codepoint codepoint;

			s8 ch = t_text_line->s_data.value[it_data];

			Codepoint_GetData(text->font, ch, &codepoint);
			rect_position_it.w = codepoint.advance;

			/// makes line-breaks selectable
			if ((ch == '\r' OR ch == '\n')) {
				rect_position_it.w = width_max - (rect_position_it.w - rect_position_it.x);
				is_line_end = true;
			}

			if (!found_start AND Rect_IsIntersecting(&point_start, &rect_position_it)) {
				found_start = true;
			}

			if (!found_end AND Rect_IsIntersecting(&point_end, &rect_position_it)) {
				found_end = true;
			}

			if ((found_start AND !found_end) OR (!found_start AND found_end)) {
				if (is_line_end) {
					/// do not show multible ' ' in case of '\r\n'
					rect_position_it.w = (added_line_end ? 0 : codepoint_space.advance);
					added_line_end = true;
				}

				Vertex_AddRect32(&cursor->vertex_select, rect_position_it, cursor->color_select);
			}

			if (found_end AND !found_cursor) {
				if (!cursor->vertex_cursor.array_id)
					cursor->vertex_cursor = Vertex_Create();

				Vertex_ClearAttributes(&cursor->vertex_cursor);

				Rect rect_cursor = rect_position_it;
				rect_cursor.w = width_cursor;

				Vertex_AddRect32(&cursor->vertex_cursor, rect_cursor, cursor->color_cursor);

				found_cursor = true;
			}

			++it_data;

			rect_position_it.x += rect_position_it.w;

			rect_position_end = rect_position_it;
		}

		rect_position_it.x  = rect.x;
		rect_position_it.y += rect_position_it.h;
	}

	/// render starting or truncated cursor position
	if (!found_cursor) {
		if (!cursor->vertex_cursor.array_id)
			cursor->vertex_cursor = Vertex_Create();

		Vertex_ClearAttributes(&cursor->vertex_cursor);

		Rect rect_cursor = (!found_end ? rect_position_start : rect_position_end );

		rect_cursor.w = width_cursor;

		Vertex_AddRect32(&cursor->vertex_cursor, rect_cursor, cursor->color_cursor);
	}
}

instant void
Text_AddLines(
	Text *text,
	Array<Vertex>    *a_vertex_chars,
	Array<Text_Line> *a_text_lines,
	bool include_offsets = true
) {
	Assert(text->shader_set);
	Assert(text->font);
	Assert(a_text_lines);

	Rect rect = text->data.rect;

	Rect_AddPadding(&rect, text->data.rect_margin);
	Rect_AddPadding(&rect, text->data.rect_padding);

	u64 width_max = rect.w;

	if (!width_max) {
		FOR_ARRAY(*a_text_lines, it_line) {
			Text_Line *t_text_line = &ARRAY_IT(*a_text_lines, it_line);

			width_max = MAX(width_max, t_text_line->width_pixel);
		}
	}

	RectF rect_position = {	rect.x, 0, 0, rect.y};

	if (include_offsets) {
		rect_position.x += text->data.rect_content.x;
		rect_position.h += text->data.rect_content.y;
	}

	bool has_cursor = text->data.is_editable;

	if (has_cursor)
		Vertex_ClearAttributes(&text->cursor.vertex_select);

	u64 it_index = 0;

	FOR_ARRAY(*a_text_lines, it_line) {
		Text_Line *t_text_line = &ARRAY_IT(*a_text_lines, it_line);

		u64 it_data = 0;

		while(it_data < t_text_line->s_data.length) {
			Codepoint codepoint;

			s8 ch = t_text_line->s_data.value[it_data];

			Codepoint_GetData(text->font, ch, &codepoint);
			Codepoint_GetPositionNext(&codepoint, &rect_position);

			u64 x_align_offset = 0;

			if (width_max > t_text_line->width_pixel) {
				if (     text->data.align_x == TEXT_ALIGN_X_MIDDLE)
					x_align_offset = (width_max - t_text_line->width_pixel) >> 1;
				else if (text->data.align_x == TEXT_ALIGN_X_RIGHT)
					x_align_offset = (width_max - t_text_line->width_pixel);
			}

			/// for unavailable characters like ' '
			if (!Texture_IsEmpty(&codepoint.texture)) {
				Vertex *t_vertex;
				Vertex_FindOrAdd(a_vertex_chars, &codepoint.texture, &t_vertex);

				Vertex_Buffer<float> *t_attribute;

				Vertex_FindOrAddAttribute(t_vertex, 2, "vertex_position", &t_attribute);
				Array_Add(&t_attribute->a_buffer, rect_position.x + x_align_offset);
				Array_Add(&t_attribute->a_buffer, rect_position.y);

				Vertex_FindOrAddAttribute(t_vertex, 3, "text_color", &t_attribute);
				Array_Add(&t_attribute->a_buffer, text->data.color.r);
				Array_Add(&t_attribute->a_buffer, text->data.color.g);
				Array_Add(&t_attribute->a_buffer, text->data.color.b);
			}

			++it_data;
			++it_index;
		}

		Codepoint_SetNewline(text->font, &rect_position, rect.x);
	}
}

instant void
Text_AddLines(
	Text *text,
	bool include_offsets = true
) {
	Assert(text);

	Text_AddLines(text, &text->a_vertex_chars, &text->a_text_lines, include_offsets);
}

instant void
Text_Clear(
	Text *text
) {
	Assert(text);

	Vertex_ClearAttributes(&text->a_vertex_chars);
}

instant void
Text_RenderLines(
	Text *text
) {
	Assert(text);

	Vertex_Render(text->shader_set, &text->a_vertex_chars);
}

instant bool
Text_Update(
	Text *text
) {
	Assert(text);

	/// redraw text
	if (Text_HasChanged(text)) {
		String_SplitWordsStatic(&text->s_data, &text->as_words);
		s32 text_height = Text_BuildLinesStatic(text, &text->as_words, &text->a_text_lines);

		if (text->data.rect.h) {
			if (     text->data.align_y == TEXT_ALIGN_Y_CENTER)
				text->data.rect_content.y = (text->data.rect.h - text_height) >> 1;
			else if (text->data.align_y == TEXT_ALIGN_Y_BOTTOM)
				text->data.rect_content.y = (text->data.rect.h - text_height);
		}

		text->data.rect_content.h = text_height;
		text->data.rect_content.w = 0;

		FOR_ARRAY(text->a_text_lines, it_line) {
			Text_Line *t_line = &ARRAY_IT(text->a_text_lines, it_line);
			text->data.rect_content.w = MAX(text->data.rect_content.w, (s64)t_line->width_pixel);
		}

		Text_Clear(text);
		Text_AddLines(text);

		Text_Cursor_SetSelection(text,
								 text->cursor.point_select_start,
								 text->cursor.point_select_end);

		Rect_Clamp(&text->data.rect_content, text->data.rect);

		text->s_data.changed = false;
		text->data_prev = text->data;

		return true;
	}

	return false;
}

instant bool
Vertex_IsEmpty(
	Vertex *vertex
) {
	Assert(vertex);

	bool result = (vertex->a_attributes.count == 0);

	/// "vertex_position"
	Vertex_Buffer<float> *t_attribute = &ARRAY_IT(vertex->a_attributes, 0);

	result |= (t_attribute->a_buffer.count == 0);

	return result;
}

instant void
Text_Render(
	Text *text,
	bool auto_update = true
) {
	Assert(text);

	bool is_fixed_size = (text->data.rect.w OR text->data.rect.h);

	if (auto_update)
		Text_Update(text);

	if (is_fixed_size)
		OpenGL_Scissor(text->shader_set->window, text->data.rect);

	/// redraw selection
	if (text->data.is_editable AND text->cursor.vertex_select.a_attributes.count) {
		ShaderSet_Use(text->shader_set, SHADER_PROG_RECT);
		Rect_Render(text->shader_set, &text->cursor.vertex_select);
	}

	/// redraw cursor
	if (text->cursor.show_cursor AND text->data.is_editable AND text->cursor.vertex_cursor.a_attributes.count) {
		if (Time_HasElapsed(&text->cursor.timer_blinking, text->cursor.blink_inverval_ms)) {
			text->cursor.is_blink_on = !text->cursor.is_blink_on;
		}

		if (text->cursor.is_blink_on) {
			AssertMessage(!Vertex_IsEmpty(&text->cursor.vertex_cursor), "Cursor vertex data does not exists.");

			ShaderSet_Use(text->shader_set, SHADER_PROG_RECT);
			Rect_Render(text->shader_set, &text->cursor.vertex_cursor);
		}
	}

	if (text->a_vertex_chars.count) {
		/// redraw last computed text
		ShaderSet_Use(text->shader_set, SHADER_PROG_TEXT);
		Vertex_Render(text->shader_set, &text->a_vertex_chars);
	}

	if (is_fixed_size)
		OpenGL_Scissor_Disable();
}

instant void
Text_GetSize(
	Text *text,
	s32 *width,
	s32 *height
) {
	Assert(text);

	/// seperate storage, so independent text container keep
	/// their already processed data
	static Array<String>    as_words;
	static Array<Text_Line> a_text_lines;

	if (height) {
		String_SplitWordsStatic(&text->s_data, &as_words);
		*height = Text_BuildLinesStatic(text, &as_words, &a_text_lines);
	}

	IF_SET(width)  = text->data.rect.w;
}

/// ::: LAYOUT
/// ===========================================================================
#define LAYOUT_BLOCK_PADDING 4
#define LAYOUT_BLOCK_SPACING 2

#define LAYOUT_PADDING 5

enum LAYOUT_TYPE {
	LAYOUT_TYPE_X,
	LAYOUT_TYPE_Y
};

enum LAYOUT_DOCK_TYPE {
	LAYOUT_DOCK_TOPLEFT,
	LAYOUT_DOCK_BOTTOMRIGHT
};

struct Layout_Data_Settings {
	Rect rect = {};
	bool auto_width  = false;
	bool auto_height = false;
};

struct Layout_Data {
	Layout_Data_Settings settings;
	Layout_Data_Settings settings_prev;
};

struct Layout_Block {
	LAYOUT_TYPE type;
	LAYOUT_DOCK_TYPE dock;
	s32 expand_index = -1;
	u32 padding = LAYOUT_BLOCK_PADDING;
	u32 spacing = LAYOUT_BLOCK_SPACING;
	Array<Layout_Data *> ap_layout_data;
};

struct Widget;

struct Layout {
	Rect rect_full;
	Rect rect_remaining;
	bool fill_last_block = true;
	u32  padding = LAYOUT_PADDING;
	Array<Layout_Block> a_blocks;
};

instant void
Layout_Create(
	Layout *layout,
	Rect rect_area,
	bool fill_last_block
) {
	Assert(layout);
	AssertMessage(!layout->a_blocks.count, "Layout already created. Layout_Resize might help.");

	*layout = {};
	layout->rect_full       = rect_area;
	layout->rect_remaining  = rect_area;
	layout->fill_last_block = fill_last_block;
}

instant void
Layout_Create(
	Layout *layout,
	Layout_Data *layout_data,
	bool fill_last_block
) {
	Assert(layout);
	Assert(layout_data);

	Rect rect_area = layout_data->settings.rect;

	Layout_Create(layout, rect_area, fill_last_block);
}

instant void
Layout_CreateBlock(
	Layout *layout,
	LAYOUT_TYPE type,
	LAYOUT_DOCK_TYPE dock_direction,
	s32 expand_index = -1,
	Layout_Block **layout_block = 0
) {
	Assert(layout);

	Layout_Block *t_block;

	Array_AddEmpty(&layout->a_blocks, &t_block);

	t_block->type = type;
	t_block->dock = dock_direction;
	t_block->expand_index = expand_index;

	if (layout_block)
		*layout_block = t_block;
}

instant void
Layout_GetLastBlock(
	Layout *layout,
	Layout_Block **layout_block
) {
	Assert(layout);

	if (!layout->a_blocks.count) {
		AssertMessage(false, "GetLastBlock: No blocks in layout found.");
		return;
	}

	*layout_block = &ARRAY_IT(layout->a_blocks, layout->a_blocks.count - 1);
}

instant void
Layout_Add(
	Layout *layout,
	Layout_Data *layout_data
) {
	Assert(layout);
	Assert(layout_data);

	Layout_Block *current_block;
	Layout_GetLastBlock(layout, &current_block);

	Array_Add(&current_block->ap_layout_data, layout_data);
}

struct Widget;

instant void
Layout_Add(Layout *layout, Widget *widget);

/// layout_block->expand_index (widgets):
///  0-max: expand widget with matching index
///     -1: balanced size (with auto-size)
///     -2: left-align (no auto-size)
///     -3: center-align (no auto-size)
///     -4: right-align (no auto-size)
///   else: see -1
instant void
Layout_ArrangeBlockX(
	Layout *layout,
	Layout_Block *layout_block
) {
	Assert(layout);
	Assert(layout_block);
	Assert(layout_block->type == LAYOUT_TYPE_X);

	Layout_Block *t_block_last = 0;
	Layout_GetLastBlock(layout, &t_block_last);

	bool is_last_block = (t_block_last == layout_block);

	s64 widget_count = layout_block->ap_layout_data.count;

	if (!widget_count)
		return;

	s32 padding_size = layout_block->padding;

	if (padding_size % 2 != 0)  ++padding_size;

	s32 padding_border   = padding_size >> 1;
	s32 width_remaining  = layout->rect_remaining.w;

	float it_x = padding_border;
	float it_y = padding_border;

	bool is_overwriting = (	    layout_block->expand_index >= -4
							AND layout_block->expand_index != -1
							AND layout_block->expand_index < widget_count);

	s32 block_height = 0;
	u64 widget_count_auto = widget_count;

	/// pre-calc height for bottom alignment
	FOR_ARRAY(layout_block->ap_layout_data, it_block) {
		Layout_Data *t_data = ARRAY_IT(layout_block->ap_layout_data, it_block);

		Rect *rect = &t_data->settings.rect;

		bool found_expander = ((s64)it_block == layout_block->expand_index);

		block_height  = MAX(block_height, rect->h);

		if (is_overwriting) {
			if (!found_expander) {
				width_remaining -= rect->w;
				--widget_count_auto;
			}
		}
		else if (!t_data->settings.auto_width) {
			width_remaining -= rect->w;
			--widget_count_auto;
		}
	}

    width_remaining -= padding_size;
	width_remaining -= layout_block->spacing * (widget_count - 1);

    block_height += padding_size;

    switch (layout_block->expand_index) {
    	case -3: {
    		it_x = (width_remaining + padding_border) >> 1;
    	} break;

    	case -4: {
    		it_x = (width_remaining + padding_border);
		} break;

		default: break;
    }

    /// start drawing at the bottom
	if (layout_block->dock == LAYOUT_DOCK_BOTTOMRIGHT) {
		if (!is_last_block OR (is_last_block AND !layout->fill_last_block)) {
			it_y += layout->rect_remaining.h - block_height;
		}
	}

	float width_avg_auto = 0;

	if (widget_count_auto)
		width_avg_auto = ceil((float)width_remaining / widget_count_auto);

	/// align horizontal
    FOR_ARRAY(layout_block->ap_layout_data, it_block) {
		Layout_Data *t_data = ARRAY_IT(layout_block->ap_layout_data, it_block);

		Rect *rect = &t_data->settings.rect;

		/// center widgets in block
		s32 center_block = (rect->h - (block_height - padding_size)) >> 1;

		rect->x = layout->rect_remaining.x + it_x;
		rect->y = layout->rect_remaining.y + it_y - center_block;

		if (layout->fill_last_block AND is_last_block)
			rect->h = layout->rect_remaining.h - padding_size;

		bool found_expander = ((s64)it_block == layout_block->expand_index);

		float x_step = rect->w;

		if (is_overwriting) {
			if (found_expander)
				x_step = width_avg_auto;
		}
		else {
			if (t_data->settings.auto_width)
				x_step = width_avg_auto;
		}

		float width_limit = layout->rect_remaining.w - it_x;

		if (x_step > width_limit)
			x_step = width_limit;

		rect->w = x_step;
		it_x += x_step + layout_block->spacing;
    }

    /// cut of the top
	if (layout_block->dock == LAYOUT_DOCK_TOPLEFT)
		layout->rect_remaining.y += block_height;

    layout->rect_remaining.h -= block_height;
}

/// layout_block->expand_index (widgets):
///  0-max: expand widget with matching index
///     -1: balanced size (with auto-size)
///     -2: top-align (no auto-size)
///     -3: middle-align (no auto-size)
///     -4: bottom-align (no auto-size)
///   else: see -1
instant void
Layout_ArrangeBlockY(
	Layout *layout,
	Layout_Block *layout_block
) {
	Assert(layout);
	Assert(layout_block);
	Assert(layout_block->type == LAYOUT_TYPE_Y);

	Layout_Block *t_block_last = 0;
	Layout_GetLastBlock(layout, &t_block_last);

	bool is_last_block = (t_block_last == layout_block);

	s64 widget_count = layout_block->ap_layout_data.count;

	if (!widget_count)
		return;

	s32 padding_size     = layout_block->padding;

	if (padding_size % 2 != 0)  ++padding_size;

	s32 padding_border   = padding_size >> 1;
	s32 height_remaining = layout->rect_remaining.h;

	float it_x = padding_border;
	float it_y = padding_border;

	bool is_overwriting = (	    layout_block->expand_index >= -4
							AND layout_block->expand_index != -1
							AND layout_block->expand_index < widget_count);

	s32 block_width = 0;
	u64 widget_count_auto = widget_count;

	/// pre-calc width for bottom alignment
	FOR_ARRAY(layout_block->ap_layout_data, it_block) {
		Layout_Data *t_data = ARRAY_IT(layout_block->ap_layout_data, it_block);

		Rect *rect = &t_data->settings.rect;

		bool found_expander = ((s64)it_block == layout_block->expand_index);

		block_width = MAX(block_width, rect->w);

		if (is_overwriting) {
			if (!found_expander) {
				height_remaining -= rect->h;
				--widget_count_auto;
			}
		}
		else if (!t_data->settings.auto_height) {
			height_remaining -= rect->h;
			--widget_count_auto;
		}
	}

	height_remaining -= padding_size;
	height_remaining -= layout_block->spacing * (widget_count - 1);

    block_width += padding_size;

    switch (layout_block->expand_index) {
    	case -3: {
    		it_y = (height_remaining + padding_border) >> 1;
    	} break;

    	case -4: {
    		it_y = (height_remaining + padding_border);
    	} break;

		default: break;
    }

    /// start drawing right
	if (layout_block->dock == LAYOUT_DOCK_BOTTOMRIGHT) {
		if (!is_last_block OR (is_last_block AND !layout->fill_last_block)) {
			it_x += layout->rect_remaining.w - block_width;
		}
	}

	float height_avg_auto = 0;

	if (widget_count_auto)
		height_avg_auto = ceil((float)height_remaining / widget_count_auto);

	/// align horizontal
    FOR_ARRAY(layout_block->ap_layout_data, it_block) {
		Layout_Data *t_data = ARRAY_IT(layout_block->ap_layout_data, it_block);

		Rect *rect = &t_data->settings.rect;

		/// center widgets in block
		s32 center_block = (rect->w - (block_width - padding_size)) >> 1;

		rect->x = layout->rect_remaining.x + it_x - center_block;
		rect->y = layout->rect_remaining.y + it_y;

		if (layout->fill_last_block AND is_last_block)
			rect->w = layout->rect_remaining.w - padding_size;

		bool found_expander = ((s64)it_block == layout_block->expand_index);

		float y_step = rect->h;

		if (is_overwriting) {
			if (found_expander)
				y_step = height_avg_auto;
		}
		else {
			if (t_data->settings.auto_height)
				y_step = height_avg_auto;
		}

		float height_limit = layout->rect_remaining.h - it_y;

		if (y_step > height_limit)
			y_step = height_limit;

		rect->h = y_step;
		it_y += y_step + layout_block->spacing;
    }

    /// cut of the left
	if (layout_block->dock == LAYOUT_DOCK_TOPLEFT)
		layout->rect_remaining.x += block_width;

    layout->rect_remaining.w -= block_width;
}

instant void
Layout_Arrange(
	Layout *layout
) {
	Assert(layout);

	layout->rect_remaining = layout->rect_full;

	if (layout->padding % 2 != 0) ++layout->padding;

	Rect_AddPadding(&layout->rect_remaining, {(float)layout->padding,
                                              (float)layout->padding,
												(s32)layout->padding,
                                                (s32)layout->padding});

	FOR_ARRAY(layout->a_blocks, it) {
		Layout_Block *t_block = &ARRAY_IT(layout->a_blocks, it);

		if (0) {}
		else if (t_block->type == LAYOUT_TYPE_X)
			Layout_ArrangeBlockX(layout, t_block);
		else if (t_block->type == LAYOUT_TYPE_Y)
			Layout_ArrangeBlockY(layout, t_block);
	}
}


instant void
Layout_Resize(
	Layout *layout,
	Rect rect_resize,
	bool auto_arrange = true
) {
	layout->rect_full       = rect_resize;
	layout->rect_remaining  = rect_resize;

	if (auto_arrange)
		Layout_Arrange(layout);
}

/// ::: Widget
/// ===========================================================================
struct Widget;

typedef void (*Widget_OwnerDraw)
	(Widget *widget);
typedef void (*Widget_UpdateCustomInputs)
	(Widget *widget, u64 it_current, Array<Widget> *subwidgets, Widget *widget_parent);

enum WIDGET_TYPE {
	WIDGET_LABEL,
	WIDGET_BUTTON,
	WIDGET_LISTBOX,
	WIDGET_CHECKBOX,
	WIDGET_PICTUREBOX,
	WIDGET_SPREADER,
	WIDGET_NUMBERPICKER,
	WIDGET_TEXTBOX
};

enum WIDGET_SCROLL_TYPE {
	WIDGET_SCROLL_ITEM,
	WIDGET_SCROLL_BLOCK
};

struct Widget_Slide {
	s64 start = 0;
	s64 end   = 0;
	s64 value = 0;
	s64 step  = 0;
};

struct Widget_Data {
	Color32 color_background       = Color_MakeGrey(0.9f);
	Color32 color_outline          = {0.0f, 0.0f, 1.0f, 1.0f};
	Color32 color_outline_selected = {1.0f, 0.0f, 0.0f, 1.0f};
	Color32 color_outline_inactive = {0.5f, 0.5f, 1.0f, 1.0f};
	Color32 color_font             = {0.0f, 0.0f, 0.0f, 1.0f};
	Color32 color_progress         = {0.2f, 0.2f, 0.6f, 1.0f};

	u64  active_row_id = 0;

	u32  border_size   = 0;
	u32  spacing       = 1;

	bool is_focusable  = true;
	bool is_scrollable = false;
	bool is_checkable  = false;

	bool has_focus  = false;
	bool has_scrollable_list = false;
	bool is_checked = false;

	WIDGET_SCROLL_TYPE scroll_type = WIDGET_SCROLL_ITEM;

	Array<String> as_row_data;
};

struct Widget {
	WIDGET_TYPE type;
	Layout_Data layout_data;
	Rect rect_content;
	Text text;
	Vertex vertex_rect;
	Window *window;

	bool trigger_autosize = false;

	Array<Widget> a_subwidgets;

	Widget_Data data;
	Widget_Data data_prev;
	Widget_Slide slide;

	Widget_OwnerDraw OwnerDraw = 0;
	Widget_UpdateCustomInputs UpdateCustomInputs = 0;
};

instant void
Widget_AddRow(
	Widget *widget,
	const char *c_row_data,
	u64 c_length = 0
) {
	if (!c_row_data)
		return;

	String *ts_data;
	Array_AddEmpty(&widget->data.as_row_data, &ts_data);
	String_Copy(ts_data, c_row_data, c_length);
}

instant void
Widget_AddRows(
	Widget *widget,
	Array<String> *as_list
) {
	Assert(widget);
	Assert(as_list);

	FOR_ARRAY(*as_list, it) {
		String *ts_item = &ARRAY_IT(*as_list, it);

		Widget_AddRow(widget, ts_item->value, ts_item->length);
	}
}

instant bool
Mouse_IsHovering(
	Widget *widget
) {
	Assert(widget);

	Point t_point;
	Mouse_GetPosition(&t_point.x, &t_point.y, widget->window);

    return Rect_IsIntersecting(&t_point, &widget->layout_data.settings.rect);
}

instant bool
Mouse_IsHovering(
	Mouse *mouse,
	Widget *widget
) {
	Assert(mouse);
	Assert(widget);

	Mouse_GetPosition(mouse, widget->window);

    return Rect_IsIntersecting(&mouse->point, &widget->layout_data.settings.rect);
}

instant bool
Widget_HasChanged(
	Widget *widget
) {
	Assert(widget);

	bool result = false;

	result = !Memory_Compare(
						&widget->layout_data.settings,
						&widget->layout_data.settings_prev,
						 sizeof(widget->layout_data.settings)
				  );

	if (result)  return result;

	result = !Memory_Compare(
						&widget->data,
						&widget->data_prev,
						 sizeof(widget->data)
				  );

	if (result)  return result;

	FOR_ARRAY(widget->data.as_row_data, it) {
		String *t_data = &ARRAY_IT(widget->data.as_row_data, it);

		if (t_data->changed) {
			result = true;
			break;
		}
	}

	if (result)  return result;

	result = Text_HasChanged(&widget->text);

	return result;
}

instant void
Widget_Redraw(
	Widget *widget
) {
	Assert(widget);

	if (widget->OwnerDraw) {
		widget->OwnerDraw(widget);
		return;
	}

	Vertex *t_vertex = &widget->vertex_rect;
	Rect    rect_box =  widget->layout_data.settings.rect;

	if (!t_vertex->array_id) Vertex_CreateStatic(t_vertex);
	else                     Vertex_ClearAttributes(t_vertex);

	switch (widget->type) {
		case WIDGET_SPREADER: {
		} break;

		case WIDGET_TEXTBOX: {
			widget->text.data.rect = widget->layout_data.settings.rect;

			Vertex_AddRect32(t_vertex, rect_box, widget->data.color_background);

			if (Widget_HasChanged(widget)) {
				Vertex_ClearAttributes(&widget->text.cursor.vertex_cursor);
				Vertex_ClearAttributes(&widget->text.cursor.vertex_select);
				Text_Cursor_SetSelection(&widget->text, {}, {});
			}

			widget->text.cursor.show_cursor = widget->data.has_focus;
			Time_Reset(&widget->text.cursor.timer_blinking);
			widget->text.cursor.is_blink_on = true;
		} break;

		case WIDGET_LABEL: {
			widget->text.data.rect = widget->layout_data.settings.rect;
			Vertex_AddRect32(t_vertex, rect_box, widget->data.color_background);
		} break;

		case WIDGET_PICTUREBOX:
		case WIDGET_LISTBOX: {
			Vertex_AddRect32(t_vertex, rect_box, widget->data.color_background);
		} break;

		case WIDGET_BUTTON: {
			widget->text.data.rect = widget->layout_data.settings.rect;

			Vertex_AddRect32(t_vertex, rect_box, widget->data.color_background);

			if (widget->data.border_size) {
				Rect_Resize(&rect_box, -1);

				if (widget->data.has_focus)
					Vertex_AddRect32(t_vertex, rect_box, widget->data.color_outline_selected);
				else
					Vertex_AddRect32(t_vertex, rect_box, widget->data.color_outline);

				Rect_Resize(&rect_box, -widget->data.border_size);
				Vertex_AddRect32(t_vertex, rect_box, widget->data.color_background);
			}
		} break;

		case WIDGET_CHECKBOX: {
			widget->text.data.rect = widget->layout_data.settings.rect;

			Vertex_AddRect32(t_vertex, rect_box, widget->data.color_background);

			s32 check_offset = 2;
			s32 check_h = widget->text.font->size - (check_offset << 1);
			s32 check_w = check_h;

			Rect rect_check = {
				rect_box.x + check_offset,
				rect_box.y + check_offset,
				check_w,
				check_h
			};

			widget->text.data.rect_margin = {
				(float)check_offset * 2 + check_w + 2,
				0,
				0,
				0
			};

			if (widget->data.has_focus)
				Vertex_AddRect32(t_vertex, rect_check, widget->data.color_outline);
			else
				Vertex_AddRect32(t_vertex, rect_check, widget->data.color_outline_inactive);

			Assert(widget->data.border_size);
			Assert(widget->data.border_size < 20);

			widget->data.border_size = (widget->text.font->size / 10);

			Rect_Resize(&rect_check, -widget->data.border_size);
			Vertex_AddRect32(t_vertex, rect_check, widget->data.color_background);

			if (widget->data.is_checked) {
				Rect_Resize(&rect_check, -1);
				Vertex_AddRect32(t_vertex, rect_check, widget->data.color_outline_selected);
			}
		} break;

		case WIDGET_NUMBERPICKER: {
			Layout layout;
			Layout_Create(&layout, &widget->layout_data, false);
			layout.padding = 0;

			Layout_Block *current_block;
			Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT, 0, &current_block);
			current_block->padding = 0;
			current_block->spacing = 2;

			/// label
			Layout_Add(&layout, &ARRAY_IT(widget->a_subwidgets, 0));
			/// spreader
			Layout_Add(&layout, &ARRAY_IT(widget->a_subwidgets, 3));
			/// button up
			Layout_Add(&layout, &ARRAY_IT(widget->a_subwidgets, 1));
			/// button down
			Layout_Add(&layout, &ARRAY_IT(widget->a_subwidgets, 2));
			Layout_Arrange(&layout);
		} break;

		default:
			AssertMessage(	false,
							"Redrawing: Unhandled widget background drawing.");
	}
}

instant void
Widget_InvalidateBackground(
	Widget *widget
) {
	Assert(widget);

	Widget_Redraw(widget);

	widget->layout_data.settings_prev = widget->layout_data.settings;
}

instant void
Widget_AddBorderSizes(
	Widget *widget,
	s32 *min_width,
	s32 *min_height
) {
	Assert(widget);

	Rect *rect_padding = &widget->text.data.rect_padding;

	if (min_width) {
		*min_width += rect_padding->x + rect_padding->w;
		*min_width += widget->data.border_size << 1;

		*min_width +=   widget->text.data.rect_margin.x
					  + widget->text.data.rect_margin.w;
	}

	if (min_height) {
		*min_height += rect_padding->y + rect_padding->h;
		*min_height += widget->data.border_size << 1;

		*min_height +=  widget->text.data.rect_margin.y
					  + widget->text.data.rect_margin.h;
	}
}

instant void
Widget_Render(
	ShaderSet *shader_set,
	Widget *widget
) {
	Assert(shader_set);
	Assert(widget);

	widget->text.shader_set = shader_set;

	/// draw non-list data
	if (!widget->data.as_row_data.count) {
		Text_Update(&widget->text);

		if (widget->trigger_autosize){
			Layout_Data_Settings *layout_data = &widget->layout_data.settings;
			Text_Data *settings = &widget->text.data;

			if (layout_data->auto_width AND settings->rect_content.w) {
				s32 width_auto = settings->rect_content.w;
				Widget_AddBorderSizes(widget, &width_auto, 0);

				if (layout_data->rect.w != width_auto) {
					layout_data->rect.w  = width_auto;
				}
			}

			if (layout_data->auto_height AND settings->rect_content.h) {
				s32 height_auto = settings->rect_content.h;
				Widget_AddBorderSizes(widget, 0, &height_auto);

				if (layout_data->rect.h != height_auto) {
					layout_data->rect.h  = height_auto;
				}
			}

			widget->trigger_autosize = false;
		}

		if (Widget_HasChanged(widget)) {
			widget->data_prev = widget->data;
			widget->text.data_prev = widget->text.data;

			Widget_InvalidateBackground(widget);
		}

		if (widget->vertex_rect.a_attributes.count) {
			ShaderSet_Use(shader_set, SHADER_PROG_RECT);
			Rect_Render(shader_set, &widget->vertex_rect);
		}

		if (widget->vertex_rect.texture.ID) {
			ShaderSet_Use(shader_set, SHADER_PROG_TEXTURE_SIZE);

			s32 width, height;
			Texture_GetSizeAndBind(&widget->vertex_rect.texture, &width, &height);

			Rect rect_tex_aspect = widget->layout_data.settings.rect;
			Rect_GetAspect(&rect_tex_aspect, width, height);

			static Vertex vertex_texture = Vertex_Create();
			Vertex_SetTexture(shader_set, &vertex_texture, &widget->vertex_rect.texture);
			vertex_texture.settings = widget->vertex_rect.settings;

			Vertex_AddRectTexture(&vertex_texture, rect_tex_aspect);
			Rect_Render(shader_set, &vertex_texture);

			Vertex_ClearAttributes(&vertex_texture);
		}

		Text_Render(&widget->text, false);

		FOR_ARRAY(widget->a_subwidgets, it_sub) {
			Widget *t_subwidget = &ARRAY_IT(widget->a_subwidgets, it_sub);
			Widget_Render(shader_set, t_subwidget);
		}

		return;
	}
	else {
		if (Widget_HasChanged(widget)) {
			widget->data_prev = widget->data;
			widget->text.data_prev = widget->text.data;

			Vertex_ClearAttributes(&widget->vertex_rect);
			Widget_InvalidateBackground(widget);

			Text *t_text = &widget->text;
			Text_Clear(t_text);

			/// x y w
			t_text->data.rect = widget->layout_data.settings.rect;
			Rect *rect_text        = &t_text->data.rect;

			s32 pad_left = 2;

			rect_text->x += t_text->data.rect_content.x + pad_left;
			rect_text->y += t_text->data.rect_content.y;

			widget->rect_content.h = 0;

			FOR_ARRAY(widget->data.as_row_data, it_row) {
				String *ts_data = &ARRAY_IT(widget->data.as_row_data, it_row);

				String_SplitWordsStatic(ts_data, &t_text->as_words);
				rect_text->h = Text_BuildLinesStatic(t_text, &t_text->as_words, &t_text->a_text_lines);

				if (Rect_IsIntersecting(rect_text, &widget->layout_data.settings.rect)) {
					Color32 t_color_rect = widget->data.color_outline;

					if (widget->data.active_row_id == it_row) {
						if (widget->data.has_focus)
							t_color_rect = widget->data.color_outline_selected;
						else
							t_color_rect = widget->data.color_outline_inactive;
					}

					Rect rect_box = *rect_text;
					rect_box.x -= pad_left;

					Vertex_AddRect32(&widget->vertex_rect, rect_box, t_color_rect);

					Text_AddLines(t_text, false);
				}

				s32 height_row_step = rect_text->h + widget->data.spacing;
				rect_text->y           += height_row_step;
				widget->rect_content.h += height_row_step;

				ts_data->changed = false;
			}

			/// revert for scissor
			*rect_text = widget->layout_data.settings.rect;
		}

		OpenGL_Scissor(shader_set->window, widget->layout_data.settings.rect);

		/// render rectangles + background
		if (widget->vertex_rect.a_attributes.count) {
			ShaderSet_Use(shader_set, SHADER_PROG_RECT);
			Rect_Render(shader_set, &widget->vertex_rect);
		}

		Text_Render(&widget->text, false);

		OpenGL_Scissor_Disable();
	}
}

instant void
Widget_Render(
	ShaderSet *shader_set,
	Array<Widget *> *ap_widgets
) {
	Assert(ap_widgets);

    FOR_ARRAY(*ap_widgets, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets, it_widget);

		Widget_Render(shader_set, t_widget);
    }
}

instant void
Widget_Destroy(
	Widget *widget
) {
	Assert(widget);

	Text_Destroy(&widget->text);
	Vertex_Destroy(&widget->vertex_rect);
}

instant void
Widget_Destroy(
	Array<Widget *> *ap_widgets
) {
	Assert(ap_widgets);

    FOR_ARRAY(*ap_widgets, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets, it_widget);

		Text_Destroy(&t_widget->text);
		Vertex_Destroy(&t_widget->vertex_rect);
    }
}

/// mouse_button
///   0: left
///   1: middle
///   2: right
///
/// keyboard trigger
///	  VK_RETURN
///   VK_SPACE
instant bool
Widget_OnClick(
	Widget *widget,
	u16 mouse_button = 0
) {
	Assert(widget);
	Assert(widget->window);

	bool result = false;

	Keyboard *keyboard = widget->window->keyboard;
	Mouse    *mouse    = widget->window->mouse;

	///@Idea: might be better in a seperate space,
	///       and let this function only handle mouse input
	if ((IF_USE(keyboard).up[VK_RETURN]) OR
		(IF_USE(keyboard).up[VK_SPACE]))
	{
		if (widget->data.has_focus)
			result = true;
	}
	else {
		///@Performance: IsHovering does not have to retrieve
		///              the mouse position again, when the
		///              updated mouse information is already
		///              stored in the window structure
		///	             (in case the data was retrieved in
		///              the message loop)
		///
		if (IF_USE(mouse).up[mouse_button]) {
			if (Mouse_IsHovering(widget)) {
				result = true;
			}
		}
	}

	return result;
}

instant bool
Widget_OnDoubleClick(
	Widget *widget,
	u16 mouse_button = 0
) {
	Assert(widget);
	Assert(widget->window);

	bool result = false;

	Mouse *mouse = widget->window->mouse;

	if (mouse) {
		if (mouse->double_click[mouse_button]) {
			if (Mouse_IsHovering(mouse, widget)) {
				result = true;
			}
		}
	}

	return result;
}

instant void
Widget_UpdateFocus(
	Array<Widget *> *ap_widgets,
	bool check_backward,
	Widget **widget_focus = 0
) {
	Assert(ap_widgets);

	bool focus_set_next = false;

	Widget *t_widget_focus_check = 0;

	if (!ap_widgets->count)
		return;

	s64 start = 0;
	s64 end   = ap_widgets->count;
	s64 step  = 1;

	if (check_backward) {
		start =  end;
		end   = -1;
		step  = -1;
	}

	for (s64 it_widget = start; it_widget != end; it_widget += step) {
		Widget *t_widget = ARRAY_IT(*ap_widgets, it_widget);

		if (!t_widget->data.is_focusable)
			continue;

		Keyboard *keyboard = t_widget->window->keyboard;
		Mouse    *mouse    = t_widget->window->mouse;

		if (focus_set_next) {
			t_widget->data.has_focus = true;
			focus_set_next = false;
		}
		else {
			if (IF_USE(keyboard).up[VK_TAB]) {
				if (t_widget->data.has_focus) {
					t_widget->data.has_focus = false;
					focus_set_next = true;
				}
			}
		}

		if (t_widget->data.has_focus)
			t_widget_focus_check = t_widget;
	}

	/// search for next focus from the beginning,
	/// if there was no focusable left past the last focused
	/// widget
	if (focus_set_next OR !t_widget_focus_check) {
		for (s64 it_widget = start; it_widget != end; it_widget += step) {
			Widget *t_widget = ARRAY_IT(*ap_widgets, it_widget);

			if (t_widget->data.is_focusable) {
				t_widget->data.has_focus = true;
				t_widget_focus_check = t_widget;

				break;
			}
		}
	}

	if (widget_focus) {
		*widget_focus = t_widget_focus_check;
	}
}

instant void
Widget_SetFocus(
	Array<Widget *> *ap_widgets,
	Widget *widget
) {
	Assert(ap_widgets);
	Assert(widget);

    FOR_ARRAY(*ap_widgets, it) {
		Widget *t_widget = ARRAY_IT(*ap_widgets, it);

		if (t_widget->data.has_focus) {
			t_widget->data.has_focus = false;
			break;
		}
    }

    widget->data.has_focus = true;
}

instant void
Widget_CalcActiveRowRect(
	Widget *widget,
	Rect *rect_row
) {
	Assert(widget);
    Assert(rect_row);

    *rect_row = {};

    String ts_data_backup = widget->text.s_data;

	Rect rect_item  = widget->layout_data.settings.rect;
	rect_item.x    += widget->text.data.rect_content.x;
	rect_item.y    += widget->text.data.rect_content.y;

    FOR_ARRAY(widget->data.as_row_data, it_row) {
		String *ts_data = &ARRAY_IT(widget->data.as_row_data, it_row);
		widget->text.s_data = *ts_data;

		s32 width, height;
		Text_GetSize(&widget->text, &width, &height);

		rect_item.w = width;
		rect_item.h = height;

		if (widget->data.active_row_id == it_row) {
			*rect_row = rect_item;
			break;
		}

		rect_item.y += rect_item.h + widget->data.spacing;
    }

    widget->text.s_data = ts_data_backup;
}

instant u64
Widget_CalcActiveRowID(
	Widget *widget,
	Mouse *mouse
) {
    Assert(widget);
    Assert(mouse);

    if (!Mouse_IsHovering(mouse, widget))
		return widget->data.active_row_id;

    u64 active_row_id = 0;

    String ts_data_backup = widget->text.s_data;

	Rect rect_item  = widget->layout_data.settings.rect;
	rect_item.x    += widget->text.data.rect_content.x;
	rect_item.y    += widget->text.data.rect_content.y;

    FOR_ARRAY(widget->data.as_row_data, it_row) {
		String *ts_data = &ARRAY_IT(widget->data.as_row_data, it_row);
		widget->text.s_data = *ts_data;

		s32 width, height;
		Text_GetSize(&widget->text, &width, &height);

		rect_item.w = width;
		rect_item.h = height;

		if (Rect_IsIntersecting(&mouse->point, &rect_item)) {
			active_row_id = it_row;
			break;
		}

		rect_item.y += rect_item.h + widget->data.spacing;
    }

    widget->text.s_data = ts_data_backup;

    return active_row_id;
}

instant void
Widget_UpdateInput(
	Widget *widget
) {
	Assert(widget);
	Assert(widget->window);

    Keyboard *keyboard = widget->window->keyboard;
    Mouse    *mouse    = widget->window->mouse;

    Rect *rect_widget = &widget->layout_data.settings.rect;

    if (widget->a_subwidgets.count) {
		FOR_ARRAY(widget->a_subwidgets, it_sub) {
			Widget *t_subwidget = &ARRAY_IT(widget->a_subwidgets, it_sub);

			if (t_subwidget->UpdateCustomInputs)
				t_subwidget->UpdateCustomInputs(t_subwidget, it_sub, &widget->a_subwidgets, widget);
			else
				Widget_UpdateInput(t_subwidget);
		}

		return;
    }

	if (!widget->data.is_focusable)
		return;

	bool got_focus = widget->data.has_focus;
	u64  prev_active_row = widget->data.active_row_id;

	bool has_text_cursor = widget->text.data.is_editable;

	bool is_scrollable_list = widget->data.has_scrollable_list;
	bool is_scrollable      = widget->data.is_scrollable;

    if (mouse) {
		bool is_hovering = Mouse_IsHovering(mouse, widget);

		/// text selection
		if (has_text_cursor AND is_hovering) {
			Text *t_text = &widget->text;

			if (mouse->pressing[0]) {
				if (mouse->is_down) {
					t_text->cursor.point_select_start = mouse->point;
					t_text->cursor.point_select_end   = mouse->point;

					t_text->cursor.point_select_start.x -= t_text->data.rect_content.x;
					t_text->cursor.point_select_start.y -= t_text->data.rect_content.y;
					t_text->cursor.point_select_end.x   -= t_text->data.rect_content.x;
					t_text->cursor.point_select_end.y   -= t_text->data.rect_content.y;

					Vertex_ClearAttributes(&t_text->cursor.vertex_select);
				}
				else {
					t_text->cursor.point_select_end = mouse->point;

					t_text->cursor.point_select_end.x   -= t_text->data.rect_content.x;
					t_text->cursor.point_select_end.y   -= t_text->data.rect_content.y;

					Text_Cursor_SetSelection(t_text,
											 t_text->cursor.point_select_start,
											 t_text->cursor.point_select_end);
				}
			}
		}

		if (mouse->up[0]) {
			got_focus = is_hovering;

			/// listbox entry selection
			if (is_scrollable_list)
				widget->data.active_row_id = Widget_CalcActiveRowID(widget, mouse);

			/// checkbox toggle
			if (got_focus AND widget->data.is_checkable) {
				widget->data.is_checked = !widget->data.is_checked;
			}

			/// focus change
			if (widget->data.has_focus != got_focus) {
				widget->data.has_focus = got_focus;
			}
		}

		/// widget + list scrolling
		if (is_hovering AND (is_scrollable_list OR is_scrollable)) {
			widget->text.data.rect_content.y += mouse->wheel;

			if (is_scrollable_list) {
				widget->text.data.rect_content.w  = widget->rect_content.w;
				widget->text.data.rect_content.h  = widget->rect_content.h;
			}

			Rect_Clamp(&widget->text.data.rect_content, widget->layout_data.settings.rect);
		}
    }

    if (keyboard AND widget->data.has_focus) {
		if (is_scrollable_list) {
			if (widget->data.active_row_id) {
				if (keyboard->down[VK_UP]) {
					--widget->data.active_row_id;
				}

				if (keyboard->down[VK_HOME]) {
					widget->data.active_row_id = 0;
				}
			}

			if (widget->data.active_row_id < widget->data.as_row_data.count - 1) {
				if (keyboard->down[VK_DOWN]) {
					++widget->data.active_row_id;
				}

				if (keyboard->down[VK_END]) {
					widget->data.active_row_id = widget->data.as_row_data.count - 1;
				}
			}
		}

		if (keyboard->up[VK_SPACE] AND widget->data.is_checkable) {
			widget->data.is_checked = !widget->data.is_checked;
		}
    }

    if (prev_active_row != widget->data.active_row_id AND is_scrollable_list) {
		Rect rect_active_row;
        Widget_CalcActiveRowRect(widget, &rect_active_row);

		if (!Rect_IsVisibleFully(&rect_active_row, rect_widget)) {
			if (widget->data.scroll_type == WIDGET_SCROLL_ITEM) {
				if (widget->data.active_row_id < prev_active_row) {
					widget->text.data.rect_content.y -= (rect_active_row.y - rect_widget->y);
				}
				else {
					widget->text.data.rect_content.y -= (rect_active_row.y - rect_widget->y);
					widget->text.data.rect_content.y += (rect_widget->h - rect_active_row.h);
				}
			}
			else if (widget->data.scroll_type == WIDGET_SCROLL_BLOCK) {
				if (!Rect_IsVisibleFully(&rect_active_row, rect_widget)) {
					if (widget->data.active_row_id < prev_active_row) {
						widget->text.data.rect_content.y -= (rect_active_row.y - rect_widget->y);
						widget->text.data.rect_content.y += (rect_widget->h - rect_active_row.h);
					}
					else {
						widget->text.data.rect_content.y -= (rect_active_row.y - rect_widget->y);
					}
				}
			}
			else {
				AssertMessage(	false,
								"Unhandled widget scroll type.");
			}
		}

		Rect_Clamp(&widget->rect_content, *rect_widget);
    }
}

instant void
Widget_UpdateInput(
	Array<Widget *> *ap_widgets
) {
	Assert(ap_widgets);

	FOR_ARRAY(*ap_widgets, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets, it_widget);
		Widget_UpdateInput(t_widget);
	}
}

instant void
Widget_GetSelectedRow(
	Widget *widget,
	String *s_row_data
) {
	Assert(widget);
	Assert(s_row_data);
	Assert(!widget->data.active_row_id OR widget->data.active_row_id < widget->data.as_row_data.count);

	if (!widget->data.as_row_data.count) {
		*s_row_data = {};
		return;
	}

	String *ts_row_data = &ARRAY_IT(widget->data.as_row_data, widget->data.active_row_id);

	String_Clear(s_row_data);
	String_Append(s_row_data, ts_row_data->value, ts_row_data->length);
}


instant u64
Widget_GetSelectedRowID(
	Widget *widget
) {
	Assert(widget);
	Assert(!widget->data.active_row_id OR widget->data.active_row_id < widget->data.as_row_data.count);

	return widget->data.active_row_id;
}

instant void
Widget_ClearRows(
	Widget *widget
) {
	Assert(widget);

	Array_Clear(&widget->data.as_row_data);
}

instant void
Layout_Add(
	Layout *layout,
	Widget *widget
) {
	Assert(layout);
	Assert(widget);

	Layout_Block *current_block = 0;
	Layout_GetLastBlock(layout, &current_block);

	Array_Add(&current_block->ap_layout_data, &widget->layout_data);
}

instant void
Widget_AddRenderTabStop(
	Array<Widget *> *ap_widgets,
	Widget *widget
) {
	Assert(ap_widgets);
	Assert(widget);

	Array_Add(ap_widgets, widget);

	FOR_ARRAY(widget->a_subwidgets, it_sub) {
		Widget *t_subwidget = &ARRAY_IT(widget->a_subwidgets, it_sub);
		Array_Add(ap_widgets, t_subwidget);
	}
}

instant void
Widget_LoadDirectoryList(
	Widget *widget,
	String *s_directory,
	Array<Directory_Entry> *a_entries_rtn,
	bool show_full_path = false
) {
	Assert(widget);
	Assert(widget->type == WIDGET_LISTBOX);
	Assert(s_directory);

	/// in case the directory string came from the to be destroyed directory entries
	static String ts_directory;
	String_Append(&ts_directory, s_directory->value, s_directory->length);

	FOR_ARRAY(*a_entries_rtn, it) {
		Directory_Entry *t_entry = &ARRAY_IT(*a_entries_rtn, it);
		String_Destroy(&t_entry->s_name);
	}

	Array_ClearContainer(a_entries_rtn);

	widget->data.active_row_id = 0;

	/// remove "\" from directroy path (f.e. C:\) for consistency
	if (String_EndWith(&ts_directory, "\\")) {
		String_Remove(&ts_directory, ts_directory.length - 1, ts_directory.length);
	}

	File_ReadDirectory(a_entries_rtn, ts_directory.value, 0, ts_directory.length, true, 0, DIR_LIST_ONLY_DIR);
	File_ReadDirectory(a_entries_rtn, ts_directory.value, 0, ts_directory.length, true, 0, DIR_LIST_ONLY_FILES);

	Widget_ClearRows(widget);

	FOR_ARRAY(*a_entries_rtn, it) {
		Directory_Entry *t_entry = &ARRAY_IT(*a_entries_rtn, it);

		String ts_entry_name = t_entry->s_name;

		if (!show_full_path) {
			ts_entry_name.value  += ts_directory.length;
			ts_entry_name.length -= ts_directory.length;
		}

		/// removing leading "\" to indicate file type
		if (t_entry->type == DIR_ENTRY_FILE) {
			ts_entry_name.value  += 1;
			ts_entry_name.length -= 1;
		}

		Widget_AddRow(widget, ts_entry_name.value, ts_entry_name.length);
	}

	String_Clear(&ts_directory);
}

instant Widget
Widget_CreateLabel(
	Window *window,
	Font *font,
	Rect rect_box,
	const char *c_data = 0,
	TEXT_ALIGN_X_TYPE text_align_x = TEXT_ALIGN_X_LEFT,
	u64 c_length = 0
) {
	Assert(window);
	Assert(font);

	Widget t_widget = {};

	t_widget.text.data.rect_padding = {1, 1, 1, 1};

	Rect *rect_padding = &t_widget.text.data.rect_padding;

	if (!rect_box.w) {
		rect_box.w = font->size;
		Widget_AddBorderSizes(&t_widget, &rect_box.w, 0);
	}

	if (!rect_box.h) {
		rect_box.h = Font_GetLineHeight(font);
		Widget_AddBorderSizes(&t_widget, 0, &rect_box.h);
	}

	t_widget.type         = WIDGET_LABEL;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.data.is_focusable = false;

	t_widget.text.data.align_x    = text_align_x;
	t_widget.text.data.rect       = rect_box;
	t_widget.text.data.color      = t_widget.data.color_font;

	t_widget.text.font = font;

	String_Append(&t_widget.text.s_data, c_data, c_length);

	t_widget.trigger_autosize = true;

	return t_widget;
}

instant Widget
Widget_CreateButton(
	Window *window,
	Font *font,
	Rect rect_box,
	const char *c_data = 0,
	u64 c_length = 0
) {
	Assert(window);
	Assert(font);

	Widget t_widget = {};

	t_widget.data.border_size = 2;

	t_widget.text.data.rect_margin = {2, 1, 2, 1};

	Rect *rect_padding = &t_widget.text.data.rect_padding;

	t_widget.type         = WIDGET_BUTTON;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.text.data.align_x    = TEXT_ALIGN_X_MIDDLE;
	t_widget.text.data.align_y    = TEXT_ALIGN_Y_CENTER;
	t_widget.text.data.color      = t_widget.data.color_font;

	t_widget.text.font       = font;

	String_Append(&t_widget.text.s_data, c_data, c_length);

	t_widget.trigger_autosize = true;

	return t_widget;
}

instant Widget
Widget_CreateListbox(
	Window *window,
	Font *font,
	Rect rect_box
) {
	Assert(window);
	Assert(font);

	Widget t_widget = {};

	t_widget.data.color_outline = {0.8f, 0.8f, 0.88f};

	t_widget.type         = WIDGET_LISTBOX;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.data.is_focusable = true;
	t_widget.data.has_scrollable_list = true;

	t_widget.text.data.rect  = rect_box;
	t_widget.text.data.color = t_widget.data.color_font;

	t_widget.text.font  = font;

	return t_widget;
}

instant Widget
Widget_CreateCheckbox(
	Window *window,
	Font *font,
	Rect rect_box,
	const char *c_data,
	bool checked,
	u64 c_length = 0
) {
	Assert(window);
	Assert(font);

	Widget t_widget = {};

	t_widget.type         = WIDGET_CHECKBOX;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.data.is_checkable = true;
	t_widget.data.is_focusable = true;
	t_widget.data.border_size  = 2;
	t_widget.data.is_checked   = checked;

	t_widget.text.data.color = t_widget.data.color_font;

	t_widget.text.font = font;

	String_Append(&t_widget.text.s_data, c_data, c_length);

	return t_widget;
}

instant Widget
Widget_CreatePictureBox(
	Window *window,
	Rect rect_box,
	Texture *texture = 0
) {
	Assert(window);

	Widget t_widget = {};

	t_widget.type         = WIDGET_PICTUREBOX;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	if (texture)
		t_widget.vertex_rect.texture = *texture;

	t_widget.data.is_focusable = false;

	return t_widget;
}

instant Widget
Widget_CreateSpreader(
	Window *window
) {
	Assert(window);

	Widget t_widget;

    t_widget.type = WIDGET_SPREADER;
    t_widget.window = window;

    t_widget.data.is_focusable = false;

    return t_widget;
}

instant void
Widget_UpdateInputNumberPicker(
	Widget *widget,
	u64 it_current,
	Array<Widget> *a_subwidgets,
	Widget *widget_parent
) {
	Assert(widget);
	Assert(a_subwidgets);
	Assert(widget_parent);

	Widget_UpdateInput(widget);

	Widget *tw_label = &ARRAY_IT(*a_subwidgets, 0);

	/// based on subwidget array index
	switch (it_current) {
		case 1:
		case 2: {
			if (Widget_OnClick(widget, 0)) {
				Widget_Slide *t_slide = &widget_parent->slide;

				t_slide->value = ToInt(&tw_label->text.s_data);

				if (0) {}
				else if (it_current == 1) {
					if (t_slide->value - t_slide->step >= t_slide->start)
						t_slide->value -= t_slide->step;
				}
				else if (it_current == 2) {
					if (t_slide->value + t_slide->step <= t_slide->end)
						t_slide->value += t_slide->step;
				}

				char *c_value = ToCString(t_slide->value);

				String_Clear(&tw_label->text.s_data);
				String_Append(&tw_label->text.s_data, c_value);

				Memory_Free(c_value);
			}
		} break;
	}

}

instant void
Widget_RedrawNumberPickerButton(
	Widget *widget
) {
	Assert(widget);

	Vertex *t_vertex = &widget->vertex_rect;
	Rect    rect_box =  widget->layout_data.settings.rect;

	if (!t_vertex->array_id) Vertex_CreateStatic(t_vertex);
	else                     Vertex_ClearAttributes(t_vertex);

	if (widget->data.has_focus)
		Vertex_AddRect32(t_vertex, rect_box, widget->data.color_outline_selected);
	else
		Vertex_AddRect32(t_vertex, rect_box, widget->data.color_outline);

	Rect_Resize(&rect_box, -2);
	Vertex_AddRect32(t_vertex, rect_box, {1, 1, 1, 1});
}

instant Widget
Widget_CreateNumberPicker(
	Window *window,
	Font *font,
	Rect rect_box,
	Widget_Slide slide
) {
	Widget t_widget;

    t_widget.type = WIDGET_NUMBERPICKER;
    t_widget.window = window;
    t_widget.rect_content = rect_box;
    t_widget.slide = slide;

    t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.data.is_focusable = false;

	Widget w_label       = Widget_CreateLabel( window, font, {0, 0, 50, 24});
	Widget w_button_up   = Widget_CreateButton(window, font, {0, 0, 24, 24}, "<");
	Widget w_button_down = Widget_CreateButton(window, font, {0, 0, 24, 24}, ">");
	Widget w_spreader    = Widget_CreateSpreader(window);

	w_button_up.OwnerDraw   = Widget_RedrawNumberPickerButton;
	w_button_down.OwnerDraw = Widget_RedrawNumberPickerButton;
	w_button_up.UpdateCustomInputs   = Widget_UpdateInputNumberPicker;
	w_button_down.UpdateCustomInputs = Widget_UpdateInputNumberPicker;

	w_label.text.data.align_x = TEXT_ALIGN_X_RIGHT;
	w_label.text.data.rect_padding = {2, 2, 2, 2};

	char *c_value = ToCString(slide.value);
	String_Append(&w_label.text.s_data, c_value);
	Memory_Free(c_value);

	Array_Add(&t_widget.a_subwidgets, w_label);
	Array_Add(&t_widget.a_subwidgets, w_button_up);
	Array_Add(&t_widget.a_subwidgets, w_button_down);
	Array_Add(&t_widget.a_subwidgets, w_spreader);

	return t_widget;
}

instant Widget
Widget_CreateTextBox(
	Window *window,
	Font *font,
	Rect rect_box
) {
	Widget t_widget;

	t_widget.text.data.rect_padding = {1, 1, 1, 1};

	Rect *rect_padding = &t_widget.text.data.rect_padding;

	if (!rect_box.w) {
		rect_box.w = font->size;
		Widget_AddBorderSizes(&t_widget, &rect_box.w, 0);
	}

	if (!rect_box.h) {
		rect_box.h = Font_GetLineHeight(font);
		Widget_AddBorderSizes(&t_widget, 0, &rect_box.h);
	}

	t_widget.type         = WIDGET_TEXTBOX;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.data.is_scrollable = true;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.text.font  = font;

	t_widget.text.data.rect  = rect_box;
	t_widget.text.data.color = t_widget.data.color_font;
	t_widget.text.data.is_editable = true;

	return t_widget;
}
