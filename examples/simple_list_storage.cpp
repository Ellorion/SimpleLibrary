#include "../SimpleLibrary/src/SLib.h"

void Storage_Load(File &file, Array<String> *as_data) {
    String s_data;

    while (File_ReadUntil(file, &s_data, S("\n"))) {
        if (String_IsEmpty(s_data, true))
            break;

        Array_Add(*as_data, s_data);
    }
}

APPLICATION_MAIN {
    Keyboard keyboard;
    Mouse mouse;

    auto window = Window_CreateOGL("Simple List Storage", 800, 460, true, &keyboard, &mouse);
    auto shader_set = ShaderSet_CreateAndSet(window);
    auto font_16 = Font_Create(window, S("default.ttf"), 16, true);

    // main
	Widget wgLabelHeader = Widget_CreateLabel(&window, &font_16, S("Simple List Storage"), TEXT_ALIGN_X_MIDDLE);
	Widget wgList = Widget_CreateListBox(&window, &font_16);

	// input
	Widget wgLabelFilter = Widget_CreateLabel(&window, &font_16, S("Filter:"));
    Widget wgTextBoxFilter = Widget_CreateTextBox(&window, &font_16, false);

	Array<Widget *> ap_widgets;
	Array_Add(ap_widgets, &wgLabelHeader);
	Array_Add(ap_widgets, &wgList);
	Array_Add(ap_widgets, &wgLabelFilter);
	Array_Add(ap_widgets, &wgTextBoxFilter);
	MemorySegment_AddWidgets(&window.a_segments_reset, &ap_widgets);

	Layout layout;
	Layout_Create(&layout, {0, 0, window.width, window.height}, true);
	{
	    Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT, 1);
		Layout_Add(&layout, &wgLabelFilter);
		Layout_Add(&layout, &wgTextBoxFilter);
		Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT, 1);
		Layout_Add(&layout, &wgLabelHeader);
		Layout_Add(&layout, &wgList);
	}

	// load data
	auto file = File_Open(S("data.txt"), "rb");
	Storage_Load(file, Widget_GetListArray(wgList));
	File_Close(file);

	while(window.is_running) {
        Window_ReadMessage(&window);
        Layout_Rearrange(&layout, &window);

        Widget_Update(&ap_widgets, &keyboard);

        if (wgTextBoxFilter.events.on_text_changed) {
            auto filter = Widget_GetTextData(wgTextBoxFilter);
            String_Overwrite(wgList.data.s_row_filter, *filter);
        }

        if (wgTextBoxFilter.events.on_trigger) {
            if (keyboard.pressing[VK_CONTROL]) {
                Widget_AddRowSingle(&wgList, Widget_GetTextDataRef(wgTextBoxFilter));
                Widget_SetText(wgTextBoxFilter, S(""));
            }
        }

        if (    keyboard.pressing[VK_CONTROL]
            AND keyboard.is_up
            AND keyboard.key_sym == KEYSYM_CONTROL('f'))
        {
            Widget_SetFocus(wgTextBoxFilter);
        }

        if (Widget_HasFocusInclSub(&wgList)) {
            if (keyboard.pressing[VK_CONTROL] AND keyboard.up[VK_DELETE]) {
                auto a_list = Widget_GetListArray(wgList);
                Array_Remove(*a_list, Widget_GetSelectedRowID(wgList));
            }
        }

        if (keyboard.up[VK_ESCAPE])
			window.is_running = false;

        OpenGL_ClearScreen();
        Widget_Render(&shader_set, &ap_widgets);
	}

	// save data
	auto [stream, success] = Stream_Open(File_Open(S("data.txt"), "wb"));
	stream << Widget_GetListArray(wgList);
	Stream_Close(stream);

    return 0;
}
