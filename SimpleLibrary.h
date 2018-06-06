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
//		Window_Clear();
//
//		Window_Update(window);
//	}
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

instant Memory_Header
Memory_GetHeader(
	void *mem
) {
	Assert(mem);

	mem = (char *)mem - sizeof(Memory_Header);

	return (*(Memory_Header *)mem);
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

	Memory_Header mem_header = Memory_GetHeader(mem);
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

	Memory_Header mem_header = Memory_GetHeader(data);

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

instant void *
Memory_Set(
	void *dest,
	int data,
	u64 length
) {
	if (dest == 0) return 0;

	u8 *cDest = (u8*)dest;
	u8  cData = data;

	while (length-- > 0)
		*cDest++ = cData;

	return dest;
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

instant Timer
Time_Reset() {
	Timer result;

	Time_Reset(&result);

	return result;
}

instant u32
Time_Get() {
	return GetTickCount();
}

instant bool
Time_HasElapsed(
	Timer *timer,
	u32 intervalMS,
	bool resetElapsed = true
) {
	Assert(timer);

	bool result = true;

	if (Time_Get() - timer->lo_timer < intervalMS)
		result = false;
	else {
		/// update timer when time has passed
		if (resetElapsed)
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

instant u32
Time_GetFPS(
	Timer *timer,
	bool getWorst = false
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

	return (getWorst ? timer->fps_worst : timer->fps);
}

/// Reach the end (from the start) in the timespan of the duration
/// Returns: step increase
instant float
Time_Move(
	Timer *timer,
	u32 timespanMS,
	float distance
) {
	Assert(timer);

	if (!timespanMS)  return 0.0f;

	double step_size = Time_Measure(timer) / timespanMS;
	return distance * step_size;
}


/// ::: String
/// ===========================================================================
#define String_Split Array_Split

struct String {
	u64  length = 0;
	char *value = 0;
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

instant u64
String_TrimLeft(
	String *s_data,
	bool move_pointer = true
) {
	if (!s_data)
		return 0;

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

    return length;
}

instant void
String_TrimRight(
	String *s_data
) {
	if (!s_data OR !s_data->length)
		return;

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
	if (!s_data)  return;

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

///@Info: will copy the elements memory pointers (inside a struct)
///       so make sure you make a new memory location
///       in case the array is supposed to handle the data
///       or the pointers might end up becoming invalid later (cloning)
template <typename T>
instant T *
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

	return &ARRAY_IT(*array, target);
}

template <typename T>
instant T *
Array_AddEmpty(
	Array<T> *array
) {
	T element = {};
	return Array_Add(array, element);
}

///@Info: does NOT have ownership and clears/destroys
///       dynamic data inside structs and such.
///       the same thing applies for inserting data
///       into empty array slots (created with addempty)
///       into dynamic pointers inside a struct element
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
	Memory_Free(array->memory);
	*array = {};
}

/// Will add memory slots on top of existing ones and add to that count
template <typename T>
instant void
Array_Reserve(
	Array<T> *array,
	u64 count,
	bool clear_zero = false
) {
	Assert(array);

	if (array->size + sizeof(T) * count > array->limit) {
		array->limit += sizeof(T) * count;
		array->memory = (T *)_Memory_Resize(array->memory, array->limit);
	}

	if (clear_zero) {
		Memory_Set(array->memory, 0, array->limit);
		array->count = 0;
	}
}

template <typename T>
instant bool
Array_Find(
	Array<T> *array,
	T find,
	u64 *index = 0
) {
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

instant CPU_ID
CPU_GetID(
	u32 id
) {
	CPU_ID cpu_regs;

	asm volatile
		("cpuid" :
		 "=a" (cpu_regs.EAX), "=b" (cpu_regs.EBX),
		 "=c" (cpu_regs.ECX), "=d" (cpu_regs.EDX)
		: "a" (id), "c" (0));

	return cpu_regs;
}

instant char *
CPU_GetVendor() {
	CPU_ID cpu_id = CPU_GetID(0);

	char *vendor = Memory_Create(char, 13);
	String_Copy(vendor + 0, (char*)&cpu_id.EBX, 4);
	String_Copy(vendor + 4, (char*)&cpu_id.EDX, 4);
	String_Copy(vendor + 8, (char*)&cpu_id.ECX, 4);

	return vendor;
}

instant CPU_Type
CPU_GetInfo() {
	CPU_ID   cpu_id = CPU_GetID(1);
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

	CPU_ID       cpu_id = CPU_GetID(1);
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

	char *c_search_file = String_CreateCBufferCopy(&ts_filename);

	String_Destroy(&ts_filename);

	bool result = false;

	if (FindFirstFile(c_search_file, &file_data) != INVALID_HANDLE_VALUE)
		result = true;

	Memory_Free(c_search_file);

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

	if (window->hRC) {
		wglMakeCurrent(0, 0);
		wglDeleteContext(window->hRC);
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
	s32 bits = 32
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

	return true;
}

instant void
Window_InitOpenGL(
	Window *window
) {
	if (!window)
		return;

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

instant bool
Window_AdjustScale(
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
		bool result = window->width != new_width OR window->height != new_height;

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

instant void
Window_Show(
	Window *window,
	int nCmdShow = 10
) {
	Assert(window);

	ShowWindow(window->hWnd, nCmdShow);

	SetForegroundWindow(window->hWnd);
	SetFocus(window->hWnd);

	Window_AdjustScale(window);
}

instant void
Window_Update(
	Window *window
) {
	Assert(window);

	SwapBuffers(window->hDC);
}

instant void
Window_Clear(
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

	RECT rc = {};
	DWORD dwStyle   = (DWORD)GetWindowLong(window->hWnd, GWL_STYLE);
	DWORD dwExStyle = (DWORD)GetWindowLong(window->hWnd, GWL_EXSTYLE);

	bool success = AdjustWindowRectEx(&rc, dwStyle, 0, dwExStyle);

	if (success) {
		width  += (rc.right  - rc.left);
		height += (rc.bottom - rc.top);

		rc = {};
		GetWindowRect(window->hWnd, &rc);

		s32 x = rc.left + (rc.right  - rc.left - width)  / 2;
		s32 y = rc.top  + (rc.bottom - rc.top  - height) / 2;

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

/// ::: OpenGL
/// ===========================================================================
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

