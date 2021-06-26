#include "src/SLib.h"
#include "test/core.h"

/// toggle auto-click with right-click (mouse)
/// then hover over the to-be-auto-clicked
/// area for repeated left-clicks (mouse)

/// toggle auto-click with right-click (mouse)
/// then hover over the to-be-auto-clicked
/// area for repeated left-clicks (mouse)

instant void
Window_HandleEvents(
	Window *window
) {
	Timer timer_autoclick;
	Time_Reset(&timer_autoclick);

	ShaderSet shader_set = ShaderSet_CreateAndSet(*window);

	Keyboard *keyboard = window->keyboard;
	Assert(keyboard);

	auto font_20 = Font_Create(*window, S("default.ttf"), 20, true);

	if (Font_HasError(&font_20)) {
		MessageBox(window->hWnd, font_20.s_error.value, 0, MB_OK);
		return;
	}

	Widget wg_autoclick = Widget_CreateButton(window, &font_20, S("Auto-Click"));

	Array<Widget *> ap_widgets;
	Array_Add(ap_widgets, &wg_autoclick);

	MemorySegment_AddWidgets(&window->a_segments_reset, &ap_widgets);

    Layout layout;
	Layout_Create(&layout, window, true);
	{
		Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT);
		Layout_Add(&layout, &wg_autoclick);
	}

	while(window->is_running) {
		/// Events
		/// ===================================================================
		Window_ReadMessage(window);
		Layout_Rearrange(&layout, window);

		Widget_Update(&ap_widgets, keyboard);

		if (keyboard->up[VK_ESCAPE])
			window->is_running = false;

		if (wg_autoclick.events.on_trigger_secondary)  {
			wg_autoclick.data.is_checked = !wg_autoclick.data.is_checked;

			if (wg_autoclick.data.is_checked)
				wg_autoclick.text.data.color = {0.0f, 0.6f, 0.0f, 1.0f};
			else
				wg_autoclick.text.data.color = {0.6f, 0.0f, 0.0f, 1.0f};
		}

		if (wg_autoclick.data.is_checked) {
			if (Time_HasElapsed(&timer_autoclick, 30)) {
				Mouse_AutoClickLeft();
			}
		}

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		Widget_Render(&shader_set, &ap_widgets);
	}

	Widget_Destroy(&ap_widgets);
	ShaderSet_Destroy(&shader_set);
}

APPLICATION_MAIN {
	Keyboard keyboard;
	Mouse    mouse;

	Window window = Window_CreateOGL("Auto-Clicker", 320, 240, true, &keyboard, &mouse);
	Window_AlwaysOnTop(&window);

	Window_HandleEvents(&window);

	Window_Destroy(&window);

	return 0;
}
