#include "src/SLib.h"
#include "test/core.h"

int main() {
	Parser parser = Parser_Load(S(R"(
		/:default
		text	FooBar
		path	"X:/test folder/"

		/:test
		dummy
	)"));

	s64 id_section;

	String s_section;
	String s_option;
	String s_default_text;
	String s_default_folder;

	String s_section_ident = S("/:");

	const char *cfg_section[] = {
		"default",
		"test"
	};

	while(Parser_IsRunning(&parser)) {
		Parser_GetString(&parser, &s_section);

		/// require sections
		if (!String_StartWith(&s_section, s_section_ident))
			continue;

		id_section = -1;

		/// remove section ident to find the section by name
		String_AddOffset(&s_section, s_section_ident.length);

		FOR(ARRAY_COUNT(cfg_section), it) {
			if (String_IsEqual(s_section, S(cfg_section[it]))) {
				id_section = it;
				break;
			}
		}

		/// sections
		/// ===================================================================
		switch (id_section) {
			case 0: { /// /:default
				while(Parser_IsRunning(&parser)) {
					Parser_GetString(&parser, &s_option, PARSER_MODE_PEEK);

					/// section switch
					if (String_StartWith(&s_option, s_section_ident))
						break;

#if 0
					Parser_GetString(&parser, &s_option);
#else
					Parser_AddOffset(&parser, s_option.length);
					Parser_SkipUntilToken(&parser);
#endif

					/// section options
					/// =======================================================
					if (s_option == "text") {
						Parser_GetString(&parser, &s_default_text);

						String_PrintLine(&s_default_text);
					}

					if (s_option == "path") {
						Parser_GetString(&parser, &s_default_folder);

						String_PrintLine(&s_default_folder);
					}
				}
			} break;

			case 1: {
				LOG_DEBUG("lala")
			} break;
		}

	}

	return 0;
}
