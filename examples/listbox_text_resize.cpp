#include "src/SLib.h"
#include "test/core.h"

int main() {
	Keyboard keyboard;
	Mouse    mouse;

	Window window = Window_Create("Test-Application", 200, 80, true, true, &keyboard, &mouse);
	OpenGL_UseVSync(&window, true);

	ShaderSet shader_set = ShaderSet_Create(&window);
	Font      font       = Font_Load(S("default.ttf"), 20);

	Widget    wg_list    = Widget_CreateListBox(&window, &font, {});

	Array<Widget *> ap_widgets;
	Array_Add(&ap_widgets, &wg_list);

	Layout layout;
	Layout_Create(&layout, {0, 0, 0, 0}, true);
	{
		Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT);
		Layout_Add(&layout, &wg_list);
	}
	Layout_Rearrange(&layout, &window);

	Widget_AddRowSingle(&wg_list, S("1. row entry"));
	Widget_AddRowSingle(&wg_list, S("2. row entry"));
	Widget_AddRowSingle(&wg_list, S("3. row entry"));
	Widget_AddRowSingle(&wg_list, S("4. row entry"));

	MemorySegment_Add(&window.a_segments_reset, window.events);
	MemorySegment_Add(&window.a_segments_reset, font.events);
	MemorySegment_AddWidgets(&window.a_segments_reset, &ap_widgets);

	while (Window_IsRunning(&window)) {
		Window_ReadMessage(&window);

		if (window.events.on_size_changed) {
			Layout_Rearrange(&layout, &window);
		}

		if (keyboard.up[VK_ESCAPE])
			Window_Close(&window);

		/// 0xBB = VK_OEM_PLUS  in winuser.h
		/// 0xBD = VK_OEM_MINUS in winuser.h
		///
		/// but (apperently) it is not useable in windows 10
		/// and there is no alternative entry for that button
		/// in there. so...
		///
		/// thank you, microsoft
		{
			if (keyboard.up[0xBB])
				Font_SetSize(&font, font.size + 2);

			if (keyboard.up[0xBD])
				Font_SetSize(&font, font.size - 2);
		}

		Widget_Update(&ap_widgets, &keyboard);

		OpenGL_ClearScreen();

		Widget_Render(&shader_set, &ap_widgets);
	}

	Font_Destroy(&font);
	ShaderSet_Destroy(&shader_set);
	Window_Destroy(&window);

	return 0;
}
