#pragma once

#define String_Split Array_Split

struct String {
	bool  is_reference  = false;
	bool  changed       = false;
	u64   length  = 0;
	char *value   = 0;
};

instant bool
String_IsEmpty(
	String *s_data
) {
	Assert(s_data);

	return (   s_data->length == 0
			OR s_data->value  == 0);
}

instant void
String_Print(
	String s_data
) {
	FOR(s_data.length, it) {
		std::cout << s_data.value[it];
	}
}

instant void
String_PrintLine(
	String s_data
) {
	String_Print(s_data);
	std::cout << std::endl;
}

/// does NOT support UTF-8
instant u64
String_GetLength(
	const char *c_data
) {
	u64 length = 0;

	if (c_data == 0)
		return length;

	while (*c_data++)
		++length;

	return length;
}

/// (temporary) string conversion
instant String
S(
	const char *c_data
) {
 	String s_data;

	s_data.value  = (char *)c_data;
	s_data.length = String_GetLength(c_data);

	s_data.is_reference = true;
	s_data.changed      = true;

	return s_data;
}

instant String
S(
	const char *c_data,
	u64 length
) {
 	String s_data;
	s_data.value  = (char *)c_data;
	s_data.length = length;

	s_data.is_reference = true;
	s_data.changed      = true;

	return s_data;
}

instant String
S(
	String s_data
) {
 	String s_data_it;
	s_data_it = s_data;

	s_data_it.is_reference = true;
	s_data_it.changed      = true;

	return s_data_it;
}

instant String
S(
	String s_data,
	u64 length
) {
 	String s_data_it;

	s_data_it.value  = s_data.value;
	s_data_it.length = length;

	s_data_it.is_reference = true;
	s_data_it.changed      = true;

	return s_data_it;
}

instant void
String_Destroy(
	String *s_data_io
) {
	Assert(s_data_io);

	if (!s_data_io->is_reference)
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

instant void
String_AddOffset(
	String *s_data_io,
	s64 offset
) {
	Assert(s_data_io);
	Assert(s_data_io->is_reference);

	s_data_io->value  += offset;
	s_data_io->length -= offset;
}

instant bool
String_Append(
	String *s_dest_io,
	String s_source,
	u64 length_append = 0
) {
	Assert(s_dest_io);
	Assert(!s_dest_io->is_reference);

    if (length_append == 0)
		length_append = s_source.length;

    if (length_append == 0)
		return false;

	Assert(length_append <= s_source.length);

	String_Resize(s_dest_io, length_append);
	Memory_Copy(s_dest_io->value + s_dest_io->length, s_source.value, length_append);

	s_dest_io->length += length_append;

	s_dest_io->changed = true;

	return true;
}

instant u64
String_Insert(
	String *s_dest_io,
	String  s_source,
	u64 index_start
) {
	Assert(s_dest_io);
 	Assert(!s_dest_io->is_reference);
	Assert(index_start <= s_dest_io->length);

	String s_dest_io_it = S(*s_dest_io);

	String s_buffer;
	String_Append(&s_buffer, s_dest_io_it, index_start);
 	String_Append(&s_buffer, s_source);

	String_AddOffset(&s_dest_io_it, index_start);
	String_Append(&s_buffer, s_dest_io_it);

	String_Destroy(s_dest_io);
	*s_dest_io = s_buffer;

	return s_source.length;
}

instant void
To_StringBuffer(
	String *s_data_out,
	const char *c_data,
	u64 c_length = 0
) {
	Assert(s_data_out);
	Assert(c_data);

	if (!c_length)
		c_length = String_GetLength(c_data);

	String s_data = S(c_data, c_length);

	String_Destroy(s_data_out);
	String_Append(s_data_out, s_data);
}

instant String
ToString(
	const char *c_data,
	u64 c_length = 0
) {
	Assert(c_data);

	String s_result;
	To_StringBuffer(&s_result, c_data, c_length);

	return s_result;
}

instant void
String_Clear(
	String *s_data_out
) {
	Assert(s_data_out);

	s_data_out->length = 0;
	s_data_out->changed = true;
}

/// does NOT add memory for '\0'
/// make sure you add an additional byte
/// in the buffer so you don't truncate the string
instant void
To_CString(
	char *c_buffer_out,
	u64   c_length,
	String *s_data
) {
	Assert(s_data);

	if (!c_length)
		return;

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

instant void
String_CreateBuffer(
	String *s_buffer_out,
	u32 buffer_size,
	bool is_reference
) {
	Assert(s_buffer_out);

	String_Resize(s_buffer_out, buffer_size);
	s_buffer_out->length  = buffer_size;
	s_buffer_out->changed = true;
	s_buffer_out->is_reference = is_reference;
}

instant char
String_ToLower(
	char value
) {
	if (value >= 'A' AND value <= 'Z')
		 return (value + 32);

	return value;
}

/// -1 = data_1 < data_2
///  0 = data_1 = data_2
/// +1 = data_1 < data_2
instant s32
String_Compare(
	String s_data_1,
	String s_data_2,
	u64 length_opt,
	bool is_case_sensitive
) {
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

instant bool
String_IsEqual(
	const char *c_first,
	const char *c_second,
	u64 length = 0
) {
	return (String_Compare(S(c_first), S(c_second), length, true) == 0);
}

instant bool
String_IsEqual(
	String s_first,
	String s_second,
	u64 length = 0
) {
	return (String_Compare(s_first, s_second, length, true) == 0);
}

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

	s_result.changed      = true;
	s_result.is_reference = false;

	return s_result;
}

instant String
String_Copy(
	String s_data
) {
	return String_Copy(s_data.value, s_data.length);
}

instant char *
String_CreateCBufferCopy(
	String s_source
) {
	char *c_buffer = Memory_Create(char, s_source.length + 1);
	Memory_Copy(c_buffer, s_source.value, s_source.length);

	return c_buffer;
}

instant s64
String_IndexOf(
	String *s_data,
	String  s_key,
	s64 index_start = 0
) {
	Assert(s_data);

	int result = -1;

	if (!s_data OR !s_data->length)
		return result;

	if (s_key.length == 0)
		return result;

	u64 length_data = s_data->length;

	if (index_start < 0)
		index_start = 0;

	FOR_START(index_start, length_data, index) {
		if (String_IsEqual(s_data->value + index, s_key.value, s_key.length)) {
			return index;
		}
	}

	return result;
}

instant s64
String_IndexOfRev(
	String *s_data,
	String  s_key,
	s64 index_start = -1
) {
	int result = -1;

	if (!s_data)
		return result;

	if (!s_data->length)
		return result;

	if (s_key.length == 0)
		return result;

	/// index_start "starts" at the end of a string,
	/// and has to be converted from count
	/// (which is +1 of an index)
	if (index_start > (s64)s_data->length OR index_start < 0)
		index_start = (s64)s_data->length - 1;

	for(s64 it = index_start; it >= 0; --it) {
		if (String_IsEqual(s_data->value + it, s_key.value, s_key.length)) {
			return it;
		}
	}

	return result;
}

instant bool
String_StartWith(
	String *s_data,
	String  s_startwith,
	bool is_case_sensitive
) {
	if (!s_startwith.length OR !s_data OR !s_data->length)
		return false;

	return (String_Compare(*s_data, s_startwith, s_startwith.length, is_case_sensitive) == 0);
}

instant bool
String_Find(
	String *s_data,
	String  s_find,
	s64 *index_found = 0,
	s64  index_start = 0
) {
	Assert(s_data);

	s64 t_index_found = String_IndexOf(s_data, s_find, index_start);

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
	String s_find,
	s64 *pos_found = 0,
	s64  pos_start = 0,
	bool pos_after_find = false
) {
	Assert(s_data);

	s64 t_pos_found = String_IndexOfRev(s_data, s_find);

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
instant void
String_Cut(
	String *s_data_io,
	u32 length
) {
	Assert(s_data_io);
	Assert(!s_data_io->is_reference);

	if (s_data_io AND length < s_data_io->length) {
		s_data_io->length  = length;
		s_data_io->changed = true;
	}
}

instant bool
String_EndWith(
	String *s_data,
	String s_endwith,
	bool is_case_sensitive
) {
	if (s_endwith.length > s_data->length)
		return false;

	String ts_data = *s_data;
	ts_data.value = ts_data.value + (ts_data.length - s_endwith.length);
	ts_data.length = s_endwith.length;

	return (String_Compare(ts_data, s_endwith, s_endwith.length, is_case_sensitive) == 0);
}

instant void
String_ToLower(
	String *s_data_io
) {
	Assert(s_data_io);
	Assert(!s_data_io->is_reference);

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
	Assert(s_data_io);
	Assert(!s_data_io->is_reference);

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
	Assert(s_data_io);
	Assert(!s_data_io->is_reference);

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
	Assert(!s_data_io->is_reference);

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
	Assert(!s_data_io->is_reference);

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
	Assert(!s_data_io->is_reference);

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
	String *s_data_io,
	String  s_find,
	String  s_replace
) {
	Assert(s_data_io);
	Assert(!s_data_io->is_reference);

	s64 index_found = 0;
	s64 index_start = 0;

  	while(String_Find(s_data_io, s_find, &index_found, index_start)) {
		String_Remove(s_data_io, index_found, index_found + s_find.length);
		String_Insert(s_data_io, s_replace, index_found);
		index_start += index_found + s_replace.length;
	}

	s_data_io->changed = true;
}

instant s64
String_Insert(
	String *s_data_io,
	const char c_data,
	u64 index_start
) {
	Assert(s_data_io);
	Assert(!s_data_io->is_reference);

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
 		String_Insert(s_data_io, S("\n"), index_start);
	}
	else {
		length = 1;

		String s_data;
		s_data.value   = (char *)&c_data;
		s_data.length  = length;
		s_data.changed = true;
		s_data.is_reference = true;

		String_Insert(s_data_io, s_data, index_start);
	}

	s_data_io->changed = true;

	return length;
}

instant void
String_Cut(
	String *s_data_io,
	String s_start,
	String s_end
) {
	Assert(s_data_io);
	Assert(!s_data_io->is_reference);

	s64 start = 0, end = 0;

	while (true) {
		start = String_IndexOf(s_data_io, s_start);

		if (start < 0)
			break;

		end = String_IndexOf(s_data_io, s_end, start);

		if (end > start)
			String_Remove(s_data_io, start, end + s_end.length);
		else
			break;
	}
}

instant s32
String_GetCodepoint(
	String *s_data,
	u64 index
) {
	Assert(s_data);

	if (s_data->length == 0) {
		LOG_WARNING("No string available to read codepoint from.");
		return 0;
	}

	if (index >= s_data->length) {
		LOG_WARNING("Index out of range. Can not read codepoint from string.");
		return -1;
	}

	return s_data->value[index];
}

instant u64
String_CalcWordCount(
	String s_data
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
instant bool
String_AppendCircle(
	String *s_data_io,
	String  s_data,
	u32 buffer_limit,
	bool reset_full_buffer
) {
	Assert(s_data_io);
	Assert(!s_data_io->is_reference);

	if (!s_data.value)		return false;
	if (!s_data.length)    return false;
	if (!buffer_limit)  	return false;

	if (s_data_io->length + s_data.length > buffer_limit) {
		if (reset_full_buffer) {
			String_Clear(s_data_io);
		}
		else {
			u32 diff = (s_data_io->length + s_data.length) - buffer_limit;
			String_Remove(s_data_io, 0, diff);
		}
	}

	String_Append(s_data_io, s_data);

	return (s_data_io->length == buffer_limit);
}

instant void
String_Overwrite(
	String *s_dest_io,
	String  s_source
) {
	Assert(s_dest_io);

	String_Clear(s_dest_io);
	String_Append(s_dest_io, s_source);
}

/// operator
/// string - string
bool
operator == (
	String &s_data1,
	String &s_data2
) {
	return String_IsEqual(s_data1, s_data2);
}

bool
operator != (
	String &s_data1,
	String &s_data2
) {
	return !String_IsEqual(s_data1, s_data2);
}

bool
operator < (
	String &s_data1,
	String &s_data2
) {
	u64 length = MIN(s_data1.length, s_data2.length);

	FOR(length, it) {
		if (s_data1.value[it] < s_data2.value[it]) return true;
		if (s_data1.value[it] > s_data2.value[it]) return false;
	}

	if (s_data1.length < s_data2.length)  return true;

	return false;
}

bool
operator > (
	String &s_data1,
	String &s_data2
) {
	u64 length = MIN(s_data1.length, s_data2.length);

	FOR(length, it) {
		if (s_data1.value[it] > s_data2.value[it]) return true;
		if (s_data1.value[it] < s_data2.value[it]) return false;
	}

	if (s_data1.length > s_data2.length)  return true;

	return false;
}

/// string - const char *
bool
operator == (
	String		&s_data1,
	const char	*c_data2
) {
	return String_IsEqual(s_data1, S(c_data2));
}

bool
operator == (
	const char 	*c_data1,
	String 		&s_data2
) {
	return (s_data2 == c_data1);
}

bool
operator != (
	String		&s_data1,
	const char	*c_data2
) {
	return !(s_data1 == c_data2);
}

bool
operator != (
	const char 	*c_data1,
	String 		&s_data2
) {
	return !(s_data2 == c_data1);
}

/// ::: Dependencies
/// ===========================================================================
/// - String_GetDelimiterSection -> Array
