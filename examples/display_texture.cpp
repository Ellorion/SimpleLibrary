#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(
	Window *window
) {
	MSG msg;
	bool running = true;
	bool ui_zoom_enabled = true;

	Timer timer_fps;
	Time_Reset(&timer_fps);

	Timer timer_fps_log;
	Time_Reset(&timer_fps_log);

	Texture texture;

#if 0
	String s_file_image;
	String_Append(&s_file_image, "32_bit_(rgba)_bitmap.bmp");

	Image image = Image_LoadBMP32(s_file_image.value, s_file_image.length);

	if (!image.data) {
		AssertMessage(0, "Image not found or failed to load.");
	}

	String_Destroy(&s_file_image);

	texture = Texture_Load(image.data, image.width, image.height, GL_ABGR_EXT, true);
	texture.flip_h = image.flip_h;

	Image_Destroy(&image);
#else
	Texture_Reload(&texture, "32_bit_(rgba)_bitmap.bmp");
#endif // 0

	ShaderSet shader_set = ShaderSet_Create(window);
	ShaderSet_Use(&shader_set, SHADER_PROG_TEXTURE_FULL);

	Vertex vertex = Vertex_Create(&shader_set, &texture);
	Vertex_AddTexturePosition(&vertex,   0,   0);
	Vertex_AddTexturePosition(&vertex, 150, 150);

	Vertex_BindAttributes(&shader_set, &vertex);

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(msg, running, window);
		OpenGL_AdjustScaleViewport(window, ui_zoom_enabled);

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		Vertex_Render(&shader_set, &vertex);

		u32 fps = Time_GetFPS(&timer_fps);

		if (Time_HasElapsed(&timer_fps_log, 1000)) {
			LOG_DEBUG(fps << " fps");
		}

		Window_UpdateAndResetInput(window);
	}

	Texture_Destroy(&texture);
	Vertex_Destroy(&vertex);
	ShaderSet_Destroy(&shader_set);
}

int main() {
	Window window;

	Keyboard keyboard;

	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
