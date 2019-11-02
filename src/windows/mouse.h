#pragma once

#define MOUSE_WHEEL_GET_DELTA(value) (GET_WHEEL_DELTA_WPARAM(value) / WHEEL_DELTA)
#define MOUSE_BUTTON_COUNT 3

struct Mouse {
	Point point          = {};
	Point point_relative = {};

	bool up[MOUSE_BUTTON_COUNT];
	bool down[MOUSE_BUTTON_COUNT];
	bool pressing[MOUSE_BUTTON_COUNT];
	s32  wheel = 0;

	bool is_up        = false;
	bool is_down      = false;
	bool is_moving    = false;
};

instant void
MemorySegment_AddMouse(
	Array<MemorySegment> *a_segments,
	Mouse *mouse
) {
	Assert(a_segments);
	Assert(mouse);

	MemorySegment_Add(a_segments, mouse->point_relative);
	MemorySegment_Add(a_segments, mouse->down);
	MemorySegment_Add(a_segments, mouse->up);

	MemorySegment_Add(a_segments, mouse->is_up);
	MemorySegment_Add(a_segments, mouse->is_down);
	MemorySegment_Add(a_segments, mouse->is_moving);

	MemorySegment_Add(a_segments, mouse->wheel);
}

instant void
Mouse_Show(
) {
	ShowCursor(true);
}

instant void
Mouse_AutoHide(
	u32 msg_message = 0,
	u32 millisec = 2000
) {
    static Timer timer_mouse_move;
    static bool is_cursor_visible = true;

    /// reset timer on mouse move window event
    if (msg_message == WM_MOUSEMOVE) {
		Time_Reset(&timer_mouse_move);
		if (!is_cursor_visible) {
			is_cursor_visible = true;
			Mouse_Show();
		}
		return;
    }

    /// auto-hide mouse cursor after x millisec. of no mouse movement
	if (is_cursor_visible) {
		if (Time_Get() - timer_mouse_move.lo_timer >= millisec) {
			is_cursor_visible = false;
			ShowCursor(false);
		}
	}
}

instant void
Mouse_GetPosition(
	float *x_out,
	float *y_out,
	Window *window
) {
	POINT point;
	GetCursorPos(&point);

	RECT rect_active;

	if (window) {
		GetWindowRect(window->hWnd, &rect_active);
		Window_UnAdjustRect(window->hWnd, &rect_active);

		RectF rect_viewport;
		glGetFloatv(GL_VIEWPORT, (GLfloat *)&rect_viewport);

		float scale_x = window->width  / rect_viewport.w;
		float scale_y = window->height / rect_viewport.h;

		point.x = (point.x - (rect_active.left + rect_viewport.x)) * scale_x;
		point.y = (point.y - (rect_active.top  + rect_viewport.y)) * scale_y;
	}

	IF_SET(x_out) = point.x;
	IF_SET(y_out) = point.y;
}

instant void
Mouse_GetPosition(
	Mouse *mouse_out,
	Window *window
) {
	Assert(window);
	Assert(mouse_out);

	float mouse_point_x_old = mouse_out->point.x;
	float mouse_point_y_old = mouse_out->point.y;

	Mouse_GetPosition(&mouse_out->point.x, &mouse_out->point.y, window);

	mouse_out->point_relative.x += mouse_out->point.x - mouse_point_x_old;
	mouse_out->point_relative.y += mouse_out->point.y - mouse_point_y_old;
}

instant bool
Mouse_Update(
	Mouse *mouse_io,
	Window *window,
	MSG *msg
) {
	if (!mouse_io)
		return false;

	if (!msg)
		return false;

	if (window AND msg->message == WM_MOUSEMOVE) {
		mouse_io->is_moving = true;
		Mouse_GetPosition(mouse_io, window);
		return true;
	}

    switch(msg->message) {
		case WM_LBUTTONDOWN: {
			mouse_io->pressing[0] = true;
			mouse_io->down[0] = true;
			mouse_io->is_down = true;
		} break;

		case WM_MBUTTONDOWN: {
			mouse_io->pressing[1] = true;
			mouse_io->down[1] = true;
			mouse_io->is_down = true;
		} break;

		case WM_RBUTTONDOWN: {
			mouse_io->pressing[2] = true;
			mouse_io->down[2] = true;
			mouse_io->is_down = true;
		} break;

		case WM_LBUTTONUP:   {
			mouse_io->pressing[0] = false;
			mouse_io->up[0] = true;
			mouse_io->is_up = true;
		} break;

		case WM_MBUTTONUP:   {
			mouse_io->pressing[1] = false;
			mouse_io->up[1] = true;
			mouse_io->is_up = true;
		} break;

		case WM_RBUTTONUP:   {
			mouse_io->pressing[2] = false;
			mouse_io->up[2] = true;
			mouse_io->is_up = true;
		} break;

		case WM_MOUSEWHEEL: {
			mouse_io->wheel += MOUSE_WHEEL_GET_DELTA(msg->wParam) * 16;
		} break;

		default: { return false; } break;
    }

    return true;
}

instant void
Mouse_AutoClickLeft(
) {
	/// auto left mouse click on current mouse position
	mouse_event(0x02, 0, 0, 0, 1);
	mouse_event(0x04, 0, 0, 0, 1);
}
