#include "src/SLib.h"
#include "src/test/core.h"

instant void
Window_HandleEvents(
	Window *window
) {
	MSG msg;
	bool running = true;

	Timer timer_fps;
	Time_Reset(&timer_fps);

	Timer timer_fps_log;
	Time_Reset(&timer_fps_log);

	ShaderSet shader_set = ShaderSet_Create(window);

	OpenGL_SetBlending(true);

	Keyboard *keyboard = window->keyboard;

	String s_font;
	String_Append(&s_font, "default.ttf");
	Font font_20 = Font_Load(&s_font, 20);

	Widget widget_listbox  = Widget_CreateListBox(window, &font_20, { 10,  20, 300, 200});
	Widget widget_click_me = Widget_CreateButton( window, &font_20, {320,  20, 100,  30}, "click me");
	Widget widget_exit     = Widget_CreateButton( window, &font_20, {320,  50, 100,  30}, "Exit");

	widget_click_me.trigger_autosize = false;
	widget_exit.trigger_autosize     = false;

	Array<Widget *> ap_widgets;
	Array_Add(&ap_widgets, &widget_listbox);
	Array_Add(&ap_widgets, &widget_click_me);
	Array_Add(&ap_widgets, &widget_exit);

	Widget_AddRow(&widget_listbox, "1");
	Widget_AddRow(&widget_listbox, "2");
	Widget_AddRow(&widget_listbox, "3");
	Widget_AddRow(&widget_listbox, "4");
	Widget_AddRow(&widget_listbox, "5");
	Widget_AddRow(&widget_listbox, "6");
	Widget_AddRow(&widget_listbox, "7");
	Widget_AddRow(&widget_listbox, "8");
	Widget_AddRow(&widget_listbox, "9");
	Widget_AddRow(&widget_listbox, "10");
	Widget_AddRow(&widget_listbox, "11");
	Widget_AddRow(&widget_listbox, "12");
	Widget_AddRow(&widget_listbox, "13");
	Widget_AddRow(&widget_listbox, "14");
	Widget_AddRow(&widget_listbox, "15");

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(msg, running, window);
		OpenGL_AdjustScaleViewport(window);

		/// hold shift-key to get reverse tab order
		Widget_Update(&ap_widgets, keyboard);

		if (keyboard->up[VK_ESCAPE] OR widget_exit.events.on_trigger)
			running = false;

		if (widget_click_me.events.on_trigger) {
			std::cout << "clicked" << std::endl;
		}

		if (widget_listbox.events.on_trigger) {
			String s_row_data = Widget_GetSelectedRow(&widget_listbox);

			LOG_DEBUG(s_row_data.value);

			String_Destroy(&s_row_data);
		}

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		Widget_Render(&shader_set, &ap_widgets);

		Window_UpdateAndResetInput(window);

		u32 fps = Time_GetFPS(&timer_fps);

		if (Time_HasElapsed(&timer_fps_log, 1000)) {
			LOG_DEBUG(fps << " fps");
		}
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
