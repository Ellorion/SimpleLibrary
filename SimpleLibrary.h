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
//	String s_file_image;
//	String_Append(&s_file_image, "test/32_bit_(rgba)_bitmap.bmp");
//
//	Image image;
//	if (!Image_LoadBMP32(&image, &s_file_image)) {
//		AssertMessage(0, "Image not found or failed to load.");
//	}
//
//	String_Destroy(&s_file_image);
//
//	Texture texture;
//	Texture_Load(&texture, image.data, image.width, image.height, false, false);
//	Image_Destroy(&image);
//
//	ShaderSet shader_set;
//	ShaderSet_Load(&shader_set, &shader_texture, window);
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
//	ShaderSet shader_set;
//	shader_set.window = window;
//
//	Keyboard *keyboard = window->keyboard;
//
//	String s_file;
//	String_Append(&s_file, "test.txt");
//
//	String s_data = File_ReadAll(&s_file);
//	String_Destroy(&s_file);
//
//	String_Replace(&s_data, "\r\n", "\n");
//
//	Text text = Text_Create(&shader_set, &font, &s_data, {10, 10, window->width - 20, 0});
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
//	ShaderSet shader_set;
//	shader_set.window = window;
//
//	OpenGL_UseBlending(true);
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
//		///@Performance: should be used once at the beginning, when rendering
//		///              is done in batches, or constant recompiling
//		///              the same shader will reduce the performance loss
//		ShaderSet_Load(&shader_set, &shader_rect, shader_set.window);
//		Rect_Render(&shader_set, &vertex_rect);
//
//		ShaderSet_Load(&shader_set, &shader_text, shader_set.window);
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
//	ShaderSet shader_set;
//	shader_set.window = window;
//
//	OpenGL_UseBlending(true);
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
//		Widget_UpdateFocus(&ap_widgets, keyboard->pressing[VK_SHIFT], 0);
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
//	OpenGL_UseBlending(true);
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
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
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
	float *scale_x = 0,
	float *scale_y = 0,
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

		if (off_x)  *off_x = floor((*width_dst - offset) / 2);
		if (scale_x)  *scale_x = ratio;
		if (scale_y)  *scale_y = 1;

		*width_dst = floor(offset);
    }
    else if (aspect_dst < aspect_src) {
		float ratio = (aspect_src / aspect_dst);
		float offset = *height_dst / ratio;

		if (off_y)  *off_y = floor((*height_dst - offset) / 2);
		if (scale_x)  *scale_x = 1;
		if (scale_y)  *scale_y = ratio;

		*height_dst = floor(offset);
    }

    if (scale_to_dest) {
		if (scale_x)  *scale_x = (float)*width_dst  / width_src;
		if (scale_y)  *scale_y = (float)*height_dst / height_src;
    }
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
	bool reset_elapsed = true
) {
	Assert(timer);

	bool result = true;

	if (Time_Get() - timer->lo_timer < interval_in_ms)
		result = false;
	else {
		/// update timer when time has passed
		if (reset_elapsed)
			timer->lo_timer = Time_Get();
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
	u64   length = 0;
	char *value  = 0;
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
	if (s_data AND length < s_data->length)  s_data->length = length;
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
}

instant void
String_TrimLeft(
	String *s_data,
	bool move_pointer = true
) {
	Assert(s_data);

	u64 length = 0;
	u64 len_max = s_data->length;

	/// does not skip '\0'
    while(length <= len_max) {
		if (s_data->value[length] <= 32 AND s_data->value[length] != 127) ++length;
		else												   break;
    }

    if (move_pointer) {
		s_data->value  += length;
		s_data->length -= length;
    }
}

instant void
String_TrimRight(
	String *s_data
) {
	Assert(s_data);

	u64 length = s_data->length - 1;

    while(length > 0) {
		if (s_data->value[length] <= 32 AND s_data->value[length] != 127)
			--length;
		else
			break;
    }

    s_data->length = length + 1;
}

instant u64
String_Remove(
	String *s_data,
	u64 index_start,
	u64 index_end
) {
	Assert(s_data);

	if (index_start == index_end)    return 0;

	if (index_start > index_end)
		Swap(&index_start, &index_end);

	if (index_end > s_data->length)
		index_end = s_data->length;

	u64 length = s_data->length - index_end;
	u64 rm_count = (index_end - index_start);

	Memory_Copy(s_data->value + index_start, s_data->value + index_end, length);
	s_data->length -= rm_count;

	return rm_count;
}

instant void
String_Clear(
	String *s_data
) {
	Assert(s_data);

	s_data->length = 0;
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

	return length;
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
File_Exists(
	String *s_pathfile
) {
	Assert(s_pathfile);

	WIN32_FIND_DATA file_data;

	char *c_search_file = String_CreateCBufferCopy(s_pathfile);

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

	bool result = File_Exists(&ts_filename);

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
	String *s_filename
) {
	Assert(file_watcher);

	*file_watcher = {};

	char *c_filename = String_CreateCBufferCopy(s_filename);

	file_watcher->file = CreateFile(
			c_filename,
			0,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL
		);

	if (file_watcher->file)
		file_watcher->exists = true;

	Memory_Free(c_filename);
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

/// does not list or includes subdirectories
instant void
File_ReadDirectory(
	Array<String> *as_files,
	const char *c_path,
	u64 c_length = 0,
	bool prefix_path = true,
	const char *extension_filter = 0,
	const char *name_filter = 0
) {
	Assert(as_files);

	Array_ClearContainer(as_files);

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

			if (is_directory)
				continue;

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
				Array_Add(as_files, ts_filename);
			}

			String_Destroy(&s_filename);
		} while (FindNextFile(id_directory, &file_data));

		FindClose(id_directory);
	}

	String_Destroy(&s_extension_filter);

	Memory_Free(c_search_path);
	String_Destroy(&s_search_path);
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
instant void Keyboard_Reset(Keyboard *keyboard);

instant void
Window_Update(
	Window *window
) {
	Assert(window);

	SwapBuffers(window->hDC);

	Mouse_Reset(window->mouse);
	Keyboard_Reset(window->keyboard);
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
OpenGL_UseBlending(
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
};

/// 32-bit BMP only!
instant bool
Image_LoadBMP32(
	Image  *image,
	String *s_filename
) {
	Assert(image);
	Assert(!image->data);
	Assert(s_filename);

	bool result = false;

	*image = {};

	if (!File_Exists(s_filename)) {
		return result;
	}

	File file = File_Open(s_filename->value, "rb", s_filename->length);

	String s_data = File_Read(&file);

	File_Close(&file);

    String s_data_it = s_data;

    if (String_StartWith(&s_data_it, "BM")) {
		BITMAPFILEHEADER *bmp_header = (BITMAPFILEHEADER *)s_data_it.value;
		s_data_it.value  += sizeof(BITMAPFILEHEADER);
		s_data_it.length -= sizeof(BITMAPFILEHEADER);
		BITMAPINFOHEADER *bmp_info = (BITMAPINFOHEADER *)s_data_it.value;

		///@Info: make sure you free the memory after usage!
		image->data   = Memory_Create(u8, bmp_info->biSizeImage);
		image->width  = bmp_info->biWidth;
		image->height = bmp_info->biHeight;
		image->bits   = bmp_info->biBitCount / 8;

		s_data_it = s_data;
		s_data_it.value  += bmp_header->bfOffBits;
		s_data_it.length -= bmp_header->bfOffBits;
		Memory_Copy(image->data, s_data_it.value, bmp_info->biSizeImage);

		result = true;
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

instant void
Texture_Load(
	Texture *texture,
	const u8 *data,
	s32 width,
	s32 height,
	bool greyscale = false,
	bool linearFilter = false
) {
	Assert(texture);

	/// check for potential memory leak
	Assert(texture->ID == 0);

    u32 id_texture;

    glGenTextures(1, &id_texture);
    glBindTexture(GL_TEXTURE_2D, id_texture);

    if (greyscale) {
		/// useful for fonts
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(	GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0,
						GL_ALPHA, GL_UNSIGNED_BYTE, data);
    }
    else {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA , width, height, 0,
						GL_ABGR_EXT, GL_UNSIGNED_BYTE, data);
    }

    if (linearFilter) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);

    texture->ID = id_texture;
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
Texture_Create(
	Texture *texture,
	s32 width,
	s32 height
) {
	Texture_Load(texture, 0, width, height);
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
	SHADER_PROG_TEXTURE,
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

static const Shader shader_texture = {
	SHADER_PROG_TEXTURE,
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

/// ::: Shader
/// ===========================================================================
struct ShaderProgram {
	u32 id = 0;
	u32 vertex_id   = 0;
	u32 geometry_id = 0;
	u32 fragment_id = 0;
};

struct ShaderSet {
	Array<ShaderProgram> a_shaders;
	u64 active_id = 0;
	Window *window = 0;
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
	ShaderSet_Add(&t_shader_set, &shader_texture);

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
	Assert(shader_set->active_id < shader_set->a_shaders.count);
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
	Assert(shader_set->active_id < shader_set->a_shaders.count);
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
	Assert(shader_set->active_id < shader_set->a_shaders.count);
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
	Assert(shader_set->active_id < shader_set->a_shaders.count);
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
	Assert(shader_set->active_id < shader_set->a_shaders.count);
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

	u64 prev_active_id = shader_set->active_id;

	switch (type) {
		case SHADER_PROG_RECT:
		case SHADER_PROG_TEXTURE:
		case SHADER_PROG_TEXT: {
			shader_set->active_id = type;
		} break;

		default: {
			AssertMessage(	false,
							"Unhandled Shader-Program type.");
		} break;
	}

	if (prev_active_id == shader_set->active_id)
		return;

	Assert(shader_set->active_id < shader_set->a_shaders.count);
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
		OpenGL_UseBlending(true);
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
	bool flip = false;
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

	/// use this uniform in a shader, if the texture index has to change
	Shader_BindAndUseIndex0(shader_set, "fragment_texture", &vertex->texture);
}

instant Vertex
Vertex_Create(
) {
	Vertex vertex = {};

	glGenVertexArrays(1, &vertex.array_id);

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
		vertex.settings.flip = true;
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

	Assert(shader_set->active_id < shader_set->a_shaders.count);
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

	AssertMessage(	a_positions->id,
					"No Attributes found.\n    Forgot to bind the attributes?");

	Assert(a_positions->group_count);

	Shader_SetValue(shader_set, "flip_h" , vertex->settings.flip);
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

		Vertex_ClearAttributes(t_vertex);
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

	bool is_up;
	bool is_down;
};

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
			ShowCursor(true);
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

	mouse->is_up   = false;
	mouse->is_down = false;

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
	Keyboard *keyboard
) {
	if (!keyboard)
		return;

	u32 vkey = keyboard->last_key_virtual;

	if (keyboard->down[vkey]) {
		keyboard->down[vkey] 		= false;
		keyboard->is_down 			= false;
		keyboard->is_key_sym 		= false;
		keyboard->key_sym 			= 0;
		keyboard->key_scan			= 0;
		keyboard->last_key_virtual	= 0;
	}

	if (keyboard->up[vkey]) {
		keyboard->up[vkey] 			= false;
		keyboard->is_up 			= false;
		keyboard->is_key_sym 		= false;
		keyboard->key_scan			= 0;
		keyboard->key_sym 			= 0;
		keyboard->last_key_virtual	= 0;
	}
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

	keyboard->down[msg->wParam] 		= false;
	keyboard->up[msg->wParam] 			= true;
	keyboard->pressing[msg->wParam] 	= false;
	keyboard->repeating[msg->wParam] 	= false;

	Keyboard_GetKeySym(keyboard, msg);

	keyboard->last_key_virtual 	= msg->wParam;
	keyboard->is_down 			= false;
	keyboard->is_up				= true;
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

	u64 result = joypad->GetState(joy_index, &joypad->state);

	return (result != XINPUT_ERROR_DEVICE_NOT_CONNECTED);
}

///@Hint: test for != 0

/// DPad
#define Joypad_IsPadUp(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_DPAD_UP)
#define Joypad_IsPadDown(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_DPAD_DOWN)
#define Joypad_IsPadLeft(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_DPAD_LEFT)
#define Joypad_IsPadRight(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_DPAD_RIGHT)

/// Center buttons
#define Joypad_IsStart(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_START)
#define Joypad_IsSelect(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_BACK)

/// Thumb button
#define Joypad_IsThumbLeft(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_LEFT_THUMB)
#define Joypad_IsThumbRight(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_RIGHT_THUMB)

/// Shoulder buttons
#define Joypad_IsShoulderLeft(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_LEFT_SHOULDER)
#define Joypad_IsShoulderRight(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_RIGHT_SHOULDER)

/// Control buttons
#define Joypad_IsA(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_A)
#define Joypad_IsB(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_B)
#define Joypad_IsX(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_X)
#define Joypad_IsY(ptr_joypad) \
	((*ptr_joypad).state.Button & XINPUT_GAMEPAD_Y)

/// for axis
/// Returns: current section
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

		return axis_value / steps;
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

    Font font;
    font.s_data = File_ReadAll(s_file->value, s_file->length, true);
    font.size = size;

    if (!font.s_data.length) {
		LOG_ERROR("Font does not exists.");
    }

    const u8 *c_data = (u8 *)font.s_data.value;
    stbtt_InitFont(&font.info, c_data, stbtt_GetFontOffsetForIndex(c_data, 0));

	float scale = stbtt_ScaleForPixelHeight(&font.info, font.size);

	stbtt_GetFontVMetrics(&font.info, &font.ascent, &font.descent, &font.linegap);
	font.ascent  *= scale;
	font.descent *= scale;
	font.linegap *= scale;

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
		Texture_Load(&result, bitmap, w, h, true, font->filter_linear);
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
Codepoint_GetPosition(
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

struct Text {
	ShaderSet *shader_set = 0;
	Font *font = 0;
	String s_data = {};
	Rect rect = {};
	Rect rect_padding = {};
	Color32 color;
	TEXT_ALIGN_X_TYPE align_x = TEXT_ALIGN_X_LEFT;
	TEXT_ALIGN_Y_TYPE align_y = TEXT_ALIGN_Y_TOP;
	float x_offset = 0;
	float y_offset = 0;
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
	text.rect       = rect;
	text.align_x    = align_x;

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

struct Text_Line {
	u64 width_pixel;
	String s_data;
};

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
	Rect  rect = text->rect;

	Rect_AddPadding(&rect, text->rect_padding);

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
		if (rect.w AND (rect_line_current.x - rect.x) + advance_word >= rect.w) {
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
Text_AddLines(
	Array<Vertex> *a_vertex,
	ShaderSet *shader_set,
	Font *font,
	Rect rect,
	Rect rect_padding,
	Color32 color,
	Array<Text_Line> *a_text_lines,
	TEXT_ALIGN_X_TYPE align_x,
	float x_offset,
	float y_offset
) {
	Assert(shader_set);
	Assert(font);
	Assert(a_text_lines);

	Rect_AddPadding(&rect, rect_padding);

	u64 width_max = rect.w;
	RectF rect_position = {rect.x, 0, 0, rect.y};

	FOR_ARRAY(*a_text_lines, it_line) {
		Text_Line *t_text_line = &ARRAY_IT(*a_text_lines, it_line);

		u64 it_data = 0;

		while(it_data < t_text_line->s_data.length) {
			Codepoint codepoint;

			s8 ch = t_text_line->s_data.value[it_data];

			Codepoint_GetData(font, ch, &codepoint);
			Codepoint_GetPosition(&codepoint, &rect_position);

			/// for unavailable characters like ' '
			if (!Texture_IsEmpty(&codepoint.texture)) {
				Vertex *t_vertex;
				Vertex_FindOrAdd(a_vertex, &codepoint.texture, &t_vertex);

				u64 x_align_offset = 0;

				if (0) {}
				else if (align_x == TEXT_ALIGN_X_MIDDLE)
					x_align_offset = (width_max - t_text_line->width_pixel) >> 1;
				else if (align_x == TEXT_ALIGN_X_RIGHT)
					x_align_offset = (width_max - t_text_line->width_pixel);

				Vertex_Buffer<float> *t_attribute;

				Vertex_FindOrAddAttribute(t_vertex, 2, "vertex_position", &t_attribute);
				Array_Add(&t_attribute->a_buffer, x_offset + rect_position.x + x_align_offset);
				Array_Add(&t_attribute->a_buffer, y_offset + rect_position.y);

				Vertex_FindOrAddAttribute(t_vertex, 3, "text_color", &t_attribute);
				Array_Add(&t_attribute->a_buffer, color.r);
				Array_Add(&t_attribute->a_buffer, color.g);
				Array_Add(&t_attribute->a_buffer, color.b);
			}

			++it_data;
		}

		Codepoint_SetNewline(font, &rect_position, rect.x);
	}
}

instant void
Text_AddLines(
	Array<Vertex>    *a_vertex,
	Text *text,
	Array<Text_Line> *a_text_lines
) {
	Assert(text);

	Text_AddLines(	a_vertex,
					text->shader_set,
					text->font,
					text->rect,
					text->rect_padding,
					text->color,
					a_text_lines,
					text->align_x,
					text->x_offset,
					text->y_offset);
}

instant void
Text_Render(
	Text *text,
	Array<Vertex> *a_vertex
) {
	Assert(text);
	Assert(a_vertex);

	Vertex_Render(text->shader_set, a_vertex);
}

instant void
Text_Render(
	Text *text
) {
	Assert(text);

	static Array<String>    as_words;
	static Array<Text_Line> a_text_lines;
	static Array<Vertex>    a_vertex;

	OpenGL_Scissor(text->shader_set->window, text->rect);

	String_SplitWordsStatic(&text->s_data, &as_words);
	s32 text_height = Text_BuildLinesStatic(text, &as_words, &a_text_lines);

	if (0) {}
	else if (text->align_y == TEXT_ALIGN_Y_CENTER)
		text->y_offset = (text->rect.h - text_height) >> 1;
	else if (text->align_y == TEXT_ALIGN_Y_BOTTOM)
		text->y_offset = (text->rect.h - text_height);

	Text_AddLines(&a_vertex, text, &a_text_lines);

	Text_Render(text, &a_vertex);

	OpenGL_Scissor_Disable();
}

instant void
Text_GetSize(
	Text *text,
	s32 *width,
	s32 *height
) {
	Assert(text);

	static Array<String>    as_words;
	static Array<Text_Line> a_text_lines;

	if (height) {
		String_SplitWordsStatic(&text->s_data, &as_words);
		*height = Text_BuildLinesStatic(text, &as_words, &a_text_lines);
	}

	IF_SET(width)  = text->rect.w;
}

/// ::: Widget
/// ===========================================================================
enum WIDGET_TYPE {
	WIDGET_LABEL,
	WIDGET_BUTTON,
	WIDGET_LISTBOX
};

enum WIDGET_SCROLL_TYPE {
	WIDGET_SCROLL_ITEM,
	WIDGET_SCROLL_BLOCK
};

struct Widget_Settings {
	Color32 color_background       = Color_MakeGrey(0.9f);
	Color32 color_outline          = {0   , 0   , 0   , 1};
	Color32 color_outline_selected = {0.3f, 0.3f, 1   , 1};
	Color32 color_outline_inactive = {0.5f, 0.5f, 1   , 1};
	Color32 color_font             = {1   , 0   , 0   , 1};
	Color32 color_progress         = {0.2f, 0.2f, 0.6f, 1};

	u32  border_size   = 0;
	u32  spacing       = 1;
	bool is_focusable  = true;
	bool is_scrollable = false;

	WIDGET_SCROLL_TYPE scroll_type = WIDGET_SCROLL_ITEM;
};

struct Widget {
	WIDGET_TYPE type;
	Rect rect_box;
	Rect rect_content;
	Text text;
	Vertex vertex_rect;
	Window *window;

	bool has_focus;

	Array<String> as_row_data;
	u64 active_row_id = 0;

	Widget_Settings setting;
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
	Array_AddEmpty(&widget->as_row_data, &ts_data);
	String_Copy(ts_data, c_row_data, c_length);
}

instant bool
Mouse_IsHovering(
	Widget *widget
) {
	Assert(widget);

	Point t_point;
	Mouse_GetPosition(&t_point.x, &t_point.y, widget->window);

    return Rect_IsIntersecting(&t_point, &widget->rect_box);
}

instant bool
Mouse_IsHovering(
	Mouse *mouse,
	Widget *widget
) {
	Assert(mouse);
	Assert(widget);

	Mouse_GetPosition(mouse, widget->window);

    return Rect_IsIntersecting(&mouse->point, &widget->rect_box);
}

instant void
Widget_Redraw(
	Widget *widget
) {
	Assert(widget);

	switch (widget->type) {
		case WIDGET_LABEL: {
			Vertex *t_vertex = &widget->vertex_rect;
			Rect    rect_box =  widget->rect_box;

			if (!t_vertex->array_id)
				*t_vertex = Vertex_Create();
			else
				Vertex_ClearAttributes(t_vertex);

			Vertex_AddRect32(t_vertex, rect_box, widget->setting.color_background);

			widget->text.rect = rect_box;
		} break;

		case WIDGET_BUTTON: {
			Vertex *t_vertex = &widget->vertex_rect;
			Rect    rect_box =  widget->rect_box;

			if (!t_vertex->array_id)
				*t_vertex = Vertex_Create();
			else
				Vertex_ClearAttributes(t_vertex);

			Vertex_AddRect32(t_vertex, rect_box, widget->setting.color_background);

			if (widget->setting.border_size) {
				Rect_Resize(&rect_box, -1);

				if (widget->has_focus)
					Vertex_AddRect32(t_vertex, rect_box, widget->setting.color_outline_selected);
				else
					Vertex_AddRect32(t_vertex, rect_box, widget->setting.color_outline);

				Rect_Resize(&rect_box, -widget->setting.border_size);
				Vertex_AddRect32(t_vertex, rect_box, widget->setting.color_background);
			}

			widget->text.rect = rect_box;
		} break;

		case WIDGET_LISTBOX: {
			Vertex *t_vertex = &widget->vertex_rect;
			Rect    rect_box =  widget->rect_box;

			if (!t_vertex->array_id)
				*t_vertex = Vertex_Create();
			else
				Vertex_ClearAttributes(t_vertex);

			Rect t_rect_box = rect_box;
			t_rect_box.w += 5;

			Vertex_AddRect32(t_vertex, t_rect_box, widget->setting.color_background);
		} break;

		default:
			AssertMessage(	false,
							"Unhandled widget background drawing.");
	}
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

	t_widget.type         = WIDGET_LABEL;
	t_widget.rect_box     = rect_box;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.setting.is_focusable = false;

	t_widget.text.align_x    = text_align_x;
	t_widget.text.rect       = t_widget.rect_box;
	t_widget.text.font       = font;
	t_widget.text.color      = t_widget.setting.color_font;

	String_Append(&t_widget.text.s_data, c_data, c_length);

	Widget_Redraw(&t_widget);

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

	t_widget.setting.border_size = 2;
	t_widget.setting.color_outline = {0.2f, 0.2f, 1.0f};
	t_widget.setting.color_outline_selected = {0.5f, 0.5f, 0.0f};

	t_widget.type         = WIDGET_BUTTON;
	t_widget.rect_box     = rect_box;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.text.align_x    = TEXT_ALIGN_X_MIDDLE;
	t_widget.text.align_y    = TEXT_ALIGN_Y_CENTER;
	t_widget.text.rect       = t_widget.rect_box;
	t_widget.text.font       = font;
	t_widget.text.color      = t_widget.setting.color_font;

	String_Append(&t_widget.text.s_data, c_data, c_length);

	Widget_Redraw(&t_widget);

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

	t_widget.setting.color_outline = {0.8f, 0.8f, 0.88f};

	t_widget.type         = WIDGET_LISTBOX;
	t_widget.rect_box     = rect_box;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.setting.is_focusable = true;
	t_widget.setting.is_scrollable = true;

	t_widget.text.rect  = t_widget.rect_box;
	t_widget.text.font  = font;
	t_widget.text.color = t_widget.setting.color_font;

	Widget_Redraw(&t_widget);

	return t_widget;
}

instant void
Widget_GetTextOffset(
	Widget *widget,
	Point *point
) {
	Assert(widget);
	Assert(point);

	*point = {widget->text.x_offset, widget->text.y_offset};
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
	if (!widget->as_row_data.count) {
		ShaderSet_Use(shader_set, SHADER_PROG_RECT);
		Rect_Render(shader_set, &widget->vertex_rect);

		ShaderSet_Use(shader_set, SHADER_PROG_TEXT);
		Text_Render(&widget->text);
		return;
	}

	static Array<Vertex> a_vertex;
	static Array<String> as_words;
	static Array<Text_Line> a_text_lines;
	Rect rect_row;

	Point pt_offset;
	Widget_GetTextOffset(widget, &pt_offset);

	OpenGL_Scissor(shader_set->window, widget->rect_box);

	widget->rect_content   = widget->rect_box;
	widget->rect_content.h = 0;

	/// render item background(s)
	/// =======================================================================
	rect_row = widget->rect_box;
	rect_row.x += pt_offset.x;
	rect_row.y += pt_offset.y;
	rect_row.h  = 0;

	Vertex_ClearAttributes(&widget->vertex_rect);
	Widget_Redraw(widget);

	FOR_ARRAY(widget->as_row_data, it_row) {
		String *ts_data = &ARRAY_IT(widget->as_row_data, it_row);

        String_SplitWordsStatic(ts_data, &as_words);

        widget->text.rect = rect_row;
        rect_row.h = Text_BuildLinesStatic(&widget->text, &as_words, &a_text_lines);

		if (Rect_IsIntersecting(&rect_row, &widget->rect_box)) {
			Color32 t_color_rect = widget->setting.color_outline;

			if (widget->active_row_id == it_row) {
				if (widget->has_focus)
					t_color_rect = widget->setting.color_outline_selected;
				else
					t_color_rect = widget->setting.color_outline_inactive;
			}

			Vertex_AddRect32(&widget->vertex_rect, rect_row, t_color_rect);

			/// store item data, so the data is drawn upon the background
			/// and as batch rendering
			widget->text.rect = rect_row;
			widget->text.rect.x -= pt_offset.x;
			widget->text.rect.y -= pt_offset.y;

			Text_AddLines(&a_vertex, &widget->text, &a_text_lines);
		}

		s32 height_row_step = rect_row.h + widget->setting.spacing;

		rect_row.y             += height_row_step;
		widget->rect_content.h += height_row_step;
	}

	ShaderSet_Use(shader_set, SHADER_PROG_RECT);
	Rect_Render(shader_set, &widget->vertex_rect);

	/// render item data
	/// =======================================================================
	ShaderSet_Use(shader_set, SHADER_PROG_TEXT);
	Text_Render(&widget->text, &a_vertex);

	OpenGL_Scissor_Disable();
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
		if (widget->has_focus)
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

instant void
_Widget_UpdateFocusForward(
	Array<Widget *> *ap_widgets
) {
	Assert(ap_widgets);

	bool focus_set_next = false;

	Widget *t_widget_focus_check = 0;

	if (!ap_widgets->count)
		return;

	FOR_ARRAY(*ap_widgets, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets, it_widget);

		if (!t_widget->setting.is_focusable)
			continue;

		Keyboard *keyboard = t_widget->window->keyboard;
		Mouse    *mouse    = t_widget->window->mouse;

		if (focus_set_next) {
			t_widget->has_focus = true;
			focus_set_next = false;

			Widget_Redraw(t_widget);
		}
		else {
			if (IF_USE(keyboard).up[VK_TAB]) {
				if (t_widget->has_focus) {
					t_widget->has_focus = false;
					focus_set_next = true;

					Widget_Redraw(t_widget);
				}
			}
		}

		if (t_widget->has_focus)
			t_widget_focus_check = t_widget;
	}

	/// search for next focus from the beginning,
	/// if there was no focusable left past the last focused
	/// widget
	if (focus_set_next OR !t_widget_focus_check) {
		FOR_ARRAY(*ap_widgets, it_widget) {
			Widget *t_widget = ARRAY_IT(*ap_widgets, it_widget);

			if (t_widget->setting.is_focusable) {
				t_widget->has_focus = true;

				Widget_Redraw(t_widget);

				break;
			}
		}
	}
}

///@Hint: copy of forward function with reversed loop
instant void
_Widget_UpdateFocusBackward(
	Array<Widget *> *ap_widgets
) {
	Assert(ap_widgets);

	bool focus_set_prev = false;

	Widget *t_widget_focus_check = 0;

	if (!ap_widgets->count)
		return;

	FOR_ARRAY_REV(*ap_widgets, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets, it_widget);

		if (!t_widget->setting.is_focusable)
			continue;

		Keyboard *keyboard = t_widget->window->keyboard;
		Mouse    *mouse    = t_widget->window->mouse;

		if (focus_set_prev) {
			t_widget->has_focus = true;
			focus_set_prev = false;

			Widget_Redraw(t_widget);
		}
		else {
			if (IF_USE(keyboard).up[VK_TAB]) {
				if (t_widget->has_focus) {
					t_widget->has_focus = false;
					focus_set_prev = true;

					Widget_Redraw(t_widget);
				}
			}
		}

		if (t_widget->has_focus)
			t_widget_focus_check = t_widget;
	}

	if (focus_set_prev OR !t_widget_focus_check) {
		FOR_ARRAY_REV(*ap_widgets, it_widget) {
			Widget *t_widget = ARRAY_IT(*ap_widgets, it_widget);

			if (t_widget->setting.is_focusable) {
				t_widget->has_focus = true;

				Widget_Redraw(t_widget);

				break;
			}
		}
	}
}

instant void
Widget_UpdateFocus(
	Array<Widget *> *ap_widgets,
	bool reverse
) {
	if (!reverse)
		_Widget_UpdateFocusForward(ap_widgets);
	else
		_Widget_UpdateFocusBackward(ap_widgets);
}

instant void
Widget_ClampTextOffset(
	Widget *widget
) {
	Assert(widget);

	/// do not show negative space below the last visible
	if (widget->text.y_offset + widget->rect_content.h < widget->rect_box.h)
		widget->text.y_offset = widget->rect_box.h - widget->rect_content.h;

	/// do not show negative space above the first visible
	if (widget->text.y_offset > 0)
		widget->text.y_offset = 0;

	/// avoid scrolling, when all content is visible
	if (widget->rect_content.h <= widget->rect_box.h)
		widget->text.y_offset = 0;
}

instant void
Widget_GetActiveRowRect(
	Widget *widget,
	Rect *rect_row
) {
	Assert(widget);
    Assert(rect_row);

    *rect_row = {};

    String ts_data_backup = widget->text.s_data;

	Rect rect_item  = widget->rect_box;
	rect_item.x    += widget->text.x_offset;
	rect_item.y    += widget->text.y_offset;

    FOR_ARRAY(widget->as_row_data, it_row) {
		String *ts_data = &ARRAY_IT(widget->as_row_data, it_row);
		widget->text.s_data = *ts_data;

		s32 width, height;
		Text_GetSize(&widget->text, &width, &height);

		rect_item.w = width;
		rect_item.h = height;

		if (widget->active_row_id == it_row) {
			*rect_row = rect_item;
			break;
		}

		rect_item.y += rect_item.h + widget->setting.spacing;
    }

    widget->text.s_data = ts_data_backup;
}

instant u64
Widget_GetActiveRowID(
	Widget *widget,
	Mouse *mouse
) {
    Assert(widget);
    Assert(mouse);

    if (!Mouse_IsHovering(mouse, widget))
		return widget->active_row_id;

    u64 active_row_id = 0;

    String ts_data_backup = widget->text.s_data;

	Rect rect_item  = widget->rect_box;
	rect_item.x    += widget->text.x_offset;
	rect_item.y    += widget->text.y_offset;

    FOR_ARRAY(widget->as_row_data, it_row) {
		String *ts_data = &ARRAY_IT(widget->as_row_data, it_row);
		widget->text.s_data = *ts_data;

		s32 width, height;
		Text_GetSize(&widget->text, &width, &height);

		rect_item.w = width;
		rect_item.h = height;

		if (Rect_IsIntersecting(&mouse->point , &rect_item)) {
			active_row_id = it_row;
			break;
		}

		rect_item.y += rect_item.h + widget->setting.spacing;
    }

    widget->text.s_data = ts_data_backup;

    return active_row_id;
}

instant void
Widget_UpdateInput(
	Widget *widget
) {
	Assert(widget);

    Keyboard *keyboard = widget->window->keyboard;
    Mouse    *mouse    = widget->window->mouse;

    if (!widget->setting.is_focusable)
		return;

	bool got_focus = widget->has_focus;
	u64  prev_active_row = widget->active_row_id;

	bool is_scrollable = widget->setting.is_scrollable;

    if (mouse) {
		bool is_hovering = Mouse_IsHovering(mouse, widget);

		if (mouse->up[0]) {
			got_focus = is_hovering;

			if (is_scrollable)
				widget->active_row_id = Widget_GetActiveRowID(widget, mouse);
		}

		if (is_hovering AND is_scrollable) {
			widget->text.y_offset += mouse->wheel;
			Widget_ClampTextOffset(widget);
		}
    }

    if (keyboard AND widget->has_focus) {
		if (is_scrollable) {
			if (widget->active_row_id) {
				if (keyboard->down[VK_UP]) {
					--widget->active_row_id;
				}

				if (keyboard->down[VK_HOME]) {
					widget->active_row_id = 0;
				}
			}

			if (widget->active_row_id < widget->as_row_data.count - 1) {
				if (keyboard->down[VK_DOWN]) {
					++widget->active_row_id;
				}

				if (keyboard->down[VK_END]) {
					widget->active_row_id = widget->as_row_data.count - 1;
				}
			}
		}
    }

    if (prev_active_row != widget->active_row_id AND is_scrollable) {
		Rect rect_active_row;
        Widget_GetActiveRowRect(widget, &rect_active_row);

		if (!Rect_IsVisibleFully(&rect_active_row, &widget->rect_box)) {
			if (widget->setting.scroll_type == WIDGET_SCROLL_ITEM) {
				if (widget->active_row_id < prev_active_row) {
					widget->text.y_offset -= (rect_active_row.y - widget->rect_box.y);
				}
				else {
					widget->text.y_offset -= (rect_active_row.y - widget->rect_box.y);
					widget->text.y_offset += (widget->rect_box.h - rect_active_row.h);
				}
			}
			else if (widget->setting.scroll_type == WIDGET_SCROLL_BLOCK) {
				if (!Rect_IsVisibleFully(&rect_active_row, &widget->rect_box)) {
					if (widget->active_row_id < prev_active_row) {
						widget->text.y_offset -= (rect_active_row.y - widget->rect_box.y);
						widget->text.y_offset += (widget->rect_box.h - rect_active_row.h);
					}
					else {
						widget->text.y_offset -= (rect_active_row.y - widget->rect_box.y);
					}
				}
			}
			else {
				AssertMessage(	false,
								"Unhandled widget scroll type.");
			}
		}

        Widget_ClampTextOffset(widget);
    }

	if (widget->has_focus != got_focus) {
		widget->has_focus = got_focus;
		Widget_Redraw(widget);
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

	if (s_row_data) {
		Assert(!widget->active_row_id OR widget->active_row_id < widget->as_row_data.count);
		String *ts_row_data = &ARRAY_IT(widget->as_row_data, widget->active_row_id);

		String_Clear(s_row_data);
		String_Append(s_row_data, ts_row_data->value, ts_row_data->length);
	}
}

