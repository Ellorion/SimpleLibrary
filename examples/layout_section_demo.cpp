#include "src/SLib.h"
#include "test/core.h"

int main() {
	Keyboard keyboard;
	Mouse mouse;

	Window window = Window_Create("Layout-Section-Demo", 800, 480, true, true, &keyboard, &mouse);
	OpenGL_UseVSync(&window, true);

	ShaderSet 	shader_set 	= ShaderSet_Create(&window);
	Font 		font 		= Font_Load(S("default.ttf"), 20);

	Widget wg_top_text		= Widget_CreateTextBox(&window, &font, {}, true);

	Widget wg_left_1_1		= Widget_CreateButton( &window, &font, {}, S("L1_1"));
	Widget wg_left_1_2		= Widget_CreateButton( &window, &font, {}, S("L1_2"));
	Widget wg_left_text_1	= Widget_CreateTextBox(&window, &font, {}, true);

	Widget wg_left_2_1		= Widget_CreateButton( &window, &font, {}, S("L2_1"));
	Widget wg_left_2_2		= Widget_CreateButton( &window, &font, {}, S("L2_2"));
	Widget wg_left_text_2	= Widget_CreateTextBox(&window, &font, {}, true);

	Widget wg_left_3_1		= Widget_CreateButton( &window, &font, {}, S("L3_1"));
	Widget wg_left_3_2		= Widget_CreateButton( &window, &font, {}, S("L3_2"));
	Widget wg_left_text_3	= Widget_CreateTextBox(&window, &font, {}, true);

	Widget wg_left_4_1		= Widget_CreateButton( &window, &font, {}, S("L4_1"));
	Widget wg_left_4_2		= Widget_CreateButton( &window, &font, {}, S("L4_2"));
	Widget wg_left_text_4	= Widget_CreateTextBox(&window, &font, {}, true);


	Array<Widget *> ap_widgets;
	Array_Add(&ap_widgets, &wg_top_text);

	Array_Add(&ap_widgets, &wg_left_1_1);
	Array_Add(&ap_widgets, &wg_left_1_2);
	Array_Add(&ap_widgets, &wg_left_text_1);

	Array_Add(&ap_widgets, &wg_left_2_1);
	Array_Add(&ap_widgets, &wg_left_2_2);
	Array_Add(&ap_widgets, &wg_left_text_2);

	Array_Add(&ap_widgets, &wg_left_3_1);
	Array_Add(&ap_widgets, &wg_left_3_2);
	Array_Add(&ap_widgets, &wg_left_text_3);

	Array_Add(&ap_widgets, &wg_left_4_1);
	Array_Add(&ap_widgets, &wg_left_4_2);
	Array_Add(&ap_widgets, &wg_left_text_4);


	Layout *layout_bottom;

	Layout layout;
	Layout_Create(&layout, {0, 0, 0, 0}, true);
	{
		/// top section
		{
			Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_BOTTOMRIGHT);
			Layout_Add(&layout, &wg_top_text);
		}
		Layout_ReserveSection(&layout, LAYOUT_SECTION_TOP, 100, &layout_bottom);

		/// bottom first section
		{
			Layout_CreateBlock(layout_bottom, LAYOUT_TYPE_Y, LAYOUT_DOCK_BOTTOMRIGHT);
			Layout_Add(layout_bottom, &wg_left_1_1);
			Layout_Add(layout_bottom, &wg_left_1_2);

			Layout_CreateBlock(layout_bottom, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT);
			Layout_Add(layout_bottom, &wg_left_text_1);
		}
		Layout_ReserveSection(layout_bottom, LAYOUT_SECTION_LEFT, 200, &layout_bottom);

		/// bottom second section
		{
			Layout_CreateBlock(layout_bottom, LAYOUT_TYPE_Y, LAYOUT_DOCK_BOTTOMRIGHT);
			Layout_Add(layout_bottom, &wg_left_2_1);
			Layout_Add(layout_bottom, &wg_left_2_2);

			Layout_CreateBlock(layout_bottom, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT);
			Layout_Add(layout_bottom, &wg_left_text_2);
		}
		Layout_ReserveSection(layout_bottom, LAYOUT_SECTION_LEFT, 200, &layout_bottom);

		/// bottom third section
		{
			Layout_CreateBlock(layout_bottom, LAYOUT_TYPE_Y, LAYOUT_DOCK_BOTTOMRIGHT);
			Layout_Add(layout_bottom, &wg_left_3_1);
			Layout_Add(layout_bottom, &wg_left_3_2);

			Layout_CreateBlock(layout_bottom, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT);
			Layout_Add(layout_bottom, &wg_left_text_3);
		}
		Layout_ReserveSection(layout_bottom, LAYOUT_SECTION_LEFT, 200, &layout_bottom);

		/// bottom forth section
		{
			Layout_CreateBlock(layout_bottom, LAYOUT_TYPE_Y, LAYOUT_DOCK_BOTTOMRIGHT);
			Layout_Add(layout_bottom, &wg_left_4_1);
			Layout_Add(layout_bottom, &wg_left_4_2);

			Layout_CreateBlock(layout_bottom, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT);
			Layout_Add(layout_bottom, &wg_left_text_4);
		}
	}

	MemorySegment_Add(&window.a_segments_reset, window.events);
	MemorySegment_Add(&window.a_segments_reset, font.events);
	MemorySegment_AddWidgets(&window.a_segments_reset, &ap_widgets);

	/// to calc. remaining layout region beforehand
	Layout_Rearrange(&layout, &window);

	Vertex vertex = Vertex_Create(VERTEX_RECT);
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
