#include "src/SLib.h"
#include "src/test/core.h"

int main() {
	String s_option;
	String s_path;
	String s_word;
	bool   is_true;

	Parser parser = Parser_Load(
		"   settings  \n"
		"path: \"D:/temp folder/\" hello world\r\n"
		"exit: true"
	);

	Parser_IsString(&parser, "settings");
	Parser_IsLinebreak(&parser);

	Parser_GetStringRef(&parser, &s_option, ":");
	Parser_GetStringRef(&parser, &s_path);
	String_Print(&s_option);
	String_PrintLine(&s_path);

	Parser_GetStringRef(&parser, &s_word);
	String_PrintLine(&s_word);

	Parser_GetStringRef(&parser, &s_word);
	String_PrintLine(&s_word);

	Parser_IsLinebreak(&parser);

	Parser_GetStringRef(&parser, &s_option, ":");
	String_PrintLine(&s_option);

	Parser_GetBoolean(&parser, &is_true);

	if (Parser_HasError(&parser))
		String_PrintLine(&parser.s_error);

	return 0;
}