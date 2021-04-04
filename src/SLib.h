#pragma once

/// @Note set this to 0 for release build
///       to increase performance
#define DEBUG_MODE				1

/// ===========================================================================
#if DEBUG_MODE

/// Log-Messages
#	define SHOW_INFO			0
#	define SHOW_WARNING			1

/// enables timing measurement output
#	define DEBUG_BENCHMARK		0
#	define DEBUG_EVENT_STATUS	0

#else

/// Log-Messages
#	define SHOW_INFO			0
#	define SHOW_WARNING			0

/// enables timing measurement output
#	define DEBUG_BENCHMARK		0
#	define DEBUG_EVENT_STATUS	0

#endif // DEBUG_MODE
/// ===========================================================================

/// Operating System: Windows
///
/// Compiler: g++ (i686-posix-dwarf-rev0, Built by MinGW-W64 project) 8.1.0
///
/// Linker flags:
///	   [-lcomdlg32]
///		-lopengl32
///		-lgdi32
///		-lws2_32
///		-lShlwapi
///		-liphlpapi
///
///     /* debug build*/
///     -g
///     /* release build*/
///     -O2
///     -s
///
///		/* release or stand-alone build */
///		-static-libgcc
///		-static-libstdc++
///		-static
///
///		/* Removes startup console window */
///     -mwindows
///
/// Compiler options:
///		-std=c++20
///
///		-fno-exceptions
///		-fno-asynchronous-unwind-tables
///		-fno-rtti
///		-Wno-unknown-pragmas
///		-Wno-unused-function
///		-Wno-unused-but-set-variable
///		-Wno-comment
///		-Wno-unused-variable
///		-Wno-uninitialized
///		-Wno-maybe-uninitialized
///
/// Convention:
/// 	a_ -> Array
///     s_ -> String
///     c_ -> C-String (when String with the same name might also be used)
///    as_ -> Array<String> ...
///     t_ -> temporary local variable (which might overlap in name with
///           a parameter / or when shadowing)
///    ap_ -> Array<something *> (an array of pointers)
///
///		Events (function delegates):
///     - nameing starts with On... (f.e. OnDidSomething)
///     - in case of an array of events: a_On... (f.e. a_OnDoThisAndThat)
///
///		Events (non-delegates):
///     - nameing starts with on... (f.e. on_happened_something)
///
///     Postfix Pointer Parameter:
///     ------------------------------------------------------
///     Function parameters with pointers who end in name with
///       _out -> everything / something (in a struct)
///               will be cleared / overwritten
///       _io  -> will read an input and generate an output
///               based on it
///       _opt -> can be 0 (pointer), but is not set with an
///               optional default value
///     otherwise, the pointer will only be used for reading
///
/// Return types: Ff a function returns a struct, it's memory needs to
///               be free'd to prevent memory leaks,
///               unless that function ends with "Ref". In that case the
///               return value does not require freeing, since it uses a reference.
///
/// Memory alignment:
/// 	Fill structs for memory alignment with:
///			bool _dummy_<number> = 0;
///     and replace <number> with an increasing number
///
///		Why: When initialising a struct with {} it will set its data
///          based on the default values given to each member inside
///          the struct.
///          BUT this does set the non-visible alignment buffer
///          to 0.
///          When using f.e. Memory_Compare, this will (very likely...)
///          cause some (unforseen) consequences.
///
///			 This does at least apply (maybe randomly, but it did happen),
///          when the alignment buffer is at the end of the struct.
///          It did not happen with alignment buffers between known struct
///          members... (yet).
///
///			 GCC/g++: use "__attribute__((packed))" to get current size
///                   with sizeof-operator and fill-up the struct accordingly
///

//#ifdef __MINGW32__
//#   undef  _WIN32_WINNT
//#   define _WIN32_WINNT 0x0501
//#endif // __MINGW32__

#ifndef _MSC_VER
#define  __forceinline __attribute__((always_inline))
#endif

#include <windows.h>
#include <GL/gl.h>

#include <iostream>
#include <math.h>
#include <winsock.h>
#include <shlwapi.h>
#include <iphlpapi.h>
#include <shlobj.h>
#include <time.h>

__attribute__((gnu_inline, always_inline))
__inline__ static void debug_break(void)
{
	__asm__ volatile("int $0x03");
}

#define s8	  char
#define s16	  short
#define s32	  int
#define slong long  			// 32-bit
#define s64	  long long

#define u8	  unsigned char
#define u16	  unsigned short
#define u32	  unsigned int
#define ulong unsigned long 	// 32-bit
#define u64	  unsigned long long

#define AND &&
#define OR  ||

#define MATH_PI 3.1415926

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

#define FOR_AUTO(_max, _it)			\
	for(auto _it = 0;				\
	_it < (_max); 					\
	++_it)

#define FOR_START(_start, _max, _it)	\
	for(u64 _it = _start; _it < _max; ++_it)

#define FOR_START_AUTO(_start, _max, _it)	\
	for(auto _it = _start; _it < _max; ++_it)

/// ::: Assert
/// ===========================================================================
#if DEBUG_MODE
#define		Assert(EX) \
				(void)((EX) OR (_Assert(#EX, __FILE__, __LINE__), 0))
#else
#define		Assert(EX)
#endif // DEBUG_MODE

#if DEBUG_MODE
#define		AssertMessage(EX, INFO) \
				(void)((EX) OR (_AssertMessage(INFO, __FILE__, __LINE__), 0))
#else
#define		AssertMessage(EX, INFO)
#endif // DEBUG_MODE

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
#	define LOG_STATUS(_text) std::cout << _text << std::endl;
#else
#	define LOG_STATUS(_text)
#endif

#define LOG_DEBUG(text) std::cout << text << std::endl;
#define LOG_ERROR(text) std::cerr << "[Error] " << text << std::endl

#if SHOW_INFO
#	define LOG_INFO(_text) std::cout << "Info: " << _text << std::endl;
#else
#	define LOG_INFO(_text)
#endif

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
		std::cout << "Measure [" << __FUNCTION__ << "]: " << _text << " " << Time_Measure(&DEBUG_tmr_measure) << " ms" << std::endl;

#define MEASURE_END_AVG(_text, _count) \
		std::cout << "Measure [" << __FUNCTION__ << "]: " << _text << " " << (Time_Measure(&DEBUG_tmr_measure) / _count) << " ms" << std::endl;
#else
	#define MEASURE_START()
	#define MEASURE_END(_text)
	#define MEASURE_END_AVG(_text, _count)
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

#define SWAP(type, ptr_a, ptr_b) \
    do { \
		type temp = *(ptr_a); \
		*(ptr_a)  = *(ptr_b); \
		*(ptr_b)  = temp;     \
    } while(0)

struct Color32 {
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;
};

namespace Color {
	Color32 White = {1.0f, 1.0f, 1.0f, 1.0f};
};

template<typename K, typename L>
struct Tuple {
	K first;
	L second;
};

struct Point {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

Point
operator + (
	Point &pt1,
	Point &pt2
) {
	Point pt;

	pt.x = pt1.x + pt2.x;
	pt.y = pt1.y + pt2.y;
	pt.z = pt1.z + pt2.z;

	return pt;
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

instant bool
IsNumeric(
	char character
) {
	return (character >= '0' AND character <= '9');
}

instant void
ToHex(
	const char value,
	char *hex_high,
	char *hex_low
) {
	Assert(hex_high);
	assert(hex_low);

	const char *c_hex = "0123456789ABCDEF";

	*hex_high = c_hex[(value >> 4) & 0xF];
	*hex_low  = c_hex[(value >> 0) & 0xF];
}

enum KEYBOARD_HOTKEY_ID {
	KEYBOARD_HOTKEY_01 = 0,
	KEYBOARD_HOTKEY_02,
	KEYBOARD_HOTKEY_03,
	KEYBOARD_HOTKEY_04,
	KEYBOARD_HOTKEY_05,
	KEYBOARD_HOTKEY_06,
	KEYBOARD_HOTKEY_07,
	KEYBOARD_HOTKEY_08,
	KEYBOARD_HOTKEY_09,
	KEYBOARD_HOTKEY_ID_COUNT,
};

#include "core/array_const.h"
#include "core/memory.h"
#include "core/array.h"
#include "core/string.h"
#include "core/array_string.h"
#include "core/memory_segment.h"

#include "utility/convert.h"

#include "core/random.h"
#include "core/mutex.h"
#include "core/thread.h"
#include "core/time.h"
#include "core/parser.h"
#include "core/sort.h"
#include "core/rect.h"
#include "core/cpu.h"
#include "core/tree.h"
#include "core/files.h"
#include "core/stream.h"
#include "core/image.h"
#include "core/map.h"

#include "utility/base64.h"
#include "core/network.h"

#include "core/application.h"

#include "utility/clipboard.h"
#include "utility/file_watcher.h"
#include "utility/csv.h"
#include "utility/helper.h"
#include "utility/profiler.h"
#include "utility/prime.h"

#include "windows/windows.h"
#include "opengl/core.h"
#include "windows/mouse.h"
#include "windows/keyboard.h"
#include "windows/joypad.h"
#include "opengl/text.h"

#include "gui/layout.h"
#include "gui/widget.h"

#include "core/console.h"
