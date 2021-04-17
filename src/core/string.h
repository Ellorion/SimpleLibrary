#pragma once

#define String_Split Array_Split
#define LOG(text) std::cout << text << std::endl;

/// set to 0 to disable it
#define STRING_BUFFER_DEFAULT_SIZE	14

enum STRING_LENGTH_TYPE {
	STRING_LENGTH_BYTES,
	STRING_LENGTH_CODEPOINTS
};

struct String {
	bool is_reference = false;
	bool has_changed  = false;
	bool is_case_sensitive = true;

	u64   length = 0;
	char *value  = 0;
};

#include "utf8.h"

/// does support UTF-8
constexpr
instant u64
String_GetLength(
	const char *c_data,
	STRING_LENGTH_TYPE type = STRING_LENGTH_BYTES
) {
	u64 result = 0;
	u64 len_bytes = 0;
	u64 len_codepoints = 0;

	if (c_data == 0)
		return len_bytes;

	while (*c_data) {
		s16 length_data = UTF8_GetByteCount(c_data);

		if (length_data < 0)
			break;

		c_data += length_data;

		len_bytes      += length_data;
		len_codepoints += 1;
	}

	result = (type == STRING_LENGTH_BYTES ? len_bytes : len_codepoints);

	return result;
}

/// (temporary) string conversion
constexpr
instant String
S(
	char c_data
) {
 	String s_data;
 	{
		s_data.value  = &c_data;
		s_data.length = 1;

		s_data.is_reference = true;
		s_data.has_changed  = true;
 	}

	return s_data;
}

constexpr
instant String
S(
	const char *c_data,
	u64 length = 0,
	bool isCaseSensitive = true
) {
 	String s_data;
 	{
		s_data.value  = (char *)c_data;
		s_data.length = (length ? length : String_GetLength(c_data));

		s_data.is_reference = true;
		s_data.has_changed  = true;
		s_data.is_case_sensitive = isCaseSensitive;
 	}

	return s_data;
}

constexpr
instant String
S(
	const String &s_data,
	u64 length = 0,
	bool isCaseSensitive = true
) {
 	String s_data_ref;
	{
		s_data_ref.value  = s_data.value;
		s_data_ref.length = (length ? length : s_data.length);

		s_data_ref.is_reference = true;
		s_data_ref.has_changed  = true;
		s_data_ref.is_case_sensitive = isCaseSensitive;
	}

	return s_data_ref;
}

instant void
String_Print(
	const String &s_data
) {
	FOR(s_data.length, it) {
		std::cout << s_data.value[it];
	}
}

instant void
String_PrintLine(
	const String &s_data
) {
	String_Print(s_data);
	std::cout << std::endl;
}

instant void
String_Print(
	const String &s_data,
	u64 length
) {
	length = MIN(s_data.length, length);

	FOR(length, it) {
		std::cout << s_data.value[it];
	}
}

instant void
String_PrintLine(
	const String &s_data,
	u64 length
) {
	String_Print(s_data, length);
	std::cout << std::endl;
}

constexpr
instant void
String_AddOffset(
	String &s_data,
	s64 offset
) {
	Assert(s_data.is_reference);

	u64 prev_length = s_data.length;
	u64 next_length = prev_length -= offset;

	/// underflow prevention
	if (    offset > 0
		AND next_length > prev_length
	) {
		s_data.value  += s_data.length;
		s_data.length -= s_data.length;
	}
	else {
		s_data.value  += offset;

		/// underflow check
		if (s_data.length - offset < s_data.length)
			s_data.length -= offset;
		else
			s_data.length  = 0;
	}
}

constexpr
instant bool
String_IsEmpty(
	const String &s_data,
	bool skip_whitespace = false
) {
	String s_data_it = S(s_data);

	if (s_data_it.value == 0)
		return true;

	if (skip_whitespace) {
		while(s_data_it.length) {
			char ch = s_data_it.value[0];

			switch (ch) {
				case ' ':
				case '\r':
				case '\n':
				case '\t': {
					String_AddOffset(s_data_it, 1);
					continue;
				} break;
			}

			break;
		}
	}

	return (s_data_it.length <= 0);
}

constexpr
instant void
String_Destroy(
	String &s_data
) {
	if (!s_data.is_reference) {
		Memory_Free(s_data.value);
	}

	s_data = {};

	s_data.has_changed = true;
}

constexpr
instant String
String_Copy(
	const char *c_source,
	u32 length = 0
) {
	Assert(c_source);

	if (length == 0)
		length = String_GetLength(c_source);

	String s_result = {};

	s_result.value = Memory_Create(char, length);
	Memory_Copy(s_result.value, c_source, length);
	s_result.length = length;

	s_result.has_changed  = true;
	s_result.is_reference = false;

	return s_result;
}

constexpr
instant void
String_Resize(
	String &s_data,
	s64 new_length
) {
	Assert(new_length > 0);

	if (new_length > (s64)s_data.length) {
		char *t_value_old = s_data.value;
		s_data.value = Memory_Resize(t_value_old, char, new_length);
	}

	s_data.length = new_length;
}

constexpr
instant bool
String_Append(
	String &s_data,
	const String &s_source,
	u64 length_append = 0
) {
    Assert(!s_data.is_reference);

    if (length_append == 0)
		length_append = s_source.length;

    if (length_append == 0) {
		LOG_WARNING("Source string length was set at 0.");
		return false;
    }

	Assert(length_append <= s_source.length);

	u64 index_start = s_data.length;
	String_Resize(s_data, s_data.length + length_append);
	Memory_Copy(s_data.value + index_start, s_source.value, length_append);

	s_data.has_changed = true;

	return true;
}

constexpr
instant u64
String_Insert(
	String &s_data,
	const String &s_source,
	u64 index_start
) {
	Assert(!s_data.is_reference);

	u64 dest_length_old = s_data.length;

	String_Resize(s_data, s_data.length + s_source.length);

	Memory_Copy(s_data.value + index_start + s_source.length,
				s_data.value + index_start,
				dest_length_old - index_start);

	Memory_Copy(s_data.value + index_start,
				s_source.value,
				s_source.length);

	return s_source.length;
}

constexpr
instant void
String_Clear(
	String &s_data_out
) {
	if (s_data_out.is_reference) {
        s_data_out = {};
	}
	else {
        s_data_out.length = 0;
        s_data_out.has_changed = true;
	}
}

constexpr
instant void
String_CreateBuffer(
	String &s_buffer_out,
	u64 buffer_size,
	bool is_reference
) {
	String_Resize(s_buffer_out, buffer_size);
	s_buffer_out.length  = buffer_size;
	s_buffer_out.has_changed = true;
	s_buffer_out.is_reference = is_reference;
}

constexpr
instant String
String_CreateBuffer(
	u64 buffer_size
) {
	String s_buffer_out = {};

	String_Resize(s_buffer_out, buffer_size);
	s_buffer_out.length = buffer_size;
	s_buffer_out.has_changed = true;

	return s_buffer_out;
}

constexpr
instant char
String_ToLower(
	char value
) {
	if (value >= 'A' AND value <= 'Z')
		 return (value + 32);

	return value;
}

constexpr
instant char
String_ToUpper(
	char value
) {
	if (value >= 'a' AND value <= 'z')
		 return (value - 32);

	return value;
}

/// @Note compares bytes, not codepoints
/// -1 = data_1 < data_2
///  0 = data_1 = data_2
/// +1 = data_1 < data_2
constexpr
instant s32
String_Compare(
	const String &s_data_1,
	const String &s_data_2,
	u64 length_opt,
	bool is_case_sensitive
) {
	/// const value comparison
	if (s_data_1.value == s_data_2.value)
		return 0;

	u64 length = length_opt;

	if (!length)
		length = MIN(s_data_1.length, s_data_2.length);

	FOR(length, it) {
		char value_1;
		char value_2;

		if (is_case_sensitive) {
			value_1 = s_data_1.value[it];
			value_2 = s_data_2.value[it];
		}
		else {
			value_1 = String_ToLower(s_data_1.value[it]);
			value_2 = String_ToLower(s_data_2.value[it]);
		}

		if (value_1 < value_2) return -1;
		if (value_1 > value_2) return +1;
	}

	if (!length_opt) {
		if (s_data_1.length < s_data_2.length)  return -1;
		if (s_data_1.length > s_data_2.length)  return +1;
	}

	return 0;
}

constexpr
instant bool
String_IsEqual(
	const String &s_first,
	const String &s_second,
	u64 length = 0,
	bool is_case_sensitive = true
) {
	if (!length AND s_first.length != s_second.length)
		return false;

	return (String_Compare(	s_first,
							s_second,
							length,
							is_case_sensitive) == 0);
}

constexpr
instant bool
String_IsEqual(
	const char *c_first,
	const char *c_second,
	u64 length = 0,
	bool is_case_sensitive = true
) {
	return String_IsEqual(	S(c_first),
							S(c_second),
							length,
							is_case_sensitive);
}

constexpr
instant void
String_CopyBuffer(
	const char *c_dest,
	const String &s_source,
	s64 c_length = -1
) {
	Assert(c_dest);

	if (c_length == 0)
		Memory_Copy(c_dest, "\0", 1);
	else
	if (c_length  < 0) {
		Memory_Copy(c_dest,
					s_source.value,
					s_source.length);
	}
	else {
		Memory_Copy(c_dest,
					s_source.value,
					MIN((u64)c_length,
					s_source.length));
	}
}

constexpr
instant String
String_Copy(
	const String &s_data
) {
	return String_Copy(s_data.value, s_data.length);
}

constexpr
instant char *
String_CreateCBufferCopy(
	const String &s_source
) {
	char *c_buffer = Memory_Create(char, s_source.length + 1);
	Memory_Copy(c_buffer, s_source.value, s_source.length);

	return c_buffer;
}

constexpr
instant s64
String_IndexOf(
	const String &s_data,
	const String &s_key,
	s64 index_start_opt,
	bool is_case_sensitive
) {
	int result = -1;

	if (String_IsEmpty(s_data))
		return result;

	if (String_IsEmpty(s_key, false))
		return result;

	u64 length_data = s_data.length;

	if (index_start_opt < 0)
		index_start_opt = 0;

	FOR_START(index_start_opt, length_data, index) {
		String s_data_ref = S(s_data);
		String_AddOffset(s_data_ref, index);

		if (String_Compare(	s_data_ref,
							s_key,
							s_key.length,
							is_case_sensitive) == 0
		) {
			return index;
		}
	}

	return result;
}

constexpr
instant s64
String_IndexOfRev(
	const String &s_data,
	const String &s_key,
	bool is_case_sensitive,
	s64 index_start = -1
) {
	int result = -1;

	if (String_IsEmpty(s_data))
		return result;

	if (String_IsEmpty(s_key, false))
		return result;

	/// index_start "starts" at the end of a string,
	/// and has to be converted from count
	/// (which is +1 of an index)
	if (index_start > (s64)s_data.length OR index_start < 0)
		index_start = (s64)s_data.length - 1;

	for(s64 it = index_start; it >= 0; --it) {
		if (String_IsEqual(	s_data.value + it,
							s_key.value,
							s_key.length,
							is_case_sensitive)
		) {
			return it;
		}
	}

	return result;
}

constexpr
instant bool
String_StartWith(
	const String &s_data,
	const String &s_startwith,
	bool is_case_sensitive
) {
	if (String_IsEmpty(s_startwith, false))  return false;
	if (String_IsEmpty(s_data))              return false;

	return (String_Compare( s_data,
                            s_startwith,
                            s_startwith.length,
                            is_case_sensitive) == 0);
}

constexpr
instant bool
String_Find(
	const String &s_data,
	const String &s_find,
	s64 *index_found = 0,
	s64  index_start = 0
) {
	s64 t_index_found = String_IndexOf(	s_data,
										s_find,
										index_start,
										true);

	if (t_index_found < 0) {
		/// useful?
		{
			t_index_found = s_data.length - index_start;

			if (t_index_found < 0)
				t_index_found = 0;

			if (index_found) *index_found = t_index_found;
		}
		return false;
	}

	if (index_found) *index_found = t_index_found;

	return true;
}

constexpr
instant bool
String_FindRev(
	const String &s_data,
	const String &s_find,
	s64 *pos_found =  0,
	s64  pos_start = -1,
	bool pos_after_find = false
) {
	s64 t_pos_found = String_IndexOfRev(s_data, s_find, true, pos_start);

	if (t_pos_found < 0) {
		if (pos_found) *pos_found = t_pos_found;
		return false;
	}

	if (pos_after_find)
		t_pos_found += s_find.length;

	if (pos_found) *pos_found = t_pos_found;

	return true;
}

/// @Risk: might corrupt / break reallocation
///        when the length is shorter than
///        the already allocated buffer
constexpr
instant void
String_Cut(
	String &s_data,
	u32 length
) {
    Assert(!s_data.is_reference);

	if (length < s_data.length) {
		s_data.length = length;
		s_data.has_changed = true;
	}
}

constexpr
instant bool
String_EndWith(
	const String &s_data,
	const String &s_endwith,
	bool is_case_sensitive
) {
	if (s_endwith.length > s_data.length)
		return false;

	String ts_data = s_data;
    String_AddOffset(ts_data, (ts_data.length - s_endwith.length));
	Assert(ts_data.length = s_endwith.length);

	return (String_Compare(	ts_data,
							s_endwith,
							s_endwith.length,
							is_case_sensitive) == 0);
}

constexpr
instant void
String_ToLower(
	String &s_data
) {
	Assert(!s_data.is_reference);

	u64 index = 0;
	u64 len_max = s_data.length + 1;

	while(index < len_max) {
        String_ToLower(s_data.value[index]);
		++index;
	}

	s_data.has_changed = true;
}

constexpr
instant void
String_ToUpper(
	String &s_data
) {
	Assert(!s_data.is_reference);

	u64 index = 0;
	u64 len_max = s_data.length + 1;

	while(index < len_max) {
        String_ToUpper(s_data.value[index]);
		++index;
	}

	s_data.has_changed = true;
}

/// no utf support
constexpr
instant void
String_Reverse(
	String &s_data
) {
    Assert(!s_data.is_reference);

	u64 length = s_data.length;

	for(u64 it = 0; it < (length >> 1); ++it) {
		char temp = s_data.value[it];
		/// don't reverse the '\0'!
		s_data.value[it] = s_data.value[length - it - 1];
		s_data.value[length - it - 1] = temp;
	}

	s_data.has_changed = true;
}

constexpr
instant void
String_TrimLeft(
	String &s_data
) {
	if (s_data.is_reference) {
		while(!String_IsEmpty(s_data)) {
			char ch = s_data.value[0];

			switch (ch) {
				case ' ':
				case '\r':
				case '\n':
				case '\t': {
					String_AddOffset(s_data, 1);
					continue;
				} break;
			}

			break;
		}
	}
	else {
		String s_data_it = S(s_data);

		u64 trim_length = 0;

		while(!String_IsEmpty(s_data_it)) {
			char ch = s_data_it.value[0];

			switch (ch) {
				case ' ':
				case '\r':
				case '\n':
				case '\t': {
					++trim_length;
					String_AddOffset(s_data_it, 1);
					continue;
				} break;
			}

			break;
		}

		if (trim_length) {
			s_data.length -= trim_length;
			Memory_Copy(s_data.value, s_data.value + trim_length, s_data.length);
			s_data.has_changed = true;
		}
	}
}

constexpr
instant void
String_TrimRight(
	String &s_data
) {
	if (String_IsEmpty(s_data, false))
		return;

	u64 length = s_data.length;

	while(length > 0) {
		if (    s_data.value[length - 1] <= 32
			AND s_data.value[length - 1] != 127
		) {
			--length;
		}
		else {
			break;
		}
	}

	s_data.length  = length;
	s_data.has_changed = true;
}

constexpr
instant void
String_Trim(
	String &s_data
) {
	String_TrimLeft(s_data);
	String_TrimRight(s_data);
}

constexpr
instant u64
String_Remove(
	String &s_data,
	s64 index_start,
	s64 index_end
) {
	Assert(!s_data.is_reference);

	Assert(index_start >= 0);
	Assert(index_end   >= 0);

	if (index_start == index_end)
		return 0;

	if (index_start > index_end)
		SWAP(s64, &index_start, &index_end);

	if (index_end > (s64)s_data.length)
		index_end = s_data.length;

	u64 length = s_data.length - index_end;
	u64 rm_count = (index_end - index_start);

	Memory_Copy(s_data.value + index_start, s_data.value + index_end, length);
	s_data.length -= rm_count;

	s_data.has_changed = true;

	return rm_count;
}

constexpr
instant void
String_Replace(
	String &s_data,
	const String &s_find,
	const String &s_replace
) {
	Assert(!s_data.is_reference);

	s64 index_found = 0;
	s64 index_start = 0;

  	while(String_Find(s_data, s_find, &index_found, index_start)) {
		String_Remove(s_data, index_found, index_found + s_find.length);
		String_Insert(s_data, s_replace, index_found);
		index_start += index_found + s_replace.length;
	}

	s_data.has_changed = true;
}

constexpr
instant void
String_RemoveLineBreaks(
	String &s_data
) {
	Assert(!s_data.is_reference);

	{ // "\r"
		String s_find = S("\r");

		s64 index_found = 0;

		while(String_Find(s_data, s_find, &index_found, index_found)) {
			String_Remove(s_data, index_found, index_found + s_find.length);
		}
	}

	{ // "\n"
		String s_find = S("\n");

		s64 index_found = 0;

		while(String_Find(s_data, s_find, &index_found, index_found)) {
			String_Remove(s_data, index_found, index_found + s_find.length);
		}
	}

	s_data.has_changed = true;
}

constexpr
instant void
String_Cut(
	String &s_data,
	const String &s_start,
	const String &s_end
) {
    Assert(!s_data.is_reference);

	s64 start = 0, end = 0;

	while (true) {
		start = String_IndexOf(s_data, s_start, 0, true);

		if (start < 0)
			break;

		end = String_IndexOf(s_data, s_end, start, true);

		if (end > start)
			String_Remove(s_data, start, end + s_end.length);
		else
			break;
	}
}

constexpr
instant s32
String_GetCodepoint(
	const String &s_data,
	s32 *utf_byte_count
) {
	Assert(utf_byte_count);

	if (String_IsEmpty(s_data)) {
		LOG_WARNING("No string available to read codepoint from.");
		return 0;
	}

	return UTF8_ToCodepoint(s_data, utf_byte_count);
}

constexpr
instant s32
String_GetCodepointAtIndex(
	const String &s_data,
	u64 index,
	s32 *utf_byte_count
) {
	Assert(utf_byte_count);

	if (String_IsEmpty(s_data)) {
		LOG_WARNING("No string available to read codepoint from.");
		return 0;
	}

	if (index >= s_data.length)
		index = s_data.length - 1;

	String s_data_it = S(s_data);
	String_AddOffset(s_data_it, index);

	s32 codepoint = UTF8_ToCodepoint(s_data_it, utf_byte_count);

	return codepoint;
}

constexpr
instant s64
String_Insert(
	String &s_data,
	const char c_data,
	u64 index_start
) {
	Assert(!s_data.is_reference);

	s32 length = 0;

	if (c_data == '\b') {
		/// begin after the to-be-removed character
		if (index_start) {
			length = 0;

			/// nothing to remove
			if (String_IsEmpty(s_data))
				return 0;

			/// overflow prevention
			if (index_start > s_data.length)
				index_start = s_data.length;

			bool is_return = false;

			/// remove 2 chars, if it is line-break
			/// ---------------------------------------------------------------
			if (s_data.value[index_start + length - 1] == '\n') {
				--length;
				is_return = true;
			}

			if (s_data.value[index_start + length - 1] == '\r') {
				--length;

				/// index between '\r\n'
				/// -> if '\r' should be removed, remove '\n' too
				if (!is_return AND (index_start < s_data.length)) {
					if (s_data.value[index_start + length + 1] == '\n') {
						--length;
						++index_start;
					}
				}
			}

			/// no linebreak chars to remove -> check for utf-8 bytes
			if (!length) {
				s32 utf_byte_count = 0;
				String_GetCodepointAtIndex(s_data, index_start - 1, &utf_byte_count);
 				length = -utf_byte_count;

				s64 index_end = index_start + length;

 				if (index_end < 0)
					index_start = index_start - index_end;
			}

			String_Remove(s_data, index_start, index_start + length);
		}
	}
	else
	if (c_data == '\r' OR c_data == '\n') {
		length = 1;
 		String_Insert(s_data, S("\n"), index_start);
	}
	else {
		length = 1;

		String s_data;
		s_data.value   = (char *)&c_data;
		s_data.length  = length;
		s_data.has_changed = true;
		s_data.is_reference = true;

		String_Insert(s_data, s_data, index_start);
	}

	s_data.has_changed = true;

	return length;
}

constexpr
instant u64
String_CalcWordCount(
	const String &s_data
) {
	u64 count_words = 0;

	FOR(s_data.length, it) {
		char value = s_data.value[it];

		if ((value == ' ') OR (value == '\n'))
			++count_words;
	}

	if (s_data.length AND !count_words)
		++count_words;

	return count_words;
}

/// 5 true:  12345+6=6
/// 5 false: 12345+6=23456
constexpr
instant bool
String_AppendCircle(
	String &s_data,
	const String &s_input,
	u32 buffer_limit,
	bool reset_full_buffer
) {
	Assert(!s_data.is_reference);

	if (String_IsEmpty(s_input, false))  return false;
	if (!buffer_limit)  	             return false;

	if (s_data.length + s_input.length > buffer_limit) {
		if (reset_full_buffer) {
			String_Clear(s_data);
		}
		else {
			u32 diff = (s_data.length + s_input.length) - buffer_limit;
			String_Remove(s_data, 0, diff);
		}
	}

	String_Append(s_data, s_data);

	return (s_data.length == buffer_limit);
}

constexpr
instant void
String_Overwrite(
	String &s_dest,
	const String &s_source
) {
	String_Clear(s_dest);
	String_Append(s_dest, s_source);
}

constexpr
instant void
String_Flush(
	String &s_data
) {
	Memory_Set(s_data.value, 0, s_data.length);
}

constexpr
instant bool
String_HasChanged(
    String &s_data,
	bool reset_status
) {
	/// it is not enouth to change the string container,
	/// the content of the string could have been modified,
	/// but f.e. could have kept the same length and storage
	/// location
	bool result = s_data.has_changed;

	if (reset_status)
		s_data.has_changed = false;

	return result;
}

/// operator
/// string - string
constexpr
bool
operator == (
	const String &s_data1,
	const String &s_data2
) {
    bool is_case_sensitive = (s_data1.is_case_sensitive && s_data2.is_case_sensitive);

	return String_IsEqual(s_data1, s_data2, 0, is_case_sensitive);
}

constexpr
bool
operator != (
	const String &s_data1,
	const String &s_data2
) {
	return !(s_data1 == s_data2);
}

constexpr
bool
operator < (
	const String &s_data1,
	const String &s_data2
) {
	u64 length = MIN(s_data1.length, s_data2.length);

	FOR(length, it) {
		if (s_data1.value[it] < s_data2.value[it]) return true;
		if (s_data1.value[it] > s_data2.value[it]) return false;
	}

	if (s_data1.length < s_data2.length)  return true;

	return false;
}

constexpr
bool
operator > (
	const String &s_data1,
	const String &s_data2
) {
	u64 length = MIN(s_data1.length, s_data2.length);

	FOR(length, it) {
		if (s_data1.value[it] > s_data2.value[it]) return true;
		if (s_data1.value[it] < s_data2.value[it]) return false;
	}

	if (s_data1.length > s_data2.length)  return true;

	return false;
}

constexpr
bool
operator <= (
	const String &s_data1,
	const String &s_data2
) {
	return !(s_data1 > s_data2);
}

constexpr
bool
operator >= (
	const String &s_data1,
	const String &s_data2
) {
	return !(s_data1 < s_data2);
}

/// string - const char *
constexpr
bool
operator == (
	const String &s_data1,
	const char	 *c_data2
) {
 	String ts_data2 = S(c_data2);
	return (String_Compare(s_data1, ts_data2, ts_data2.length, true) == 0);
}

constexpr
bool
operator == (
	const char 	 *c_data1,
	const String &s_data2
) {
	return (s_data2 == c_data1);
}

constexpr
bool
operator != (
	const String &s_data1,
	const char	 *c_data2
) {
	return !(s_data1 == c_data2);
}

constexpr
bool
operator != (
	const char 	 *c_data1,
	const String &s_data2
) {
	return !(s_data2 == c_data1);
}

/// ::: Dependencies
/// ===========================================================================
/// - String_GetDelimiterSection -> Array
