#pragma once

instant void
Test_Parser(
) {
	Parser parser = Parser_Load(S(R"(
		/:default
		text		FooBar
		path		"X:/app folder/"

		/:save
		path		"X:/app folder/saves"
		auto		true

		/:music
		enabled		false
		volume 		97.5

		/:default
		debug		false
	)"));

	s64 id_section = -1;

	const char *c_section_ident = "/:";

	const char *cfg_section[] = {
		"default",
		"save",
		"music"
	};

	String s_data;
	bool   is_true;

	u32 test_option_count  = 0;

	while(Parser_IsRunning(&parser)) {
		Parser_GetStringRef(&parser, &s_data, PARSER_MODE_SEEK, false);

		/// require sections
		if (!String_StartWith(&s_data, S(c_section_ident), true))
			continue;

		id_section = -1;

		/// remove section ident to find the section by name
		String_AddOffset(&s_data, String_GetLength(c_section_ident));

		FOR(ARRAY_COUNT(cfg_section), it) {
			if (s_data == cfg_section[it]) {
				id_section = it;
				break;
			}
		}

		/// sections
		/// ===================================================================
		switch (id_section) {
			case 0: {	/// /:default
				while(Parser_IsRunning(&parser)) {
					Parser_GetStringRef(&parser, &s_data, PARSER_MODE_PEEK, false);

					/// section switch
					if (String_StartWith(&s_data, S(c_section_ident), true))
						break;

					Parser_GetStringRef(&parser, &s_data, PARSER_MODE_SEEK, false);

					/// section options
					/// =======================================================
					if (s_data == "text") {
						Parser_GetStringRef(&parser, &s_data, PARSER_MODE_SEEK, false);
						test_option_count += 1;

						AssertMessage(	!parser.has_error
										AND s_data == "FooBar", "[Test] Parsing error");
					}

					if (s_data == "path") {
						Parser_GetStringRef(&parser, &s_data, PARSER_MODE_SEEK, false);
						test_option_count += 1;

						AssertMessage(	    !parser.has_error
										AND s_data == "X:/app folder/", "[Test] Parsing error");
					}

					if (s_data == "debug") {
						Parser_GetBoolean(&parser, &is_true);
						test_option_count += 1;

						AssertMessage(!parser.has_error, "[Test] Parsing error");
					}
				}
			} break;

			case 1: {	/// /:save
				while(Parser_IsRunning(&parser)) {
					Parser_GetStringRef(&parser, &s_data, PARSER_MODE_PEEK, false);

					/// section switch
					if (String_StartWith(&s_data, S(c_section_ident), true))
						break;

					Parser_GetStringRef(&parser, &s_data, PARSER_MODE_SEEK, false);

					/// section options
					/// =======================================================
					if (s_data == "path") {
						Parser_GetStringRef(&parser, &s_data, PARSER_MODE_SEEK, false);
						test_option_count += 1;

						AssertMessage(	    !parser.has_error
										AND s_data == "X:/app folder/saves", "[Test] Parsing error");
					}

					if (s_data == "auto") {
						Parser_GetBoolean(&parser, &is_true);
						test_option_count += 1;

						AssertMessage(!parser.has_error, "[Test] Parsing error");
					}
				}
			} break;

			case 2: {	/// /:music
				while(Parser_IsRunning(&parser)) {
					Parser_GetStringRef(&parser, &s_data, PARSER_MODE_PEEK, false);

					/// section switch
					if (String_StartWith(&s_data, S(c_section_ident), true))
						break;

					Parser_GetStringRef(&parser, &s_data, PARSER_MODE_SEEK, false);

					/// section options
					/// =======================================================
					if (s_data == "enabled") {
						Parser_GetBoolean(&parser, &is_true);
						test_option_count += 1;

						AssertMessage(!parser.has_error, "[Test] Parsing error");
					}

					if (s_data == "volume") {
						Parser_GetNumber(&parser, &s_data);
						test_option_count += 1;

						AssertMessage(	    !parser.has_error
										AND s_data == "97.5", "[Test] Parsing error");
					}

				}
			} break;
		}
	}

	AssertMessage(test_option_count == 7, "[Test] Section options were not parsed");
}
