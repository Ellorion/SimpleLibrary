#include "src/SLib.h"
#include "src/test/core.h"

int main() {
	String s_option;
	String s_path;
	String s_word;
	String s_number;
	bool   is_true;

	Parser parser = Parser_Load(
		"   #abc\n"
		"   settings  \n"
		"path: \"D:/temp folder/\" hello world #blub\r\n"
		"#test comment\r\n"
		"exit: true\r\n"
		"12345 -67.89"
	);

	Parser_IsString(&parser, "settings");

	Parser_GetStringRef(&parser, &s_option, ":");
	Parser_GetStringRef(&parser, &s_path);
	String_Print(&s_option);
	String_PrintLine(&s_path);

	Parser_GetStringRef(&parser, &s_word);
	String_PrintLine(&s_word);

	Parser_GetStringRef(&parser, &s_word);
	String_PrintLine(&s_word);

	Parser_GetStringRef(&parser, &s_option, ":");
	String_PrintLine(&s_option);

	Parser_GetBoolean(&parser, &is_true);

	Parser_GetNumber(&parser, &s_number);
	String_PrintLine(&s_number);

	Parser_GetNumber(&parser, &s_number);
	String_PrintLine(&s_number);

	if (Parser_HasError(&parser))
		String_PrintLine(&parser.s_error);

	return 0;
}
