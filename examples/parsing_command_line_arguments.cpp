#include "src/SLib.h"
#include "test/core.h"

APPLICATION_MAIN {
	Parser parser_args = Parser_Load(S(_cmd_text));

	String s_command_arg;
	while(Parser_IsRunning(&parser_args)) {
		Parser_GetStringRef(&parser_args, &s_command_arg, PARSER_MODE_SEEK, false);

		String_Print(S("Arg: "));
		String_PrintLine(s_command_arg);

		if (s_command_arg == "/test") {
			LOG_DEBUG("test worked");
		}
	}

	return 0;
}
