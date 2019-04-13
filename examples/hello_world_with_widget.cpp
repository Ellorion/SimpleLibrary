#include "src/SLib.h"
#include "test/core.h"

int main() {
	Keyboard keyboard;
	Mouse    mouse;

	Window window = Window_Create(
		"Demo-Application",
		800, 480,
		true, true,
		&keyboard, &mouse
	);

	OpenGL_UseVSync(&window, true);

	/// Initialisation
	/// =======================================================================
	window.icon.always_visible = true;

	ShaderSet shader_set = ShaderSet_Create(&window);
	Font      font       = Font_Load(S("default.ttf"), 30);

	if (Font_HasError(&font)) {
		MessageBox(window.hWnd, font.s_error.value, 0, MB_OK);
		return 0;
	}

	Widget wg_label = Widget_CreateLabel(
		&window,
		&font,
		{},
		S("Hello, World!"),
		TEXT_ALIGN_X_MIDDLE
	);

	wg_label.text.data.align_y = TEXT_ALIGN_Y_CENTER;

	Array<Widget *> ap_widgets;
	Array_Add(&ap_widgets, &wg_label);

	Layout layout;
	Layout_Create(&layout, {0, 0, 800, 480}, true);
	{
		Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT);
		Layout_Add(&layout, &wg_label);
	}

	while(window.is_running) {
		/// Events
		/// ===================================================================
		Window_ReadMessage(&window);
		Layout_Rearrange(&layout, &window);

		Widget_Update(&ap_widgets, &keyboard);

		if (keyboard.up[VK_ESCAPE])
			window.is_running = false;

		/// Render
		/// ===================================================================
		if (Window_IsVisible(&window)) {
			OpenGL_ClearScreen();
			Widget_Render(&shader_set, &ap_widgets);
		}

		Window_Render(&window);
		Widget_Reset(&ap_widgets);
		Font_ResetEvents(&font);
	}

	Widget_Destroy(&ap_widgets);
	ShaderSet_Destroy(&shader_set);
	Window_Destroy(&window);

	return 0;
}
