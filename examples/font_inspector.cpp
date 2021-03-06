#include "src/SLib.h"
#include "test/core.h"

String s_text_sample = S("Lorem ipsum dolor sit amet, consetetur sadipscing elitr,"
						 " sed diam nonumy eirmod tempor invidunt ut labore et dolore"
						 " magna aliquyam erat, sed diam voluptua. At vero eos et"
						 " accusam et justo duo dolores et ea rebum. Stet clita"
						 " kasd gubergren, no sea takimata sanctus est Lorem ipsum"
						 " dolor sit amet. Lorem ipsum dolor sit amet, consetetur"
						 " sadipscing elitr, sed diam nonumy eirmod tempor invidunt"
						 " ut labore et dolore magna aliquyam erat, sed diam voluptua."
						 " At vero eos et accusam et justo duo dolores et ea rebum."
						 " Stet clita kasd gubergren, no sea takimata sanctus est"
						 " Lorem ipsum dolor sit amet. "
						 "Lorem ipsum dolor sit amet, consetetur sadipscing elitr,"
						 " sed diam nonumy eirmod tempor invidunt ut labore et dolore"
						 " magna aliquyam erat, sed diam voluptua. At vero eos et"
						 " accusam et justo duo dolores et ea rebum. Stet clita"
						 " kasd gubergren, no sea takimata sanctus est Lorem ipsum"
						 " dolor sit amet. Lorem ipsum dolor sit amet, consetetur"
						 " sadipscing elitr, sed diam nonumy eirmod tempor invidunt"
						 " ut labore et dolore magna aliquyam erat, sed diam voluptua."
						 " At vero eos et accusam et justo duo dolores et ea rebum."
						 " Stet clita kasd gubergren, no sea takimata sanctus est"
						 " Lorem ipsum dolor sit amet. ");


int main() {
	Keyboard keyboard;
	Mouse mouse;

	Window window = Window_Create("Font-Inspector", 800, 480, true, true, &keyboard, &mouse);
	OpenGL_UseVSync(&window, true);

	ShaderSet 	shader_set 		= ShaderSet_Create(&window);
	Font 		font_dynamic 	= Font_Load(S("default.ttf"), 20);

	String s_font_directory = S("./fonts");

	Widget wg_label 		= Widget_CreateLabel(  &window, &font_dynamic , {}, S("Dir: "));
	Widget wg_list  		= Widget_CreateListBox(&window, &font_dynamic , {});
	Widget wg_font_plus 	= Widget_CreateButton( &window, &font_dynamic , {}, S("+"));
	Widget wg_font_minus 	= Widget_CreateButton( &window, &font_dynamic , {}, S("-"));
	Widget wg_textbox 		= Widget_CreateTextBox(&window, &font_dynamic, {}, true);

	String *s_data_label = Widget_GetTextData(&wg_label);
	String_Append(s_data_label, s_font_directory);

	Text_UseWordWrap(&wg_textbox.text, true);

	Array<Widget *> ap_widgets;
	Array_Add(&ap_widgets, &wg_label);
	Array_Add(&ap_widgets, &wg_list);
	Array_Add(&ap_widgets, &wg_font_plus);
	Array_Add(&ap_widgets, &wg_font_minus);
	Array_Add(&ap_widgets, &wg_textbox);

	Layout *layout_remaining;

	Layout layout;
	Layout_Create(&layout, &window, true);
	{
		Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT);
		Layout_Add(&layout, &wg_font_minus);
		Layout_Add(&layout, &wg_font_plus);

		Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_BOTTOMRIGHT, 1);
		Layout_Add(&layout, &wg_label);
		Layout_Add(&layout, &wg_list);
	}
	Layout_ReserveSection(&layout, LAYOUT_SECTION_LEFT, 250, &layout_remaining);
	{
		Layout_CreateBlock(layout_remaining, LAYOUT_TYPE_Y, LAYOUT_DOCK_BOTTOMRIGHT);
		Layout_Add(layout_remaining, &wg_textbox);
	}

	MemorySegment_Add(&window.a_segments_reset, window.events);
	MemorySegment_Add(&window.a_segments_reset, font_dynamic.events);

#if 1
	MemorySegment_AddWidgets(&window.a_segments_reset, &ap_widgets);
#else
	/// alternative method
	Widget_ForEach(&ap_widgets, [&](Widget *widget) {
		MemorySegment_Add(&window.a_segments_reset, widget->events);
	});
#endif // 0

	Array<Directory_Entry> a_font_files;
	File_ReadDirectory(&a_font_files, s_font_directory, DIR_LIST_ONLY_FILES, true, "ttf");

	FOR_ARRAY(a_font_files, it) {
		Directory_Entry *dir = &ARRAY_IT(a_font_files, it);

		String s_font_file = S(dir->s_name);
		String_AddOffset(&s_font_file, s_font_directory.length);

		if (String_StartWith(&s_font_file, S("\\"), true))
			String_AddOffset(&s_font_file, 1);

		Widget_AddRowSingle(&wg_list, S(s_font_file, s_font_file.length -4));
	}

	String *ts_data = Widget_GetTextData(&wg_textbox);
	String_Overwrite(ts_data, s_text_sample);

	if (a_font_files.count) {
		u64 id = Widget_GetSelectedRowID(&wg_list);
		Directory_Entry *t_dir = &ARRAY_IT(a_font_files, id);

		s32 current_font_size = font_dynamic.size;
		Font_Destroy(&font_dynamic);
		font_dynamic = Font_Load(t_dir->s_name, current_font_size);
	}

	while(Window_IsRunning(&window)) {
		Window_ReadMessage(&window);
		Layout_Rearrange(&layout, &window);

		Widget_Update(&ap_widgets, &keyboard);

		if (wg_list.events.on_list_change_index) {
			if (a_font_files.count) {
				u64 id = Widget_GetSelectedRowID(&wg_list);
				Directory_Entry *t_dir = &ARRAY_IT(a_font_files, id);

				s32 current_font_size = font_dynamic.size;
				Font_Destroy(&font_dynamic);
				font_dynamic = Font_Load(t_dir->s_name, current_font_size);
			}
		}

		if (wg_font_plus.events.on_trigger)
			Font_SetSize(&font_dynamic, font_dynamic.size + 2);

		if (wg_font_minus.events.on_trigger)
			Font_SetSize(&font_dynamic, font_dynamic.size - 2);

		if (keyboard.up[VK_ESCAPE])
			Window_Close(&window);

		if (font_dynamic.events.on_size_changed) {
            Widget_ForEach(&ap_widgets, [](Widget *widget) {
				widget->trigger_autosize = true;
			});

			Widget_Update(&ap_widgets, &keyboard);
		}

		OpenGL_ClearScreen();

		Widget_Render(&shader_set, &ap_widgets);
	}

	Window_Destroy(&window);
}
