#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(
	Window *window
) {
	MSG msg;
	bool running = true;

	ShaderSet shader_set = ShaderSet_Create(window);

	OpenGL_SetBlending(true);

	Keyboard *keyboard = window->keyboard;

	Font font_20 = Font_Load(S("default.ttf"), 20);

	Widget widget_label    = Widget_CreateLabel( window, &font_20, {  10,  20, 300, 200}, S("Label"));
	Widget widget_click_me = Widget_CreateButton(window, &font_20, { 320,  20, 100,  30}, S("click me"));
	Widget widget_exit     = Widget_CreateButton(window, &font_20, { 320,  50, 100,  30}, S("Exit"));

	Array<Widget *> ap_widgets;
	Array_Add(&ap_widgets, &widget_label);
	Array_Add(&ap_widgets, &widget_click_me);
	Array_Add(&ap_widgets, &widget_exit);

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(window, &msg, &running, false);
		OpenGL_AdjustScaleViewport(window);

		/// hold shift-key to get reverse order
		Widget_Update(&ap_widgets, keyboard);

		if (keyboard->up[VK_ESCAPE] OR widget_exit.events.on_trigger)
			running = false;

		if (widget_click_me.events.on_trigger) {
			std::cout << "clicked" << std::endl;
		}

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		Widget_Render(&shader_set, &ap_widgets);

		Window_UpdateAndResetInput(window);
	}

	Widget_Destroy(&ap_widgets);

	ShaderSet_Destroy(&shader_set);
}

int main() {
	Window window;

	Keyboard keyboard;
	Mouse    mouse;

	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard, &mouse);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
