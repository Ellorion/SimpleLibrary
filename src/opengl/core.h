#pragma once

#include "extensions.h"

instant void
OpenGL_Init(
	Window *window_io
) {
	Assert(window_io);

	if (window_io->uses_opengl)
		return;

	if (window_io->hDC) {
		window_io->hRC = wglCreateContext(window_io->hDC);
		wglMakeCurrent(window_io->hDC, window_io->hRC);

		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		Window_InitOpenGL_Ext();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		window_io->uses_opengl = true;
	}
}

/// call before Window_Destroy
instant void
OpenGL_Destroy(
	Window *window_io
) {
	Assert(window_io);

	if (window_io->hRC) {
		wglMakeCurrent(0, 0);
		wglDeleteContext(window_io->hRC);
	}
}

instant void
OpenGL_UseVSync(
	Window *window_io,
	bool enable
) {
	Assert(window_io);

	if(wglSwapIntervalEXT)
		wglSwapIntervalEXT(enable);

	window_io->use_VSync = enable;

	LOG_DEBUG("VSync: " << (enable
							? "enabled."
							: "disabled."));
}

instant void
OpenGL_ClearScreen(
	float r = 0.f,
	float g = 0.f,
	float b = 0.f,
	float a = 0.f
) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor4f(1.f, 1.f, 1.f, 1.f);
}

/// keeps aspect ratio by changing the viewport,
/// or ignores aspect ratio and sets the viewport
/// to the current window size
instant bool
OpenGL_AdjustScaleViewport(
	Window *window_io,
	WINDOW_SCALE_TYPE type
) {
	Assert(window_io);

	bool result = false;

	RECT rect_window;
	GetClientRect(window_io->hWnd, &rect_window);
	s32 new_width  = rect_window.right  - rect_window.left;
	s32 new_height = rect_window.bottom - rect_window.top;

	float x = 0.f;
	float y = 0.f;

	float prev_scale_x = window_io->scale_x;
	float prev_scale_y = window_io->scale_y;

	if (   type == WINDOW_SCALE_EXPAND
		OR type == WINDOW_SCALE_ASPECT_RATIO
	) {
		if (type == WINDOW_SCALE_ASPECT_RATIO) {
			Rect_GetAspect(
				window_io->width,
				window_io->height,
				&new_width,
				&new_height,
				&x,
				&y
			);
		}

		window_io->x_viewport = x;
		window_io->y_viewport = y;

		result =    window_io->width  != new_width
		         OR window_io->height != new_height;

		if (window_io->default_width > new_width)
			window_io->scale_x = (float)window_io->default_width / new_width;
		else
			window_io->scale_x = (float)new_width / window_io->default_width;


		if (window_io->default_height > new_height)
			window_io->scale_y = (float)window_io->default_height / new_height;
		else
			window_io->scale_y = (float)new_height / window_io->default_height;

		glViewport(x, y, new_width, new_height);

		window_io->width  = new_width;
		window_io->height = new_height;
	}
	else {
		Assert(false);
	}

	if (prev_scale_x != window_io->scale_x)  result = true;
	if (prev_scale_y != window_io->scale_y)  result = true;

	if (result)
		window_io->events.on_resized = true;

	return result;
}

instant s32
OpenGL_GetTextureSizeMax() {
	s32 max = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
	return max;
}

instant void
OpenGL_Scissor(
	Window *window,
	float x,
	float y,
	s32 w,
	s32 h
) {
	Assert(window);

	/// convert to right-hand coordinate system
	float t_x = x + window->x_viewport;
	float t_y = (window->height - y - h) + window->y_viewport;
	s32   t_w = w;
	s32   t_h = h;

	glEnable(GL_SCISSOR_TEST);
	glScissor(t_x, t_y, t_w, t_h);
}

instant void
OpenGL_Scissor(
	Window *window,
	Rect rect
) {
	Assert(window);

	float x = rect.x;
	float y = rect.y;
	s32   w = rect.w;
	s32   h = rect.h;

	/// convert to right-hand coordinate system
	float t_x = x + window->x_viewport;
	float t_y = (window->height - y - h) + window->y_viewport;
	s32   t_w = w;
	s32   t_h = h;

	glEnable(GL_SCISSOR_TEST);
	glScissor(t_x, t_y, t_w, t_h);
}

instant void
OpenGL_Scissor_Disable() {
	glDisable(GL_SCISSOR_TEST);
}

instant void
OpenGL_SetBlending(
	bool enabled
) {
	if (enabled) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		glDisable(GL_BLEND);
	}
}

#include "texture.h"
#include "glsl.h"
#include "shader.h"
#include "vertex.h"
//#include "codepoint.h"
