#pragma once

struct Parser {
	String s_data;

	bool has_error = false;
	String s_error;
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
	const char *c_data,
	u64         c_length = 0
) {
  	Assert(c_data);

  	if (!c_length)
		c_length = String_GetLength(c_data);

	Parser parser = {};

	parser.s_data.value   = (char *)c_data;
	parser.s_data.length  = c_length;
	parser.s_data.changed = true;
	parser.s_data.is_reference = true;

	Parser_SkipUntilToken(&parser);

	return parser;
}

instant void
Parser_IsString(
	Parser *parser_io,
	const char *c_data,
	u64         c_length = 0
) {
	Assert(parser_io);
	Assert(c_data);

	if (Parser_HasError(parser_io))
		return;

	if (!c_length)
		c_length = String_GetLength(c_data);

	bool is_equal = String_IsEqual(&parser_io->s_data, c_data, c_length);

	if (!is_equal) {
		parser_io->has_error = true;
		Assert(!parser_io->s_error.value);

		String_Append(&parser_io->s_error, "String \"");
		String_Append(&parser_io->s_error, c_data, c_length);
		String_Append(&parser_io->s_error, "\" not found");

		return;
	}

	Parser_AddOffset(parser_io, c_length);
	Parser_SkipUntilToken(parser_io);
}

instant void
Parser_GetStringRef(
	Parser *parser_io,
	String *s_data_out,
	const char* c_until_match,
	u64         c_length = 0
) {
	Assert(parser_io);
	Assert(s_data_out);
	Assert(c_until_match);

	if (Parser_HasError(parser_io))
		return;

	if (!c_length)
		c_length = String_GetLength(c_until_match);

	s64 index_found;
	if (!String_Find(&parser_io->s_data, c_until_match, c_length, &index_found)) {
		parser_io->has_error = true;
		Assert(!parser_io->s_error.value);

		String_Append(&parser_io->s_error, "Could not parse string. No \"");
		String_Append(&parser_io->s_error, c_until_match, c_length);
		String_Append(&parser_io->s_error, "\" was found.");

		return;
	}

	s_data_out->value   = parser_io->s_data.value;
	s_data_out->length  = index_found;
	s_data_out->changed = true;
	s_data_out->is_reference = true;

	Parser_AddOffset(parser_io, index_found + c_length);

	Parser_SkipUntilToken(parser_io);
}

instant void
Parser_GetStringRef(
	Parser *parser_io,
	String *s_data_out
) {
	Assert(parser_io);
	Assert(s_data_out);

	if (Parser_HasError(parser_io))
		return;

	if (String_StartWith(&parser_io->s_data, "\"")) {
		Parser_AddOffset(parser_io, 1);

		s64 index_found;
		if (!String_Find(&parser_io->s_data, "\"", 0, &index_found)) {
			parser_io->has_error = true;
			Assert(!parser_io->s_error.value);

			String_Append(&parser_io->s_error, "Could not parse string. No \" was found");

			return;
		}

		s_data_out->value   = parser_io->s_data.value;
		s_data_out->length  = index_found;
		s_data_out->changed = true;
		s_data_out->is_reference = true;

		Parser_AddOffset(parser_io, index_found + 1);
		Parser_SkipUntilToken(parser_io);

		return;
	}

	s_data_out->value   = parser_io->s_data.value;
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

	/// has to stay in the defined order
	const char *values[] = {
		"0",
		"false",
		"1",
		"true"
	};

	FOR(4, it) {
		if (String_StartWith(&parser_io->s_data, values[it])) {
			Parser_AddOffset(parser_io, String_GetLength(values[it]));
			Parser_SkipUntilToken(parser_io);

			*is_true_out = (it == 2 OR it == 3);
			return;
		}
	}

	parser_io->has_error = true;
	Assert(!parser_io->s_error.value);

	String_Append(&parser_io->s_error, "No valid boolean value could be parsed");
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

	has_error |= String_EndWith(s_number_out, ".", 1);
	has_error |= (s_number_out->length == 0);

	if (has_error) {
		parser_io->has_error = true;
		Assert(!parser_io->s_error.value);

		String_Append(&parser_io->s_error, "No valid number could be parsed");

		s_number_out->value  = 0;
		s_number_out->length = 0;

		Parser_AddOffset(parser_io, -index_parsing);

		return;
	}

	Parser_SkipUntilToken(parser_io);
}
