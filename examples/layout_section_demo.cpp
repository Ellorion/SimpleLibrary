#include "src/SLib.h"
#include "test/core.h"

int main() {
	Keyboard keyboard;
	Mouse mouse;

	Window window = Window_Create("Layout-Section-Demo", 800, 480, true, true, &keyboard, &mouse);
	OpenGL_UseVSync(&window, true);

	ShaderSet 	shader_set 	= ShaderSet_Create(&window);
	Font 		font 		= Font_Load(S("default.ttf"), 20);

	Widget wg_left_1		= Widget_CreateButton(&window, &font, {}, S("L1"));
	Widget wg_left_2		= Widget_CreateButton(&window, &font, {}, S("L2"));
	Widget wg_left_3		= Widget_CreateButton(&window, &font, {}, S("L3"));
	Widget wg_left_4		= Widget_CreateButton(&window, &font, {}, S("L4"));
	Widget wg_left_text		= Widget_CreateTextBox(&window, &font, {}, true);

	Widget wg_right_1		= Widget_CreateButton(&window, &font, {}, S("R1"));
	Widget wg_right_2		= Widget_CreateButton(&window, &font, {}, S("R2"));
	Widget wg_right_3		= Widget_CreateButton(&window, &font, {}, S("R3"));
	Widget wg_right_4		= Widget_CreateButton(&window, &font, {}, S("R4"));
	Widget wg_right_text	= Widget_CreateTextBox(&window, &font, {}, true);

	Widget wg_top_1			= Widget_CreateButton(&window, &font, {}, S("T1"));
	Widget wg_top_2			= Widget_CreateButton(&window, &font, {}, S("T2"));
	Widget wg_top_3			= Widget_CreateButton(&window, &font, {}, S("T3"));
	Widget wg_top_4			= Widget_CreateButton(&window, &font, {}, S("T4"));
	Widget wg_top_text		= Widget_CreateTextBox(&window, &font, {}, true);

	Widget wg_bottom_1		= Widget_CreateButton(&window, &font, {}, S("B1"));
	Widget wg_bottom_2		= Widget_CreateButton(&window, &font, {}, S("B2"));
	Widget wg_bottom_3		= Widget_CreateButton(&window, &font, {}, S("B3"));
	Widget wg_bottom_4		= Widget_CreateButton(&window, &font, {}, S("B4"));
	Widget wg_bottom_text	= Widget_CreateTextBox(&window, &font, {}, true);

	Array<Widget *> ap_widgets;
	Array_Add(&ap_widgets, &wg_left_1);
	Array_Add(&ap_widgets, &wg_left_2);
	Array_Add(&ap_widgets, &wg_left_3);
	Array_Add(&ap_widgets, &wg_left_4);
	Array_Add(&ap_widgets, &wg_left_text);

	Array_Add(&ap_widgets, &wg_right_1);
	Array_Add(&ap_widgets, &wg_right_2);
	Array_Add(&ap_widgets, &wg_right_3);
	Array_Add(&ap_widgets, &wg_right_4);
	Array_Add(&ap_widgets, &wg_right_text);

	Array_Add(&ap_widgets, &wg_top_1);
	Array_Add(&ap_widgets, &wg_top_2);
	Array_Add(&ap_widgets, &wg_top_3);
	Array_Add(&ap_widgets, &wg_top_4);
	Array_Add(&ap_widgets, &wg_top_text);

	Array_Add(&ap_widgets, &wg_bottom_1);
	Array_Add(&ap_widgets, &wg_bottom_2);
	Array_Add(&ap_widgets, &wg_bottom_3);
	Array_Add(&ap_widgets, &wg_bottom_4);
	Array_Add(&ap_widgets, &wg_bottom_text);

	Layout layout;
	Layout_Create(&layout, {0, 0, 0, 0}, true);
	{
		/// left section
		{
			Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT, 0);
			Layout_Add(&layout, &wg_left_1);
			Layout_Add(&layout, &wg_left_2);

			Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT, 1);
			Layout_Add(&layout, &wg_left_3);
			Layout_Add(&layout, &wg_left_4);

			Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT);
			Layout_Add(&layout, &wg_left_text);
		}
		Layout_CreateSection(&layout, LAYOUT_SECTION_LEFT, 200);

		/// right section
		{
			Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT, -3);
			Layout_Add(&layout, &wg_right_1);
			Layout_Add(&layout, &wg_right_2);

			Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT, -1);
			Layout_Add(&layout, &wg_right_3);
			Layout_Add(&layout, &wg_right_4);

			Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT);
			Layout_Add(&layout, &wg_right_text);
		}
		Layout_CreateSection(&layout, LAYOUT_SECTION_RIGHT, 200);

		/// top section
		{
			Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT, -4);
			Layout_Add(&layout, &wg_top_1);
			Layout_Add(&layout, &wg_top_2);

			Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_BOTTOMRIGHT, -2);
			Layout_Add(&layout, &wg_top_3);
			Layout_Add(&layout, &wg_top_4);

			Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT);
			Layout_Add(&layout, &wg_top_text);
		}
		Layout_CreateSection(&layout, LAYOUT_SECTION_TOP, 100);

		/// bottom section
		{
			Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT);
			Layout_Add(&layout, &wg_bottom_1);
			Layout_Add(&layout, &wg_bottom_2);

			Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_BOTTOMRIGHT);
			Layout_Add(&layout, &wg_bottom_3);
			Layout_Add(&layout, &wg_bottom_4);

			Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT);
			Layout_Add(&layout, &wg_bottom_text);
		}
		Layout_CreateSection(&layout, LAYOUT_SECTION_BOTTOM, 100);
	}

	MemorySegment_Add(&window.a_segments_reset, window.events);
	MemorySegment_Add(&window.a_segments_reset, font.events);
	MemorySegment_AddWidgets(&window.a_segments_reset, &ap_widgets);

	/// to calc. remaining layout region beforehand
	Layout_Rearrange(&layout, &window);

	Vertex vertex = Vertex_Create();
	Vertex_AddRect32(&vertex, layout.section.rect_remaining, {0, 0, 1, 1});

	while(Window_IsRunning(&window)) {
		Window_ReadMessage(&window);
		Layout_Rearrange(&layout, &window);

		Widget_Update(&ap_widgets, &keyboard);

		if (keyboard.up[VK_ESCAPE])
			Window_Close(&window);

		OpenGL_ClearScreen();

		Widget_Render(&shader_set, &ap_widgets);

#if 0	/// set to 1 to make remaining layout section visible
		/// (excludes padding)
		///
		/// will NOT be correctly shown when resized
		ShaderSet_Use(&shader_set, SHADER_PROG_RECT);
		Rect_Render(&shader_set, &vertex);
#endif // 0
	}

	Window_Destroy(&window);
}
