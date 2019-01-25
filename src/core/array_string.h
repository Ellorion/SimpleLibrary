#pragma once

#define String_SplitRefBuffer	Array_SplitRefBuffer
#define String_SplitRef			Array_SplitRef

#define String_SplitBuffer		Array_SplitBuffer
#define String_Split			Array_Split

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
		c_length = String_GetLength(c_delimiter);

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
	u64 len_delim = String_GetLength(c_delimiter);

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

/// returns number of line-breaks
instant u64
Array_SplitWordsBuffer(
	String *s_data,
	Array<String> *as_words_out
) {
	Assert(s_data);
	Assert(as_words_out);

	MEASURE_START();

	if (as_words_out->max) {
		Array_Clear(as_words_out);
	}
	else {
		as_words_out->by_reference = true;
		Array_Reserve(as_words_out, String_CalcWordCount(s_data));
	}

	if (!s_data->length)
		return 0;

	String *s_element;

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

	s_element->value = s_data->value + index_start;

	/// for everything (left) that has no space or line-break
	if (index_end - index_start > 0)
		s_element->length = (index_end - index_start) + 1;
	else
	if (index_start == index_end)
		s_element->length = s_data->length - index_start;

	MEASURE_END("");

	return number_of_linebreaks;
}

instant bool
Array_FindFirstString(
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
	as_section = Array_Split(s_data, c_delimiter);

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