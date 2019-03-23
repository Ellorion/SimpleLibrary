#include "src/SLib.h"
#include "test/core.h"

int main() {

String s_test = S(R"(
int main() {
	Window window;

	Keyboard keyboard;
	Mouse    mouse;

	Window_Create(&window, "Demo-Application", 512, 512 / 16 * 9, 32, &keyboard, &mouse);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
})");

	/// only automatic skipping will be disabled internally,
	/// manually skipping this way is still supported
	Parser parser = Parser_Load(s_test, false);
	Parser_SkipUntilToken(&parser);

	String s_token;
	while(Parser_IsRunning(&parser)) {
		Parser_Token_Get(&parser, &s_token);

		if (s_token == "window")
			String_Print(S("another_window"));
		else
			String_Print(s_token);
	}

	return 0;
}
