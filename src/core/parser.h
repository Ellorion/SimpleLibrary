#pragma once

#ifdef ARRAY_COUNT
#define 	PARSER_ARRAY_COUNT ARRAY_COUNT
#else
#define 	PARSER_ARRAY_COUNT(_array) \
			(sizeof(_array)/sizeof(_array[0]))
#endif

struct Parser {
	String s_data;
	String s_comment_identifier;

	bool has_error = false;
	String s_error;
};

enum PARSER_MODE_TYPE {
	PARSER_MODE_SEEK,
	PARSER_MODE_PEEK
};

instant bool
Parser_HasError(
	Parser *parser
) {
	Assert(parser);

	return parser->has_error;
}

instant bool
Parser_IsRunning(
	Parser *parser
) {
	Assert(parser);

	if (Parser_HasError(parser))
		return false;

	if (String_IsEmpty(&parser->s_data))
		return false;

	return true;
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

    bool is_comment = false;

    while(Parser_IsRunning(parser_io)) {
		char ch = parser_io->s_data.value[0];

		/// skip whitespaces
		if (ch == ' ' OR ch == '\t') {
			Parser_AddOffset(parser_io, 1);
			continue;
		}

		/// skip comment
		if (   parser_io->s_data.value == parser_io->s_comment_identifier
			OR is_comment
		) {
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
	String s_data,
	String s_comment_identifier_opt = S("")
) {
	Parser parser = {};

	parser.s_data               = S(s_data);
	parser.s_comment_identifier = s_comment_identifier_opt;

	if (!String_IsEmpty(&parser.s_comment_identifier))
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

	bool is_equal = String_IsEqual(parser_io->s_data, s_data, s_data.length);

	if (!is_equal) {
		parser_io->has_error = true;
		Assert(!parser_io->s_error.value);

		String_Append(&parser_io->s_error, S("String \""));
		String_Append(&parser_io->s_error, s_data);
		String_Append(&parser_io->s_error, S("\" not found"));

		return;
	}

	Parser_AddOffset(parser_io, s_data.length);

	if (!String_IsEmpty(&parser_io->s_comment_identifier))
		Parser_SkipUntilToken(parser_io);
}

instant void
Parser_GetStringRef(
	Parser *parser_io,
	String *s_data_out,
	String  s_until_match,
	PARSER_MODE_TYPE type
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

		if (!String_IsEmpty(&parser_io->s_comment_identifier))
			Parser_SkipUntilToken(parser_io);
	}
}

instant void
Parser_GetStringRef(
	Parser *parser_io,
	String *s_data_out,
	PARSER_MODE_TYPE type,
	bool include_quotes
) {
	Assert(parser_io);
	Assert(s_data_out);

	if (Parser_HasError(parser_io))
		return;

	String_Destroy(s_data_out);

	s64 offset_parser = 0;

	if (String_StartWith(&parser_io->s_data, S("\""), true)) {
		++offset_parser;
		Parser_AddOffset(parser_io, 1);

		s64 index_found;
		if (!String_Find(&parser_io->s_data, S("\""), &index_found)) {
			parser_io->has_error = true;
			Assert(!parser_io->s_error.value);

			String_Append(&parser_io->s_error, S("Could not parse string. No \" was found"));

			return;
		}

		*s_data_out = S(parser_io->s_data.value, index_found);

		if (include_quotes AND offset_parser) {
			/// include starting & ending '\"'
			String_AddOffset(s_data_out, -offset_parser);
			s_data_out->length += 1;
		}

		if (type == PARSER_MODE_PEEK) {
			/// reverse starting '\"'
			Parser_AddOffset(parser_io, -offset_parser);
		}
		else
		if (type == PARSER_MODE_SEEK) {
			/// include ending '\"'
			Parser_AddOffset(parser_io, index_found + 1);

			if (!String_IsEmpty(&parser_io->s_comment_identifier))
				Parser_SkipUntilToken(parser_io);
		}

		return;
	}

	*s_data_out = S(parser_io->s_data.value, 0);

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
		Parser_AddOffset(parser_io, -s_data_out->length);
	else
	if (type == PARSER_MODE_SEEK) {
		if (!String_IsEmpty(&parser_io->s_comment_identifier))
			Parser_SkipUntilToken(parser_io);
	}
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

	FOR(PARSER_ARRAY_COUNT(values_false), it) {
		String ts_value = S(values_false[it]);

		if (String_StartWith(&parser_io->s_data, ts_value, true)) {
			Parser_AddOffset(parser_io, ts_value.length);

			if (!String_IsEmpty(&parser_io->s_comment_identifier))
				Parser_SkipUntilToken(parser_io);

			*is_true_out = false;
			return;
		}
	}

	const char *values_true[] = {
		"1",
		"true"
	};

	FOR(PARSER_ARRAY_COUNT(values_true), it) {
		String ts_value = S(values_true[it]);

		if (String_StartWith(&parser_io->s_data, ts_value, true)) {
			Parser_AddOffset(parser_io, ts_value.length);

			if (!String_IsEmpty(&parser_io->s_comment_identifier))
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

	has_error |= String_EndWith(s_number_out, S("."), true);
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

	if (!String_IsEmpty(&parser_io->s_comment_identifier))
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
	Parser_GetStringRef(parser_io, &s_data, PARSER_MODE_PEEK, false);

	return String_StartWith(&s_data, s_section_id, true);
}

instant bool
Parser_GetSectionNameRef(
	Parser *parser_io,
	String *s_data_out,
	String s_section_id
) {
	Assert(parser_io);
	Assert(s_data_out);
	Assert(s_section_id.length);

	bool is_section = Parser_IsSection(parser_io, s_section_id);

	Parser_GetStringRef(parser_io, s_data_out, PARSER_MODE_SEEK, false);

	if (is_section)
		String_AddOffset(s_data_out, s_section_id.length);

	return is_section;
}

instant bool
Parser_Token_CanTokenize(
	char character
) {
	#define INCL(_value, _min, _max) \
		((_min) <= (_value) AND (_value) <= (_max))

	if (   INCL(character,   0,  44)
		OR INCL(character,  45,  47)
		OR INCL(character,  58,  64)
		OR INCL(character,  91,  91)
		OR INCL(character,  93,  94)
		OR INCL(character,  96,  96)
		OR INCL(character, 123, 127)
	) {
		return false;
	}

	return true;
}

instant void
Parser_Token_Peek(
	Parser *parser,
	String *s_token_out,
	bool include_quotes
) {
	Assert(parser);
	Assert(s_token_out);

	if (Parser_HasError(parser))
		return;

	if (!String_IsEmpty(&parser->s_comment_identifier))
		Parser_SkipUntilToken(parser);

	String s_data_it = S(parser->s_data);

	String_Destroy(s_token_out);
	s_token_out->is_reference = true;
	s_token_out->value        = s_data_it.value;

	while(!String_IsEmpty(&s_data_it)) {
		++s_token_out->length;

		if (s_data_it.length == 1)
			break;

		if (s_data_it.value[0] == '\"') {
			Parser_GetStringRef(parser, s_token_out, PARSER_MODE_PEEK, include_quotes);
			break;
		}

		/// since '\r' and '\n' are not tokenizeable
		/// with Parser_Token_CanTokenize, group
		/// them this way
		if (s_data_it.value[0] == '\r') {
			String_AddOffset(&s_data_it, 1);

			if (s_data_it.value[0] == '\n') {
				++s_token_out->length;
				String_AddOffset(&s_data_it, 1);
			}

			break;
		}

		if (!Parser_Token_CanTokenize(s_data_it.value[0]))
			break;

		if (!Parser_Token_CanTokenize(s_data_it.value[1]))
			break;

		String_AddOffset(&s_data_it, 1);
	}
}

instant void
Parser_Token_Get(
    Parser *parser,
    String *s_token_out,
    bool include_quotes
) {
	if (Parser_HasError(parser))
		return;

	Parser_Token_Peek(parser, s_token_out, include_quotes);
	Parser_AddOffset(parser, s_token_out->length);
}

instant bool
Parser_Token_IsMatch(
	Parser *parser,
	String  s_token
) {
	Assert(parser);

	if (Parser_HasError(parser))
		return false;

	String ts_token;
	Parser_Token_Peek(parser, &ts_token, false);

	if (!(s_token == ts_token)) {
		parser->has_error = true;
		String_Overwrite(&parser->s_error, S("Token in parser did not match expected result."));
		return false;
	}

	Parser_AddOffset(parser, s_token.length);

	return true;
}

instant void
Parser_SkipUntil(
	Parser *parser_io,
	String  s_find,
	bool    skip_past_token
) {
	Assert(parser_io);

	s64 index_found;

	if (Parser_HasError(parser_io))
		return;

	if (String_Find(&parser_io->s_data, s_find, &index_found, 0)) {
		Parser_AddOffset(parser_io, index_found);

		if (skip_past_token)
			Parser_AddOffset(parser_io, s_find.length);
	}
	else {
		Parser_AddOffset(parser_io, parser_io->s_data.length);
	}
}
