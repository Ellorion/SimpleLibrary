#pragma once

/// Compiler: g++

#include <iostream>
#include <windows.h>

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

/// Iterators
/// ===========================================================================
#define FOR(_max, _it)				\
	for(u64 _it = 0;				\
	it < (_max); 					\
	++_it)

#define FOR_START(_start, _max, _it)	\
	for(u64 _it = _start; _it < _max; ++_it)

/// Assert
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

/// Debug
/// ===========================================================================
#define LOG_DEBUG(text) std::cout << text << std::endl;

/// Utilities
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

/// Memory
/// ===========================================================================
#define Memory_Create(type, len) (type *)_Memory_Alloc_Empty(sizeof(type) * len)
#define Memory_Resize(buffer, type, len) (buffer ? (type *) _Memory_Resize(buffer, sizeof(type) * len) : (type *) Memory_Create(type, len));

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
	u64 len
) {
	if (!dest OR !src OR !len)  return;
	if (dest == src)            return;

    char *c_dest = (char *)dest;
    char *c_src  = (char *)src;

    if (dest > src) {
		while(len-- > 0)
			c_dest[len] = c_src[len];
    }
    else {
		FOR(len, it) {
			c_dest[it] = c_src[it];
		}
    }
}

instant void *
Memory_Set(
	void *dest,
	int data,
	u64 len
) {
	if (dest == 0) return 0;

	u8 *cDest = (u8*)dest;
	u8  cData = data;

	while (len-- > 0)
		*cDest++ = cData;

	return dest;
}

/// Time
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


/// String
/// ===========================================================================
#define String_Split Array_Split

struct String {
	u64  len    = 0;
	char *value = 0;
};

instant u64
String_Length(
	String *s_data
) {
	if (!s_data)  return 0;

	return s_data->len;
}

/// does NOT support UTF-8
instant u64
String_Length(
	const char *c_data
) {
	u64 len = 0;

	if (c_data == 0)
		return len;

	while (*c_data++)
		++len;

	return len;
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
	s64 len_delta
) {
	Assert(s_data);

	s_data->value = Memory_Resize(s_data->value, char, s_data->len + len_delta);
}

instant void
String_Append(
	String *s_data,
	const char *c_data,
	u64 len_append = 0
) {
    Assert(s_data);

    if (len_append == 0)  len_append = String_Length(c_data);
    if (len_append == 0)  return;

	String_Resize(s_data, len_append);
	Memory_Copy(s_data->value + s_data->len, (char *)c_data, len_append);
	s_data->len += len_append;
}

instant void
To_String(
	String *s_data,
	const char *c_data,
	u64 len = 0
) {
	Assert(s_data);
	Assert(c_data);

	*s_data = {};
	String_Append(s_data, c_data, len);
}

/// does NOT add memory for '\0'
/// make sure you add an additional byte
/// in the buffer so you don't truncate the string
instant void
To_CString(
	char *c_buffer,
	u64 buffer_len,
	String *s_data
) {
	Assert(s_data);

	if (!buffer_len)  return;

	if (!s_data->len) {
		c_buffer[0] = '\0';
		return;
	}

	/// stop copying memory after '\0'
	if (buffer_len > s_data->len + 1) {
		buffer_len = s_data->len + 1;
	}

	/// stop before '\0'
	FOR(buffer_len - 1, it) {
		c_buffer[it] = s_data->value[it];
	}

	/// set final '\0'
	c_buffer[buffer_len - 1] = '\0';
}

instant bool
String_IsEqual(
	const char *c_text1,
	const char *c_text2,
	u64 len = 0
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
		if (len)
			if (--len <= 0) break;
	}

	return result;
}

instant bool
String_IsEqual(
	String     *s_data,
	const char *c_data,
	u64 len = 0
) {
	/// check if both have equally nothing
	if (!s_data AND !c_data)
		return true;

	/// in case one has more than nothing
	if (!s_data OR !c_data)
		return false;

	u64 len_c = String_Length(c_data);

	if (!len) {
		/// different lengths
		if (s_data->len != len_c)
			return false;
	}

	/// check min length needed to check
	u64 len_min = 0;
	u64 len_max = (len_c < s_data->len)
						? len_c
						: s_data->len;

	if (len > 0 AND len > len_max)  len_max = len;

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
	String *s_source,
	u32 len = 0
) {
	Assert(s_dest);
	Assert(s_source);

	/// overwrite protection check
	Assert(!s_dest->value);

	if (len == 0)
		len = s_source->len;

	s_dest->value = Memory_Create(char, len);
	Memory_Copy(s_dest->value, s_source->value, len);
	s_dest->len = len;
}

instant void
String_Copy(
	char *dest,
	const char *src,
	u32 len = 0
) {
	if (!src)
		return;

	char *result = dest;

	if (!len)
		len = String_Length(src);

	/// for '0'-terminator
	++len;

	while (*src AND --len > 0)
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

	if (!s_data OR !s_data->len)
		return result;

	int len_key  = String_Length(key);

	if (len_key == 0)
		return result;

	u64 len_text = s_data->len;

	if (index_start < 0) index_start = 0;

	FOR_START(index_start, len_text, index) {
		if (String_IsEqual(s_data->value + index, key, len_key)) {
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

	if (!s_data OR !s_data->len)
		return result;

	int len_key  = String_Length(key);

	if (len_key == 0)
		return result;

	if (index_start > (s64)s_data->len OR index_start < 0)
		index_start = (s64)s_data->len;

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
	if (!start_with OR !s_data OR !s_data->len)
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
	u32 len
) {
	if (s_data AND len < s_data->len)  s_data->len = len;
}

instant bool
String_EndWith(
	String *s_data,
	const char *endwith,
	u64 len = 0
) {
	if (!len)  len = String_Length(endwith);

	if (len > s_data->len)
		return false;

	String ts_data = *s_data;
	ts_data.value = ts_data.value + (ts_data.len - len);
	ts_data.len = len;

	return String_IsEqual(&ts_data, endwith, len);
}

instant void
String_ToLower(
	String *s_data
) {
	if (!s_data)
		return;

	u64 index = 0;
	u64 len_max = s_data->len + 1;

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
	u64 len_max = s_data->len + 1;

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

	u64 len = s_data->len;

	for(u64 it = 0; it < (len >> 1); ++it) {
		char temp = s_data->value[it];
		/// don't reverse the '\0'!
		s_data->value[it] = s_data->value[len - it - 1];
		s_data->value[len - it - 1] = temp;
	}
}

instant u64
String_TrimLeft(
	String *s_data,
	bool move_pointer = true
) {
	if (!s_data)
		return 0;

	u64 len = 0;
	u64 len_max = s_data->len;

	/// does not skip '\0'
    while(len <= len_max) {
		if (s_data->value[len] <= 32 AND s_data->value[len] != 127) ++len;
		else												   break;
    }

    if (move_pointer) {
		s_data->value += len;
		s_data->len   -= len;
    }

    return len;
}

instant void
String_TrimRight(
	String *s_data
) {
	if (!s_data OR !s_data->len)
		return;

	u64 len = s_data->len - 1;

    while(len > 0) {
		if (s_data->value[len] <= 32 AND s_data->value[len] != 127)
			--len;
		else
			break;
    }

    s_data->len = len + 1;
}

instant u64
String_Insert(
	String *s_data,
	const char *c_data,
	u64 index_start
) {
	if (!s_data)  return 0;
	if (!c_data)  return 0;

	u64 len = String_Length(c_data);

	if (s_data->value) {
		Memory_Resize(s_data->value, char, s_data->len + len);

		/// move memory
		Memory_Copy(s_data->value + index_start + len, s_data->value + index_start, s_data->len - index_start);
	}
	else {
		s_data->value = Memory_Create(char, index_start + len);
		Memory_Set(s_data->value, ' ', index_start + len);
	}

	FOR_START(index_start, index_start + len, it) {
		s_data->value[index_start + (it - index_start)] = *c_data++;
	}

	s_data->len += len;

	return len;
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

	if (index_end > s_data->len)
		index_end = s_data->len;

	u64 len = s_data->len - index_end;
	u64 rm_count = (index_end - index_start);

	Memory_Copy(s_data->value + index_start, s_data->value + index_end, len);
	s_data->len -= rm_count;

	return rm_count;
}

instant void
String_Clear(
	String *s_data
) {
	if (!s_data)  return;

	s_data->len = 0;
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
	s64 find_len     = String_Length(find);
	s64 replace_len  = String_Length(replace);

	while(String_Find(s_data, find, &pos_found, pos_start)) {
		String_Remove(s_data, pos_found, pos_found + find_len);
		String_Insert(s_data, replace, pos_found);
		pos_start += pos_found + replace_len;
	}
}

instant void
String_Replace(
	String     *s_data,
	const char *find,
	String     *replace
) {
	Assert(s_data);
	Assert(find);
	Assert(replace);

	char *c_data = Memory_Create(char, replace->len + 1);
	To_CString(c_data, replace->len + 1, replace);
	String_Replace(s_data, find, c_data);
	Memory_Free(c_data);
}

instant char *
String_Encode64(
	const char *text
) {
	const char encodingTable[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int len = String_Length(text);
	char *encoded = Memory_Create(char, (len + 2) / 3 * 4 + 1);
	int enc_index = -1;

	for (int index = 0; index < len; index += 3) {
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
	return String_IsEqual(&s_data1, s_data2.value, s_data2.len);
}

bool
operator < (
	String &s_data1,
	String &s_data2
) {
	if (s_data1.len < s_data2.len)  return true;
	if (s_data1.len > s_data2.len)  return false;

	FOR(s_data1.len, it) {
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
	if (s_data1.len > s_data2.len)  return true;
	if (s_data1.len < s_data2.len)  return false;

	FOR(s_data1.len, it) {
		if (s_data1.value[it] > s_data2.value[it]) return true;
		if (s_data1.value[it] < s_data2.value[it]) return false;
	}

	return false;
}


/// Array
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

	constexpr u64 len = 1;

	if (array->size + sizeof(T) * len > array->limit) {
		array->limit += sizeof(T) * len;
		array->memory = (T *)_Memory_Resize(array->memory, array->limit);;
	}

	u64 target = array->size / sizeof(T);

	array->memory[target] = element;
	array->size += sizeof(T) * len;

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
		s_data_it.len   -= pos_found + len_delim;
	}

	if (s_data_it.len > 0) {
		String s_element;
		String_Append(&s_element, s_data_it.value, s_data_it.len);
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


/// CPU
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

/// Files
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

	if (!s_extension->len)  return result;

	Array<String> as_extentions = Array_Split(s_extension, "|");

    FOR_ARRAY(as_extentions, it) {
    	String s_data_it = ARRAY_IT(as_extentions, it);

		if (String_EndWith(s_filename, s_data_it.value, s_data_it.len)) {
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
	String_Append(&ts_filename, s_path->value, s_path->len);
	String_Append(&ts_filename, "/", 1);
	String_Append(&ts_filename, s_filename->value, s_filename->len);

	char *c_search_file = Memory_Create(char, ts_filename.len + 1);
	To_CString(c_search_file, ts_filename.len + 1, &ts_filename);

	bool result = false;

	if (FindFirstFile(c_search_file, &file_data) != INVALID_HANDLE_VALUE)
		result = true;

	Memory_Free(c_search_file);

	return result;
}
