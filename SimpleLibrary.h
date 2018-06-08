#pragma once

/// Compiler: g++
///
/// Linker flags:
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
///           a parameter) and will not be used as return value
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
//	String_Append(&s_file_image, "32_bit_(rgba)_bitmap.bmp");
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
//	Vertex vertex;
//	Vertex_Create(&shader_set, &vertex, &texture, {});
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
//	/// will destroy the linked texture
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
//	Window_Create(&window, "Hello, World!", 800, 480, 32, &mouse);
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

#define SetIfValid(_element)	\
	if (&(_element)) (_element)

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

struct Point {
	float x = 0.0f;
	float y = 0.0f;
};

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

/// ::: Memory
/// ===========================================================================
#define Memory_Create(type, length) (type *)_Memory_Alloc_Empty(sizeof(type) * length)
#define Memory_Resize(buffer, type, length) (buffer ? (type *) _Memory_Resize(buffer, sizeof(type) * length) : (type *) Memory_Create(type, length));

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

instant void
String_Append(
	String *s_data,
	const char *c_data,
	u64 length_append = 0
) {
    Assert(s_data);

    if (length_append == 0)  length_append = String_Length(c_data);
    if (length_append == 0)  return;

	String_Resize(s_data, length_append);
	Memory_Copy(s_data->value + s_data->length, (char *)c_data, length_append);
	s_data->length += length_append;
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
	Assert(s_data->length);

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


/// :::Array
/// ===========================================================================
#define ARRAY_IT(_array, _it) \
	((_array).memory)[_it]

#define FOR_ARRAY(_array, _it) 		\
	for(u64 _it = 0;        		\
	_it < (_array).count;       	\
	++_it)

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
instant void
Array_AddEmpty(
	Array<T> *array,
	T **element_empty
) {
	Assert(array);
	Assert(element_empty);

	T t_element_empty = {};
	Array_Add(array, t_element_empty);
	*element_empty = &ARRAY_IT(*array, array->count - 1);
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
Array_Clear(
	Array<T> *array,
	bool use_generic = false
) {
	Assert(array);

	array->size = 0;
	array->count = 0;
}

template<typename T>
instant void
Array_Destroy(
	Array<T> *array,
	bool use_generic = false
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
			SetIfValid(*index) = it;
			return true;
		}
	}

	return false;
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

template<typename T>
instant void
Array_Destroy(
	Array<String> *array
) {
	Assert(array);

 	while(array->count) {
		String s_data_it = Array_Remove(array, 0);
		String_Destroy(&s_data_it);
    }
    Array_Destroy(array);
}

/// Will copy string values, so array content has to be free'd
instant Array<String>
Array_Split(
	String *s_data,
	const char *delimiter,
	bool exclude_delim = true
) {
	Assert(s_data);

	Array<String> as_result;

	String s_data_it = *s_data;
	u64 len_delim = String_Length(delimiter);

	s64 pos_found;
	while(String_Find(&s_data_it, delimiter, &pos_found, 0, false)) {
		String s_element;
		String_Append(&s_element, s_data_it.value, pos_found);
		Array_Add(&as_result, s_element);
		s_data_it.value += pos_found + len_delim;
		s_data_it.length   -= pos_found + len_delim;
	}

	if (s_data_it.length > 0) {
		String s_element;
		String_Append(&s_element, s_data_it.value, s_data_it.length);
		Array_Add(&as_result, s_element);
	}

	return as_result;
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

instant void
File_Open(
	File *file,
	String *s_filename,
	const char *mode
) {
	Assert(file);
	Assert(s_filename);
	Assert(mode);

	*file = {};

	if (s_filename->length) {
		char *c_filename = String_CreateCBufferCopy(s_filename);

		file->fp = fopen(c_filename, mode);

		Memory_Free(c_filename);
	}
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

instant void
File_Read(
	String *s_data,
	File *file
) {
	Assert(s_data);

	u64 length = File_Size(file);

	s_data->value  = Memory_Resize(s_data->value, char, length);
	s_data->length = length;
	fread(s_data->value, sizeof(char), sizeof(char) * length, file->fp);
}


struct File_Watcher {
	String *s_filename = 0;
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

	Array_Clear(as_files);

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
		if (Mouse_Update(_ptr_window->mouse, _ptr_window, &_msg))	continue;	\
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

	RECT windowRect = {};
	GetClientRect(GetDesktopWindow(), &windowRect);

	/// get desktop window center
	/// -------------------------------------
	x = (windowRect.right  - width)  >> 1;
	y = (windowRect.bottom - height) >> 1;

	windowRect.right  = width;
	windowRect.bottom = height;

	u32 dwExStyle = WS_EX_APPWINDOW;
	u32 dwStyle   = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (window->isFullscreen) {
		dwStyle  |= WS_POPUP;
	}
	else {
		dwExStyle |= WS_EX_WINDOWEDGE;
		dwStyle   |= WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&windowRect, dwStyle, false, dwExStyle);

	/// create window
	/// -------------------------------------
	HWND hWnd = CreateWindowEx(	dwExStyle,
								class_name, title,
								dwStyle,
								x, y,
								windowRect.right - windowRect.left,
								windowRect.bottom - windowRect.top,
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

	window->title = title;
	window->width = width;
	window->height = height;

	if (mouse)
		window->mouse = mouse;

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

instant void
Window_Update(
	Window *window
) {
	Assert(window);

	SwapBuffers(window->hDC);

	Mouse_Reset(window->mouse);
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

	RECT rctWindow;
	GetClientRect(window->hWnd, &rctWindow);
	s32 new_width  = rctWindow.right  - rctWindow.left;
	s32 new_height = rctWindow.bottom - rctWindow.top;

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
OpenGL_Scissor_Disable() {
	glDisable(GL_SCISSOR_TEST);
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

	File file;
	File_Open(&file, s_filename, "rb");

	String s_data;
	File_Read(&s_data, &file);

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

struct Shader {
	const char *code[3];
};

static const Shader shader_texture = {
R"(
	#version 330 core

	uniform vec4 viewport = vec4(0, 0, 800, 480);
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

	out Vertex_Data {
		mat4 proj_matrix;
	} o_Vertex;

	void main() {
		gl_Position = vertex_position;
		o_Vertex.proj_matrix = proj_matrix;
	}
)",

R"(
	#version 330 core

	layout(points) in;
	layout(triangle_strip, max_vertices = 4) out;

	uniform sampler2D fragment_texture;
	vec2 size = textureSize(fragment_texture, 0);

	uniform float scale_x = 1.0f;
	uniform float scale_y = 1.0f;
	uniform bool  flip_h = false;

	in Vertex_Data {
		mat4 proj_matrix;
	} i_Vertex[];

	out Vertex_Data {
		vec2 tex_coords;
		vec4 position;
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
struct ShaderSet {
	u32 program_id    = 0;
	u32 vertex_id     = 0;
	u32 geometry_id   = 0;
	u32 fragment_id   = 0;
	Window *window = 0;
};

instant void
ShaderSet_Add(
	ShaderSet *shader_set,
	u32 type,
	const char *code
) {
	Assert(shader_set);

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
		case GL_VERTEX_SHADER:   { shader_set->vertex_id   = id_shader; } break;
		case GL_GEOMETRY_SHADER: { shader_set->geometry_id = id_shader; } break;
		case GL_FRAGMENT_SHADER: { shader_set->fragment_id = id_shader; } break;
		default: {
			LOG_DEBUG("Unimplemented shader type");
		}
	}
}

inline void
ShaderSet_Use(
	ShaderSet *shader_set,
	const Shader *shader
) {
	Assert(shader_set);
	Assert(shader);

	if (shader->code[0])  ShaderSet_Add(shader_set, GL_VERTEX_SHADER  , shader->code[0]);
	if (shader->code[1])  ShaderSet_Add(shader_set, GL_GEOMETRY_SHADER, shader->code[1]);
	if (shader->code[2])  ShaderSet_Add(shader_set, GL_FRAGMENT_SHADER, shader->code[2]);

	if (shader_set->program_id)
		glDeleteProgram(shader_set->program_id);

	shader_set->program_id = glCreateProgram();

	if (shader->code[0])  glAttachShader(shader_set->program_id, shader_set->vertex_id);
	if (shader->code[1])  glAttachShader(shader_set->program_id, shader_set->geometry_id);
	if (shader->code[2])  glAttachShader(shader_set->program_id, shader_set->fragment_id);
	glLinkProgram(shader_set->program_id);

	GLint result = GL_FALSE;
	s32 length_info_log;
	glGetProgramiv(shader_set->program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(shader_set->program_id, GL_INFO_LOG_LENGTH, &length_info_log);

	if (length_info_log) {
		/// make space for '\0'
		char *c_error_msg = Memory_Create(char, length_info_log + 1);
		glGetProgramInfoLog(shader_set->program_id, length_info_log, 0, c_error_msg);
		LOG_DEBUG(c_error_msg);
		Memory_Free(c_error_msg);
	}

	glDeleteShader(shader_set->vertex_id);
	glDeleteShader(shader_set->geometry_id);
	glDeleteShader(shader_set->fragment_id);
}

inline void
ShaderSet_Destroy(
	ShaderSet *shader_set
) {
	Assert(shader_set);

    if (shader_set->program_id) {
		glDeleteProgram(shader_set->program_id);
		shader_set->program_id = 0;
    }
}

inline void
Shader_SetValue(
	ShaderSet *shader_set,
	const char *name,
	const float buffer
) {
	s32 loc_id = glGetUniformLocation(shader_set->program_id, name);

	if (loc_id >= 0)
		glProgramUniform1f(shader_set->program_id, loc_id, buffer);
}

inline void
Shader_SetValue(
	ShaderSet   *shader_set,
	const char  *name,
	const float *buffer,
	u32 count
) {
	s32 loc_id = glGetUniformLocation(shader_set->program_id, name);

	if (loc_id >= 0) {
		switch (count) {
			case 3: {
				glProgramUniform3fv(shader_set->program_id, loc_id, 1, buffer);
			} break;

			case 4: {
				glProgramUniform4fv(shader_set->program_id, loc_id, 1, buffer);
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
	s32 loc_id = glGetUniformLocation(shader_set->program_id, name);

	if (loc_id >= 0)
		glProgramUniform1i(shader_set->program_id, loc_id, value);
}

inline void
Shader_SetValue(
	ShaderSet *shader_set,
	const char *name,
	int value_a,
	int value_b
) {
	s32 loc_id = glGetUniformLocation(shader_set->program_id, name);

	if (loc_id >= 0) {
		glProgramUniform2i(shader_set->program_id, loc_id, value_a, value_b);
	}
}

inline void
Shader_SetValue(
	ShaderSet *shader_set,
	const char *name,
	float value_a,
	float value_b
) {
	s32 loc_id = glGetUniformLocation(shader_set->program_id, name);

	if (loc_id >= 0) {
		glProgramUniform2f(shader_set->program_id, loc_id, value_a, value_b);
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
ShaderSet_Load(
	ShaderSet *shader_set,
	const Shader *shader,
	Window *window = 0
) {
	Assert(shader_set);

	if (window)  shader_set->window = window;

	Assert(shader_set->window);

	if (shader_set->program_id)
		ShaderSet_Destroy(shader_set);

	ShaderSet_Use(shader_set, shader);
	glUseProgram(shader_set->program_id);

	if (shader_set->window) {
		RectF viewport = {shader_set->window->x_viewport, shader_set->window->y_viewport, (float)shader_set->window->width, (float)shader_set->window->height};
		Shader_SetValue(shader_set, "viewport", (float *)&viewport, 4);
		glDisable(GL_BLEND);
	}
}

/// ::: Vertex
/// ===========================================================================
template <typename T>
struct Vertex_Buffer {
	const char *name;
	u32 id;
	const T *data;
	u32 size;
	u32 group_count;
};

struct Vertex_Settings {
	bool flip = false;
	float scale_x = 1.0f;
	float scale_y = 1.0f;
};

struct Vertex {
	u32 array_id = 0;
	Array<Vertex_Buffer<float>> a_attributes;
	Texture *texture = 0;
	Vertex_Settings settings;
};

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
	s32 *width,
	s32 *height
) {
	Assert(vertex);
    Assert(vertex->texture);

    if (width)  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH , width);
	if (height) glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height);
}

instant void
Vertex_AddAttribute(
	Vertex *vertex,
	u32 group_count,
	const char *name,
	const float *data,
	u32 size
) {
	Assert(vertex);
	Assert(vertex->array_id > 0);

	Vertex_Buffer<float> vertex_buffer;
	glBindVertexArray(vertex->array_id);

	vertex_buffer.name = name;
	vertex_buffer.group_count = group_count;
	vertex_buffer.data = data;
	vertex_buffer.size = size;

	u64 find_it;
	bool found_entry = Array_Find(&vertex->a_attributes, vertex_buffer, &find_it);

	if (!found_entry) {
		/// new attribute
		glGenBuffers(1, &vertex_buffer.id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer.id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);

		Array_Add(&vertex->a_attributes, vertex_buffer);
	}
	else {
		/// update opengl attribute data
		Vertex_Buffer<float> *t_buffer = &ARRAY_IT(vertex->a_attributes, find_it);
		vertex_buffer.id = t_buffer->id;
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer.id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);

		/// update data in vertex->a_attribute
		*t_buffer = vertex_buffer;
	}
}

inline void
Vertex_Destroy(
	Vertex *vertex
) {
	Assert(vertex);

	if (!vertex->array_id)  return;

	FOR_ARRAY(vertex->a_attributes, it) {
		Vertex_Buffer<float> *vertex_buffer = &ARRAY_IT(vertex->a_attributes, it);
		glDeleteBuffers(1, &vertex_buffer->id);
	}

	Array_Destroy(&vertex->a_attributes);

	glDeleteVertexArrays(1, &vertex->array_id);

	Texture_Destroy(vertex->texture);

	*vertex = {};
}

inline void
Vertex_Create(
	Vertex *vertex,
	u8 point_count,
	const float *vertex_data,
	u32 vertex_size
) {
	Assert(vertex);

	if (!vertex->array_id)
		glGenVertexArrays(1, &vertex->array_id);

	///@Info: make sure this attribute exists in the shader
	Vertex_AddAttribute(vertex, point_count, "vertex_position", vertex_data, vertex_size);
}

inline void
Vertex_UseTextureIndex(
	ShaderSet *shader_set,
	Vertex *vertex,
	Texture *texture
) {
	Assert(shader_set);
	Assert(vertex);
	Assert(texture);

	/// use this uniform in a shader, if the texture index has to change
	Shader_BindAndUseIndex0(shader_set, "fragment_texture", vertex->texture);
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

	vertex->texture = texture;

	Vertex_UseTextureIndex(shader_set, vertex, texture);
}

instant void
Vertex_Load(
	ShaderSet *shader_set,
	Vertex *vertex
) {
	Assert(shader_set);
	Assert(vertex);
	Assert(shader_set->program_id);

	FOR_ARRAY(vertex->a_attributes, it) {
		Vertex_Buffer<float> *entry = &ARRAY_IT(vertex->a_attributes, it);
		s32 attrib_position = glGetAttribLocation(shader_set->program_id, entry->name);

		if (attrib_position < 0)
			continue;

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

	if (vertex->texture) {
		Vertex_SetTexture(shader_set, vertex, vertex->texture);
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

	Vertex_Buffer<float> a_vertices = ARRAY_IT(vertex->a_attributes, 0);

	Shader_SetValue(shader_set, "flip_h" , vertex->settings.flip);
	Shader_SetValue(shader_set, "scale_x", vertex->settings.scale_x);
	Shader_SetValue(shader_set, "scale_y", vertex->settings.scale_y);

	Vertex_Load(shader_set, vertex);
	glDrawArrays(GL_POINTS, 0,  a_vertices.size / sizeof(GLfloat) / a_vertices.group_count);
}

instant bool
Vertex_Create(
	ShaderSet *shader_set,
	Vertex *vertex,
	Texture *texture,
	Rect rect
) {
	Assert(vertex);
	Assert(texture);

	if (!texture->ID)
		return false;

	s32 width, height;

	Vertex_SetTexture(shader_set, vertex, texture);
	Vertex_GetTextureSize(vertex, &width, &height);

	if (!rect.w)  rect.w = width;
	if (!rect.h)  rect.h = height;

	float x_off = 0, y_off = 0, scale_x, scale_y;
	Rect_GetAspect(width, height, &rect.w, &rect.h, &x_off, &y_off, &scale_x, &scale_y, true);

	vertex->settings.flip = true;
	vertex->settings.scale_x = scale_x;
	vertex->settings.scale_y = scale_y;

	if (scale_y > scale_x)
		rect.x += x_off;
	else
		rect.y += y_off;

	/// only use x, y -> will show full texture
	Vertex_Create(vertex, 2, (float *)&rect, sizeof(float) * 2);

	return true;
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
Mouse_GetPos(
	float *x,
	float *y
) {
	POINT point;

	GetCursorPos(&point);

	if (x) *x = point.x;
	if (y) *y = point.y;
}

instant void
Mouse_GetPos(
	Mouse *mouse,
	Window *window
) {
	Assert(window);
	Assert(mouse);

	RECT rect_active;
	GetWindowRect(window->hWnd, &rect_active);
	Window_UnAdjustRect(window->hWnd, &rect_active);

	Mouse t_mouse = *mouse;

	Mouse_GetPos(&mouse->point.x, &mouse->point.y);

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

	Mouse_Reset(mouse);

	if (window AND msg->message == WM_MOUSEMOVE) {
		Mouse_GetPos(mouse, window);
		return true;
	}

	if (!msg)
		return false;

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
