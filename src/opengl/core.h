#pragma once

#include "extensions.h"

instant void
OpenGL_Init(
	Window *window_io
) {
	Assert(window_io);

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
OpenGL_SetVSync(
	Window *window_out,
	bool enable
) {
	Assert(window_out);

	if(wglSwapIntervalEXT)
		wglSwapIntervalEXT(enable);

	window_out->useVSync = enable;

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
	bool zooming = true
) {
	Assert(window_io);

	RECT rect_window;
	GetClientRect(window_io->hWnd, &rect_window);
	s32 new_width  = rect_window.right  - rect_window.left;
	s32 new_height = rect_window.bottom - rect_window.top;

	float x = 0.f;
	float y = 0.f;

	if (!zooming) {
		bool result =    window_io->width  != new_width
		              OR window_io->height != new_height;

		glViewport(x, y, new_width, new_height);
		window_io->scale_x = 1.0f;
		window_io->scale_y = 1.0f;
		window_io->width  = new_width;
		window_io->height = new_height;

		return result;
	}

	Rect_GetAspect(
		window_io->width,
		window_io->height,
		&new_width,
		&new_height,
		&x,
		&y
	);

    window_io->x_viewport = x;
    window_io->y_viewport = y;

	float prev_scale_x = window_io->scale_x;
	float prev_scale_y = window_io->scale_y;

	if (window_io->width > new_width)
		window_io->scale_x = (float)new_width / window_io->width;
	else
		window_io->scale_x = (float)window_io->width / new_width;


	if (window_io->height > new_height)
		window_io->scale_y = (float)new_height / window_io->height;
	else
		window_io->scale_y = (float)window_io->height / new_height;

	glViewport(x, y, new_width, new_height);

    if (prev_scale_x != window_io->scale_x) return true;
    if (prev_scale_y != window_io->scale_y) return true;

    return false;
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
	float t_x = x / window->scale_x + window->x_viewport;
	float t_y = (window->height - y - h) / window->scale_y + window->y_viewport;
	s32   t_w = w / window->scale_x;
	s32   t_h = h / window->scale_y;

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
	float t_x = x / window->scale_x + window->x_viewport;
	float t_y = (window->height - y - h) / window->scale_y + window->y_viewport;
	s32   t_w = w / window->scale_x;
	s32   t_h = h / window->scale_y;

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