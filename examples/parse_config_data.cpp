#include "src/SLib.h"
#include "src/test/core.h"

int main() {
	Parser parser = Parser_Load(R"(
		/:default
		text	FooBar
		path	"X:/test folder/"

		/:test
		dummy
	)");

	s64 id_section;

	String s_section;
	String s_option;
	String s_default_text;
	String s_default_folder;

	const char *c_section_ident = "/:";

	const char *cfg_section[] = {
		"default"
	};

	while(Parser_IsRunning(&parser)) {
		Parser_GetStringRef(&parser, &s_section);

		/// require sections
		if (!String_StartWith(&s_section, c_section_ident))
			continue;

		id_section = -1;

		/// remove section ident to find the section by name
		String_AddOffset(&s_section, String_GetLength(c_section_ident));

		FOR(ARRAY_COUNT(cfg_section), it) {
			if (String_IsEqual(&s_section, cfg_section[it])) {
				id_section = it;
				break;
			}
		}

		/// sections
		/// ===================================================================
		switch (id_section) {
			case 0: { /// /:default
				while(Parser_IsRunning(&parser)) {
					Parser_GetStringRef(&parser, &s_option);

					/// section switch
					if (String_StartWith(&s_option, c_section_ident))
						break;

					/// section options
					/// =======================================================
					if (String_IsEqual(&s_option, "text")) {
						Parser_GetStringRef(&parser, &s_default_text);

						String_PrintLine(&s_default_text);
					}

					if (String_IsEqual(&s_option, "path")) {
						Parser_GetStringRef(&parser, &s_default_folder);

						String_PrintLine(&s_default_folder);
					}
				}
			} break;
		}

	}

	return 0;
}
