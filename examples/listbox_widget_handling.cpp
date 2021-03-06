#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(
	Window *window
) {
	Timer timer_fps;
	Time_Reset(&timer_fps);

	Timer timer_fps_log;
	Time_Reset(&timer_fps_log);

	ShaderSet shader_set = ShaderSet_Create(window);

	OpenGL_SetBlending(true);

	Keyboard *keyboard = window->keyboard;

	Font font_20 = Font_Load(S("default.ttf"), 20);

	Widget widget_listbox  = Widget_CreateListBox(window, &font_20, { 10,  20, 300, 200});
	Widget widget_click_me = Widget_CreateButton( window, &font_20, {320,  20, 100,  30}, S("click me"));
	Widget widget_exit     = Widget_CreateButton( window, &font_20, {320,  50, 100,  30}, S("Exit"));

	widget_click_me.trigger_autosize = false;
	widget_exit.trigger_autosize     = false;

	Array<Widget *> ap_widgets;
	Array_Add(&ap_widgets, &widget_listbox);
	Array_Add(&ap_widgets, &widget_click_me);
	Array_Add(&ap_widgets, &widget_exit);

	Widget_AddRowSingle(&widget_listbox, S("1"));
	Widget_AddRowSingle(&widget_listbox, S("2"));
	Widget_AddRowSingle(&widget_listbox, S("3"));
	Widget_AddRowSingle(&widget_listbox, S("4"));
	Widget_AddRowSingle(&widget_listbox, S("5"));
	Widget_AddRowSingle(&widget_listbox, S("6"));
	Widget_AddRowSingle(&widget_listbox, S("7"));
	Widget_AddRowSingle(&widget_listbox, S("8"));
	Widget_AddRowSingle(&widget_listbox, S("9"));
	Widget_AddRowSingle(&widget_listbox, S("10"));
	Widget_AddRowSingle(&widget_listbox, S("11"));
	Widget_AddRowSingle(&widget_listbox, S("12"));
	Widget_AddRowSingle(&widget_listbox, S("13"));
	Widget_AddRowSingle(&widget_listbox, S("14"));
	Widget_AddRowSingle(&widget_listbox, S("15"));

	MemorySegment_Add(&window->a_segments_reset, window->events);
	MemorySegment_Add(&window->a_segments_reset, font_20.events);
	MemorySegment_AddWidgets(&window->a_segments_reset, &ap_widgets);

	while(Window_IsRunning(window)) {
		/// Events
		/// ===================================================================
		Window_ReadMessage(window);

		/// hold shift-key to get reverse tab order
		Widget_Update(&ap_widgets, keyboard);

		if (keyboard->up[VK_ESCAPE] OR widget_exit.events.on_trigger)
			Window_Close(window);

		if (widget_click_me.events.on_trigger) {
			std::cout << "clicked" << std::endl;
		}

		if (widget_listbox.events.on_trigger) {
			String s_row_data = Widget_GetSelectedRowRef(&widget_listbox);

			LOG_DEBUG(s_row_data.value);
		}

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		Widget_Render(&shader_set, &ap_widgets);

		u32 fps = Time_GetFPS(&timer_fps);

		if (Time_HasElapsed(&timer_fps_log, 1000)) {
			LOG_DEBUG(fps << " fps");
		}
	}

	Widget_Destroy(&ap_widgets);

	ShaderSet_Destroy(&shader_set);
}

int main() {
	Keyboard keyboard;
	Mouse    mouse;

	Window window = Window_Create("Hello, World!", 800, 480, true, true, &keyboard, &mouse);

	OpenGL_UseVSync(&window, true);

	Window_HandleEvents(&window);

	Window_Destroy(&window);

	return 0;
}
