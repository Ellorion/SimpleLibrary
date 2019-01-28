#pragma once

#define SHOW_WARNING			0

#define DEBUG_ALWAYS_UPDATE		0
#define DEBUG_EVENT_STATUS		0
#define DEBUG_BENCHMARK			0

/// Compiler: g++ (6.3.0) (mingw)
///
/// Linker flags:
///		-lcomdlg32
///		-lopengl32
///		-lgdi32
///
/// Compiler options:
///		-fno-exceptions
///		-fno-asynchronous-unwind-tables
///		-fno-rtti
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
///       _opt -> can be 0 (pointer), but does not set it as
///               optional default value
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

//#ifdef __MINGW32__
//#   undef  _WIN32_WINNT
//#   define _WIN32_WINNT 0x0501
//#endif // __MINGW32__

#include <windows.h>
#include <GL/gl.h>

#include <iostream>
#include <math.h>		/// floor

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
#include "external/stb_truetype.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#define instant static inline

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
#	define LOG_STATUS(_text) std::cout << _text;
#else
#	define LOG_STATUS(_text)
#endif

#define LOG_DEBUG(text) std::cout << text << std::endl;
#define LOG_INFO(_text) std::cout << "Info: " << _text << std::endl;

#if SHOW_WARNING
#	define LOG_WARNING(_text) std::cout << "Warning: " << _text << std::endl;
#else
#	define LOG_WARNING(_text)
#endif

#if DEBUG_BENCHMARK
	#define MEASURE_START() \
		Timer DEBUG_tmr_measure; \
		Time_Measure(&DEBUG_tmr_measure);

	#define MEASURE_END(_text) \
		std::cout << "Measure [" << __FUNCTION__ << "]: " << _text << Time_Measure(&DEBUG_tmr_measure) << " ms" << std::endl;
#else
	#define MEASURE_START()
	#define MEASURE_END(_text)
#endif

/// ::: Utilities
/// ===========================================================================
#define MIN(val_1, val_2) ((val_1 < val_2) ? val_1 : val_2)
#define MAX(val_1, val_2) ((val_1 < val_2) ? val_2 : val_1)

#define GETBYTE(x, bit_start) LOBYTE((x) >> (bit_start))
#define GETBIT(x, bit_start) (((x) >> (bit_start)) & 0x1)

#define IF_SET(pointer) \
	if (pointer) (*pointer)

#define IF_USE(pointer) \
	(pointer) AND (*pointer)

#define XOR(_a, _b) \
	(((_a) AND !(_b)) OR (!(_a) AND (_b)))

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

struct Point {
	float x = 0.0f;
	float y = 0.0f;
};

struct Color32 {
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;
};

template<typename K, typename L>
struct Tuple {
	K first;
	L second;
};

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

/// independent
#include "core/memory.h"
#include "core/time.h"
#include "core/string.h"
#include "core/array.h"
#include "core/rect.h"

/// hierarchical dependencies (including independent)
#include "core/cpu.h"
#include "core/array_string.h"
#include "core/tree.h"	// array + array_string (if needed)
#include "core/files.h"
#include "core/image.h"

/// interlocking dependencies
#include "windows/windows.h"
#include "opengl/core.h"
#include "windows/mouse.h"
#include "windows/keyboard.h"
#include "windows/joypad.h"
#include "opengl/text.h"

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

instant s32
ToInt(
	String *s_data
) {
	Assert(s_data);

	char *c_data = String_CreateCBufferCopy(s_data->value, s_data->length);
	s32 result = atoi(c_data);
	Memory_Free(c_data);
	return result;
}

#include "gui/layout.h"
#include "gui/widget.h"
