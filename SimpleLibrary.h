#pragma once

#define DEBUG_ALWAYS_UPDATE		0
#define DEBUG_EVENT_STATUS		0
#define DEBUG_BENCHMARK			1

/// Compiler: g++ (6.3.0) (mingw)
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
///           a parameter / or when shadowing)
///
///     Postfix Pointer Parameter:
///     ------------------------------------------------------
///     Function parameters with pointers who end in name with
///       _out -> everything / something (in a struct)
///               will be cleared / overwritten
///       _io  -> will read an input and generate an output
///               based on it
///     otherwise, the pointer will only be used for reading
///
///     Function names that end with "Buffer" have a byRef return parameter
///     which needs manual freeing, in case it is not a static object / variable (tbd)
///
///     Function names that end with "Temp" issue a return value, which is only
///     valid until that function is called again. (for temporary ownership)
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
//
//	while(running) {
//		msg = {};
//
//		/// Events
//		/// ===================================================================
//		Window_ReadMessage(msg, running, window);
//		OpenGL_AdjustScaleViewport(window, false);
//
//		/// Render
//		/// ===================================================================
//		OpenGL_ClearScreen();
//
//		Window_UpdateAndResetInput(window);
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
//		Window_UpdateAndResetInput(window);
//	}
//
//	Texture_Destroy(&texture);
//	Vertex_Destroy(&vertex);
//	ShaderSet_Destroy(&shader_set);
//}

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
//		Window_UpdateAndResetInput(window);
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
//		Window_UpdateAndResetInput(window);
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
//		Window_UpdateAndResetInput(window);
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
//	text.data.color = {1, 0, 0, 1};
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
//		Text_Update(&text);
//		Text_Render(&text);
//
//		Window_UpdateAndResetInput(window);
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
//	OpenGL_SetVSync(&window, false);
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
//		text_box.data.rect  = rect_box;
//		text_box.font       = &font;
//		text_box.data.color = Color_MakeGrey(0.0f);
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
//		Window_UpdateAndResetInput(window);
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
//	OpenGL_SetVSync(&window, false);
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
//	Array_Add(&ap_widgets, &widget_label);
//	Array_Add(&ap_widgets, &widget_click_me);
//	Array_Add(&ap_widgets, &widget_exit);
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
//		if (keyboard->up[VK_ESCAPE] OR widget_exit.events.on_trigger)
//			running = false;
//
//		if (widget_click_me.events.on_trigger) {
//			std::cout << "clicked" << std::endl;
//		}
//
//		/// Render
//		/// ===================================================================
//		OpenGL_ClearScreen();
//
//		Widget_Render(&shader_set, &ap_widgets);
//
//		Window_UpdateAndResetInput(window);
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
//	widget_click_me.trigger_autosize = false;
//	widget_exit.trigger_autosize     = false;
//
//	Array<Widget *> ap_widgets;
//	Array_Add(&ap_widgets, &widget_listbox);
//	Array_Add(&ap_widgets, &widget_click_me);
//	Array_Add(&ap_widgets, &widget_exit);
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
//		Widget_Update(&ap_widgets, keyboard);
//
//		if (keyboard->up[VK_ESCAPE] OR widget_exit.events.on_trigger)
//			running = false;
//
//		if (widget_click_me.events.on_trigger) {
//			std::cout << "clicked" << std::endl;
//		}
//
//		if (widget_listbox.events.on_trigger) {
//			String s_row_data = Widget_GetSelectedRow(&widget_listbox);
//
//			LOG_DEBUG(s_row_data.value);
//
//			String_Destroy(&s_row_data);
//		}
//
//		/// Render
//		/// ===================================================================
//		OpenGL_ClearScreen();
//
//		Widget_Render(&shader_set, &ap_widgets);
//
//		Window_UpdateAndResetInput(window);
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
//
//	Widget w_button_top_1    = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, "top_1");
//	Widget w_button_top_2    = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, "top_2");
//	Widget w_combobox        = Widget_CreateComboBox(window, &font_20, {}, 200);
//
//	Widget *w_combo_list = &ARRAY_IT(w_combobox.a_subwidgets, 2);
//
//	Widget_AddRow(&w_combobox, "Hello");
//
//	w_combo_list->data.color_background = Color_MakeGrey(0.5f);
//
//	Widget w_button_center_1 = Widget_CreateCheckBox(window, &font_20, {0, 0, 100, 30}, "center_1", false);
//	Widget w_spreader        = Widget_CreateSpreader(window);
//	Widget w_numpic          = Widget_CreateNumberPicker(window, &font_20, {0, 0, 100, 30}, {0, 5, 3, 1});
//
//	w_button_top_2.layout_data.settings.auto_width = false;
//
//	Layout layout;
//	Layout_Create(&layout, {0, 0, window->width, window->height}, true);
//
//	Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT);
//	Layout_Add(&layout, &w_numpic);
//	Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT, -1);
//	Layout_Add(&layout, &w_button_top_1);
//	Layout_Add(&layout, &w_button_top_2);
//	Layout_Add(&layout, &w_combobox);
//	Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT,  -1);
//	Layout_Add(&layout, &w_button_menu_1);
//	Layout_Add(&layout, &w_button_menu_2);
//	Layout_Add(&layout, &w_spreader);
//	Layout_Add(&layout, &w_button_menu_3);
//	Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT);
//	Layout_Add(&layout, &w_button_center_1);
//
//	Array<Widget *> ap_widgets;
//	Array_Add(&ap_widgets, &w_button_top_1);
//	Array_Add(&ap_widgets, &w_button_top_2);
//	Array_Add(&ap_widgets, &w_combobox);
//	Array_Add(&ap_widgets, &w_button_menu_1);
//	Array_Add(&ap_widgets, &w_button_menu_2);
//	Array_Add(&ap_widgets, &w_button_menu_3);
//	Array_Add(&ap_widgets, &w_button_center_1);
//	Array_Add(&ap_widgets, &w_numpic);
//
//	while(running) {
//		msg = {};
//
//		/// Events
//		/// ===================================================================
//		Window_ReadMessage(msg, running, window);
//		OpenGL_AdjustScaleViewport(window);
//
//		Layout_Rearrange(&layout, {0, 0, window->width, window->height});
//		Layout_Arrange(&layout);
//
//		/// hold shift-key to get reverse tab order
//		Widget_Update(&ap_widgets, keyboard);
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
//		LOG_DEBUG(Widget::widget_focus_current->type)
//
//		Window_UpdateAndResetInput(window);
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

#ifdef __MINGW32__
#   undef  _WIN32_WINNT
#   define _WIN32_WINNT 0x0501
#endif // __MINGW32__

#include <windows.h>

#include <iostream>
#include <math.h>
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

/// ::: Globals
/// ===========================================================================
static u64 global_frame_count = 0;

/// ::: Iterators
/// ===========================================================================
#define FOR(_max, _it)				\
	for(u64 _it = 0;				\
	_it < (_max); 					\
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

/// ::: Messages
/// ===========================================================================
#if DEBUG_EVENT_STATUS
	#define LOG_STATUS(_text) std::cout << _text;
#else
	#define LOG_STATUS(_text)
#endif

#define LOG_DEBUG(text) std::cout << /*"Frame [" << global_frame_count << "]: " <<*/ text << std::endl;

#define LOG_WARNING(_text) std::cout << "Warning: " << _text << std::endl;

#if DEBUG_BENCHMARK
	#define MEASURE_START() \
		Timer DEBUG_tmr_measure; \
		Time_Measure(&DEBUG_tmr_measure);

	#define MEASURE_END(_text) \
		std::cout << "Measure [" << __FUNCTION__ << "]: " << _text << Time_Measure(&DEBUG_tmr_measure) << " ms" << std::endl;
#else
	#define MEASURE_START()
	#define MEASURE_END()
#endif

/// ::: Utilities
/// ===========================================================================
#define MIN(val_1, val_2) ((val_1 < val_2) ? val_1 : val_2)
#define MAX(val_1, val_2) ((val_1 < val_2) ? val_2 : val_1)

#define MIN_LIMIT(val_1, val_2, _limit)					\
	(													\
		(val_1 < val_2) 								\
			? ((val_1 > _limit)							\
				? val_1									\
				: ((val_2 > _limit) ? val_2 : _limit))	\
			: ((val_2 > _limit) 						\
				? val_2 								\
				: ((val_1 > _limit) ? val_1 : _limit))	\
	)

//#define LOBYTE(x) ((x) & 0xFF)
//#define HIBYTE(x) LOBYTE((x) >> 8)

#define GETBYTE(x, bit_start) LOBYTE((x) >> (bit_start))
#define GETBIT(x, bit_start) (((x) >> (bit_start)) & 0x1)

#define IF_USE(pointer) \
	(pointer) AND (*pointer)

#define IF_SET(pointer) \
	if (pointer) (*pointer)

#define XOR(_a, _b) \
	(((_a) AND !(_b)) OR (!(_a) AND (_b)))

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
operator == (
	Point p1,
	Point p2
) {
	if (p1.x == p2.x OR p1.y == p2.y)
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

bool
operator != (
	Rect r1,
	Rect r2
) {
	if (r1.x != r2.x)  return true;
	if (r1.y != r2.y)  return true;
	if (r1.w != r2.w)  return true;
	if (r1.h != r2.h)  return true;

	return false;
}

template <typename T>
instant void
Swap(
	T *first_io,
	T *second_io
) {
	Assert(first_io);
	Assert(second_io);

	T temp = *first_io;
	*first_io = *second_io;
	*second_io = temp;
}

instant void
Rect_GetAspect(
	s32 width_src,
	s32 height_src,
	s32 *width_dst,
	s32 *height_dst,
	float *off_x_io,
	float *off_y_io,
	float *ratio_x_out = 0,
	float *ratio_y_out = 0,
	bool scale_to_dest = false
) {
	Assert(width_dst);
	Assert(height_dst);

	float aspect_src = (float) width_src /  height_src;
    float aspect_dst = (float)*width_dst / *height_dst;

	if (aspect_dst > aspect_src) {
		float ratio = (aspect_dst / aspect_src);
		float offset = *width_dst / ratio;

		if (off_x_io)  *off_x_io += floor((*width_dst - offset) / 2);
		if (ratio_x_out)  *ratio_x_out = ratio;
		if (ratio_y_out)  *ratio_y_out = 1;

		*width_dst = floor(offset);
    }
    else
	if (aspect_dst < aspect_src) {
		float ratio = (aspect_src / aspect_dst);
		float offset = *height_dst / ratio;

		if (off_y_io)  *off_y_io += floor((*height_dst - offset) / 2);
		if (ratio_x_out)  *ratio_x_out = 1;
		if (ratio_y_out)  *ratio_y_out = ratio;

		*height_dst = floor(offset);
    }

    if (scale_to_dest) {
		if (ratio_x_out)  *ratio_x_out = (float)*width_dst  / width_src;
		if (ratio_y_out)  *ratio_y_out = (float)*height_dst / height_src;
    }
}

instant void
Rect_GetAspect(
	Rect *rect_convert_to_dest,
	s32 width_src,
	s32 height_src,
	float *ratio_x_out = 0,
	float *ratio_y_out = 0,
	bool scale_to_dest = false
) {
	Assert(rect_convert_to_dest);

	Rect_GetAspect(	 width_src,
					 height_src,
					&rect_convert_to_dest->w,
					&rect_convert_to_dest->h,
					&rect_convert_to_dest->x,
					&rect_convert_to_dest->y,
					ratio_x_out,
					ratio_y_out,
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
Rect_IsIntersectingBorderless(
	Point *point,
	Rect  *rect
) {
	Assert(point);
	Assert(rect);

	if (point->x < rect->x)  return false;
	if (point->y < rect->y)  return false;

	if (point->x >= rect->x + rect->w)  return false;
	if (point->y >= rect->y + rect->h)  return false;

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
	Rect *rect_io,
	s32 pixel_offset
) {
	Assert(rect_io);

	rect_io->x -= pixel_offset;
	rect_io->y -= pixel_offset;
	rect_io->w += (pixel_offset << 1);
	rect_io->h += (pixel_offset << 1);
}

instant void
Rect_AddPadding(
	Rect *rect_io,
	Rect rect_padding
) {
	Assert(rect_io);

	rect_io->x += rect_padding.x;
	rect_io->y += rect_padding.y;
	rect_io->w -= (rect_padding.x + rect_padding.w);
	rect_io->h -= (rect_padding.y + rect_padding.h);
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
	s64 *value_io,
	s64 min,
	s64 max
) {
	Assert(value_io);

	if (*value_io < min)  *value_io = min;
	if (*value_io > max)  *value_io = max;
}

instant void
Clamp(
	u64 *value_io,
	u64 min,
	u64 max
) {
	Assert(value_io);

	if (*value_io < min)  *value_io = min;
	if (*value_io > max)  *value_io = max;
}

instant void
Rect_ClampY(
	float *y_io,
	s32 height,
	s32 max_height
) {
	Assert(y_io);

	/// do not show negative space below the last visible
	if (*y_io + height < max_height)
		*y_io = max_height - height;

	/// do not show negative space above the first visible
	if (*y_io > 0)
		*y_io = 0;

	/// avoid scrolling, when all content is visible
	if (height <= max_height)
		*y_io = 0;
}

///@Obsolete
instant void
Rect_ClampY(
	Rect *rect_io,
	Rect  rect_limit
) {
	Assert(rect_io);

	/// do not show negative space below the last visible
	if (rect_io->y + rect_io->h < rect_limit.h)
		rect_io->y = rect_limit.h - rect_io->h;

	/// do not show negative space above the first visible
	if (rect_io->y > 0)
		rect_io->y = 0;

	/// avoid scrolling, when all content is visible
	if (rect_io->h <= rect_limit.h)
		rect_io->y = 0;
}

/// ::: Memory
/// ===========================================================================
#define Memory_Create(type, length) \
		((type *)_Memory_Alloc_Empty(sizeof(type) * length))

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
	Memory_Header *header_out,
	void *mem
) {
	Assert(header_out);
	Assert(mem);

	mem = (char *)mem - sizeof(Memory_Header);

	*header_out = (*(Memory_Header *)mem);
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

/// will set the free'd pointer to 0
/// - extra security
#define Memory_Free(_data) \
	_data = _Memory_Free(_data);

template <typename T>
instant T
_Memory_Free(
	T data
) {
	Assert(std::is_pointer<T>::value);

	if (data == 0)
		return 0;

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

	return 0;
}

instant void
Memory_Copy(
	const void *dest_out,
	const void *src,
	u64 length
) {
	if (!dest_out OR !src OR !length)	return;
	if (dest_out == src)				return;

    char *c_dest = (char *)dest_out;
    char *c_src  = (char *)src;

    if (dest_out > src) {
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
	void *dest_out,
	int data,
	u64 length
) {
	if (dest_out == 0) return;

	u8 *cDest = (u8*)dest_out;
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
	Timer *timer_out
) {
	if (!timer_out)
		return;

	timer_out->lo_timer = GetTickCount();

	LARGE_INTEGER largeCounter;
	QueryPerformanceCounter(&largeCounter);

	timer_out->hi_timer = largeCounter.QuadPart;
	timer_out->counter = 0;
}

instant u32
Time_Get() {
	return GetTickCount();
}

instant bool
Time_HasElapsed(
	Timer *timer_io,
	u32 interval_in_ms,
	bool run_once = false
) {
	Assert(timer_io);

	if (!timer_io->lo_timer) {
		Time_Reset(timer_io);
		return false;
	}

	bool result = true;

	if (Time_Get() - timer_io->lo_timer < interval_in_ms) {
		result = false;
	}
	else {
		if (!run_once) {
			timer_io->lo_timer = Time_Get();
		}
	}

	return result;
}

/// milliseconds
instant double
Time_Measure(
	Timer *timer_io
) {
	if (!timer_io)
		return 0.f;

	LARGE_INTEGER largeCounter;
	QueryPerformanceCounter(&largeCounter);

	/// init timer
	if (timer_io->hi_timer == 0) {
		timer_io->hi_timer = largeCounter.QuadPart;
		return 0.f;
	}

	u64 current_time = largeCounter.QuadPart;

	LARGE_INTEGER largeFreq;
	QueryPerformanceFrequency(&largeFreq);

	double diff = ((double)( (current_time - timer_io->hi_timer)
							* 1000.0)
							/ largeFreq.QuadPart);

	timer_io->hi_timer = current_time;

	return diff;
}

/// has to be used every frame or the calculation will be wrong
instant u32
Time_GetFPS(
	Timer *timer_io,
	bool get_worst = false
) {
	if (!timer_io)
		return 0;

	u32 diff = Time_Get() - timer_io->lo_timer;

	if (timer_io->lo_timer > 0 AND diff < 1000) {
		++timer_io->counter;
	}
	else {
		timer_io->fps_worst = MIN(	timer_io->fps_worst,
									timer_io->counter);

		if (timer_io->fps_worst == 0)
			timer_io->fps_worst = timer_io->counter;

		timer_io->fps = timer_io->counter;
		timer_io->counter = 1;
		timer_io->lo_timer = Time_Get();
	}

	return (get_worst ? timer_io->fps_worst : timer_io->fps);
}

/// Reach the end (from the start) in the timespan of the duration
/// Returns: step increase
instant float
Time_Move(
	Timer *timer_io,
	u32 timespan_in_ms,
	float distance
) {
	Assert(timer_io);

	if (!timespan_in_ms)  return 0.0f;

	double step_size = Time_Measure(timer_io) / timespan_in_ms;
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
	String *s_data_io
) {
	Assert(s_data_io);

	Memory_Free(s_data_io->value);
	*s_data_io = {};

	s_data_io->changed = true;
}

instant void
String_Resize(
	String *s_data_io,
	s64 length_delta
) {
	Assert(s_data_io);

	s_data_io->value = Memory_Resize(s_data_io->value, char, s_data_io->length + length_delta);
}

instant bool
String_Append(
	String *s_data_io,
	const char *c_data,
	u64 length_append = 0
) {
    if (!s_data_io)
		return false;

    if (length_append == 0)  length_append = String_Length(c_data);
    if (length_append == 0)  return false;

	String_Resize(s_data_io, length_append);
	Memory_Copy(s_data_io->value + s_data_io->length, (char *)c_data, length_append);
	s_data_io->length += length_append;

	s_data_io->changed = true;

	return true;
}

instant u64
String_Insert(
	String *s_data_io,
	u64 index_start,
	const char *c_data,
	u64 c_length = 0
) {
	if (!s_data_io)  return 0;
	if (!c_data)  return 0;

	if (!c_length)
		c_length = String_Length(c_data);

	Assert(index_start < s_data_io->length + c_length);

    s_data_io->value = Memory_Resize(s_data_io->value, char, s_data_io->length + c_length);

	Memory_Copy(s_data_io->value + index_start + c_length,
				s_data_io->value + index_start,
				s_data_io->length - index_start);

	FOR_START(index_start, index_start + c_length, it) {
		s_data_io->value[index_start + (it - index_start)] = *c_data++;
	}

	s_data_io->length += c_length;

	s_data_io->changed = true;

	return c_length;
}

instant String
To_String(
	const char *c_data,
	u64 c_length = 0
) {
	Assert(c_data);

	String s_result;
	String_Append(&s_result, c_data, c_length);

	return s_result;
}

instant void
To_StringBuffer(
	String *s_data_out,
	const char *c_data,
	u64 c_length = 0
) {
	Assert(s_data_out);
	Assert(c_data);

	String_Destroy(s_data_out);
	String_Append(s_data_out, c_data, c_length);
}

instant void
String_Clear(
	String *s_data_out
) {
	Assert(s_data_out);

	s_data_out->length = 0;

	s_data_out->changed = true;
}

instant String
To_StringTemp(
	const char *c_data,
	u64 c_length = 0
) {
	Assert(c_data);

	static String s_data_storage;
	String_Clear(&s_data_storage);

	String_Append(&s_data_storage, c_data, c_length);

	return s_data_storage;
}

/// does NOT add memory for '\0'
/// make sure you add an additional byte
/// in the buffer so you don't truncate the string
instant void
To_CString(
	char *c_buffer_out,
	u64 c_length,
	String *s_data
) {
	Assert(s_data);

	if (!c_length)  return;

	if (!s_data->length) {
		c_buffer_out[0] = '\0';
		return;
	}

	/// stop copying memory after '\0'
	if (c_length > s_data->length + 1) {
		c_length = s_data->length + 1;
	}

	/// stop before '\0'
	FOR(c_length - 1, it) {
		c_buffer_out[it] = s_data->value[it];
	}

	/// set final '\0'
	c_buffer_out[c_length - 1] = '\0';
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

instant String
String_CreateBuffer(
	u32 buffer_size
) {
	String s_buffer;
	String_Resize(&s_buffer, buffer_size);
	s_buffer.length = buffer_size;
	s_buffer.changed = true;

	return s_buffer;
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
	u64 c_len = String_Length(c_data);

	if (!s_data OR (s_data AND !s_data->length)) {
		if (!c_len)
			return true;
	}

	if (!length)
		length = c_len;

	if (!s_data AND length)
		return false;

	if (length > s_data->length)
		return false;

	u64 it = 0;

	while(it < length) {
		if (s_data->value[it] != c_data[it])
			return false;

		++it;
	}

	return true;
}

instant String
String_Copy(
	const char *c_source,
	u32 length = 0
) {
	Assert(c_source);

	if (length == 0)
		length = String_Length(c_source);

	String s_result = {};

	s_result.value = Memory_Create(char, length);
	Memory_Copy(s_result.value, c_source, length);
	s_result.length = length;

	s_result.changed = true;

	return s_result;
}

instant void
String_Copy(
	char *dest_out,
	const char *src,
	u32 length = 0
) {
	if (!src)
		return;

	char *result = dest_out;

	if (!length)
		length = String_Length(src);

	/// for '0'-terminator
	++length;

	while (*src AND --length > 0)
		*dest_out++ = *src++;

	*dest_out = '\0';

	dest_out = result;
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
	const char *c_key,
	u64 c_length = 0,
	s64 index_start = 0
) {
	int result = -1;

	if (!s_data OR !s_data->length)
		return result;

	if (!c_length)
		c_length  = String_Length(c_key);

	if (c_length == 0)
		return result;

	u64 length_data = s_data->length;

	if (index_start < 0)
		index_start = 0;

	FOR_START(index_start, length_data, index) {
		if (String_IsEqual(s_data->value + index, c_key, c_length)) {
			return index;
		}
	}

	return result;
}

instant s64
String_IndexOfRev(
	String *s_data,
	const char *c_key,
	u64 c_length = 0,
	s64 index_start = -1
) {
	int result = -1;

	if (!s_data OR !s_data->length)
		return result;

	if (!c_length)
		c_length  = String_Length(c_key);

	if (c_length == 0)
		return result;

	if (index_start > (s64)s_data->length OR index_start < 0)
		index_start = (s64)s_data->length;

	for(s64 it = index_start; it >= 0; --it) {
		if (String_IsEqual(s_data->value + it, c_key, c_length)) {
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
	const char *c_find,
	u64 c_length = 0,
	s64 *index_found = 0,
	s64  index_start = 0
) {
	Assert(s_data);

	s64 t_index_found = String_IndexOf(s_data, c_find, c_length, index_start);

	if (t_index_found < 0) {
		t_index_found = s_data->length - index_start;

		if (t_index_found < 0)
			t_index_found = 0;

		if (index_found) *index_found = t_index_found;
		return false;
	}

	if (index_found) *index_found = t_index_found;

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
	String *s_data_io,
	u32 length
) {
	if (s_data_io AND length < s_data_io->length) {
		s_data_io->length  = length;
		s_data_io->changed = true;
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
	String *s_data_io
) {
	if (!s_data_io)
		return;

	u64 index = 0;
	u64 len_max = s_data_io->length + 1;

	while(index < len_max) {
		if (s_data_io->value[index] >= 'A' AND s_data_io->value[index] <= 'Z') {
			s_data_io->value[index] = s_data_io->value[index] + 32;
		}
		++index;
	}

	s_data_io->changed = true;
}

instant void
String_ToUpper(
	String *s_data_io
) {
	if (!s_data_io)
		return;

	u64 index = 0;
	u64 len_max = s_data_io->length + 1;

	while(index < len_max) {
		if (s_data_io->value[index] >= 'a' AND s_data_io->value[index] <= 'z') {
			s_data_io->value[index] = s_data_io->value[index] - 32;
		}
		++index;
	}

	s_data_io->changed = true;
}

instant void
String_Reverse(
	String *s_data_io
) {
	if (!s_data_io)
		return;

	u64 length = s_data_io->length;

	for(u64 it = 0; it < (length >> 1); ++it) {
		char temp = s_data_io->value[it];
		/// don't reverse the '\0'!
		s_data_io->value[it] = s_data_io->value[length - it - 1];
		s_data_io->value[length - it - 1] = temp;
	}

	s_data_io->changed = true;
}

instant void
String_TrimLeft(
	String *s_data_io
) {
	Assert(s_data_io);

	u64 length = 0;
	u64 len_max = s_data_io->length;

	/// does not skip '\0'
    while(length <= len_max) {
		if (s_data_io->value[length] <= 32 AND s_data_io->value[length] != 127) ++length;
		else break;
    }

    Memory_Copy(s_data_io->value, s_data_io->value + length, s_data_io->length - length);
    s_data_io->length -= length;

    s_data_io->changed = true;
}

instant void
String_TrimRight(
	String *s_data_io
) {
	Assert(s_data_io);

	u64 length = s_data_io->length;

    while(length > 0) {
		if (s_data_io->value[length - 1] <= 32 AND s_data_io->value[length - 1] != 127)
			--length;
		else
			break;
    }

    s_data_io->length = length;

    s_data_io->changed = true;
}

instant u64
String_Remove(
	String *s_data_io,
	u64 index_start,
	u64 index_end
) {
	Assert(s_data_io);

	if (index_start == index_end)
		return 0;

	if (index_start > index_end)
		Swap(&index_start, &index_end);

	if (index_end > s_data_io->length)
		index_end = s_data_io->length;

	u64 length = s_data_io->length - index_end;
	u64 rm_count = (index_end - index_start);

	Memory_Copy(s_data_io->value + index_start, s_data_io->value + index_end, length);
	s_data_io->length -= rm_count;

	s_data_io->changed = true;

	return rm_count;
}

instant void
String_Replace(
	String     *s_data_io,
	const char *find,
	const char *replace
) {
	Assert(s_data_io);
	Assert(find);
	Assert(replace);

	s64 index_found = 0;
	s64 index_start = 0;
	s64 find_length     = String_Length(find);
	s64 replace_length  = String_Length(replace);

  	while(String_Find(s_data_io, find, 0, &index_found, index_start)) {
		String_Remove(s_data_io, index_found, index_found + find_length);
		String_Insert(s_data_io, index_found, replace);
		index_start += index_found + replace_length;
	}

	s_data_io->changed = true;
}

instant void
String_Replace(
	String     *s_data_io,
	const char *find,
	String     *s_replace
) {
	Assert(s_data_io);
	Assert(find);
	Assert(s_replace);

	char *c_replace = String_CreateCBufferCopy(s_replace);
	String_Replace(s_data_io, find, c_replace);
	Memory_Free(c_replace);

	s_data_io->changed = true;
}

instant s64
String_Insert(
	String *s_data_io,
	u64 index_start,
	const char c_data
) {
	Assert(s_data_io);

	s64 length = 0;

	if (c_data == '\b') {
		/// make sure there is something to remove
		if (index_start) {
			length = 0;
			if (s_data_io->value[index_start + length - 1] == '\n')  --length;
			if (s_data_io->value[index_start + length - 1] == '\r')  --length;
			if (!length) --length;

			String_Remove(s_data_io, index_start + length, index_start);
		}
	}
	else
	if (c_data == '\r' OR c_data == '\n') {
		length = 1;
		String_Insert(s_data_io, index_start, "\n", length);
	}
	else {
		length = 1;
		String_Insert(s_data_io, index_start, &c_data, length);
	}

	s_data_io->changed = true;

	return length;
}

instant void
String_Cut(
	String *s_data_io,
	const char *c_start,
	const char *c_end
) {
	Assert(s_data_io);

	s64 start = 0, end = 0;
	u64 len_end = String_Length(c_end);

	while (true) {
		start = String_IndexOf(s_data_io, c_start);

		if (start < 0)
			break;

		end = String_IndexOf(s_data_io, c_end, 0, start);

		if (end > start)
			String_Remove(s_data_io, start, end + len_end);
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
		_it < (_array).count;       \
		++_it)

#define FOR_ARRAY_REV(_array, _it)	\
	for(s64 _it = (_array).count - 1;	\
		_it >= 0;						\
		--_it)

#define FOR_ARRAY_START(_array, _start, _it) 	\
	for(u64 _it = (_start);   					\
		_it < (_array).count;       			\
		++_it)

template <typename T>
struct Array {
	T    *memory   = 0;
	u64   size     = 0;
	u64   limit    = 0; /// in bytes
	u64   count    = 0;

	/// for string chunks for now
	bool  by_reference = false;
};

///@Info: will copy the struct element pointers only
///
///       if the array should take ownership of all data
///       make sure you copy every data that is connected
///       to a pointer (clone) or don't free the
///       passed / connected data
template <typename T>
instant T *
Array_Add(
	Array<T> *array_io,
	T element
) {
	Assert(array_io);

	constexpr u64 length = 1;

	if (array_io->size + sizeof(T) * length > array_io->limit) {
		array_io->limit += sizeof(T) * length;
		array_io->memory = (T *)_Memory_Resize(array_io->memory, array_io->limit);;
	}

	u64 target = array_io->size / sizeof(T);

	array_io->memory[target] = element;
	array_io->size += sizeof(T) * length;

	++array_io->count;

	return &array_io->memory[target];
}

template <typename T>
instant u64
Array_AddEmpty(
	Array<T> *array_io,
	T **element_empty_out
) {
	Assert(array_io);
	Assert(element_empty_out);

	T t_element_empty = {};
	Array_Add(array_io, t_element_empty);
	*element_empty_out = &ARRAY_IT(*array_io, array_io->count - 1);

	return array_io->count - 1;
}

template<typename T>
instant void
Array_ClearContainer(
	Array<T> *array_out
) {
	Assert(array_out);

	array_out->size = 0;
	array_out->count = 0;
}

template<typename T>
instant void
Array_DestroyContainer(
	Array<T> *array_out
) {
	Assert(array_out);

	Memory_Free(array_out->memory);
	*array_out = {};
}

/// Will add memory slots on top of existing ones and add to that count
template <typename T>
instant void
Array_ReserveAdd(
	Array<T> *array_io,
	u64 count_delta,
	bool clear_zero = false
) {
	Assert(array_io);

	u64 old_limit = array_io->limit;

	if (array_io->size + sizeof(T) * count_delta > array_io->limit) {
		array_io->limit += sizeof(T) * count_delta;
		array_io->memory = (T *)_Memory_Resize(array_io->memory, array_io->limit);
	}

	if (clear_zero) {
		/// only clear new reserved data
		Memory_Set(array_io->memory + array_io->count, 0, array_io->limit - old_limit);
	}
}

template <typename T>
instant void
Array_Reserve(
	Array<T> *array_io,
	u64 count,
	bool clear_zero = false
) {
	Assert(array_io);

	u64 old_limit = array_io->limit;
	u64 new_limit = array_io->size + sizeof(T) * count;

	if (new_limit > array_io->limit) {
		array_io->limit = new_limit;
		array_io->memory = (T *)_Memory_Resize(array_io->memory, array_io->limit);
	}

	if (clear_zero) {
		/// only clear new reserved data
		Memory_Set(array_io->memory + array_io->count, 0, array_io->limit - old_limit);
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
			if (index)  *index = it;
			return true;
		}
	}

	return false;
}

template <typename T>
instant bool
Array_FindOrAdd(
	Array<T> *array_io,
	T find,
	T **entry_out
) {
	Assert(array_io);
	Assert(entry_out);

	u64 t_index_find;
	bool found_element = Array_Find(array_io, find, &t_index_find);

	if (found_element) {
		*entry_out = &ARRAY_IT(*array_io, t_index_find);
	}
	else {
        Array_AddEmpty(array_io, entry_out);

        /// store what you want to find, if it does not exists,
        /// so it does not have to be assigned manually all the time
        **entry_out = find;
	}

	return found_element;
}

/// Returns T, so dynamic memory can still be free'd
template <typename T>
instant T
Array_Remove(
	Array<T> *array_io,
	u64 index
) {
	Assert(array_io);
	Assert(index < array_io->count);

	T result = ARRAY_IT(*array_io, index);

	FOR_ARRAY_START(*array_io, index, it) {
		if (it + 1 >= array_io->count)
			break;

		ARRAY_IT(*array_io, it) = ARRAY_IT(*array_io, it + 1);
	}

	array_io->count -= 1;
	array_io->size  -= sizeof(T);

	return result;
}

instant void
Array_Destroy(
	Array<String> *array_out
) {
	Assert(array_out);

	if (!array_out->by_reference) {
		while(array_out->count) {
			///@Idea: might be faster to remove from
			///       the end to avoid reordering
			String s_data_it = Array_Remove(array_out, 0);
			String_Destroy(&s_data_it);
		}
	}

    Array_DestroyContainer(array_out);
}

instant bool
String_FindFirst(
	String *s_data,
	Array<String> *as_find,
	s64 *index_delimiter_used_out = 0,
	s64 *index_found_out = 0,
	s64 pos_start = 0
) {
	Assert(s_data);
	Assert(as_find);

	s64 index_lowest = s_data->length;

	s64 t_index_delimiter_used = -1;

	FOR_ARRAY(*as_find, it) {
		String *ts_find = &ARRAY_IT(*as_find, it);
		s64 t_index_found = String_IndexOf(s_data, ts_find->value, ts_find->length);

		if (t_index_found >= 0) {
			if (t_index_found < index_lowest) {
			 	t_index_delimiter_used = it;
			}

			index_lowest = MIN(index_lowest, t_index_found);
		}
	}

	if (index_delimiter_used_out)
		*index_delimiter_used_out = t_index_delimiter_used;

	if (t_index_delimiter_used < 0) {
		if (index_found_out)
			*index_found_out = index_lowest;

		return false;
	}

	if (index_found_out)
		*index_found_out = index_lowest;

	return true;
}

enum DELIMITER_TYPE {
	DELIMITER_IGNORE,
	/// Insert delimiter at the beginning of the next token
	DELIMITER_ADD_FRONT,
	/// Insert delimiter at the end if the current token
	DELIMITER_ADD_BACK
};

instant void
Array_Clear(
	Array<String> *array_out
) {
	Assert(array_out);

	if (!array_out->by_reference) {
		FOR_ARRAY(*array_out, it) {
			String *ts_data = &ARRAY_IT(*array_out, it);
			String_Destroy(ts_data);
		}
	}

    Array_ClearContainer(array_out);
}

///@Info: slower than String_SplitWordsBuffer
///@RemoveMe(?)
instant void
Array_SplitWordsRefBuffer(
	Array<String> *as_buffer_out,
	String *s_data
) {
	Assert(s_data);

	Array_Clear(as_buffer_out);
	as_buffer_out->by_reference = true;

	String s_data_it = *s_data;

	DELIMITER_TYPE type_delim;
	s64 index_found;

	bool is_running = (s_data_it.length > 0);

	s64 index_delimiter_used = 0;
	static Array<String> as_delimiter;
	Array_Reserve(&as_delimiter, 2);

	Array_Clear(&as_delimiter);
	Array_Add(&as_delimiter, To_String("\n"));
	Array_Add(&as_delimiter, To_String(" "));

	u64 c_length = 1;

	while(is_running) {
		is_running = String_FindFirst(&s_data_it, &as_delimiter, &index_delimiter_used, &index_found);

		String *s_element;
		Array_AddEmpty(as_buffer_out, &s_element);

		if (index_delimiter_used == 0)
			type_delim = DELIMITER_ADD_BACK;
		else
			type_delim = DELIMITER_IGNORE;

		if (type_delim == DELIMITER_ADD_BACK OR index_found == 0)
			s_element->length += c_length;

		s_element->value   = s_data_it.value;
		s_element->length += index_found;

		if (type_delim == DELIMITER_ADD_FRONT AND as_buffer_out->count > 1) {
			s_element->value  -= c_length;
			s_element->length += c_length;
		}

		s_data_it.value  += index_found + c_length;
		s_data_it.length -= index_found + c_length;
	}
}

instant void
Array_SplitRefBuffer(
	Array<String> *as_buffer_out,
	String *s_data,
	const char *c_delimiter,
	u64 c_length,
	DELIMITER_TYPE type
) {
	Assert(s_data);

	Array_Clear(as_buffer_out);
	as_buffer_out->by_reference = true;

	String s_data_it = *s_data;

	if (!c_length)
		c_length = String_Length(c_delimiter);

	s64 index_found;

	bool is_running = (s_data_it.length > 0);

	while(is_running) {
		if (!String_Find(&s_data_it, c_delimiter, c_length, &index_found)) {
			/// add (the last or first) line without a delimiter
			index_found = s_data_it.length;
			type = DELIMITER_IGNORE;

			Assert(s_data_it.length <= s_data->length);

			if (!s_data_it.length)
				break;

			is_running = false;
		}

		String *s_element;
		Array_AddEmpty(as_buffer_out, &s_element);

		if (type == DELIMITER_ADD_BACK OR index_found == 0)
			s_element->length += c_length;

		s_element->value   = s_data_it.value;
		s_element->length += index_found;

		if (type == DELIMITER_ADD_FRONT AND as_buffer_out->count > 1) {
			s_element->value  -= c_length;
			s_element->length += c_length;
		}

		s_data_it.value  += index_found + c_length;
		s_data_it.length -= index_found + c_length;
	}
}

instant Array<String>
Array_SplitRef(
	String *s_data,
	const char *c_delimiter,
	u64 c_length = 0,
	DELIMITER_TYPE type = DELIMITER_IGNORE

) {
	Array<String> as_result;

	Array_SplitRefBuffer(&as_result, s_data, c_delimiter, c_length, type);

	return as_result;
}

/// Will copy string values, so array content has to be free'd
instant void
Array_SplitBuffer(
	Array<String> *as_buffer_out,
	String *s_data,
	const char *c_delimiter,
	u64 c_length = 0,
	DELIMITER_TYPE type = DELIMITER_IGNORE

) {
	Assert(s_data);
	Assert(as_buffer_out);

	Array_Clear(as_buffer_out);

	String s_data_it = *s_data;
	u64 len_delim = String_Length(c_delimiter);

	s64 pos_found;
	while(String_Find(&s_data_it, c_delimiter, c_length, &pos_found)) {
		if (pos_found) {
			String s_element;

			if (type == DELIMITER_ADD_FRONT AND as_buffer_out->count) {
				String_Append(&s_element, c_delimiter, len_delim);
			}

			String_Append(&s_element, s_data_it.value, pos_found);

			if (type == DELIMITER_ADD_BACK) {
				String_Append(&s_element, c_delimiter, len_delim);
			}

			Array_Add(as_buffer_out, s_element);
		}
		else {
			/// in case of f.e: "\n\n\n" with "\n" as delimiter
			String *s_element;
			Array_AddEmpty(as_buffer_out, &s_element);

			if (type == DELIMITER_ADD_BACK) {
				String_Append(s_element, c_delimiter, len_delim);
			}
		}

		s_data_it.value  += pos_found + len_delim;
		s_data_it.length -= pos_found + len_delim;
	}

	if (s_data_it.length > 0) {
		String s_element;

		if (type == DELIMITER_ADD_FRONT AND as_buffer_out->count)
			String_Append(&s_element, c_delimiter, len_delim);

		String_Append(&s_element, s_data_it.value, s_data_it.length);
		Array_Add(as_buffer_out, s_element);
	}
}

instant Array<String>
Array_Split(
	String *s_data,
	const char *c_delimiter,
	u64 c_length = 0,
	DELIMITER_TYPE type = DELIMITER_IGNORE
) {
	Array<String> as_result;

	Array_SplitBuffer(&as_result, s_data, c_delimiter, c_length, type);

	return as_result;
}

instant u64
String_CalcWordCount(
	String *s_data
) {
	Assert(s_data);

	u64 count_words = 0;

	FOR(s_data->length, it) {
		char value = s_data->value[it];

		if ((value == ' ') OR (value == '\n'))
			++count_words;
	}

	return count_words;
}

/// returns number of line-breaks
instant u64
String_SplitWordsBuffer(
	String *s_data,
	Array<String> *as_words_out
) {
	Assert(s_data);
	Assert(as_words_out);

	MEASURE_START();

	if (as_words_out->limit) {
		Array_Clear(as_words_out);
	}
	else {
		as_words_out->by_reference = true;
		Array_Reserve(as_words_out, String_CalcWordCount(s_data));
	}

	String *s_element;

	if (s_data->length)
		Array_AddEmpty(as_words_out, &s_element);

	u64 index_start = 0;
	u64 index_end   = 0;

	u64 number_of_linebreaks = 0;

	FOR(s_data->length, it) {
		char value = s_data->value[it];

		index_end = it;

		if ((value == ' ') OR (value == '\n')) {
			++index_end;

			if (index_end - index_start > 0) {
				s_element->value  = s_data->value + index_start;
				s_element->length = (index_end - index_start);
			}

			Array_AddEmpty(as_words_out, &s_element);

			if (value == '\n')
				++number_of_linebreaks;

			/// add number of chars of the value identifier
			index_start = it + 1;
		}
	}

	s_element->value  = s_data->value + index_start;

	/// for everything (left) that has no space or line-break
	if (index_end - index_start > 0)
		s_element->length = (index_end - index_start) + 1;

	/// for a curser position past the last one
	s_element->length += 1;

	MEASURE_END("");

	return number_of_linebreaks;
}

template <typename T>
instant void
Array_Sort_Quick_Ascending(
	T *begin_io,
	T *end_io
) {
	Assert(begin_io);
	Assert(end_io);

	if (begin_io == end_io)
		return;

    T *pivot = begin_io;
    T *next  = begin_io;
	++next;

	while(next <= end_io) {
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

    if (begin_io < pivot)  Array_Sort_Quick_Ascending(begin_io	, pivot - 1	);
	if (end_io   > pivot)  Array_Sort_Quick_Ascending(pivot + 1	, end_io	);
}

template <typename T>
instant void
Array_Sort_Quick_Descending(
	T *begin_io,
	T *end_io
) {
	Assert(begin_io);
	Assert(end_io);

	if (begin_io == end_io)
		return;

    T *pivot = begin_io;
    T *next  = begin_io;
	++next;

	while(next <= end_io) {
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

    if (begin_io < pivot)  Array_Sort_Quick_Descending(begin_io , pivot - 1	);
	if (end_io   > pivot)  Array_Sort_Quick_Descending(pivot + 1, end_io	);
}

template <typename T>
instant void
Array_Sort_Ascending(
	Array<T> *array_io
) {
	Assert(array_io);
	Assert(array_io->count);

	Array_Sort_Quick_Ascending( &array_io->memory[0], &array_io->memory[array_io->count - 1]);
}

template <typename T>
instant void
Array_Sort_Descending(
	Array<T> *array_io
) {
	Assert(array_io);
	Assert(array_io->count);

	Array_Sort_Quick_Descending(&array_io->memory[0], &array_io->memory[array_io->count - 1]);
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

template <typename T>
instant Array<T>
Array_CreateBuffer(
	u64 count
) {
	Array<T> a_buffer;
	Array_ReserveAdd(&a_buffer, count, true);
	a_buffer.count = count;
	a_buffer.size = a_buffer.limit;

	return a_buffer;
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
	CPU_ID *cpu_id_out
) {
	Assert(cpu_id_out);

	asm volatile
		("cpuid" :
		 "=a" (cpu_id_out->EAX), "=b" (cpu_id_out->EBX),
		 "=c" (cpu_id_out->ECX), "=d" (cpu_id_out->EDX)
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

instant Array<const char *>
CPU_GetFeaturesName(
) {
	Array<const char *> a_features_out;

	CPU_Features cpu_features = CPU_GetFeatures();

	if (cpu_features.mmx)     Array_Add(&a_features_out, "MMX");
	if (cpu_features.mmx_ext) Array_Add(&a_features_out, "MMX Ext");

	if (cpu_features.sse)     Array_Add(&a_features_out, "SSE");
	if (cpu_features.sse2)    Array_Add(&a_features_out, "SSE2");
	if (cpu_features.sse3)    Array_Add(&a_features_out, "SSE3");
	if (cpu_features.sse4_1)  Array_Add(&a_features_out, "SSE4.1");
	if (cpu_features.sse4_2)  Array_Add(&a_features_out, "SSE4.2");

	if (cpu_features._3dnow)     Array_Add(&a_features_out, "3DNow");
	if (cpu_features._3dnow_ext) Array_Add(&a_features_out, "3DNow Ext");

	return a_features_out;
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
	File *file_io
) {
	Assert(file_io);

	/// Returns 0 on success
	return (fclose(file_io->fp) == 0);
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
	const char *data_out,
	u64 length = 0
) {
	Assert(file);

	if (!length)
		length = String_Length(data_out);

    fwrite(data_out, sizeof(char), sizeof(char) * length, file->fp);
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

	MEASURE_START();

    File file;

    if (as_binary)
		file = File_Open(c_filename, "rb", c_length);
	else
		file = File_Open(c_filename, "r" , c_length);

	if (!file.fp)
		LOG_WARNING("File \"" << c_filename << "\" does not exists.");

	String s_data = File_Read(&file);

	File_Close(&file);

	s_data.changed = true;

	MEASURE_END("(" << c_filename << ") ");

	return s_data;
}

struct File_Watcher {
	HANDLE file = 0;
	bool exists = false;
	FILETIME lastWriteTime = {};
};

instant void
File_Watch(
	File_Watcher *file_watcher_out,
	const char *c_filename,
	u64 c_length = 0
) {
	Assert(file_watcher_out);

	*file_watcher_out = {};

	char *tc_filename = String_CreateCBufferCopy(c_filename, c_length);

	file_watcher_out->file = CreateFile(
			tc_filename,
			0,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL
		);

	if (file_watcher_out->file)
		file_watcher_out->exists = true;

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

	if (lwt->dwHighDateTime != prevWriteTime.dwHighDateTime)
		has_changed = true;
	else
	if (lwt->dwLowDateTime  != prevWriteTime.dwLowDateTime )
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

///@HINT: test this with backup data, before using
///       or it could leave a mess
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

	s_result = String_Copy(s_data->value + pos_ext, s_data->length - pos_ext);

	return s_result;
}

/// ::: Windows (OpenGL)
/// ===========================================================================
#define Windows_Main 	\
	APIENTRY			\
	WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_text, int nCmdShow)

#define LOG_ERROR(text) std::cerr << "[Error] " << text << std::endl

#define Window_IsCreated(window) (window->hWnd != 0)

#define Window_ReadMessage(_msg, _running, _ptr_window) 				            \
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
	Window *window_out,
	const char *title
) {
	Assert(window_out);
	Assert(title);

	window_out->title = title;

	SetWindowText(window_out->hWnd, window_out->title);
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

/// ::: OpenGL
/// ===========================================================================
instant void
OpenGL_Init(
	Window *window_io
) {
	Assert(window_io);

	if (window_io->hDC) {
		window_io->hRC = wglCreateContext(window_io->hDC);
		wglMakeCurrent(window_io->hDC, window_io->hRC);

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
	Window *window_io
) {
	Assert(window_io);

	if (window_io->hRC) {
		wglMakeCurrent(0, 0);
		wglDeleteContext(window_io->hRC);
	}
}

instant void
OpenGL_SetVSync(
	Window *window_out,
	bool enable
) {
	Assert(window_out);

	if(wglSwapIntervalEXT)
		wglSwapIntervalEXT(enable);

	window_out->useVSync = enable;

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
	Window *window_io,
	bool zooming = true
) {
	Assert(window_io);

	RECT rect_window;
	GetClientRect(window_io->hWnd, &rect_window);
	s32 new_width  = rect_window.right  - rect_window.left;
	s32 new_height = rect_window.bottom - rect_window.top;

	float x = 0.f;
	float y = 0.f;

	if (!zooming) {
		bool result =    window_io->width  != new_width
		              OR window_io->height != new_height;

		glViewport(x, y, new_width, new_height);
		window_io->scale_x = 1.0f;
		window_io->scale_y = 1.0f;
		window_io->width = new_width;
		window_io->height = new_height;

		return result;
	}

	Rect_GetAspect(
		window_io->width,
		window_io->height,
		&new_width,
		&new_height,
		&x,
		&y
	);

    window_io->x_viewport = x;
    window_io->y_viewport = y;

	float prev_scale_x = window_io->scale_x;
	float prev_scale_y = window_io->scale_y;

	if (window_io->width > new_width)
		window_io->scale_x = (float)new_width / window_io->width;
	else
		window_io->scale_x = (float)window_io->width / new_width;


	if (window_io->height > new_height)
		window_io->scale_y = (float)new_height / window_io->height;
	else
		window_io->scale_y = (float)window_io->height / new_height;

	glViewport(x, y, new_width, new_height);

    if (prev_scale_x != window_io->scale_x) return true;
    if (prev_scale_y != window_io->scale_y) return true;

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
	Image *image_out
) {
	Assert(image_out);

	Memory_Free(image_out->data);

	*image_out = {};
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
	Array<Tuple<Texture, Array<float>>> *array_out
) {
	Assert(array_out);

	FOR_ARRAY(*array_out, it) {
		Tuple<Texture, Array<float>> *t_tuple = &ARRAY_IT(*array_out, it);
		Array<float> *a_second = &t_tuple->second;

		Array_DestroyContainer(a_second);
	}

	Array_DestroyContainer(array_out);
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
	s32 *width_out,
	s32 *height_out
) {
    Assert(texture);

    glBindTexture(GL_TEXTURE_2D, texture->ID);
    if (width_out)  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH , width_out);
	if (height_out) glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height_out);
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
	Texture *texture_out
) {
	Assert(texture_out);

	glDeleteTextures(1, &texture_out->ID);
	texture_out->ID = 0;
}

instant void
Texture_Reload(
	Texture *texture_io,
	const char *c_filename,
	u64 c_length = 0
) {
	Assert(texture_io);

	if (File_HasExtension(c_filename, ".bmp", c_length)) {
		u32 format_input;

		/// 32-bit bmp only
		Image image = Image_LoadBMP32(c_filename, c_length);

		if (image.flip_h)  format_input = GL_ABGR_EXT;
		else               format_input = GL_RGBA;

		Texture_Destroy(texture_io);
		*texture_io = Texture_Load(image.data, image.width, image.height, format_input, false);
		texture_io->flip_h = image.flip_h;

		Image_Destroy(&image);

		return;
	}

	if (File_HasExtension(c_filename, ".jpg|.png|.gif", c_length)) {
		char *tc_filename = String_CreateCBufferCopy(c_filename, c_length);

		s32 width, height, bits;
		u8 *c_data = stbi_load(tc_filename, &width, &height, &bits, 4);

		Texture_Destroy(texture_io);
		*texture_io = Texture_Load(c_data, width, height, GL_RGBA, true);

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

	uniform float x_offset = 0.0f;
	uniform float y_offset = 0.0f;

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
		gl_Position           = vec4(vertex_position.x + x_offset,
									 vertex_position.y + y_offset,
									 vertex_position.z + x_offset,
									 vertex_position.w + y_offset);

		o_Vertex.proj_matrix  = proj_matrix;
		o_Vertex.scale_matrix = scale_matrix;
		o_Vertex.rect_color   = rect_color;
	}
)",

R"(
	#version 330 core

	layout(points) in;
	layout(triangle_strip) out;

	/// would show a disconnected section between 2 rects
	/// when using 4 vertices and nvidia gfx card (tested with 1050)
	/// so 6 vertices would overlap that area
	layout(max_vertices = 6) out;

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

		// ---------------------------------
		gl_Position = v_pos_1;
		rect_color = i_Vertex[0].rect_color;
		EmitVertex();

		gl_Position = v_pos_2;
		rect_color = i_Vertex[0].rect_color;
		EmitVertex();

		gl_Position = v_pos_3;
		rect_color = i_Vertex[0].rect_color;
		EmitVertex();

		// ---------------------------------
		gl_Position = v_pos_4;
		rect_color = i_Vertex[0].rect_color;
		EmitVertex();

		gl_Position = v_pos_1;
		rect_color = i_Vertex[0].rect_color;
		EmitVertex();

		// ---------------------------------

		gl_Position = v_pos_2;
		rect_color = i_Vertex[0].rect_color;
		EmitVertex();

		EndPrimitive();
	}
)",

R"(
	#version 330 core

	layout(origin_upper_left) in vec4 gl_FragCoord;

	in  vec4 rect_color;
	out vec4 out_frag_color;

	void main() {
		out_frag_color = rect_color;
	}
)"};

static const Shader shader_text = {
	SHADER_PROG_TEXT,
R"(
	#version 330 core

	uniform vec4  viewport = vec4(0, 0, 800, 480);
	uniform float scale_x  = 1.0f;
	uniform float scale_y  = 1.0f;

	uniform float x_offset = 0.0f;
	uniform float y_offset = 0.0f;

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
		gl_Position = vec4( vertex_position.x + x_offset,
							vertex_position.y + y_offset,
							0,
							1);

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

		/// v1 (top-left)
		gl_Position = v_pos_1;
		tex_coords = vec2(0, 0);
		text_color = i_Vertex[0].text_color;
		EmitVertex();

		/// v2 (bottom-left)
		gl_Position = v_pos_2;
		tex_coords = vec2(1, 0);
		text_color = i_Vertex[0].text_color;
		EmitVertex();

		/// v3 (top-right)
		gl_Position = v_pos_3;
		tex_coords = vec2(0, 1);
		text_color = i_Vertex[0].text_color;
		EmitVertex();

		/// v4 (bottom-rights)
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

	out vec4 out_frag_color;

	void main() {
		vec4 color_greyscale = texture2D(fragment_texture, tex_coords);
		out_frag_color = vec4(text_color.xyz, color_greyscale.a);
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

	out vec4 out_frag_color;

	void main() {
		out_frag_color = texture2D(fragment_texture, i_Vertex.tex_coords);
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

	out vec4 out_frag_color;

	void main() {
		out_frag_color = texture2D(fragment_texture, tex_coords);
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
	ShaderProgram *shader_prog_out,
	u32 type,
	const char *code
) {
	Assert(shader_prog_out);

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
		case GL_VERTEX_SHADER:   { shader_prog_out->vertex_id   = id_shader; } break;
		case GL_GEOMETRY_SHADER: { shader_prog_out->geometry_id = id_shader; } break;
		case GL_FRAGMENT_SHADER: { shader_prog_out->fragment_id = id_shader; } break;
		default: {
			LOG_DEBUG("Unimplemented shader type");
		}
	}
}

/// returns shader array index
instant u32
ShaderSet_Add(
	ShaderSet *shader_set_io,
	const Shader *shader
) {
	Assert(shader_set_io);
    Assert(shader);

	ShaderProgram *shader_prog;
	u64 array_id = Array_AddEmpty(&shader_set_io->a_shaders, &shader_prog);

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
	ShaderProgram *shader_prog_out
) {
	Assert(shader_prog_out);

    if (shader_prog_out->id) {
		glDeleteProgram(shader_prog_out->id);
		glDeleteShader(shader_prog_out->vertex_id);
		glDeleteShader(shader_prog_out->geometry_id);
		glDeleteShader(shader_prog_out->fragment_id);

		*shader_prog_out = {};
    }
}

inline void
ShaderSet_Destroy(
	ShaderSet *shader_set_out
) {
    Assert(shader_set_out);

    FOR_ARRAY(shader_set_out->a_shaders, it) {
		ShaderProgram *t_shader_prog = &ARRAY_IT(shader_set_out->a_shaders, it);
		ShaderProgram_Destroy(t_shader_prog);
    }

    *shader_set_out = {};
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
	ShaderSet *shader_set_io,
	SHADER_PROG_TYPE type
) {
	Assert(shader_set_io);
	Assert(shader_set_io->window);

	s64 prev_active_id = shader_set_io->active_id;

	switch (type) {
		case SHADER_PROG_RECT:
		case SHADER_PROG_TEXT:
		case SHADER_PROG_TEXTURE_FULL:
		case SHADER_PROG_TEXTURE_SIZE: {
			shader_set_io->active_id = type;
		} break;

		default: {
			AssertMessage(	false,
							"Unhandled Shader-Program type.");
		} break;
	}

	/// reset uniforms
	Shader_SetValue(shader_set_io, "x_offset", 0.0f);
	Shader_SetValue(shader_set_io, "y_offset", 0.0f);

	if (prev_active_id == shader_set_io->active_id)
		return;

	Assert((u64)shader_set_io->active_id < shader_set_io->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set_io->a_shaders, shader_set_io->active_id);

	glUseProgram(shader_prog->id);

	if (shader_set_io->window) {
		RectF viewport;
		Window *t_window = shader_set_io->window;

		viewport.x = t_window->x_viewport;
		viewport.y = t_window->y_viewport;
		viewport.w = (float)t_window->width;
		viewport.h = (float)t_window->height;

		Shader_SetValue(shader_set_io, "viewport", (float *)&viewport, 4);

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

instant void
Vertex_GetTextureSize(
	Vertex *vertex,
	s32 *width_out,
	s32 *height_out
) {
	Assert(vertex);
	Assert(vertex->texture.ID);

	GLint id_bound = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &id_bound);

	bool need_bind_restoring = false;

	if (vertex->texture.ID != (u32)id_bound) {
		glBindTexture(GL_TEXTURE_2D, vertex->texture.ID);
		need_bind_restoring = true;
	}

    if (width_out)  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH , width_out);
	if (height_out) glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height_out);

	if (need_bind_restoring) {
		glBindTexture(GL_TEXTURE_2D, id_bound);
		LOG_DEBUG("Waring [Performance]: temporary bindings will decrease performance.");
	}
}

inline void
Vertex_Destroy(
	Vertex *vertex_out
) {
	Assert(vertex_out);

	FOR_ARRAY(vertex_out->a_attributes, it) {
		Vertex_Buffer<float> *t_attribute = &ARRAY_IT(vertex_out->a_attributes, it);
		glDeleteBuffers(1, &t_attribute->id);
		Array_DestroyContainer(&t_attribute->a_buffer);
	}

	glDeleteVertexArrays(1, &vertex_out->array_id);
	Array_DestroyContainer(&vertex_out->a_attributes);

	*vertex_out = {};
}

inline void
Vertex_SetTexture(
	ShaderSet *shader_set,
	Vertex *vertex_out,
	Texture *texture
) {
	Assert(shader_set);
	Assert(vertex_out);
	Assert(texture);

	vertex_out->texture = *texture;
	vertex_out->settings.flip_h = texture->flip_h;

	/// use this uniform in a shader, if the texture index has to change
	Shader_BindAndUseIndex0(shader_set, "fragment_texture", &vertex_out->texture);
}

instant void
Vertex_Create(
	Vertex *vertex_out
) {
	Assert(vertex_out);
	Assert(vertex_out->array_id == 0);

	glGenVertexArrays(1, &vertex_out->array_id);
}

instant Vertex
Vertex_Create(
) {
	Vertex vertex = {};

	Vertex_Create(&vertex);

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
	Vertex *vertex_out
) {
	Assert(vertex_out);

	FOR_ARRAY(vertex_out->a_attributes, it) {
		auto *t_attribute = &ARRAY_IT(vertex_out->a_attributes, it);
		Array_ClearContainer(&t_attribute->a_buffer);
	}
}

instant void
Vertex_ClearAttributes(
	Array<Vertex> *a_vertex_out
) {
	Assert(a_vertex_out);

	FOR_ARRAY(*a_vertex_out, it_vertex) {
		Vertex *t_vertex = &ARRAY_IT(*a_vertex_out, it_vertex);

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
	Array<Vertex> *a_vertex_io,
	Texture *texture_find,
	Vertex **entry_out
) {
	Assert(a_vertex_io);
	Assert(texture_find);
	Assert(entry_out);

	Vertex *t_vertex_entry;
	Vertex t_vertex_find;
	t_vertex_find.texture = *texture_find;

	Array_FindOrAdd(a_vertex_io, t_vertex_find, &t_vertex_entry);

	if (!t_vertex_entry->array_id)
		Vertex_Create(t_vertex_entry);

	*entry_out = t_vertex_entry;
}

instant void
Vertex_FindOrAddAttribute(
	Vertex *vertex_io,
	u32 group_count,
	const char *c_attribute_name,
	Vertex_Buffer<float> **a_buffer_out
) {
	Assert(vertex_io);
	Assert(c_attribute_name);
	Assert(a_buffer_out);

	Vertex_Buffer<float> t_attribute_find;
	t_attribute_find.name = c_attribute_name;
	t_attribute_find.group_count = group_count;

	Array_FindOrAdd(&vertex_io->a_attributes, t_attribute_find, a_buffer_out);
}

instant void
Vertex_AddTexturePosition(
	Vertex *vertex_io,
	float x,
	float y
) {
	Assert(vertex_io);

	Vertex_Buffer<float> *t_attribute;

	Vertex_FindOrAddAttribute(vertex_io, 2, "vertex_position", &t_attribute);
	Array_Reserve(&t_attribute->a_buffer, 2);
	Array_Add(&t_attribute->a_buffer, x);
	Array_Add(&t_attribute->a_buffer, y);
}

instant void
Vertex_AddRect32(
	Vertex *vertex_io,
	Rect rect,
	Color32 color
) {
	Assert(vertex_io);

	Vertex_Buffer<float> *t_attribute;

	Vertex_FindOrAddAttribute(vertex_io, 4, "vertex_position", &t_attribute);
	Array_Reserve(&t_attribute->a_buffer, 4);
	Array_Add(&t_attribute->a_buffer, (float)rect.x);
	Array_Add(&t_attribute->a_buffer, (float)rect.y);
	Array_Add(&t_attribute->a_buffer, (float)rect.x + rect.w);
	Array_Add(&t_attribute->a_buffer, (float)rect.y + rect.h);

	Vertex_FindOrAddAttribute(vertex_io, 4, "rect_color", &t_attribute);
	Array_Reserve(&t_attribute->a_buffer, 4);
	Array_Add(&t_attribute->a_buffer, (float)color.r);
	Array_Add(&t_attribute->a_buffer, (float)color.g);
	Array_Add(&t_attribute->a_buffer, (float)color.b);
	Array_Add(&t_attribute->a_buffer, (float)color.a);
}

instant void
Vertex_AddRectTexture(
	Vertex *vertex_io,
	Rect rect
) {
	Assert(vertex_io);

	Vertex_Buffer<float> *t_attribute;

	Vertex_FindOrAddAttribute(vertex_io, 4, "vertex_position", &t_attribute);
	Array_Reserve(&t_attribute->a_buffer, 4);
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
#define MOUSE_WHEEL_GET_DELTA(value) (GET_WHEEL_DELTA_WPARAM(value) / WHEEL_DELTA)
#define MOUSE_BUTTON_COUNT 3

struct Mouse {
	Point point          = {};
	Point point_relative = {};

	bool up[MOUSE_BUTTON_COUNT];
	bool down[MOUSE_BUTTON_COUNT];
	bool pressing[MOUSE_BUTTON_COUNT];
	s32  wheel = 0;

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
	Mouse *mouse_out
) {
	if (!mouse_out)
		return;

	mouse_out->wheel = 0;

	FOR(MOUSE_BUTTON_COUNT, it) {
		mouse_out->down[it] = false;
		mouse_out->up[it]   = false;
	}

	mouse_out->is_up        = false;
	mouse_out->is_down      = false;
	mouse_out->is_moving    = false;

	mouse_out->point_relative = {};
}

instant void
Mouse_GetPosition(
	float *x_out,
	float *y_out,
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


	if (x_out) *x_out = point.x;
	if (y_out) *y_out = point.y;
}

instant void
Mouse_GetPosition(
	Mouse *mouse_out,
	Window *window
) {
	Assert(window);
	Assert(mouse_out);

	RECT rect_active;
	GetWindowRect(window->hWnd, &rect_active);
	Window_UnAdjustRect(window->hWnd, &rect_active);

	Mouse t_mouse = *mouse_out;

	Mouse_GetPosition(&mouse_out->point.x, &mouse_out->point.y, 0);

	RectF rect_viewport;

	glGetFloatv(GL_VIEWPORT, (GLfloat *)&rect_viewport);

	float scale_x = window->width  / rect_viewport.w;
	float scale_y = window->height / rect_viewport.h;

	mouse_out->point.x = (mouse_out->point.x - (rect_active.left + rect_viewport.x)) * scale_x;
	mouse_out->point.y = (mouse_out->point.y - (rect_active.top  + rect_viewport.y)) * scale_y;

	mouse_out->point_relative.x = mouse_out->point.x - t_mouse.point.x;
	mouse_out->point_relative.y = mouse_out->point.y - t_mouse.point.y;
}

instant bool
Mouse_Update(
	Mouse *mouse_io,
	Window *window,
	MSG *msg
) {
	if (!mouse_io)
		return false;

	if (!msg)
		return false;

	Mouse_Reset(mouse_io);

	if (window AND msg->message == WM_MOUSEMOVE) {
		mouse_io->is_moving = true;
		Mouse_GetPosition(mouse_io, window);
		return true;
	}

    switch(msg->message) {
		case WM_LBUTTONDOWN: {
			mouse_io->pressing[0] = true;
			mouse_io->down[0] = true;
			mouse_io->is_down = true;
		} break;

		case WM_MBUTTONDOWN: {
			mouse_io->pressing[1] = true;
			mouse_io->down[1] = true;
			mouse_io->is_down = true;
		} break;

		case WM_RBUTTONDOWN: {
			mouse_io->pressing[2] = true;
			mouse_io->down[2] = true;
			mouse_io->is_down = true;
		} break;

		case WM_LBUTTONUP:   {
			mouse_io->pressing[0] = false;
			mouse_io->up[0] = true;
			mouse_io->is_up = true;
		} break;

		case WM_MBUTTONUP:   {
			mouse_io->pressing[1] = false;
			mouse_io->up[1] = true;
			mouse_io->is_up = true;
		} break;

		case WM_RBUTTONUP:   {
			mouse_io->pressing[2] = false;
			mouse_io->up[2] = true;
			mouse_io->is_up = true;
		} break;

		case WM_MOUSEWHEEL: {
			mouse_io->wheel = MOUSE_WHEEL_GET_DELTA(msg->wParam) * 16;
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

instant void
Keyboard_Reset(
	Keyboard *keyboard_out,
	bool full_reset = false
) {
	Assert(keyboard_out);

	bool pressing[KEYBOARD_KEYCOUNT] = {};

	if (!full_reset)
		Memory_Copy(&pressing, &keyboard_out->pressing, sizeof(bool) * KEYBOARD_KEYCOUNT);

    *keyboard_out = {};

    if (!full_reset)
		Memory_Copy(&keyboard_out->pressing, &pressing, sizeof(bool) * KEYBOARD_KEYCOUNT);
}

instant void
Keyboard_ResetKey(
	Keyboard *keyboard_out,
	u32 key_virtual
) {
	if (!keyboard_out)
		return;

	keyboard_out->down[key_virtual] = false;
	keyboard_out->up[key_virtual] 	= false;

	keyboard_out->is_down 			= false;
	keyboard_out->is_up 			= false;

	keyboard_out->is_key_sym 		= false;
	keyboard_out->key_sym 			= 0;
	keyboard_out->key_scan			= 0;
	keyboard_out->last_key_virtual	= 0;
}

/// reset last key pressed
/// to prevent continuous input events
instant void
Keyboard_ResetLastKey(
	Keyboard *keyboard_out
) {
	if (!keyboard_out)
		return;

	u32 vkey = keyboard_out->last_key_virtual;

	Keyboard_ResetKey(keyboard_out, keyboard_out->last_key_virtual);
}

instant void
Keyboard_GetKeySym(
	Keyboard *keyboard_io,
	MSG *msg
) {
	Assert(keyboard_io);
	Assert(msg);

	wchar_t ch = 0;
	GetKeyboardState(keyboard_io->key_states);

	keyboard_io->is_key_sym	= false;
	keyboard_io->key_sym    = 0;

	s32 result = ToUnicode(
					msg->wParam,
					MapVirtualKey(msg->wParam, 0),
					keyboard_io->key_states, &ch, 1, 0
				 );

	if (result > 0) {
		if (ch == VK_ESCAPE)  return;

		keyboard_io->is_key_sym 	= true;
		keyboard_io->key_sym 		= ch; // (char)LOBYTE(ch);
	}
}

instant void
Keyboard_SetDown(
	Keyboard *keyboard_io,
	MSG *msg
) {
	Assert(keyboard_io);
	Assert(msg);

	keyboard_io->key_scan = MapVirtualKey(msg->wParam, 0);

	keyboard_io->down[msg->wParam] 		= true;
	keyboard_io->up[msg->wParam] 		= false;
	keyboard_io->pressing[msg->wParam] 	= true;
	keyboard_io->repeating[msg->wParam] = GETBIT(msg->lParam, 30);

	Keyboard_GetKeySym(keyboard_io, msg);

	keyboard_io->last_key_virtual 	= msg->wParam;
	keyboard_io->is_down 			= true;
	keyboard_io->is_up				= false;
}

instant void
Keyboard_SetUp(
	Keyboard *keyboard_io,
	MSG *msg
) {
	Assert(keyboard_io);
	Assert(msg);

	keyboard_io->key_scan = MapVirtualKey(msg->wParam, 0);
	keyboard_io->last_key_virtual 	= msg->wParam;

	if (keyboard_io->pressing[msg->wParam]) {
		keyboard_io->down[msg->wParam] 		= false;
		keyboard_io->up[msg->wParam] 		= true;
		keyboard_io->pressing[msg->wParam] 	= false;
		keyboard_io->repeating[msg->wParam] = false;

		Keyboard_GetKeySym(keyboard_io, msg);

		keyboard_io->is_down 			= false;
		keyboard_io->is_up				= true;
	}
}

instant bool
Keyboard_Update(
	Keyboard *keyboard_io,
	MSG *msg
) {
	bool result = false;

	if (!keyboard_io)
		return result;

	Assert(msg);

	switch (msg->message) {
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN: {
			Keyboard_SetDown(keyboard_io, msg);
			result = true;
		} break;

		case WM_SYSKEYUP:
		case WM_KEYUP: {
			Keyboard_SetUp(keyboard_io, msg);
			result = true;
		} break;
	}

	return result;
}

instant void
Keyboard_Insert(
	Keyboard *keyboard,
	String *s_data_io
) {
	Assert(keyboard);
	Assert(s_data_io);

	String_Insert(s_data_io, s_data_io->length, keyboard->key_sym);
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
	Keyboard *keyboard_out,
	bool *pending_event,
	u32 delay_in_ms = 400
) {
	Assert(keyboard_out);
	Assert(pending_event);

	if (*pending_event) {
		static Timer timer_input_delay;

		if (Time_HasElapsed(&timer_input_delay, delay_in_ms, true)) {
			if (!Keyboard_IsPressingAnyKey(keyboard_out)) {
				*pending_event = false;
			}
		}

		Keyboard_Reset(keyboard_out);
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
	Joypad *joypad_out
) {
	Assert(joypad_out);

    char sys_dir[MAX_PATH];

    u64 sys_dir_len = GetSystemDirectory(sys_dir, sizeof(sys_dir));
    if (!sys_dir_len OR sys_dir_len > sizeof(sys_dir))  return false;

    strcat(sys_dir, "\\xinput1_3.dll");

    joypad_out->dll = LoadLibrary(sys_dir);
    if (!joypad_out->dll)  return false;

    joypad_out->GetState = (proc_XInputGetState)GetProcAddress(joypad_out->dll, "XInputGetState");

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
	float scale = 0.0f;
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

		font.scale = stbtt_ScaleForPixelHeight(&font.info, font.size);

		stbtt_GetFontVMetrics(&font.info, &font.ascent, &font.descent, &font.linegap);
		font.ascent  *= font.scale;
		font.descent *= font.scale;
		font.linegap *= font.scale;
    }

    return font;
}

instant void Codepoint_Destroy(Codepoint *);

instant void
Font_Destroy(
	Font *font_out
) {
	Assert(font_out);

	FOR_ARRAY(font_out->a_codepoint, it) {
		Codepoint *t_codepoint = &ARRAY_IT(font_out->a_codepoint, it);
        Codepoint_Destroy(t_codepoint);
	}

	String_Destroy(&font_out->s_data);

	*font_out = {};
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

/// does NOT return codepoint data,
/// because it will be stored inside the font-struct for fast access,
/// otherwise it could be assumed it would need freeing, which would
/// corrupt the array data inside the font-struct
/// -> font => codepoint-array has ownership
instant void
Codepoint_GetData(
	Font *font,
	s32 codepoint,
	Codepoint *entry_out
) {
	Assert(font);
	Assert(entry_out);

    u64 t_index_find;

    Codepoint t_codepoint_find;
    t_codepoint_find.font      = font;
    t_codepoint_find.codepoint = codepoint;

    Codepoint *t_entry;

    if (!Array_FindOrAdd(&font->a_codepoint, t_codepoint_find, &t_entry)) {
		/// get texture
		t_entry->texture = Codepoint_ToTexture(font, codepoint);

		/// get advance / left side bearing
		stbtt_GetCodepointHMetrics(&font->info,
									codepoint,
									&t_entry->advance,
									&t_entry->left_side_bearing);

		t_entry->advance *= font->scale;
		t_entry->left_side_bearing *= font->scale;

		/// get subpixel
		stbtt_GetCodepointBitmapBoxSubpixel(&font->info,
											codepoint,
											font->scale,
											font->scale,
											0,
											0,
											&t_entry->rect_subpixel.x,
											&t_entry->rect_subpixel.y,
											&t_entry->rect_subpixel.w,
											&t_entry->rect_subpixel.h);
    }

	*entry_out = *t_entry;
}

instant s32
Codepoint_GetTabWidth(
	float x,
	s32 advance_space,
	u32 tab_space_count = 4
) {
	s32 tab_width = (advance_space * (tab_space_count));

	s32 tab_width_remaining = tab_width - (s32)x % tab_width;

	/// extend max. tab width for compatability with other editors
	if (tab_width_remaining < advance_space)
		tab_width_remaining += tab_width;

	return tab_width_remaining;
}

instant void
Codepoint_GetDataConditional(
	Font *font,
	s32 codepoint,
	Codepoint *entry_out,
	float x,
	s32 advance_space
) {
	Assert(font);
	Assert(entry_out);

	Codepoint_GetData(font, codepoint, entry_out);

	if (entry_out->codepoint == '\t')
		entry_out->advance = Codepoint_GetTabWidth(x, advance_space);
}

instant void
Codepoint_GetPositionNext(
	Codepoint *codepoint,
	RectF *rect_io
) {
	Assert(codepoint);
	Assert(codepoint->font);
	Assert(rect_io);

	/// rect_x: starting position for each line
	///         to reset -> set 0 or x-offset
	/// rect_h: lower end baseline for text drawing
	///         increasing it will skip to the next line
	///         has to get the value from itself, or it
	///         will reset to 0
	/// rect_w: store advance of prev codepoint to
	///         set the correct start position for the
	///         next codepoint

	rect_io->x = rect_io->x + rect_io->w + codepoint->left_side_bearing;
	rect_io->y = rect_io->h + codepoint->rect_subpixel.y + codepoint->font->size + codepoint->font->descent;
	rect_io->w = (float)codepoint->advance - codepoint->left_side_bearing;
}

instant s32
Codepoint_GetAdvance(
	Font *font,
	s32 codepoint
) {
	Assert(font);

	s32 advance;

	stbtt_GetCodepointHMetrics(&font->info, codepoint, &advance, 0);
	advance *= font->scale;

	return advance;
}

instant u64
Codepoint_GetStringAdvance(
	Font *font,
	float x,
	s32 advance_space,
	String *s_data
) {
	Assert(font);
	Assert(s_data);

	u64 advance_word = 0;

	FOR(s_data->length, it) {
		char ch = s_data->value[it];

		if (ch != '\t')
			advance_word += Codepoint_GetAdvance(font, ch);
		else
			advance_word += Codepoint_GetTabWidth(x, advance_space);
	}

	return advance_word;
}

instant void
Codepoint_SetNewline(
	Font *font,
	RectF *rect_position_io,
	float x_offset_start = 0
) {
	Assert(font);
	Assert(rect_position_io);

	rect_position_io->h += Font_GetLineHeight(font);
	rect_position_io->x = x_offset_start;
	rect_position_io->w = 0;
}

instant void
Codepoint_Destroy(
	Codepoint *codepoint_out
) {
	Assert(codepoint_out);
	Assert(codepoint_out->font);

	Texture_Destroy(&codepoint_out->texture);
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

enum CURSOR_MOVE_TYPE {
	CURSOR_MOVE_NONE,
	CURSOR_MOVE_X,
	CURSOR_MOVE_Y,
	CURSOR_MOVE_LINE_BORDER,
};

struct Text_Line {
	u64 width_pixel;
	String s_data;
};

struct Text_Cursor_Data {
	u64 index_select_start = 0;
	u64 index_select_end   = 0; /// also current cursor position

	Color32 color_cursor = {1.0f, 0.0f, 0.0f, 1.0f};
	Color32 color_select = {0.5f, 0.5f, 1.0f, 1.0f};
};

struct Text_Cursor {
	CURSOR_MOVE_TYPE move_type = CURSOR_MOVE_NONE;
	s64 move_index_x = 0;
	s64 move_index_y = 0;

	Text_Cursor_Data data;
	Text_Cursor_Data data_prev;

	bool is_selecting = false;

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
	Color32 color 		= {0.0f, 0.0f, 0.0f, 1.0f};

	TEXT_ALIGN_X_TYPE align_x = TEXT_ALIGN_X_LEFT;
	TEXT_ALIGN_Y_TYPE align_y = TEXT_ALIGN_Y_TOP;

	s32 content_width  = 0;
	s32 content_height = 0;

	bool is_editable      = false;
	bool use_word_wrap    = true;
	bool use_no_linebreak = false;
};

struct Text {
	ShaderSet *shader_set = 0;
	Font *font = 0;
	String s_data = {};

	/// always update with shader uniform
	float offset_x = 0.0f;
	float offset_y = 0.0f;

	Text_Data data;
	Text_Data data_prev;

	Text_Cursor cursor;

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
	Text *text_out
) {
	Assert(text_out);

	String_Destroy(&text_out->s_data);
}

instant bool
Text_HasChanged(
	Text *text_io,
	bool update_changes
) {
	Assert(text_io);

	bool has_changed = text_io->s_data.changed;

	has_changed |= !Memory_Compare(&text_io->data, &text_io->data_prev, sizeof(text_io->data));

	if (has_changed AND update_changes) {
		text_io->data_prev = text_io->data;
		text_io->s_data.changed = false;
	}

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

/// returns number of lines
instant u64
Text_CalcLineCount(
	Text *text,
	Array<String> *as_words
) {
	Assert(text);
	Assert(as_words);

	u64 count_lines = 0;

	MEASURE_START();

	Font *font = text->font;
	Rect  rect = text->data.rect;

	Rect_AddPadding(&rect, text->data.rect_padding);

	Rect rect_line_current = {rect.x, rect.y, 0, 0};

	u64 advance_space = Codepoint_GetAdvance(font, ' ');
	u64 index_line = 0;
	bool line_start = true;

	/// words will be in sequential order in memory!
	String *ts_data_it = &ARRAY_IT(*as_words, 0);

	if (as_words->count)
		++count_lines;

	u64 index_data = 0;

	FOR_ARRAY(*as_words, it_word) {
		String *ts_word = &ARRAY_IT(*as_words, it_word);

		u64 advance_word = Codepoint_GetStringAdvance(
								font,
								rect_line_current.x - rect.x,
								advance_space,
								ts_word
							);

		/// word wrap
		if (    text->data.use_word_wrap
			AND !line_start
			AND rect.w > 0
			AND (rect_line_current.x - rect.x) + advance_word > rect.w
			AND !text->data.use_no_linebreak
		) {
			++count_lines;
			line_start = true;

			rect_line_current.x  = rect.x;
		}

		index_data += ts_word->length;

		if (String_EndWith(ts_word, "\n", 1)) {
			++count_lines;
			line_start = true;

			rect_line_current.x = rect.x;

			continue;
		}

		rect_line_current.x += advance_word;
		line_start = false;
	}

	MEASURE_END("");

	return count_lines;
}

/// returns height in pixel
instant s32
Text_BuildLines(
	Text *text,
	Array<String> *as_words,
	u64 number_of_linebreaks,
	Array<Text_Line> *a_text_line_out
) {
	Assert(text);
	Assert(as_words);
	Assert(a_text_line_out);

	s32 max_height = 0;

	MEASURE_START();

	Array_ClearContainer(a_text_line_out);

	/// convert to number of fixed line-breaks
	if (!text->data.use_word_wrap) {
		Array_Reserve(a_text_line_out, number_of_linebreaks + 1);

		max_height = (number_of_linebreaks + 1) * Font_GetLineHeight(text->font);

		u64 limit_prev = a_text_line_out->limit;

		Text_Line *text_line = 0;

		/// words will be in sequential order in memory!
		String *ts_data_it = &ARRAY_IT(*as_words, 0);

		if (as_words->count) {
			Array_AddEmpty(a_text_line_out, &text_line);
			text_line->s_data.value = ts_data_it->value;
		}

		u64 index_data = 0;

		FOR_ARRAY(*as_words, it_word) {
			String *ts_word = &ARRAY_IT(*as_words, it_word);

			text_line->s_data.length += ts_word->length;

			index_data += ts_word->length;

			if (String_EndWith(ts_word, "\n", 1)) {
				Array_AddEmpty(a_text_line_out, &text_line);
				text_line->s_data.value = &ts_data_it->value[index_data];
			}
		}

		/// make sure reservation worked
		Assert(    limit_prev == a_text_line_out->limit
			   AND a_text_line_out->size == a_text_line_out->limit);

		MEASURE_END("(static line-break) ");
	}
	else {
		/// do NOT calc. number of linebreaks (incl. word-wrap) and reserve
		/// the amount of space needed to fit into the array
		/// there is no benefit, since the amount of time needed to read all
		/// the codepoint data is greater than the reduction in speed by
		/// preallocating memory.
		///
		/// this works without word-wrap (in the code above) due to the lack
		/// of codepoint data retrieval

		Font *font = text->font;
		Rect  rect = text->data.rect;

		Rect_AddPadding(&rect, text->data.rect_padding);

		Rect rect_line_current = {rect.x, rect.y, 0, 0};

		u64 advance_space = Codepoint_GetAdvance(font, ' ');
		s32 line_height = Font_GetLineHeight(font);
		u64 index_line = 0;
		bool line_start = true;

		Text_Line *text_line = 0;

		/// words will be in sequential order in memory!
		String *ts_data_it = &ARRAY_IT(*as_words, 0);

		if (as_words->count) {
			max_height += line_height;
			Array_AddEmpty(a_text_line_out, &text_line);
			text_line->s_data.value = ts_data_it->value;
		}

		u64 index_data = 0;

		FOR_ARRAY(*as_words, it_word) {
			String *ts_word = &ARRAY_IT(*as_words, it_word);

			u64 advance_word = Codepoint_GetStringAdvance(
									font,
									rect_line_current.x - rect.x,
									advance_space,
									ts_word
								);

			/// word wrap
			if (    text->data.use_word_wrap
				AND !line_start
				AND rect.w > 0
				AND (rect_line_current.x - rect.x) + advance_word > rect.w
				AND !text->data.use_no_linebreak
			) {
				Array_AddEmpty(a_text_line_out, &text_line);
				line_start = true;

				text_line->s_data.value = &ts_data_it->value[index_data];

				rect_line_current.x  = rect.x;
				rect_line_current.y += line_height;

				max_height += line_height;
			}

			text_line->s_data.length += ts_word->length;

			index_data += ts_word->length;

			if (String_EndWith(ts_word, "\n", 1)) {
				Array_AddEmpty(a_text_line_out, &text_line);
				line_start = true;

				text_line->s_data.value = &ts_data_it->value[index_data];

				rect_line_current.x  = rect.x;
				rect_line_current.y += line_height;

				max_height += line_height;

				continue;
			}

			rect_line_current.x    += advance_word;
			text_line->width_pixel += advance_word;
			line_start = false;
		}

		MEASURE_END("(word-wrap) ");
	}

	return max_height;
}

/////@TODO: make this faster
//instant s32
//Text_BuildLines(
//	Text *text,
//	Array<String> *as_words,
//	Array<Text_Line> *a_text_line_out,
//	bool will_append_cursor_end
//) {
//	Assert(text);
//	Assert(as_words);
//	Assert(a_text_line_out);
//
//	MEASURE_START();
//
//	Font *font = text->font;
//	Rect  rect = text->data.rect;
//
//	Rect_AddPadding(&rect, text->data.rect_padding);
//
//	/// clear existing lines
//	FOR_ARRAY(*a_text_line_out, it_line) {
//		Text_Line *t_text_line = &ARRAY_IT(*a_text_line_out, it_line);
//		String_Destroy(&t_text_line->s_data);
//		t_text_line->width_pixel = 0;
//	}
//	Array_ClearContainer(a_text_line_out);
//
//	s32 height_max  = 0;
//
//	Text_Line *t_text_line;
//	Rect rect_line_current = {rect.x, rect.y, 0, 0};
//	u64 advance_space = Codepoint_GetAdvance(font, ' ');
//
//	/// always add ' ' at the end
//	if (as_words->count == 0) {
//		if (will_append_cursor_end) {
//			Array_AddEmpty(a_text_line_out, &t_text_line);
//
//			t_text_line->width_pixel += advance_space;
//			String_Append(&t_text_line->s_data, " ", 1);
//		}
//		return height_max;
//	}
//
//	Assert(a_text_line_out->count == 0);
//
//	s32 height_line = Font_GetLineHeight(font);
//	bool line_start = true;
//
//	/// add first empty line to be filled
//	if (as_words->count) {
//		height_max += height_line;
//		Array_AddEmpty(a_text_line_out, &t_text_line);
//	}
//
//    FOR_ARRAY(*as_words, it_words) {
//    	bool is_last_word = (it_words + 1 == as_words->count);
//
//		String *ts_word = &ARRAY_IT(*as_words, it_words);
//
//		u64 advance_word = Codepoint_GetStringAdvance(
//								font,
//								rect_line_current.x - rect.x,
//								advance_space,
//								ts_word
//							);
//
//		/// word wrap
//		if (    text->data.use_word_wrap
//			AND !line_start
//			AND rect.w > 0
//			AND (rect_line_current.x - rect.x) + advance_word > rect.w
//			AND !text->data.use_no_linebreak
//		) {
//			Array_AddEmpty(a_text_line_out, &t_text_line);
//			line_start = true;
//
//			rect_line_current.x  = rect.x;
//			rect_line_current.y += height_line;
//
//			height_max += height_line;
//		}
//
//		if (   String_EndWith(ts_word, "\n")
//			OR String_EndWith(ts_word, "\r")
//		) {
//			Assert(!text->data.use_no_linebreak);
//
//			t_text_line->width_pixel += advance_word;
//			String_Append(&t_text_line->s_data, ts_word->value, ts_word->length);
//
//			Array_AddEmpty(a_text_line_out, &t_text_line);
//			line_start = true;
//
//			rect_line_current.x  = rect.x;
//			rect_line_current.y += height_line;
//
//			height_max += height_line;
//
//			/// append ' ' on the last line-break
//			if (is_last_word AND will_append_cursor_end) {
//				t_text_line->width_pixel += advance_space;
//				String_Append(&t_text_line->s_data, " ", 1);
//			}
//
//			continue;
//		}
//
//		rect_line_current.x      += advance_word;
//		t_text_line->width_pixel += advance_word;
//		String_Append(&t_text_line->s_data, ts_word->value, ts_word->length);
//
//		/// append ' ' on the last word
//		if (is_last_word AND will_append_cursor_end) {
//			t_text_line->width_pixel += advance_space;
//			String_Append(&t_text_line->s_data, " ", 1);
//		}
//
//		line_start = false;
//    }
//
//    MEASURE_END("");
//
//	return height_max;
//}

///@Hint: line will NOT adjust, wenn word-wrap is disabled AND
///       the text-line is visibly longer, otherwise it would not
///       be possible to see the cursor at the beginning of such
///       a line, if text editing is allowed
instant u64
Text_GetAlignOffsetX(
	Text *text,
	u64 max_width,
	Text_Line *text_line
) {
	Assert(text);
	Assert(text_line);

	u64 x_align_offset = 0;

	if (max_width > text_line->width_pixel) {
		if (text->data.align_x == TEXT_ALIGN_X_MIDDLE)
			x_align_offset = (max_width - text_line->width_pixel) >> 1;
		else
		if (text->data.align_x == TEXT_ALIGN_X_RIGHT)
			x_align_offset = (max_width - text_line->width_pixel);
	}

	return x_align_offset;
}

instant void
Text_AddLines(
	Text *text,
	Array<Vertex>    *a_vertex_chars_io,
	Array<Text_Line> *a_text_lines,
	bool include_offsets = true
) {
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

	float x_line_start = rect.x;

	RectF rect_position = {	x_line_start, 0, 0, rect.y};

	if (include_offsets) {
		rect_position.x += text->offset_x;
		rect_position.h += text->offset_y;
		x_line_start    += text->offset_x;
	}

	bool has_cursor = text->data.is_editable;

	if (has_cursor)
		Vertex_ClearAttributes(&text->cursor.vertex_select);

	Codepoint codepoint_space;
	Codepoint_GetData(text->font, ' ', &codepoint_space);

	FOR_ARRAY(*a_text_lines, it_line) {
		Text_Line *text_line = &ARRAY_IT(*a_text_lines, it_line);

		u64 it_data = 0;

		while(it_data < text_line->s_data.length) {
			Codepoint codepoint;

			s8 ch = text_line->s_data.value[it_data];

			Codepoint_GetDataConditional(
				text->font,
				ch,
				&codepoint,
				rect_position.x - x_line_start,
				codepoint_space.advance
			);

			Codepoint_GetPositionNext(&codepoint, &rect_position);

			u64 x_align_offset = Text_GetAlignOffsetX(text, width_max, text_line);

			/// for unavailable characters like ' '
			if (!Texture_IsEmpty(&codepoint.texture)) {
				Vertex *t_vertex;
				Vertex_FindOrAdd(a_vertex_chars_io, &codepoint.texture, &t_vertex);

				Vertex_Buffer<float> *t_attribute;

				/// exclude offsets
				float x_pos = rect_position.x - text->offset_x;
				float y_pos = rect_position.y - text->offset_y;

				{
					Vertex_FindOrAddAttribute(t_vertex, 2, "vertex_position", &t_attribute);
					Array_ReserveAdd(&t_attribute->a_buffer, 2);
					Array_Add(&t_attribute->a_buffer, x_pos + x_align_offset);
					Array_Add(&t_attribute->a_buffer, y_pos);
				}
				{
					Vertex_FindOrAddAttribute(t_vertex, 3, "text_color", &t_attribute);
					Array_ReserveAdd(&t_attribute->a_buffer, 3);
					Array_Add(&t_attribute->a_buffer, text->data.color.r);
					Array_Add(&t_attribute->a_buffer, text->data.color.g);
					Array_Add(&t_attribute->a_buffer, text->data.color.b);
				}
			}

			++it_data;
		}

		Codepoint_SetNewline(text->font, &rect_position, x_line_start);
	}
}

instant void
Text_AddLines(
	Text *text_io,
	bool include_offsets = true
) {
	Assert(text_io);

	MEASURE_START();

	Text_AddLines(text_io, &text_io->a_vertex_chars, &text_io->a_text_lines, include_offsets);

	MEASURE_END("");
}

instant void
Text_Clear(
	Text *text_out
) {
	Assert(text_out);

	Vertex_ClearAttributes(&text_out->a_vertex_chars);
}

instant void
Text_RenderLines(
	Text *text
) {
	Assert(text);

	Vertex_Render(text->shader_set, &text->a_vertex_chars);
}

instant void Text_Cursor_Update(Text *text);

instant bool
Text_Exists(
	Text *text
) {
	Assert(text);

	return (text->s_data.length > 0);
}

instant bool
Text_Update(
	Text *text_io
) {
	Assert(text_io);

	if (!text_io->font) {
		if (text_io->s_data.length)
			LOG_WARNING("No font is set, while text is available.");

		return false;
	}

#if !DEBUG_ALWAYS_UPDATE
	if (!Text_HasChanged(text_io, false))
		return false;
#endif

	MEASURE_START();

	/// redraw text
	if (text_io->data.use_no_linebreak) {
		String_Replace(&text_io->s_data, "\n", "");
		String_Replace(&text_io->s_data, "\r", "");
	}

	u64 number_of_lines = 0;

#if !DEBUG_ALWAYS_UPDATE
	if (text_io->s_data.changed)
		number_of_lines = String_SplitWordsBuffer(&text_io->s_data, &text_io->as_words);
#else
	number_of_lines = String_SplitWordsBuffer(&text_io->s_data, &text_io->as_words);
#endif

	/// put " " at the end of the text, when it's editable for text cursor
	s32 text_height = Text_BuildLines(text_io, &text_io->as_words, number_of_lines, &text_io->a_text_lines);

	if (text_io->data.rect.h) {
		s32 pad_height = text_io->data.rect_padding.y + text_io->data.rect_padding.h;

		if (text_io->data.align_y == TEXT_ALIGN_Y_CENTER)
			text_io->offset_y = (text_io->data.rect.h - pad_height - text_height) >> 1;
		else
		if (text_io->data.align_y == TEXT_ALIGN_Y_BOTTOM)
			text_io->offset_y = (text_io->data.rect.h - pad_height - text_height);
	}

	text_io->data.content_height = text_height;
	text_io->data.content_width = 0;

	FOR_ARRAY(text_io->a_text_lines, it_line) {
		Text_Line *t_line = &ARRAY_IT(text_io->a_text_lines, it_line);
		text_io->data.content_width = MAX(text_io->data.content_width, (s64)t_line->width_pixel);
	}

	Text_Clear(text_io);
	Text_AddLines(text_io, true);

	Text_Cursor_Update(text_io);

	MEASURE_END("");

	text_io->data_prev = text_io->data;
	text_io->s_data.changed = false;

	return true;
}

instant u64
Text_Cursor_FindIndex(
	Text *text,
	Point point
) {
	Assert(text);

	u64 cursor_index = 0;
	bool found_index = false;

	Text_Cursor *cursor = &text->cursor;

	Rect rect = text->data.rect;

	Rect_AddPadding(&rect, text->data.rect_margin);
	Rect_AddPadding(&rect, text->data.rect_padding);

	Codepoint codepoint_space;
	Codepoint_GetData(text->font, ' ', &codepoint_space);

	u64 width_max = rect.w;

	if (!width_max) {
		FOR_ARRAY(text->a_text_lines, it_line) {
			Text_Line *t_text_line = &ARRAY_IT(text->a_text_lines, it_line);

			width_max = MAX(width_max, t_text_line->width_pixel);
		}
	}

	float x_pos_start = rect.x + text->offset_x;

	Rect rect_position_it = {
		x_pos_start,
		rect.y + text->offset_y,
		0,
		Font_GetLineHeight(text->font)
	};

	/// check the area above the text
	/// to select the first char
	if (point.y < rect_position_it.y) {
		return cursor_index;
	}

	FOR(text->a_text_lines.count, it_line) {
		Text_Line *text_line = &ARRAY_IT(text->a_text_lines, it_line);

		if (rect_position_it.y + rect_position_it.h < point.y) {
			cursor_index += text_line->s_data.length;
			rect_position_it.y += rect_position_it.h;
			continue;
		}

		u64 x_align_offset = Text_GetAlignOffsetX(text, width_max, text_line);
		rect_position_it.x += x_align_offset;

		bool is_newline_char_once = false;

		FOR(text_line->s_data.length, it_data) {
			bool is_last_char = (it_data + 1 == text_line->s_data.length);

			Codepoint codepoint;

			s8 character = text_line->s_data.value[it_data];

			Codepoint_GetDataConditional(
				text->font,
				character,
				&codepoint,
				rect_position_it.x - x_pos_start,
				codepoint_space.advance
			);

			rect_position_it.w = codepoint.advance;

			bool is_newline_char = (character == '\r' OR character == '\n');

			/// makes end-of-line chars selectable
			/// by including the remaining width of a line in the last char
			/// when using line-breaks or word-wrap
			/// - newline char can happen before the last char,
			///   if '\r\n' is used
			if (is_newline_char OR is_last_char)
				rect_position_it.w = width_max - (rect_position_it.w - rect_position_it.x);
			else
				rect_position_it.w = codepoint.advance;

			if (it_data == 0) {
				/// check the "free" space in front of the first char too,
				/// to make it target the first char
				/// -> same as with the last char
				Rect rect_position_start = rect_position_it;

				float pos_w = rect_position_start.x + rect_position_start.w;
				rect_position_start.x = x_pos_start;
				rect_position_start.w = pos_w - rect_position_start.x;

				if (Rect_IsIntersectingBorderless(&point, &rect_position_start)) {
					found_index = true;
					return cursor_index;
				}
			}
			else
			if (Rect_IsIntersectingBorderless(&point, &rect_position_it)) {
				found_index = true;
				return cursor_index;
			}

			if (!found_index) {
				++cursor_index;
				rect_position_it.x += rect_position_it.w;
			}

			if (is_newline_char) {
				rect_position_it.w = codepoint.advance;
			}
		}

		rect_position_it.x  = x_pos_start;
		rect_position_it.y += rect_position_it.h;
	}

	if (!found_index AND cursor_index)
		--cursor_index;

	return cursor_index;
}

instant void
Text_Cursor_Flush(
	Text *text_io
) {
	Assert(text_io);

	Text_Cursor *cursor = &text_io->cursor;

	cursor->move_index_x = 0;
	cursor->move_index_y = 0;

	if (!text_io->cursor.is_selecting)
		text_io->cursor.data.index_select_start = text_io->cursor.data.index_select_end;
}

instant void
Text_Cursor_FlushFull(
	Text *text_io
) {
	Assert(text_io);

	text_io->cursor.move_type = CURSOR_MOVE_NONE;
	Text_Cursor_Flush(text_io);
}

instant bool
Text_Cursor_HasChanged(
	Text_Cursor *cursor,
	bool update_changes
) {
	Assert(cursor);

	bool has_changed = !Memory_Compare(&cursor->data, &cursor->data_prev, sizeof(cursor->data));

	if (has_changed AND update_changes) {
		cursor->data_prev = cursor->data;
	}

	return has_changed;
}

instant void
Text_Cursor_Update(
    Text *text_io
) {
	Assert(text_io);

	if (!text_io->data.is_editable)
		return;

	/// cursor selection -> end = current
	bool found_start    = false;
	bool found_end      = false;
	bool found_end_once = false;

	const s32 width_cursor = 2;

	u64 cursor_index = 0;

	Text_Cursor *cursor = &text_io->cursor;

	Rect rect = text_io->data.rect;

	Rect_AddPadding(&rect, text_io->data.rect_margin);
	Rect_AddPadding(&rect, text_io->data.rect_padding);

	Codepoint codepoint_space;
	Codepoint_GetData(text_io->font, ' ', &codepoint_space);

	u64 width_max = rect.w;

	if (!width_max) {
		FOR_ARRAY(text_io->a_text_lines, it_line) {
			Text_Line *t_text_line = &ARRAY_IT(text_io->a_text_lines, it_line);

			width_max = MAX(width_max, t_text_line->width_pixel);
		}
	}

	if (!cursor->vertex_select.array_id)
		cursor->vertex_select = Vertex_Create();

	Vertex_ClearAttributes(&cursor->vertex_select);

	float x_pos_start = rect.x + text_io->offset_x;

	Rect rect_position_it = {
		x_pos_start,
		rect.y + text_io->offset_y,
		0,
		Font_GetLineHeight(text_io->font)
	};

	/// lower boundary index check
	if (cursor->move_index_x < 0) {
		if (cursor->data.index_select_end + cursor->move_index_x > cursor->data.index_select_end) {
			cursor->move_index_x = 0;

			if (!cursor->is_selecting)
				cursor->data.index_select_start = 0;

			cursor->data.index_select_end = 0;
		}
	}

	if (cursor->move_type == CURSOR_MOVE_X) {
		if (!cursor->is_selecting)
			cursor->data.index_select_start += cursor->move_index_x;

		cursor->data.index_select_end += cursor->move_index_x;
	}

	FOR(text_io->a_text_lines.count, it_line) {
		Text_Line *text_line = &ARRAY_IT(text_io->a_text_lines, it_line);

		if (    cursor->data.index_select_start > cursor_index + text_line->s_data.length
			AND cursor->data.index_select_end   > cursor_index + text_line->s_data.length
		) {
			cursor_index += text_line->s_data.length;
			rect_position_it.y += rect_position_it.h;
			continue;
		}

		u64 x_align_offset = Text_GetAlignOffsetX(text_io, width_max, text_line);
		rect_position_it.x += x_align_offset;

		bool is_newline_char_once = false;

		FOR(text_line->s_data.length, it_data) {
			Codepoint codepoint;

			s8 character = text_line->s_data.value[it_data];

			Codepoint_GetDataConditional(
				text_io->font,
				character,
				&codepoint,
				rect_position_it.x - x_pos_start,
				codepoint_space.advance
			);

			rect_position_it.w = codepoint.advance;

			bool is_newline_char = (character == '\r' OR character == '\n');

			if (cursor->data.index_select_start == cursor_index AND !found_start)
				found_start = true;

			if (cursor->data.index_select_end   == cursor_index AND !found_end) {
				switch (cursor->move_type) {
					case CURSOR_MOVE_NONE:
					case CURSOR_MOVE_X: {
						found_end = true;
					} break;

					case CURSOR_MOVE_Y: {
						if (cursor->move_index_y == 0) {
							found_end = true;
						}
						else
						if (cursor->move_index_y > 0) {
							Point pt_line = {
								rect_position_it.x,
								rect_position_it.y + Font_GetLineHeight(text_io->font)
							};

							if (!cursor->is_selecting)
								found_start = false;

							cursor->data.index_select_end = Text_Cursor_FindIndex(text_io, pt_line);

							Text_Cursor_Flush(text_io);
						}
						else {
							Point pt_line = {
								rect_position_it.x,
								rect_position_it.y - Font_GetLineHeight(text_io->font)
							};

							cursor->data.index_select_end = Text_Cursor_FindIndex(text_io, pt_line);

							Text_Cursor_Flush(text_io);

							return Text_Cursor_Update(text_io);
						}
					} break;

					case CURSOR_MOVE_LINE_BORDER: {
						if (cursor->move_index_x == 0) {
							found_end = true;
						}
						else
						if (cursor->move_index_x > 0) {
							cursor->data.index_select_end -= it_data;
							cursor->data.index_select_end += text_line->s_data.length;
							cursor->data.index_select_end -= 1;

							if (String_EndWith(&text_line->s_data, "\r\n"))
								cursor->data.index_select_end -= 1;

							if (cursor->data.index_select_end != cursor_index) {
								if (!cursor->is_selecting)
									found_start = false;
							}
							else {
                                found_end = true;
							}

							Text_Cursor_Flush(text_io);
							cursor->move_type = CURSOR_MOVE_X;
						}
						else {
							cursor->data.index_select_end -= it_data;

							Text_Cursor_Flush(text_io);
							cursor->move_type = CURSOR_MOVE_X;

							return Text_Cursor_Update(text_io);
						}
					} break;
				}
			}

			if (XOR(found_start, found_end)) {
				/// do not show multible ' ' in case of '\r\n'
				/// or selection would look strechted
				if (is_newline_char)
					rect_position_it.w = (is_newline_char_once
											? 0
											: codepoint_space.advance
										 );

				Rect rect_no_offset = rect_position_it;
				rect_no_offset.x -= text_io->offset_x;
				rect_no_offset.y -= text_io->offset_y;

				Vertex_AddRect32(
					&cursor->vertex_select,
					rect_no_offset,
					cursor->data.color_select
				);
			}

			if (found_end AND !found_end_once) {
				if (!is_newline_char_once) {
					/// update index, if you seek to '\r' and skip to '\n'
					cursor->data.index_select_end = cursor_index;

					Rect rect_cursor = rect_position_it;

					float *x_offset = &text_io->offset_x;
					float *y_offset = &text_io->offset_y;

					/// exclude offsets
					float cursor_pos_x         = rect_cursor.x - *x_offset;
					float cursor_pos_y         = rect_cursor.y - *y_offset;

					bool is_past_right_border  = (rect_cursor.x + rect_cursor.w > rect.x + rect.w);
					bool is_past_left_border   = (rect_cursor.x < rect.x);

					bool is_past_top_border    = (rect_cursor.y < rect.y);
					bool is_past_bottom_border = (rect_cursor.y + rect_cursor.h > rect.y + rect.h);


					rect_cursor.w = width_cursor;

					if (!cursor->vertex_cursor.array_id)
						cursor->vertex_cursor = Vertex_Create();

					Vertex_ClearAttributes(&cursor->vertex_cursor);

					Rect rect_no_offset = rect_cursor;
					rect_no_offset.x -= text_io->offset_x;
					rect_no_offset.y -= text_io->offset_y;

					Vertex_AddRect32(
						&cursor->vertex_cursor,
						rect_no_offset,
						cursor->data.color_cursor
					);

					if (cursor->move_type == CURSOR_MOVE_Y AND (is_past_top_border OR is_past_bottom_border)) {
						*x_offset = 0;

						if (is_past_top_border) {
							*y_offset = rect.y - cursor_pos_y;
						}
						else
						if (is_past_bottom_border) {
							*y_offset = (rect.y + rect.h) - (cursor_pos_y + rect_cursor.h);
						}

						Text_Cursor_Flush(text_io);
						return;
					}

					if (cursor->move_type == CURSOR_MOVE_X AND (is_past_right_border OR is_past_left_border)) {
						/// skip to see which chars are coming in either direction
						u32 skip_space_mul_x = codepoint_space.advance * 4;

						if (is_past_left_border) {
							*x_offset = rect.x - cursor_pos_x + skip_space_mul_x;

							if (*x_offset > 0)
								*x_offset = 0;
						}
						else
						if (is_past_right_border) {
							*x_offset = (rect.x + rect.w) - (cursor_pos_x + rect_cursor.w) - skip_space_mul_x;
						}

						Text_Cursor_Flush(text_io);
						return;
					}

					found_end_once = true;

					///@Performance: will not help with large file
					///              while moving the cursor at the
					///              lower section of that text
					if (found_start) {
						Text_Cursor_Flush(text_io);
						return;
					}
				}
				else {
					/// to move to the start of the
					/// line-break when using '\r\n'
					if (cursor->move_index_x < 0) {
						Text_Cursor_Flush(text_io);

						cursor->move_type    = CURSOR_MOVE_X;
						cursor->move_index_x = -1;

						return Text_Cursor_Update(text_io);
					}

					/// skipping '\n' here when '\r\n'
					/// is present in the current line
				}
			}

			++cursor_index;
			rect_position_it.x += rect_position_it.w;

			is_newline_char_once = is_newline_char;
		}

		rect_position_it.x  = x_pos_start;
		rect_position_it.y += rect_position_it.h;
	}

	/// make sure there is always a line
	/// with at least a ' ' in it
	Assert(cursor_index);

	/// upper boundary index check
	if (!found_end_once) {
		cursor->data.index_select_end = cursor_index - 1;
		Text_Cursor_Flush(text_io);
		return Text_Cursor_Update(text_io);
	}

 	Text_Cursor_FlushFull(text_io);
}

///@TODO: clipboard support(?)
///@TODO: add support for '\t' input by locking widget
///       and preventing it to switch to another widget
///       unless another key is also pressed.
instant void
Text_UpdateInput(
    Text *text_io,
    Keyboard *keyboard,
    bool *text_changed_out = 0,
    bool *cursor_changed_out = 0
) {
	Assert(text_io);
	Assert(keyboard);

	if (text_changed_out)    *text_changed_out   = false;
	if (cursor_changed_out)  *cursor_changed_out = false;

	if (!text_io->data.is_editable)
		return;

	s8 offset_index_x = 0;
	s8 offset_index_y = 0;

	CURSOR_MOVE_TYPE *move_type = &text_io->cursor.move_type;

	Text_Cursor *cursor = &text_io->cursor;

	*move_type = CURSOR_MOVE_NONE;

	if (keyboard->down[VK_LEFT])  { offset_index_x = -1;
									*move_type = CURSOR_MOVE_X; }
	if (keyboard->down[VK_RIGHT]) { offset_index_x =  1;
									*move_type = CURSOR_MOVE_X; }

	if (keyboard->down[VK_UP])    { offset_index_y = -1;
									*move_type = CURSOR_MOVE_Y; }
	if (keyboard->down[VK_DOWN])  { offset_index_y =  1;
									*move_type = CURSOR_MOVE_Y; }

	if (keyboard->down[VK_HOME])  { offset_index_x = -1;
									*move_type = CURSOR_MOVE_LINE_BORDER; }
	if (keyboard->down[VK_END])   { offset_index_x =  1;
									*move_type = CURSOR_MOVE_LINE_BORDER; }

	cursor->is_selecting = (keyboard->pressing[VK_SHIFT] AND !keyboard->is_key_sym);

	bool was_selecting = (cursor->data.index_select_start != cursor->data.index_select_end);

	/// when selection turns off, seek selection start / end select position,
	/// depending on the x-direction cursor movement
	if (was_selecting AND !cursor->is_selecting AND offset_index_x != 0) {
		if (offset_index_x < 0)
			cursor->data.index_select_start = MIN(cursor->data.index_select_start, cursor->data.index_select_end);
		else
			cursor->data.index_select_start = MAX(cursor->data.index_select_start, cursor->data.index_select_end);

		cursor->data.index_select_end = cursor->data.index_select_start;

		offset_index_x = 0;

		Text_Cursor_Update(text_io);
	}

	if (offset_index_x != 0 OR offset_index_y != 0) {
		if (!cursor->is_selecting)
			cursor->data.index_select_start = cursor->data.index_select_end;

		cursor->move_index_x += offset_index_x;
		cursor->move_index_y += offset_index_y;

		Text_Cursor_Update(text_io);

		if (cursor_changed_out)  *cursor_changed_out = true;
	}


	if (keyboard->is_key_sym AND keyboard->is_down) {
		char key = LOWORD(keyboard->key_sym);

		bool is_linebreak = (key == '\n' OR key == '\r');

		if (text_io->data.use_no_linebreak AND is_linebreak)
			return;

		/// control + a
		/// - select all text
		if (key == 1) {
			cursor->data.index_select_start = 0;
			cursor->data.index_select_end = text_io->s_data.length;
			cursor->is_selecting = true;

			if (cursor_changed_out)  *cursor_changed_out = true;

			Text_Cursor_Update(text_io);
			return;
		}

		*move_type = CURSOR_MOVE_X;

		bool was_selection_removed = false;

		/// overwrite selected text
		if (cursor->data.index_select_start != cursor->data.index_select_end) {
			String_Remove(
				&text_io->s_data,
				cursor->data.index_select_start,
				cursor->data.index_select_end
			);

			/// cursor and selection bounds will start at the
			/// beginning of the selection
			if (cursor->data.index_select_end > cursor->data.index_select_start)
				cursor->data.index_select_end   = cursor->data.index_select_start;
			else
				cursor->data.index_select_start = cursor->data.index_select_end;

			was_selection_removed = true;
		}

		/// tab-char is not supported for now
		if (    key != '\t'
			AND (   !was_selection_removed
				 OR (    was_selection_removed
					 AND key != '\b'))
		) {
			cursor->move_index_x = String_Insert(
										&text_io->s_data,
										cursor->data.index_select_end,
										key
									);

			if (cursor_changed_out)  *cursor_changed_out = true;
		}
	}

	/// string could have been appended, removed or cleared
	if (text_changed_out)  *text_changed_out = text_io->s_data.changed;;
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
	Text *text_io
) {
	Assert(text_io);

	bool is_fixed_size = (text_io->data.rect.w OR text_io->data.rect.h);

	if (is_fixed_size)
		OpenGL_Scissor(text_io->shader_set->window, text_io->data.rect);

	/// redraw selection
	if (text_io->data.is_editable AND text_io->cursor.vertex_select.a_attributes.count) {
		ShaderSet_Use(text_io->shader_set, SHADER_PROG_RECT);
		Shader_SetValue(text_io->shader_set, "x_offset", text_io->offset_x);
		Shader_SetValue(text_io->shader_set, "y_offset", text_io->offset_y);

		Rect_Render(text_io->shader_set, &text_io->cursor.vertex_select);
	}

	/// redraw cursor
	if (    text_io->cursor.show_cursor
		AND text_io->data.is_editable
		AND text_io->cursor.vertex_cursor.a_attributes.count
	) {
		if (Time_HasElapsed(&text_io->cursor.timer_blinking, text_io->cursor.blink_inverval_ms)) {
			text_io->cursor.is_blink_on = !text_io->cursor.is_blink_on;
		}

		if (text_io->cursor.is_blink_on) {
			AssertMessage(!Vertex_IsEmpty(&text_io->cursor.vertex_cursor), "Cursor vertex data does not exists.");

			ShaderSet_Use(text_io->shader_set, SHADER_PROG_RECT);
			Shader_SetValue(text_io->shader_set, "x_offset", text_io->offset_x);
			Shader_SetValue(text_io->shader_set, "y_offset", text_io->offset_y);

			Rect_Render(text_io->shader_set, &text_io->cursor.vertex_cursor);
		}
	}

	if (text_io->a_vertex_chars.count) {
		/// redraw last computed text
		ShaderSet_Use(text_io->shader_set, SHADER_PROG_TEXT);
		Shader_SetValue(text_io->shader_set, "x_offset", text_io->offset_x);
		Shader_SetValue(text_io->shader_set, "y_offset", text_io->offset_y);

		Vertex_Render(text_io->shader_set, &text_io->a_vertex_chars);
	}

	if (is_fixed_size)
		OpenGL_Scissor_Disable();
}

instant void
Text_GetSize(
	Text *text,
	s32 *width_out,
	s32 *height_out
) {
	Assert(text);

	/// seperate storage, so independent text container keep
	/// their already processed data
	static Array<String>    as_words;
	static Array<Text_Line> a_text_lines;

	if (height_out) {
		u64 number_of_lines = String_SplitWordsBuffer(&text->s_data, &as_words);
		*height_out = Text_BuildLines(text, &as_words, number_of_lines, &a_text_lines);
	}

	IF_SET(width_out)  = text->data.rect.w;
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
	Array<Layout_Block> a_layout_blocks;

	/// convenience
	/// -> to be rendered items for this layout
	Array<Widget *> ap_widgets;
};

instant void
Layout_Create(
	Layout *layout_out,
	Rect rect_area,
	bool fill_last_block
) {
	Assert(layout_out);
	AssertMessage(!layout_out->a_layout_blocks.count, "Layout already created.");

	*layout_out = {};
	layout_out->rect_full       = rect_area;
	layout_out->rect_remaining  = rect_area;
	layout_out->fill_last_block = fill_last_block;
}

instant void
Layout_Create(
	Layout *layout_out,
	Window *window,
	bool fill_last_block
) {
	Assert(window);
	Assert(layout_out);

	Layout_Create(layout_out, {0, 0, window->width, window->height}, fill_last_block);
}

instant void
Layout_Create(
	Layout *layout_out,
	Layout_Data *layout_data,
	bool fill_last_block
) {
	Assert(layout_out);
	Assert(layout_data);

	Rect rect_area = layout_data->settings.rect;

	Layout_Create(layout_out, rect_area, fill_last_block);
}

instant void
Layout_CreateBlock(
	Layout *layout_io,
	LAYOUT_TYPE type,
	LAYOUT_DOCK_TYPE dock_direction,
	s32 expand_index = -1,
	Layout_Block **layout_block_out = 0
) {
	Assert(layout_io);

	Layout_Block *t_block;

	Array_AddEmpty(&layout_io->a_layout_blocks, &t_block);

	t_block->type = type;
	t_block->dock = dock_direction;
	t_block->expand_index = expand_index;

	if (layout_block_out)
		*layout_block_out = t_block;
}

instant bool
Layout_GetLastBlock(
	Layout *layout,
	Layout_Block **layout_block_out
) {
	Assert(layout);

	if (!layout->a_layout_blocks.count) {
		LOG_WARNING("GetLastBlock: No blocks in layout found.");
		return false;
	}

	*layout_block_out = &ARRAY_IT(layout->a_layout_blocks, layout->a_layout_blocks.count - 1);

	return true;
}

struct Widget;

instant void Layout_Add(Layout *layout_io, Widget *widget);

instant void
Layout_Add(
	Layout *layout_io,
	Layout_Data *layout_data
) {
	Assert(layout_io);
	Assert(layout_data);

	Layout_Block *current_block;

	if(!Layout_GetLastBlock(layout_io, &current_block)) {
		LOG_WARNING("Using default layout block: top-left - vertival align (y-axis).");
		Layout_CreateBlock(layout_io, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT, -1, &current_block);
	}

	Array_Add(&current_block->ap_layout_data, layout_data);
}

/// layout_block->expand_index (widgets):
///  0-max: expand widget with matching index
///     -1: balanced size (with auto-size)
///     -2: left-align (no auto-size)
///     -3: center-align (no auto-size)
///     -4: right-align (no auto-size)
///   else: see -1
instant void
Layout_ArrangeBlockX(
	Layout *layout_io,
	Layout_Block *layout_block_io
) {
	Assert(layout_io);
	Assert(layout_block_io);
	Assert(layout_block_io->type == LAYOUT_TYPE_X);

	Layout_Block *t_block_last = 0;
	Layout_GetLastBlock(layout_io, &t_block_last);

	bool is_last_block = (t_block_last == layout_block_io);

	s64 widget_count = layout_block_io->ap_layout_data.count;

	if (!widget_count)
		return;

	s32 padding_size = layout_block_io->padding;

	if (padding_size % 2 != 0)  ++padding_size;

	s32 padding_border   = padding_size >> 1;
	s32 width_remaining  = layout_io->rect_remaining.w;

	float it_x = padding_border;
	float it_y = padding_border;

	bool is_overwriting = (	    layout_block_io->expand_index >= -4
							AND layout_block_io->expand_index != -1
							AND layout_block_io->expand_index < widget_count);

	s32 block_height = 0;
	u64 widget_count_auto = widget_count;

	/// pre-calc height for bottom alignment
	FOR_ARRAY(layout_block_io->ap_layout_data, it_block) {
		Layout_Data *t_data = ARRAY_IT(layout_block_io->ap_layout_data, it_block);

		Rect *rect = &t_data->settings.rect;

		bool found_expander = ((s64)it_block == layout_block_io->expand_index);

		block_height  = MAX(block_height, rect->h);

		if (is_overwriting) {
			if (!found_expander) {
				width_remaining -= rect->w;
				--widget_count_auto;
			}
		}
		else
		if (!t_data->settings.auto_width) {
			width_remaining -= rect->w;
			--widget_count_auto;
		}
	}

    width_remaining -= padding_size;
	width_remaining -= layout_block_io->spacing * (widget_count - 1);

    block_height += padding_size;

    switch (layout_block_io->expand_index) {
    	case -3: {
    		it_x = (width_remaining + padding_border) >> 1;
    	} break;

    	case -4: {
    		it_x = (width_remaining + padding_border);
		} break;

		default: break;
    }

    /// start drawing at the bottom
	if (layout_block_io->dock == LAYOUT_DOCK_BOTTOMRIGHT) {
		if (!is_last_block OR (is_last_block AND !layout_io->fill_last_block)) {
			it_y += layout_io->rect_remaining.h - block_height;
		}
	}

	float width_avg_auto = 0;

	if (widget_count_auto)
		width_avg_auto = ceil((float)width_remaining / widget_count_auto);

	/// align horizontal
    FOR_ARRAY(layout_block_io->ap_layout_data, it_block) {
		Layout_Data *t_data = ARRAY_IT(layout_block_io->ap_layout_data, it_block);

		Rect *rect = &t_data->settings.rect;

		/// center widgets in block
		s32 center_block = (rect->h - (block_height - padding_size)) >> 1;

		rect->x = layout_io->rect_remaining.x + it_x;
		rect->y = layout_io->rect_remaining.y + it_y - center_block;

		if (layout_io->fill_last_block AND is_last_block)
			rect->h = layout_io->rect_remaining.h - padding_size;

		bool found_expander = ((s64)it_block == layout_block_io->expand_index);

		float x_step = rect->w;

		if (is_overwriting) {
			if (found_expander)
				x_step = width_avg_auto;
		}
		else {
			if (t_data->settings.auto_width)
				x_step = width_avg_auto;
		}

		float width_limit = layout_io->rect_remaining.w - it_x;

		if (x_step > width_limit)
			x_step = width_limit;

		rect->w = x_step;
		it_x += x_step + layout_block_io->spacing;
    }

    /// cut of the top
	if (layout_block_io->dock == LAYOUT_DOCK_TOPLEFT)
		layout_io->rect_remaining.y += block_height;

    layout_io->rect_remaining.h -= block_height;
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
	Layout *layout_io,
	Layout_Block *layout_block_io
) {
	Assert(layout_io);
	Assert(layout_block_io);
	Assert(layout_block_io->type == LAYOUT_TYPE_Y);

	Layout_Block *t_block_last = 0;
	Layout_GetLastBlock(layout_io, &t_block_last);

	bool is_last_block = (t_block_last == layout_block_io);

	s64 widget_count = layout_block_io->ap_layout_data.count;

	if (!widget_count)
		return;

	s32 padding_size     = layout_block_io->padding;

	if (padding_size % 2 != 0)  ++padding_size;

	s32 padding_border   = padding_size >> 1;
	s32 height_remaining = layout_io->rect_remaining.h;

	float it_x = padding_border;
	float it_y = padding_border;

	bool is_overwriting = (	    layout_block_io->expand_index >= -4
							AND layout_block_io->expand_index != -1
							AND layout_block_io->expand_index < widget_count);

	s32 block_width = 0;
	u64 widget_count_auto = widget_count;

	/// pre-calc width for bottom alignment
	FOR_ARRAY(layout_block_io->ap_layout_data, it_block) {
		Layout_Data *t_data = ARRAY_IT(layout_block_io->ap_layout_data, it_block);

		Rect *rect = &t_data->settings.rect;

		bool found_expander = ((s64)it_block == layout_block_io->expand_index);

		block_width = MAX(block_width, rect->w);

		if (is_overwriting) {
			if (!found_expander) {
				height_remaining -= rect->h;
				--widget_count_auto;
			}
		}
		else
		if (!t_data->settings.auto_height) {
			height_remaining -= rect->h;
			--widget_count_auto;
		}
	}

	height_remaining -= padding_size;
	height_remaining -= layout_block_io->spacing * (widget_count - 1);

    block_width += padding_size;

    switch (layout_block_io->expand_index) {
    	case -3: {
    		it_y = (height_remaining + padding_border) >> 1;
    	} break;

    	case -4: {
    		it_y = (height_remaining + padding_border);
    	} break;

		default: break;
    }

    /// start drawing right
	if (layout_block_io->dock == LAYOUT_DOCK_BOTTOMRIGHT) {
		if (!is_last_block OR (is_last_block AND !layout_io->fill_last_block)) {
			it_x += layout_io->rect_remaining.w - block_width;
		}
	}

	float height_avg_auto = 0;

	if (widget_count_auto)
		height_avg_auto = ceil((float)height_remaining / widget_count_auto);

	/// align horizontal
    FOR_ARRAY(layout_block_io->ap_layout_data, it_block) {
		Layout_Data *t_data = ARRAY_IT(layout_block_io->ap_layout_data, it_block);

		Rect *rect = &t_data->settings.rect;

		/// center widgets in block
		s32 center_block = (rect->w - (block_width - padding_size)) >> 1;

		rect->x = layout_io->rect_remaining.x + it_x - center_block;
		rect->y = layout_io->rect_remaining.y + it_y;

		if (layout_io->fill_last_block AND is_last_block)
			rect->w = layout_io->rect_remaining.w - padding_size;

		bool found_expander = ((s64)it_block == layout_block_io->expand_index);

		float y_step = rect->h;

		if (is_overwriting) {
			if (found_expander)
				y_step = height_avg_auto;
		}
		else {
			if (t_data->settings.auto_height)
				y_step = height_avg_auto;
		}

		float height_limit = layout_io->rect_remaining.h - it_y;

		if (y_step > height_limit)
			y_step = height_limit;

		rect->h = y_step;
		it_y += y_step + layout_block_io->spacing;
    }

    /// cut of the left
	if (layout_block_io->dock == LAYOUT_DOCK_TOPLEFT)
		layout_io->rect_remaining.x += block_width;

    layout_io->rect_remaining.w -= block_width;
}

instant void
Layout_Arrange(
	Layout *layout_io
) {
	Assert(layout_io);

	if (!layout_io->rect_full.w)  return;
	if (!layout_io->rect_full.h)  return;

	layout_io->rect_remaining = layout_io->rect_full;

	if (layout_io->padding % 2 != 0) ++layout_io->padding;

	Rect_AddPadding(&layout_io->rect_remaining, {(float)layout_io->padding,
                                              (float)layout_io->padding,
												(s32)layout_io->padding,
                                                (s32)layout_io->padding});

	FOR_ARRAY(layout_io->a_layout_blocks, it) {
		Layout_Block *t_block = &ARRAY_IT(layout_io->a_layout_blocks, it);

		if (t_block->type == LAYOUT_TYPE_X)
			Layout_ArrangeBlockX(layout_io, t_block);
		else
		if (t_block->type == LAYOUT_TYPE_Y)
			Layout_ArrangeBlockY(layout_io, t_block);
	}
}


instant void
Layout_Rearrange(
	Layout *layout_io,
	Rect rect_resize
) {
	Assert(layout_io);

	/// always arrange, since the elements in the layout
	/// could have changed while the layout size has
	/// stayed the same
	layout_io->rect_full       = rect_resize;
	layout_io->rect_remaining  = rect_resize;

	Layout_Arrange(layout_io);
}

instant void
Layout_Rearrange(
	Layout *layout_io,
	Window *window
) {
	Assert(layout_io);
	Assert(window);

	Layout_Rearrange(layout_io, {0, 0, window->width, window->height});
}

instant Array<Widget *>
Layout_GetWidgetArray(
	Layout *layout
) {
	Assert(layout);

	return layout->ap_widgets;
}

/// ::: Widget
/// ===========================================================================
struct Widget;

typedef void (*Widget_OwnerDraw)
	(Widget *widget_io);
typedef void (*Widget_UpdateCustomInputsSub)
	(Widget *widget_parent_io, u64 sub_index);

enum WIDGET_TYPE {
	WIDGET_LABEL,
	WIDGET_BUTTON,
	WIDGET_LISTBOX,
	WIDGET_CHECKBOX,
	WIDGET_PICTUREBOX,
	WIDGET_SPREADER,
	WIDGET_NUMBERPICKER,
	WIDGET_TEXTBOX,
	WIDGET_COMBOBOX,
	WIDGET_PROGRESSBAR
};

enum WIDGET_COMBOBOX_TYPE {
	WIDGET_COMBOBOX_TEXT = 0,
	WIDGET_COMBOBOX_TOGGLE,
	WIDGET_COMBOBOX_LIST
};

enum WIDGET_SCROLL_TYPE {
	WIDGET_SCROLL_ITEM,
	WIDGET_SCROLL_BLOCK
};

enum WIDGET_SELECT_TYPE {
	WIDGET_SELECT_ON_RETURN,
	WIDGET_SELECT_ON_INDEX_CHANGE
};

struct Widget_Slide {
	s64 start = 0;
	s64 end   = 0;
	s64 value = 0;
	s64 step  = 0;
};

struct Widget_Data {
	Color32 color_background       = Color_MakeGrey(0.9f);
	Color32 color_outline          = {0.8f, 0.8f, 0.8f, 1.0f};
	Color32 color_outline_selected = {1.0f, 0.0f, 0.0f, 1.0f};
	Color32 color_outline_inactive = {0.5f, 0.5f, 1.0f, 1.0f};
	Color32 color_progress         = {0.5f, 0.5f, 1.0f, 1.0f};

	u64  active_row_id = 0;

	u32  border_size   = 0;
	u32  spacing       = 1;

	bool is_focusable  = true;
	bool is_scrollable = false;
	bool is_checkable  = false;

	bool can_popout_focus_change = false;

	/// overlay
	bool is_floating   = false; /// is overlay
	bool is_popout     = false; /// visible overlay

	bool has_focus  = false;
	bool is_checked = false;

	bool has_scrollable_list = false;
	WIDGET_SCROLL_TYPE scroll_type = WIDGET_SCROLL_ITEM;

	Array<String> as_row_data;

	String s_row_filter;
	Array<String> a_filter_data;
};

struct Widget {
	Window *window = 0;

	/// Defining Properties
	WIDGET_TYPE type;
	Widget_Data data;
	Widget_Data data_prev;
	Widget_Slide slide;

	/// On Demand
	static Widget *widget_focus_current;
	Widget *widget_focus_on_popout = 0;
	WIDGET_SELECT_TYPE type_select = WIDGET_SELECT_ON_RETURN;

	/// Triggers (per Frame)
	bool trigger_autosize = false;
	bool trigger_popout   = false;

	/// Events (per Frame)
	struct Widget_Events {
		bool on_list_change_index = false;
		bool on_list_change_final = false;

		bool on_text_change = false;

		bool on_trigger = false;
		bool on_trigger_secondary = false;
	} events;

	/// Custom Events
	Widget_OwnerDraw OwnerDraw = 0;
	Widget_UpdateCustomInputsSub UpdateCustomInputs = 0;

	/// Rendering
	Text   text;
	Vertex vertex_rect;

	/// Layout / Size
	Layout_Data layout_data;
	Rect rect_content; /// x,y = offsets

	/// Content
	Array<Widget> a_subwidgets;
};

Widget *Widget::widget_focus_current = 0;

instant Widget *
Widget_GetSubWidget(
	Widget *widget,
	u64 index
) {
	Assert(widget);

	if (!widget->a_subwidgets.count) {
		LOG_WARNING("Subwidget does not exists. Default back to main widget.");
		return widget;
	}

	if (index >= widget->a_subwidgets.count) {
		LOG_WARNING("Subwidget index out of bounds. Default back to main widget.");
		return widget;
	}

	return &ARRAY_IT(widget->a_subwidgets, index);
}

instant void
Widget_Cursor_ResetBlinking(
	Widget *widget_io
) {
	Assert(widget_io);

	widget_io->text.cursor.show_cursor = widget_io->data.has_focus;
	Time_Reset(&widget_io->text.cursor.timer_blinking);
	widget_io->text.cursor.is_blink_on = true;
}

instant bool
Widget_IsListType(
	Widget *widget
) {
	Assert(widget);

	if (widget->type == WIDGET_LISTBOX)  return true;

	return false;
}

instant void
Widget_AddRow(
	Widget *widget_io,
	const char *c_row_data,
	u64 c_length = 0
) {
	if (!c_row_data)
		return;

	/// list contained in subwidgets
	switch (widget_io->type) {
		case WIDGET_COMBOBOX: {
			widget_io = &ARRAY_IT(widget_io->a_subwidgets, 2);
		} break;

		default: {} break;
	}

	String *ts_data;
	Array_AddEmpty(&widget_io->data.as_row_data, &ts_data);
	*ts_data = String_Copy(c_row_data, c_length);
}

instant void
Widget_AddRows(
	Widget *widget_io,
	Array<String> *as_list
) {
	Assert(widget_io);
	Assert(as_list);

	FOR_ARRAY(*as_list, it) {
		String *ts_item = &ARRAY_IT(*as_list, it);

		Widget_AddRow(widget_io, ts_item->value, ts_item->length);
	}
}

instant bool
Mouse_IsHovering(
	Widget *widget,
	Mouse *mouse_out = 0
) {
	Assert(widget);

	Point t_point;
	bool is_hovering_popout = false;

	if (mouse_out) {
		Mouse_GetPosition(mouse_out, widget->window);
		t_point = mouse_out->point;
	}
	else {
		Mouse_GetPosition(&t_point.x, &t_point.y, widget->window);
	}

	/// overlay check
	if (    widget != widget->widget_focus_current
		AND widget->widget_focus_current
		AND widget->widget_focus_current->data.is_floating
	) {
		if (widget->widget_focus_current->data.is_popout) {
			is_hovering_popout = Mouse_IsHovering(widget->widget_focus_current, mouse_out);
		}
	}

	if (is_hovering_popout) {
		return false;
	}

    return Rect_IsIntersecting(&t_point, &widget->layout_data.settings.rect);
}

instant bool
Widget_HasChanged(
	Widget *widget_io,
	bool update_changes
) {
	Assert(widget_io);

	bool result = false;

	///@Note: updating text changed will happen
	///       in a Text_Update
	result = Text_HasChanged(&widget_io->text, false);

	if (!result) {
		result = !Memory_Compare(
							&widget_io->layout_data.settings,
							&widget_io->layout_data.settings_prev,
							 sizeof(widget_io->layout_data.settings)
					  );
	}

	if (!result) {
		result = !Memory_Compare(
							&widget_io->data,
							&widget_io->data_prev,
							 sizeof(widget_io->data)
					  );
	}

	if (!result) {
		FOR_ARRAY(widget_io->data.as_row_data, it) {
			String *t_data = &ARRAY_IT(widget_io->data.as_row_data, it);

			if (t_data->changed) {
				result = true;

				if (!update_changes)
					break;

				t_data->changed = false;
			}
		}
	}

	if (update_changes) {
		widget_io->layout_data.settings_prev = widget_io->layout_data.settings;
		widget_io->data_prev = widget_io->data;
	}

/// @NOTE: do NOT check subwidgets, since they will be added to the render list anyway
///        and might mess up the update checking

	return result;
}

instant void
Widget_AddBorderSizes(
	Widget *widget,
	s32 *min_width_io,
	s32 *min_height_io
) {
	Assert(widget);

	Rect *rect_padding = &widget->text.data.rect_padding;

	if (min_width_io) {
		*min_width_io += rect_padding->x + rect_padding->w;

		/// border size is used for the checbox,
		/// not the border of the widget itself
		if (widget->type != WIDGET_CHECKBOX)
			*min_width_io += widget->data.border_size << 1;

		*min_width_io +=   widget->text.data.rect_margin.x
					  + widget->text.data.rect_margin.w;
	}

	if (min_height_io) {
		*min_height_io += rect_padding->y + rect_padding->h;

		/// border size is used for the checbox,
		/// not the border of the widget itself
		if (widget->type != WIDGET_CHECKBOX)
			*min_height_io += widget->data.border_size << 1;

		*min_height_io +=  widget->text.data.rect_margin.y
					  + widget->text.data.rect_margin.h;
	}
}

instant void
Widget_SetFocus(
	Widget *widget_io
) {
	Assert(widget_io);

	if (!widget_io->data.is_focusable)
		return;

    if (widget_io->widget_focus_current) {
    	widget_io->widget_focus_current->data.has_focus = false;
    }

    widget_io->data.has_focus = true;
    widget_io->widget_focus_current = widget_io;

    if (widget_io->data.is_floating)
		widget_io->data.is_popout = true;
}

instant bool
Widget_Update(
	Widget *widget_io
) {
	Assert(widget_io);

	bool result = false;

	LOG_STATUS("Update: " << widget_io->type << " ");

	if (widget_io->a_subwidgets.count) {
		LOG_STATUS("- Subwidgets: " << widget_io->a_subwidgets.count << " ");
		LOG_STATUS("\n----------------------\n");
	}

	FOR_ARRAY(widget_io->a_subwidgets, it) {
		Widget *t_widget = &ARRAY_IT(widget_io->a_subwidgets, it);
		Widget_Update(t_widget);
	}

	if (widget_io->a_subwidgets.count) {
		LOG_STATUS("----------------------");
		LOG_STATUS("\nResume update: " << widget_io->type << " ");
	}

	/// text needs to be processed at least once when no input in handled,
	/// but do NOT do it for lists. lists take the list data and pass each row
	/// through the text struct to generate text + positions.
	/// using the update function here again, would invalidate that data,
	/// since the data would definitely have changed
	if (!Widget_IsListType(widget_io))
		Text_Update(&widget_io->text);

	if (widget_io->trigger_autosize){
		Layout_Data_Settings *layout_data = &widget_io->layout_data.settings;
		Text_Data *settings = &widget_io->text.data;

		if (layout_data->auto_width AND settings->content_width) {
			s32 width_auto = settings->content_width;
			Widget_AddBorderSizes(widget_io, &width_auto, 0);

			if (layout_data->rect.w != width_auto) {
				layout_data->rect.w  = width_auto;
				result = true;
			}
		}

		if (layout_data->auto_height AND settings->content_height) {
			s32 height_auto = settings->content_height;
			Widget_AddBorderSizes(widget_io, 0, &height_auto);

			if (layout_data->rect.h != height_auto) {
				layout_data->rect.h  = height_auto;
				result = true;
			}
		}

		widget_io->trigger_autosize = false;
	}

	if (widget_io->trigger_popout) {
		widget_io->data.is_popout = !widget_io->data.is_popout;
		widget_io->trigger_popout = false;

		if (widget_io->data.can_popout_focus_change) {
			if (widget_io->data.is_popout) {
				Widget_SetFocus(widget_io);
			}
			else {
				if (widget_io->widget_focus_on_popout) {
					Widget_SetFocus(widget_io->widget_focus_on_popout);
				}
			}
		}
	}

	if (widget_io->data.s_row_filter.changed) {
		Array_Clear(&widget_io->data.a_filter_data);
		widget_io->data.a_filter_data.by_reference = true;
		widget_io->data.active_row_id = 0;

		if (widget_io->data.s_row_filter.length) {
			FOR_ARRAY(widget_io->data.as_row_data, it_row) {
				String *ts_data = &ARRAY_IT(widget_io->data.as_row_data, it_row);

				if (String_IndexOf(	ts_data,
									widget_io->data.s_row_filter.value,
									widget_io->data.s_row_filter.length) >= 0
				) {
					Array_Add(&widget_io->data.a_filter_data, *ts_data);
				}
			}
		}

		widget_io->data.s_row_filter.changed = false;
	}

	LOG_STATUS("completed\n");

	return result;
}

instant void
Widget_Redraw(
	Widget *widget_io
) {
	Assert(widget_io);

	if (widget_io->OwnerDraw) {
		widget_io->OwnerDraw(widget_io);
		return;
	}

	Vertex *t_vertex = &widget_io->vertex_rect;
	Rect    rect_box =  widget_io->layout_data.settings.rect;

	if (!t_vertex->array_id) Vertex_Create(t_vertex);
	else                     Vertex_ClearAttributes(t_vertex);

	switch (widget_io->type) {
		case WIDGET_SPREADER: {
		} break;

		case WIDGET_TEXTBOX: {
			widget_io->text.data.rect = widget_io->layout_data.settings.rect;

			Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_background);

			Widget_Cursor_ResetBlinking(widget_io);
		} break;

		case WIDGET_COMBOBOX: {
			Rect *rect_layout = &widget_io->layout_data.settings.rect;

			rect_layout->h = 0;

			/// calc. max. visible widget height to be used for the layout system,
			/// in order to get preper horizontal alignment
            FOR_ARRAY(widget_io->a_subwidgets, it) {
				Widget *t_widget = &ARRAY_IT(widget_io->a_subwidgets, it);

                if (!t_widget->data.is_floating) {
					rect_layout->h = MAX(rect_layout->h, t_widget->layout_data.settings.rect.h);
                }
            }

			Layout layout;
			Layout_Create(&layout, &widget_io->layout_data, false);
			layout.padding = 0;

			Layout_Block *t_layout_block = 0;

			Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT, 0, &t_layout_block);
			t_layout_block->padding = 0;

			Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, WIDGET_COMBOBOX_TEXT));
			Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, WIDGET_COMBOBOX_TOGGLE));

			Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT, 0, &t_layout_block);
			t_layout_block->padding = 0;

			Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, WIDGET_COMBOBOX_LIST));

			Layout_Arrange(&layout);
		} break;

		case WIDGET_LABEL: {
			widget_io->text.data.rect = widget_io->layout_data.settings.rect;
			Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_background);
		} break;

		case WIDGET_PICTUREBOX:
		case WIDGET_LISTBOX: {
			Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_background);
		} break;

		case WIDGET_BUTTON: {
			widget_io->text.data.rect = widget_io->layout_data.settings.rect;

			Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_background);

			if (widget_io->data.border_size) {
				Rect_Resize(&rect_box, -1);

				if (widget_io->data.has_focus)
					Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_outline_selected);
				else
					Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_outline_inactive);

				Rect_Resize(&rect_box, -widget_io->data.border_size);
				Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_background);
			}
		} break;

		case WIDGET_CHECKBOX: {
			widget_io->text.data.rect = widget_io->layout_data.settings.rect;

			Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_background);

			s32 check_offset = 2;
			s32 check_h = widget_io->text.font->size - (check_offset << 1);
			s32 check_w = check_h;

			Rect rect_check = {
				rect_box.x + check_offset + widget_io->text.data.rect_padding.x,
				rect_box.y + check_offset + widget_io->text.data.rect_padding.y,
				check_w,
				check_h
			};

			widget_io->text.data.rect_margin = {
				(float)check_offset * 2 + check_w + 2,
				0,
				0,
				0
			};

			if (widget_io->data.has_focus)
				Vertex_AddRect32(t_vertex, rect_check, widget_io->data.color_outline_selected);
			else
				Vertex_AddRect32(t_vertex, rect_check, widget_io->data.color_outline_inactive);

			Assert(widget_io->data.border_size);
			Assert(widget_io->data.border_size < 20);

			widget_io->data.border_size = (widget_io->text.font->size / 10);

			Rect_Resize(&rect_check, -widget_io->data.border_size);
			Vertex_AddRect32(t_vertex, rect_check, widget_io->data.color_background);

			if (widget_io->data.is_checked) {
				Rect_Resize(&rect_check, -1);
				Vertex_AddRect32(t_vertex, rect_check, widget_io->data.color_outline_selected);
			}
		} break;

		case WIDGET_NUMBERPICKER: {
			Layout layout;
			Layout_Create(&layout, &widget_io->layout_data, false);
			layout.padding = 0;

			Layout_Block *current_block;
			Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT, 0, &current_block);
			current_block->padding = 0;
			current_block->spacing = 2;

			/// label
			Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, 0));
			/// spreader
			Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, 3));
			/// button up
			Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, 1));
			/// button down
			Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, 2));
			Layout_Arrange(&layout);
		} break;

		case WIDGET_PROGRESSBAR: {
			widget_io->text.data.rect = widget_io->layout_data.settings.rect;

			Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_background);

			Rect rect_progress = rect_box;
			Widget_Slide *slide = &widget_io->slide;

			float percent = (float)slide->value / (slide->end - slide->start);

			rect_progress.w *= percent;

			Vertex_AddRect32(t_vertex, rect_progress, widget_io->data.color_progress);
		} break;

		default:
			AssertMessage(	false,
							"Redrawing: Unhandled widget background drawing.");
	}
}

instant void
Widget_InvalidateBackground(
	Widget *widget_io
) {
	Assert(widget_io);

	Widget_Redraw(widget_io);
}

instant void
Widget_GetListArrayFiltered(
	Widget *widget,
	Array<String> **as_row_data_out
) {
	Assert(widget);
	Assert(as_row_data_out);

	Array<String> *as_target = &widget->data.as_row_data;

	if (widget->data.s_row_filter.length) {
		as_target = &widget->data.a_filter_data;
		Clamp(&widget->data.active_row_id, 0, widget->data.a_filter_data.count - 1);
	}

	*as_row_data_out = as_target;
}

/// return an non-rendered overlay widget (if exists)
instant Widget *
Widget_Render(
	ShaderSet *shader_set,
	Widget *widget_io,
	bool render_overlay = false
) {
	Assert(shader_set);
	Assert(widget_io);

	widget_io->text.shader_set = shader_set;

	/// draw non-list data
	if (!Widget_IsListType(widget_io)) {
		if (widget_io->data.is_floating AND !render_overlay) {
			return widget_io;
		}

		if (Widget_HasChanged(widget_io, true)) {
			Widget_InvalidateBackground(widget_io);
		}

		if (widget_io->vertex_rect.a_attributes.count) {
			ShaderSet_Use(shader_set, SHADER_PROG_RECT);
			Rect_Render(shader_set, &widget_io->vertex_rect);
		}

		if (widget_io->vertex_rect.texture.ID) {
			ShaderSet_Use(shader_set, SHADER_PROG_TEXTURE_SIZE);

			s32 width, height;
			Texture_GetSizeAndBind(&widget_io->vertex_rect.texture, &width, &height);

			Rect rect_tex_aspect = widget_io->layout_data.settings.rect;
			Rect_GetAspect(&rect_tex_aspect, width, height);

			static Vertex vertex_texture = Vertex_Create();
			Vertex_SetTexture(shader_set, &vertex_texture, &widget_io->vertex_rect.texture);
			vertex_texture.settings = widget_io->vertex_rect.settings;

			Vertex_AddRectTexture(&vertex_texture, rect_tex_aspect);
			Rect_Render(shader_set, &vertex_texture);

			Vertex_ClearAttributes(&vertex_texture);
		}

		Text_Render(&widget_io->text);
	}
	else {
		/// do not draw a list widget,
		/// if it should not be rendered as a popout
		if (widget_io->data.is_floating AND (!widget_io->data.is_popout OR !render_overlay)) {
			return widget_io;
		}

		///@Refactor: update for list data, might make more sense
		///           to move it into a seperate update function
		if (Widget_HasChanged(widget_io, true)) {
			Vertex_ClearAttributes(&widget_io->vertex_rect);
			Widget_InvalidateBackground(widget_io);

			Text *t_text = &widget_io->text;

			Text_Clear(t_text);

			t_text->data.rect = widget_io->layout_data.settings.rect;
			Rect *rect_text   = &t_text->data.rect;

			s32 pad_left = 2;

			rect_text->x += t_text->offset_x + pad_left;
			rect_text->y += t_text->offset_y;

			widget_io->rect_content.h = 0;

			Array<String> *as_target;
			Widget_GetListArrayFiltered(widget_io, &as_target);

			FOR_ARRAY(*as_target, it_row) {
				String *ts_data = &ARRAY_IT(*as_target, it_row);

				u64 number_of_lines = String_SplitWordsBuffer(ts_data, &t_text->as_words);
				rect_text->h = Text_BuildLines(t_text, &t_text->as_words, number_of_lines, &t_text->a_text_lines);

				if (Rect_IsIntersecting(rect_text, &widget_io->layout_data.settings.rect)) {
					Color32 t_color_rect = widget_io->data.color_outline;

					if (widget_io->data.active_row_id == it_row) {
						if (widget_io->data.has_focus)
							t_color_rect = widget_io->data.color_outline_selected;
						else
							t_color_rect = widget_io->data.color_outline_inactive;
					}

					Rect rect_box = *rect_text;
					rect_box.x -= pad_left;

					Vertex_AddRect32(&widget_io->vertex_rect, rect_box, t_color_rect);

					Text_AddLines(t_text, false);
				}

				s32 height_row_step = rect_text->h + widget_io->data.spacing;
				rect_text->y           += height_row_step;
				widget_io->rect_content.h += height_row_step;

				ts_data->changed = false;
			}

			/// revert for scissor
			*rect_text = widget_io->layout_data.settings.rect;

			t_text->data_prev = t_text->data;
		}

		OpenGL_Scissor(shader_set->window, widget_io->layout_data.settings.rect);

		/// render rectangles + background
		if (widget_io->vertex_rect.a_attributes.count) {
			ShaderSet_Use(shader_set, SHADER_PROG_RECT);
			Rect_Render(shader_set, &widget_io->vertex_rect);
		}

		Text_Render(&widget_io->text);

		OpenGL_Scissor_Disable();
	}

	Widget *wg_overlay = 0;

	FOR_ARRAY(widget_io->a_subwidgets, it_sub) {
		Widget *t_subwidget = &ARRAY_IT(widget_io->a_subwidgets, it_sub);

		wg_overlay = Widget_Render(shader_set, t_subwidget);
	}

	/// Reset per Frame Event Data
	widget_io->events = {};

	return wg_overlay;
}

instant void
Widget_Render(
	ShaderSet *shader_set,
	Array<Widget *> *ap_widgets_io
) {
	Assert(ap_widgets_io);

	Widget *wg_overlay = 0;

    FOR_ARRAY(*ap_widgets_io, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets_io, it_widget);

		Widget *twg_overlay_buffer = Widget_Render(shader_set, t_widget, false);

		if (twg_overlay_buffer) {
			///@Note: there should be only one visible overlay
			if (!wg_overlay)
				wg_overlay = twg_overlay_buffer;
			else
				Assert(false);
		}
    }

    if (wg_overlay) {
    	Widget_Render(shader_set, wg_overlay, true);
    }
}

instant void
Widget_Destroy(
	Widget *widget_out
) {
	Assert(widget_out);

	Text_Destroy(&widget_out->text);
	Vertex_Destroy(&widget_out->vertex_rect);
}

instant void
Widget_Destroy(
	Array<Widget *> *ap_widgets_out
) {
	Assert(ap_widgets_out);

    FOR_ARRAY(*ap_widgets_out, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets_out, it_widget);

		Text_Destroy(&t_widget->text);
		Vertex_Destroy(&t_widget->vertex_rect);
    }
}

instant bool
Widget_IsFocusable(
	Widget *widget
) {
	Assert(widget);

	bool is_focusable = widget->data.is_focusable;

	if (is_focusable)  return is_focusable;

	FOR_ARRAY(widget->a_subwidgets, it) {
		Widget *t_widget = &ARRAY_IT(widget->a_subwidgets, it);
		is_focusable = Widget_IsFocusable(t_widget);

		if (is_focusable)
			break;
	}

	return is_focusable;
}

instant void
Widget_UpdateFocus(
	Array<Widget *> *ap_widgets_io,
	bool check_backward
) {
	Assert(ap_widgets_io);

	bool focus_set_next = false;

	if (!ap_widgets_io->count)
		return;

	static Array<Widget *> ap_widgets_all;
	Array_ClearContainer(&ap_widgets_all);

	FOR_ARRAY(*ap_widgets_io, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets_io, it_widget);
		Array_Add(&ap_widgets_all, t_widget);

		FOR_ARRAY(t_widget->a_subwidgets, it_sub) {
			Widget *t_subwidget = &ARRAY_IT(t_widget->a_subwidgets, it_sub);
			Array_Add(&ap_widgets_all, t_subwidget);
		}
	}

	s64 start = 0;
	s64 end   = ap_widgets_all.count;
	s64 step  = 1;

	if (check_backward) {
		start =  end - 1;
		end   = -1;
		step  = -1;
	}

	for (s64 it_widget = start; it_widget != end; it_widget += step) {
		Widget *t_widget = ARRAY_IT(ap_widgets_all, it_widget);

		if (!Widget_IsFocusable(t_widget))
			continue;

		Keyboard *keyboard = t_widget->window->keyboard;
		Mouse    *mouse    = t_widget->window->mouse;

		bool is_visible = (   !t_widget->data.is_floating
		                   OR  t_widget->data.is_popout);

		if (    focus_set_next
			AND is_visible
			AND t_widget->data.is_focusable
		) {
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

		if (t_widget->data.has_focus) {
			if (!is_visible) {
				t_widget->data.has_focus = false;
                focus_set_next = true;
			}
			else {
				t_widget->widget_focus_current = t_widget;
			}
		}
	}

	/// search for next focus from the beginning,
	/// if there was no focusable left past the last focused
	/// widget
	if (focus_set_next OR !Widget::widget_focus_current) {
		for (s64 it_widget = start; it_widget != end; it_widget += step) {
			Widget *t_widget = ARRAY_IT(ap_widgets_all, it_widget);

			bool is_visible = (   !t_widget->data.is_floating
							   OR  t_widget->data.is_popout);

			if (t_widget->data.is_focusable AND is_visible) {
				t_widget->data.has_focus = true;
				t_widget->widget_focus_current = t_widget;

				break;
			}
		}
	}
}

instant void
Widget_CalcActiveRowRect(
	Widget *widget,
	Rect *rect_row_out
) {
	Assert(widget);
    Assert(rect_row_out);

    *rect_row_out = {};

    String ts_data_backup = widget->text.s_data;

	Rect rect_item  = widget->layout_data.settings.rect;
	rect_item.x    += widget->text.offset_x;
	rect_item.y    += widget->text.offset_y;

    FOR_ARRAY(widget->data.as_row_data, it_row) {
		String *ts_data = &ARRAY_IT(widget->data.as_row_data, it_row);
		widget->text.s_data = *ts_data;

		s32 width, height;
		Text_GetSize(&widget->text, &width, &height);

		rect_item.w = width;
		rect_item.h = height;

		if (widget->data.active_row_id == it_row) {
			*rect_row_out = rect_item;
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

    if (!Mouse_IsHovering(widget, mouse))
		return widget->data.active_row_id;

    u64 active_row_id = 0;

    String ts_data_backup = widget->text.s_data;

	Rect rect_item  = widget->layout_data.settings.rect;
	rect_item.x    += widget->text.offset_x;
	rect_item.y    += widget->text.offset_y;

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
	Widget *widget_io
) {
	Assert(widget_io);
	Assert(widget_io->window);

    Keyboard *keyboard = widget_io->window->keyboard;
    Mouse    *mouse    = widget_io->window->mouse;

    Rect *rect_widget = &widget_io->layout_data.settings.rect;

    if (widget_io->a_subwidgets.count) {
		FOR_ARRAY(widget_io->a_subwidgets, it_sub) {
			Widget *t_subwidget = &ARRAY_IT(widget_io->a_subwidgets, it_sub);

			if (t_subwidget->UpdateCustomInputs)
				t_subwidget->UpdateCustomInputs(widget_io, it_sub);
			else
				Widget_UpdateInput(t_subwidget);
		}

		return;
    }

	if (!widget_io->data.is_focusable)
		return;

	if (widget_io->data.is_floating AND !widget_io->data.is_popout)
		return;

	bool got_focus = widget_io->data.has_focus;
	u64  prev_active_row = widget_io->data.active_row_id;

	bool has_text_cursor = widget_io->text.data.is_editable;

	bool is_scrollable_list = widget_io->data.has_scrollable_list;
	bool is_scrollable      = widget_io->data.is_scrollable;

    if (mouse) {
		bool is_hovering = Mouse_IsHovering(widget_io, mouse);

		/// text selection
		if (has_text_cursor AND is_hovering) {
			Text *text = &widget_io->text;

			text->cursor.is_selecting = mouse->pressing[0];

			if (text->cursor.is_selecting) {
				widget_io->data.has_focus = true;

				if (mouse->is_down) {
					u64 index = Text_Cursor_FindIndex(text, mouse->point);

					text->cursor.data.index_select_start = index;
					text->cursor.data.index_select_end   = index;
				}
				else {
					u64 index = Text_Cursor_FindIndex(text, mouse->point);

					text->cursor.data.index_select_end   = index;

					Widget_Cursor_ResetBlinking(widget_io);
				}

				Text_Cursor_Update(text);
			}
		}

		/// left mouse button
		if (mouse->up[0]) {
			got_focus = is_hovering;

			if (is_hovering) {
				widget_io->events.on_trigger = true;
			}

			/// listbox entry selection
			if (is_scrollable_list) {
				u64 new_active_row_id = Widget_CalcActiveRowID(widget_io, mouse);

				if (new_active_row_id != widget_io->data.active_row_id) {
					widget_io->data.active_row_id = new_active_row_id;
					widget_io->events.on_list_change_index = true;
				}
				else {
					widget_io->events.on_list_change_final = true;
				}
			}

			/// checkbox toggle
			if (got_focus AND widget_io->data.is_checkable) {
				widget_io->data.is_checked = !widget_io->data.is_checked;
			}

			/// focus change
			widget_io->data.has_focus = got_focus;
		}

		/// right mouse button
		if (mouse->up[2]) {
			got_focus = is_hovering;

			if (is_hovering) {
				widget_io->events.on_trigger_secondary = true;
			}
		}

		/// widget_io + list scrolling
		if (is_hovering AND (is_scrollable_list OR is_scrollable)) {
			///@TODO: scroll horizontally when pressing shift + wheel(?)
			widget_io->text.offset_y += mouse->wheel;

			if (is_scrollable_list) {
				widget_io->text.data.content_width  = widget_io->rect_content.w;
				widget_io->text.data.content_height = widget_io->rect_content.h;
			}

			Rect_ClampY(
				&widget_io->text.offset_y,
				widget_io->text.data.content_height,
				widget_io->text.data.rect.h
			);
		}
    }

    if (keyboard AND widget_io->data.has_focus) {
		bool is_key_return = keyboard->up[VK_RETURN];
		bool is_key_space  = keyboard->up[VK_SPACE];
		bool is_key_f10    = keyboard->up[VK_F10];

        if (is_key_return OR is_key_space) {
        	widget_io->events.on_trigger = true;
        }

        if (is_key_f10) {
        	widget_io->events.on_trigger_secondary = true;
        }

		if (widget_io->data.is_checkable
			AND (   is_key_return
				 OR is_key_space)
		) {
			widget_io->data.is_checked = !widget_io->data.is_checked;
		}

		if (is_scrollable_list) {
			if (widget_io->data.active_row_id) {
				if (keyboard->down[VK_UP]) {
					--widget_io->data.active_row_id;
					widget_io->events.on_list_change_index = true;
				}

				if (keyboard->down[VK_HOME]) {
					if (widget_io->data.active_row_id != 0) {
						widget_io->data.active_row_id = 0;
						widget_io->events.on_list_change_index = true;
					}
				}
			}

			if (    widget_io->data.as_row_data.count
				AND widget_io->data.active_row_id < widget_io->data.as_row_data.count - 1
			) {
				if (keyboard->down[VK_DOWN]) {
					++widget_io->data.active_row_id;
					widget_io->events.on_list_change_index = true;
				}

				if (keyboard->down[VK_END]) {
					widget_io->data.active_row_id = widget_io->data.as_row_data.count - 1;
					widget_io->events.on_list_change_index = true;
				}
			}
		}

		bool has_text_changed;
		bool has_cursor_changed;

		Text_UpdateInput(&widget_io->text, keyboard, &has_text_changed, &has_cursor_changed);

		widget_io->events.on_text_change = has_text_changed;

		if (has_cursor_changed) {
			Widget_Cursor_ResetBlinking(widget_io);
		}
    }

    if (prev_active_row != widget_io->data.active_row_id AND is_scrollable_list) {
		Rect rect_active_row;
        Widget_CalcActiveRowRect(widget_io, &rect_active_row);

		if (!Rect_IsVisibleFully(&rect_active_row, rect_widget)) {
			if (widget_io->data.scroll_type == WIDGET_SCROLL_ITEM) {
				if (widget_io->data.active_row_id < prev_active_row) {
					widget_io->text.offset_y -= (rect_active_row.y - rect_widget->y);
				}
				else {
					widget_io->text.offset_y -= (rect_active_row.y - rect_widget->y);
					widget_io->text.offset_y += (rect_widget->h - rect_active_row.h);
				}
			}
			else
			if (widget_io->data.scroll_type == WIDGET_SCROLL_BLOCK) {
				if (!Rect_IsVisibleFully(&rect_active_row, rect_widget)) {
					if (widget_io->data.active_row_id < prev_active_row) {
						widget_io->text.offset_y -= (rect_active_row.y - rect_widget->y);
						widget_io->text.offset_y += (rect_widget->h - rect_active_row.h);
					}
					else {
						widget_io->text.offset_y -= (rect_active_row.y - rect_widget->y);
					}
				}
			}
			else {
				AssertMessage(	false,
								"Unhandled widget scroll type.");
			}
		}

		Rect_ClampY(&widget_io->rect_content, *rect_widget);
    }
}

instant void
Widget_GetSelectedRowBuffer(
	Widget *widget,
	String *s_row_data_out
) {
	Assert(widget);
	Assert(s_row_data_out);
	Assert(!widget->data.active_row_id OR widget->data.active_row_id < widget->data.as_row_data.count);

	if (!widget->data.as_row_data.count) {
		String_Clear(s_row_data_out);
		return;
	}

	Array<String> *as_target;
	Widget_GetListArrayFiltered(widget, &as_target);

	String *ts_row_data = &ARRAY_IT(*as_target, widget->data.active_row_id);

	String_Clear(s_row_data_out);
	String_Append(s_row_data_out, ts_row_data->value, ts_row_data->length);
}

instant String
Widget_GetSelectedRow(
	Widget *widget
) {
	Assert(widget);
	Assert(!widget->data.active_row_id OR widget->data.active_row_id < widget->data.as_row_data.count);

	String s_result = {};

	if (!widget->data.as_row_data.count) {
		return s_result;
	}

	String *ts_row_data = &ARRAY_IT(widget->data.as_row_data, widget->data.active_row_id);

	String_Append(&s_result, ts_row_data->value, ts_row_data->length);

	return s_result;
}

instant u64
Widget_GetSelectedRowID(
	Widget *widget
) {
	Assert(widget);
	Assert(!widget->data.active_row_id OR widget->data.active_row_id < widget->data.as_row_data.count);

	return widget->data.active_row_id;
}

instant bool
Widget_Update(
	Array<Widget *> *ap_widgets_io,
	Keyboard *keyboard
) {
	Assert(ap_widgets_io);
	Assert(keyboard);

	bool result = false;

	FOR_ARRAY(*ap_widgets_io, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets_io, it_widget);
		Widget_UpdateInput(t_widget);
	}

	FOR_ARRAY(*ap_widgets_io, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets_io, it_widget);

		bool did_update = Widget_Update(t_widget);

		if (did_update)  result = true;
	}

	Widget_UpdateFocus(ap_widgets_io, keyboard->pressing[VK_SHIFT]);

	return result;
}

instant void
Widget_ClearRows(
	Widget *widget_out
) {
	Assert(widget_out);

	Array_Clear(&widget_out->data.as_row_data);
}

instant void
Layout_Add(
	Layout *layout_io,
	Widget *widget
) {
	Assert(layout_io);
	Assert(widget);

	Layout_Add(layout_io, &widget->layout_data);

	/// convinence
	Array_Add(&layout_io->ap_widgets, widget);
}

instant void
Widget_LoadDirectoryList(
	Widget *widget_io,
	String *s_directory,
	Array<Directory_Entry> *a_entries_out,
	bool show_full_path = false
) {
	Assert(widget_io);
	Assert(widget_io->type == WIDGET_LISTBOX);
	Assert(s_directory);

	/// in case the directory string came from the to be destroyed directory entries
	static String ts_directory;
	String_Append(&ts_directory, s_directory->value, s_directory->length);

	FOR_ARRAY(*a_entries_out, it) {
		Directory_Entry *t_entry = &ARRAY_IT(*a_entries_out, it);
		String_Destroy(&t_entry->s_name);
	}

	Array_ClearContainer(a_entries_out);

	widget_io->data.active_row_id = 0;

	/// remove "\" from directroy path (f.e. C:\) for consistency
	if (String_EndWith(&ts_directory, "\\")) {
		String_Remove(&ts_directory, ts_directory.length - 1, ts_directory.length);
	}

	/// will still include path into dir array, even if it is not rendering,
	/// so the path does not have to concatonate the find the targeted file,
	/// which is more practical, than simply knowing the filename
	File_ReadDirectory(a_entries_out, ts_directory.value, 0, ts_directory.length, true, 0, DIR_LIST_ONLY_DIR);
	File_ReadDirectory(a_entries_out, ts_directory.value, 0, ts_directory.length, true, 0, DIR_LIST_ONLY_FILES);

	Widget_ClearRows(widget_io);

	FOR_ARRAY(*a_entries_out, it) {
		Directory_Entry *t_entry = &ARRAY_IT(*a_entries_out, it);

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

		Widget_AddRow(widget_io, ts_entry_name.value, ts_entry_name.length);
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
	t_widget.text.data.rect_padding = {2, 2, 2, 2};

	Rect *rect_padding = &t_widget.text.data.rect_padding;

	t_widget.type         = WIDGET_BUTTON;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.text.data.align_x = TEXT_ALIGN_X_MIDDLE;
	t_widget.text.data.align_y = TEXT_ALIGN_Y_CENTER;

	t_widget.text.font = font;

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

	t_widget.text.data.rect = rect_box;

	t_widget.text.font = font;

	return t_widget;
}

instant Widget
Widget_CreateCheckBox(
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

	t_widget.text.font = font;

	String_Append(&t_widget.text.s_data, c_data, c_length);

	t_widget.text.data.rect_padding = {2, 2, 2, 2};

	t_widget.trigger_autosize = true;

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

	Widget t_widget = {};

    t_widget.type = WIDGET_SPREADER;
    t_widget.window = window;

    t_widget.data.is_focusable = false;

    return t_widget;
}

instant void
Widget_UpdateInputNumberPicker(
	Widget *widget_parent_io,
	u64 sub_index
) {
	Assert(widget_parent_io);

	Widget *widget = &ARRAY_IT(widget_parent_io->a_subwidgets, sub_index);

	Widget_UpdateInput(widget);

	Widget *tw_label = &ARRAY_IT(widget_parent_io->a_subwidgets, 0);

	/// based on subwidget array index
	switch (sub_index) {
		case 1:
		case 2: {
			if (widget->events.on_trigger) {
				Widget_Slide *t_slide = &widget_parent_io->slide;

				t_slide->value = ToInt(&tw_label->text.s_data);

				if (sub_index == 1) {
					if (t_slide->value - t_slide->step >= t_slide->start)
						t_slide->value -= t_slide->step;
				}
				else
				if (sub_index == 2) {
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
	Widget *widget_io
) {
	Assert(widget_io);

	Vertex *t_vertex = &widget_io->vertex_rect;
	Rect    rect_box =  widget_io->layout_data.settings.rect;

	if (!t_vertex->array_id) Vertex_Create(t_vertex);
	else                     Vertex_ClearAttributes(t_vertex);

	if (widget_io->data.has_focus)
		Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_outline_selected);
	else
		Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_outline);

	Rect_Resize(&rect_box, -2);
	Vertex_AddRect32(t_vertex, rect_box, {1, 1, 1, 1});

	widget_io->text.data.rect = widget_io->layout_data.settings.rect;
	widget_io->text.data.rect.x += 1;
	widget_io->text.data.rect.h -= 3;
}

instant Widget
Widget_CreateNumberPicker(
	Window *window,
	Font *font,
	Rect rect_box,
	Widget_Slide slide
) {
	Widget t_widget = {};

    t_widget.type = WIDGET_NUMBERPICKER;
    t_widget.window = window;
    t_widget.rect_content = rect_box;
    t_widget.slide = slide;

    t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.data.is_focusable = false;

	Widget wg_label       = Widget_CreateLabel( window, font, {0, 0, 50, 24});
	Widget wg_button_up   = Widget_CreateButton(window, font, {0, 0, 24, 24}, "<");
	Widget wg_button_down = Widget_CreateButton(window, font, {0, 0, 24, 24}, ">");
	Widget wg_spreader    = Widget_CreateSpreader(window);

	wg_button_up.trigger_autosize   = false;
	wg_button_down.trigger_autosize = false;

	wg_button_up.OwnerDraw   = Widget_RedrawNumberPickerButton;
	wg_button_down.OwnerDraw = Widget_RedrawNumberPickerButton;
	wg_button_up.UpdateCustomInputs   = Widget_UpdateInputNumberPicker;
	wg_button_down.UpdateCustomInputs = Widget_UpdateInputNumberPicker;

	wg_label.text.data.align_x = TEXT_ALIGN_X_RIGHT;
	wg_label.text.data.rect_padding = {2, 2, 2, 2};

	char *c_value = ToCString(slide.value);
	String_Append(&wg_label.text.s_data, c_value);
	Memory_Free(c_value);

	Array_Add(&t_widget.a_subwidgets, wg_label);
	Array_Add(&t_widget.a_subwidgets, wg_button_up);
	Array_Add(&t_widget.a_subwidgets, wg_button_down);
	Array_Add(&t_widget.a_subwidgets, wg_spreader);

	return t_widget;
}

instant Widget
Widget_CreateTextBox(
	Window *window,
	Font *font,
	Rect rect_box
) {
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

	t_widget.type         = WIDGET_TEXTBOX;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.data.is_scrollable = true;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.text.font  = font;

	t_widget.text.data.rect = rect_box;
	t_widget.text.data.is_editable = true;

	return t_widget;
}

instant bool
Widget_HasFocusInclSub(
	Widget *widget
) {
	Assert(widget);

	bool result = false;

	result = widget->data.has_focus;
	if (result)  return result;

	FOR_ARRAY(widget->a_subwidgets, it_sub) {
		Widget *t_subwidget = &ARRAY_IT(widget->a_subwidgets, it_sub);

		result = Widget_HasFocusInclSub(t_subwidget);
		if (result)  return result;
	}

	return result;
}

instant void
Widget_TriggerPopout(
	Widget *widget,
	bool can_focus_change
) {
	Assert(widget);

	widget->data.can_popout_focus_change = can_focus_change;
	widget->trigger_popout = true;
}

instant void
Widget_UpdateInputComboBox(
	Widget *widget_parent_io,
	u64 sub_index
) {
	Assert(widget_parent_io);

	Widget *widget = &ARRAY_IT(widget_parent_io->a_subwidgets, sub_index);
	Widget_UpdateInput(widget);

	Widget *wg_text   = &ARRAY_IT(widget_parent_io->a_subwidgets, WIDGET_COMBOBOX_TEXT);
	Widget *wg_button = &ARRAY_IT(widget_parent_io->a_subwidgets, WIDGET_COMBOBOX_TOGGLE);
	Widget *wg_list   = &ARRAY_IT(widget_parent_io->a_subwidgets, WIDGET_COMBOBOX_LIST);

	Assert(wg_list->window);

	Keyboard *keyboard = wg_list->window->keyboard;
	Mouse    *mouse    = wg_list->window->mouse;

	switch (sub_index) {
		case WIDGET_COMBOBOX_TEXT: {
			if (widget->data.has_focus) {
				if (keyboard->up[VK_F4]) {
					Widget_TriggerPopout(wg_list, false);
				}

				if (keyboard->up[VK_DOWN]) {
					Widget_SetFocus(wg_list);
				}
			}
		} break;

		case WIDGET_COMBOBOX_TOGGLE: {
			if (widget->events.on_trigger) {
				Widget_TriggerPopout(wg_list, true);
			}
		} break;

		case WIDGET_COMBOBOX_LIST: {
			bool hide_popout = false;
			bool update_label = false;

			if (wg_list->data.has_focus) {
				if (widget_parent_io->type_select == WIDGET_SELECT_ON_INDEX_CHANGE) {
					update_label = true;
				}

				if (    widget_parent_io->type_select == WIDGET_SELECT_ON_RETURN
					AND keyboard->up[VK_RETURN]
				) {
					update_label = true;
				}

				if (keyboard->up[VK_RETURN]) {
					hide_popout = true;

					Keyboard_ResetKey(keyboard, VK_RETURN);
				}

				if (keyboard->up[VK_ESCAPE]) {
					hide_popout = true;

					Keyboard_ResetKey(keyboard, VK_ESCAPE);
				}

				if (widget->events.on_trigger) {
					if (!wg_list->events.on_list_change_index) {
						update_label = true;
						hide_popout = true;
					}
				}

				if (update_label) {
					static String s_row_data;
					Widget_GetSelectedRowBuffer(wg_list, &s_row_data);

					if (!String_IsEqual(&wg_text->text.s_data, s_row_data.value, s_row_data.length)) {
						String_Clear(&wg_text->text.s_data);
						String_Append(&wg_text->text.s_data, s_row_data.value, s_row_data.length);
						wg_text->events.on_text_change = true;

						wg_text->text.cursor.data.index_select_end   = s_row_data.length;
						wg_text->text.cursor.data.index_select_start = 0;

						/// if this is not set, flushing cursor data after cursor was updated
						/// will set both cursor indexes to the same value.
						/// due to that, it won't be possible to check of something was prev.
						/// selected, even if the selection is still visible (because flushing
						/// is happening after selection process of done)
						wg_text->text.cursor.is_selecting = true;
					}
				}
			}
			else {
				/// so text can be typed into the combobox
				/// while the list is visible
				if (!Widget_HasFocusInclSub(widget_parent_io)) {
					hide_popout = true;
				}
			}

			if (hide_popout) {
				if (wg_list->data.is_popout) {
					widget->events.on_list_change_final = true;

					Widget_TriggerPopout(widget, true);
				}
			}
		} break;
	}
}

instant Widget
Widget_CreateComboBox(
	Window *window,
	Font *font,
	Rect rect_box,
	s32 combo_height
) {
	Widget t_widget = {};

	if (!rect_box.w) {
		rect_box.w = font->size;
		Widget_AddBorderSizes(&t_widget, &rect_box.w, 0);
	}

	if (!rect_box.h) {
		rect_box.h = Font_GetLineHeight(font);
		Widget_AddBorderSizes(&t_widget, 0, &rect_box.h);
	}

	t_widget.type         = WIDGET_COMBOBOX;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.data.is_focusable = false;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_width = true;

	Widget wg_text   = Widget_CreateTextBox(window, font, {});
	Widget wg_button = Widget_CreateButton( window, font, {}, "+");
	Widget wg_list   = Widget_CreateListbox(window, font, {0, 0, 0, combo_height});

	wg_text.UpdateCustomInputs   = Widget_UpdateInputComboBox;
	wg_button.UpdateCustomInputs = Widget_UpdateInputComboBox;
	wg_list.UpdateCustomInputs   = Widget_UpdateInputComboBox;

	wg_list.data.is_floating = true;
	wg_list.data.color_background = Color_MakeGrey(0.8f);

	wg_text.text.data.rect_padding  = {2, 1, 2, 1};
	wg_text.text.data.use_no_linebreak = true;

	wg_button.text.data.rect_margin  = {};
	wg_button.text.data.rect_padding = {3, -1, 3, -1};
	wg_button.data.border_size = 2;

	Widget *twg_text = Array_Add(&t_widget.a_subwidgets, wg_text);
	Assert(t_widget.a_subwidgets.count - 1 == WIDGET_COMBOBOX_TEXT);

	Widget *twg_button = Array_Add(&t_widget.a_subwidgets, wg_button);
	Assert(t_widget.a_subwidgets.count - 1 == WIDGET_COMBOBOX_TOGGLE);

	Widget *twg_list   = Array_Add(&t_widget.a_subwidgets, wg_list);
	Assert(t_widget.a_subwidgets.count - 1 == WIDGET_COMBOBOX_LIST);

	twg_list->widget_focus_on_popout = twg_text;

	return t_widget;
}

instant Widget
Widget_CreateProgressbar(
	Window *window,
	Font *font,
	Rect rect_box,
	Widget_Slide slide
) {
	Widget t_widget = {};

	if (!rect_box.w) {
		rect_box.w = font->size;
		Widget_AddBorderSizes(&t_widget, &rect_box.w, 0);
	}

	if (!rect_box.h) {
		rect_box.h = Font_GetLineHeight(font) + 2;
		Widget_AddBorderSizes(&t_widget, 0, &rect_box.h);
	}

	t_widget.type         = WIDGET_PROGRESSBAR;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;
	t_widget.slide        = slide;

	t_widget.data.is_focusable = false;

	t_widget.text.font         = font;
	t_widget.text.data.align_x = TEXT_ALIGN_X_MIDDLE;
	t_widget.text.data.align_y = TEXT_ALIGN_Y_CENTER;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_width = true;

	return t_widget;
}
