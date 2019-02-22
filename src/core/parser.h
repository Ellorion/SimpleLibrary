#pragma once

struct Parser {
	String s_data;

	bool has_error = false;
	String s_error;
};

enum PARSER_MODE_TYPE {
	PARSER_MODE_SEEK,
	PARSER_MODE_PEEK
};

instant bool
Parser_IsRunning(
	Parser *parser
) {
	Assert(parser);

	if (parser->has_error)
		return false;

	if (parser->s_data.length <= 0)
		return false;

	return true;
}

instant bool
Parser_HasError(
	Parser *parser
) {
	Assert(parser);

	return parser->has_error;
}

instant void
Parser_AddOffset(
	Parser *parser_io,
	s64 offset
) {
	Assert(parser_io);

	parser_io->s_data.value  += offset;
	parser_io->s_data.length -= offset;
}

/// ignores whitespaces, linebreaks and
/// comments starting with # until newline
instant void
Parser_SkipUntilToken(
	Parser *parser_io
) {
    Assert(parser_io);

    bool is_comment    = false;

    while(Parser_IsRunning(parser_io)) {
		char ch = parser_io->s_data.value[0];

		/// skip whitespaces
		if (ch == ' ' OR ch == '\t') {
			Parser_AddOffset(parser_io, 1);
			continue;
		}

		/// skip comment
		if (ch == '#' OR is_comment) {
			is_comment = true;

			if (!(ch == '\r' OR ch == '\n')) {
				Parser_AddOffset(parser_io, 1);
				continue;
			}

			/// until newline
			is_comment = false;
		}

		/// skip newline
		if (ch == '\r' OR ch == '\n') {
			Parser_AddOffset(parser_io, 1);
			continue;
		}

		break;
    }
}

instant void
Parser_Destroy(
	Parser *parser_io
) {
	Assert(parser_io);

	String_Destroy(&parser_io->s_data);
	String_Destroy(&parser_io->s_error);
	parser_io->has_error = false;
}

instant Parser
Parser_Load(
	String s_data
) {
	Parser parser = {};

	parser.s_data         = s_data;
	parser.s_data.changed = true;
	parser.s_data.is_reference = true;

	Parser_SkipUntilToken(&parser);

	return parser;
}

instant void
Parser_IsString(
	Parser *parser_io,
	String s_data
) {
	Assert(parser_io);

	if (Parser_HasError(parser_io))
		return;

	bool is_equal = (parser_io->s_data == s_data);

	if (!is_equal) {
		parser_io->has_error = true;
		Assert(!parser_io->s_error.value);

		String_Append(&parser_io->s_error, S("String \""));
		String_Append(&parser_io->s_error, s_data);
		String_Append(&parser_io->s_error, S("\" not found"));

		return;
	}

	Parser_AddOffset(parser_io, s_data.length);
	Parser_SkipUntilToken(parser_io);
}

instant void
Parser_GetString(
	Parser *parser_io,
	String *s_data_out,
	String  s_until_match,
	PARSER_MODE_TYPE type = PARSER_MODE_SEEK
) {
	Assert(parser_io);
	Assert(s_data_out);

	if (Parser_HasError(parser_io))
		return;

	s64 index_found;
	if (!String_Find(&parser_io->s_data, s_until_match, &index_found)) {
		parser_io->has_error = true;
		Assert(!parser_io->s_error.value);

		String_Append(&parser_io->s_error, S("Could not parse string. No \""));
		String_Append(&parser_io->s_error, s_until_match);
		String_Append(&parser_io->s_error, S("\" was found."));

		return;
	}

	s_data_out->value   = parser_io->s_data.value;
	s_data_out->length  = index_found;
	s_data_out->changed = true;
	s_data_out->is_reference = true;

	if (type == PARSER_MODE_SEEK) {
		Parser_AddOffset(parser_io, index_found + s_until_match.length);
		Parser_SkipUntilToken(parser_io);
	}
}

instant void
Parser_GetString(
	Parser *parser_io,
	String *s_data_out,
	u64 start_index = 0,
	PARSER_MODE_TYPE type = PARSER_MODE_SEEK
) {
	Assert(parser_io);
	Assert(s_data_out);

	if (Parser_HasError(parser_io))
		return;

	if (String_StartWith(&parser_io->s_data, S("\""))) {
		Parser_AddOffset(parser_io, 1);

		s64 index_found;
		if (!String_Find(&parser_io->s_data, S("\""), &index_found)) {
			parser_io->has_error = true;
			Assert(!parser_io->s_error.value);

			String_Append(&parser_io->s_error, S("Could not parse string. No \" was found"));

			return;
		}

		s_data_out->value   = parser_io->s_data.value;
		s_data_out->length  = index_found;
		s_data_out->changed = true;
		s_data_out->is_reference = true;

		if (type == PARSER_MODE_PEEK) {
			/// reverse starting "\""
			Parser_AddOffset(parser_io, -1);
		}
		else
		if (type == PARSER_MODE_SEEK) {
			/// include ending "\""
			Parser_AddOffset(parser_io, index_found + 1);
			Parser_SkipUntilToken(parser_io);
		}

		return;
	}

	s_data_out->value   = parser_io->s_data.value + start_index;
	s_data_out->length  = 0;
	s_data_out->changed = true;
	s_data_out->is_reference = true;

	while(Parser_IsRunning(parser_io)) {
        char ch = parser_io->s_data.value[0];

		bool is_word_ending = false;
		is_word_ending |= (ch == ' ');
		is_word_ending |= (ch == '\t');
		is_word_ending |= (ch == '\r');
		is_word_ending |= (ch == '\n');

		if (is_word_ending)
			break;

		s_data_out->length += 1;

		Parser_AddOffset(parser_io, 1);
	}

	if (type == PARSER_MODE_PEEK)
		Parser_AddOffset(parser_io, -(s_data_out->length + start_index));
	else
	if (type == PARSER_MODE_SEEK)
		Parser_SkipUntilToken(parser_io);
}

instant void
Parser_GetBoolean(
	Parser *parser_io,
	bool *is_true_out
) {
	Assert(parser_io);
	Assert(is_true_out);

	if (Parser_HasError(parser_io))
		return;

	const char *values_false[] = {
		"0",
		"false"
	};

	FOR(ARRAY_COUNT(values_false), it) {
		String ts_value = S(values_false[it]);

		if (String_StartWith(&parser_io->s_data, ts_value)) {
			Parser_AddOffset(parser_io, ts_value.length);
			Parser_SkipUntilToken(parser_io);

			*is_true_out = false;
			return;
		}
	}

	const char *values_true[] = {
		"1",
		"true"
	};

	FOR(ARRAY_COUNT(values_true), it) {
		String ts_value = S(values_true[it]);

		if (String_StartWith(&parser_io->s_data, ts_value)) {
			Parser_AddOffset(parser_io, ts_value.length);
			Parser_SkipUntilToken(parser_io);

			*is_true_out = true;
			return;
		}
	}

	parser_io->has_error = true;
	Assert(!parser_io->s_error.value);

	String_Append(&parser_io->s_error, S("No valid boolean value could be parsed"));
}

instant void
Parser_GetNumber(
	Parser *parser_io,
	String *s_number_out
) {
	Assert(parser_io);
	Assert(s_number_out);

	if (Parser_HasError(parser_io))
		return;

	s_number_out->value   = parser_io->s_data.value;
	s_number_out->length  = 0;
	s_number_out->is_reference = true;

	bool has_error = false;
	bool found_dot = false;
	u64 index_parsing = 0;

	while(Parser_IsRunning(parser_io)) {
        char ch = parser_io->s_data.value[0];

        if (!IsNumeric(ch)) {
			bool is_valid = false;

        	if (index_parsing == 0 AND ch == '-')
				is_valid = true;

			if (ch == '.') {
				if (!found_dot) {
					is_valid  = true;
					found_dot = true;
				}
				else {
					has_error = true;
				}
			}

			if (!is_valid)
				break;
        }

		s_number_out->length += 1;
		Parser_AddOffset(parser_io, 1);

        index_parsing += 1;
	}

	has_error |= String_EndWith(s_number_out, S("."));
	has_error |= (s_number_out->length == 0);

	if (has_error) {
		parser_io->has_error = true;
		Assert(!parser_io->s_error.value);

		String_Append(&parser_io->s_error, S("No valid number could be parsed"));

		s_number_out->value  = 0;
		s_number_out->length = 0;

		Parser_AddOffset(parser_io, -index_parsing);

		return;
	}

	Parser_SkipUntilToken(parser_io);
}

instant bool
Parser_IsSection(
	Parser *parser_io,
	String s_section_id
) {
	Assert(parser_io);
	Assert(s_section_id.length);

	String s_data;
	Parser_GetString(parser_io, &s_data, 0, PARSER_MODE_PEEK);

	return String_StartWith(&s_data, s_section_id);
}

instant bool
Parser_GetSectionName(
	Parser *parser_io,
	String *s_data_out,
	String s_section_id
) {
	Assert(parser_io);
	Assert(s_data_out);
	Assert(s_section_id.length);

	bool is_section = Parser_IsSection(parser_io, s_section_id);

	Parser_GetString(parser_io, s_data_out);

	if (is_section)
		String_AddOffset(s_data_out, s_section_id.length);

	return is_section;
}
