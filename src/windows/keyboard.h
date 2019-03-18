#pragma once

#define KEYBOARD_KEYCOUNT 0xFFFF

struct Keyboard {
    bool up[KEYBOARD_KEYCOUNT] 			= {};
    bool down[KEYBOARD_KEYCOUNT] 		= {};
    bool pressing[KEYBOARD_KEYCOUNT] 	= {};
    bool repeating[KEYBOARD_KEYCOUNT] 	= {};
    bool toggled[KEYBOARD_KEYCOUNT] 	= {};

    /// last_key_virtual = msg.wParam = SL_KEYBOARD_KEYCOUNT (max.)
	u32  last_key_virtual	= 0;
	u16  key_sym			= 0;		/// translated key (incl. shift + alt)
	u32  key_scan			= 0;		/// scancode
    bool is_down			= false;	/// "any" key is down
    bool is_up				= false;	/// "any" key is up
    bool is_key_sym			= false;	/// key could be translated
    u8   key_states[256]	= {};

    /// to prevent starting an action when app was started with return key,
    /// since return key would register as (unintended) keyup or trigger event
    bool enable_return_post_startup = true;
};

instant void
Keyboard_Reset(
	Keyboard *keyboard_out,
	bool full_reset = false
) {
	if (!keyboard_out)
		return;

	bool pressing[KEYBOARD_KEYCOUNT] = {};
	bool toggled[KEYBOARD_KEYCOUNT]  = {};

	bool is_enabled = keyboard_out->enable_return_post_startup;

	if (!full_reset) {
		Memory_Copy(&pressing, &keyboard_out->pressing, sizeof(bool) * KEYBOARD_KEYCOUNT);
		Memory_Copy(&toggled , &keyboard_out->toggled , sizeof(bool) * KEYBOARD_KEYCOUNT);
	}

    *keyboard_out = {};

    if (!full_reset) {
		Memory_Copy(&keyboard_out->pressing, &pressing, sizeof(bool) * KEYBOARD_KEYCOUNT);
		Memory_Copy(&keyboard_out->toggled , &toggled , sizeof(bool) * KEYBOARD_KEYCOUNT);
    }

    keyboard_out->enable_return_post_startup = is_enabled;
}

instant void
Keyboard_ResetKey(
	Keyboard *keyboard_out,
	u32 key_virtual
) {
	if (!keyboard_out)
		return;

	keyboard_out->down[key_virtual] = false;
	keyboard_out->up[key_virtual] 	= false;

	keyboard_out->is_down 			= false;
	keyboard_out->is_up 			= false;

	keyboard_out->is_key_sym 		= false;
	keyboard_out->key_sym 			= 0;
	keyboard_out->key_scan			= 0;
	keyboard_out->last_key_virtual	= 0;
}

/// reset last key pressed
/// to prevent continuous input events
instant void
Keyboard_ResetLastKey(
	Keyboard *keyboard_out
) {
	if (!keyboard_out)
		return;

	u32 vkey = keyboard_out->last_key_virtual;

	Keyboard_ResetKey(keyboard_out, keyboard_out->last_key_virtual);
}

instant void
Keyboard_GetKeySym(
	Keyboard *keyboard_io,
	MSG *msg
) {
	Assert(keyboard_io);
	Assert(msg);

	wchar_t ch = 0;
	GetKeyboardState(keyboard_io->key_states);

	keyboard_io->is_key_sym	= false;
	keyboard_io->key_sym    = 0;

	s32 result = ToUnicode(
					msg->wParam,
					MapVirtualKey(msg->wParam, 0),
					keyboard_io->key_states, &ch, 1, 0
				 );

	if (result > 0) {
		if (ch == VK_ESCAPE)  return;

		keyboard_io->is_key_sym 	= true;
		keyboard_io->key_sym 		= ch; // (char)LOBYTE(ch);
	}
}

/// @Info:
/// 'a' -> msg.wParam                 'A'
/// 'a' -> keyboard_io->key_sym       'a'
/// VK_ESCAPE -> msg.wParam           VK_ESCAPE
/// VK_ESCAPE -> keyboard_io->key_sym '\0'      (invalid)
instant void
Keyboard_SetDown(
	Keyboard *keyboard_io,
	MSG *msg
) {
	Assert(keyboard_io);
	Assert(msg);

	keyboard_io->key_scan = MapVirtualKey(msg->wParam, 0);

	keyboard_io->down[msg->wParam] 		= true;
	keyboard_io->up[msg->wParam] 		= false;
	keyboard_io->pressing[msg->wParam] 	= true;
	keyboard_io->repeating[msg->wParam] = GETBIT(msg->lParam, 30);

	Keyboard_GetKeySym(keyboard_io, msg);

	keyboard_io->last_key_virtual 	= msg->wParam;
	keyboard_io->is_down 			= true;
	keyboard_io->is_up				= false;
}

instant void
Keyboard_SetUp(
	Keyboard *keyboard_io,
	MSG *msg
) {
	Assert(keyboard_io);
	Assert(msg);

	if (keyboard_io->enable_return_post_startup) {
		if (keyboard_io->pressing[VK_RETURN]) {
			if (msg->wParam == VK_RETURN)
				keyboard_io->enable_return_post_startup = false;

			return;
		}
		else {
			keyboard_io->enable_return_post_startup = false;
		}
	}

	if (keyboard_io->pressing[msg->wParam]) {
		keyboard_io->key_scan = MapVirtualKey(msg->wParam, 0);
		keyboard_io->last_key_virtual 		= msg->wParam;

		keyboard_io->down[msg->wParam] 		= false;
		keyboard_io->up[msg->wParam] 		= true;
		keyboard_io->pressing[msg->wParam] 	= false;
		keyboard_io->repeating[msg->wParam] = false;
		keyboard_io->toggled[msg->wParam]   = !keyboard_io->toggled[msg->wParam];

		Keyboard_GetKeySym(keyboard_io, msg);

		keyboard_io->is_down = false;
		keyboard_io->is_up	 = true;
	}
}

instant bool
Keyboard_Update(
	Keyboard *keyboard_io,
	MSG *msg
) {
	bool result = false;

	if (!keyboard_io)
		return result;

	Assert(msg);

	switch (msg->message) {
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN: {
			Keyboard_SetDown(keyboard_io, msg);
			result = true;
		} break;

		case WM_SYSKEYUP:
		case WM_KEYUP: {
			Keyboard_SetUp(keyboard_io, msg);
			result = true;
		} break;
	}

	return result;
}

instant void
Keyboard_AppendKey(
	Keyboard *keyboard,
	String *s_data_io
) {
	Assert(keyboard);
	Assert(s_data_io);

	String_Insert(s_data_io, s_data_io->length, keyboard->key_sym);
}

instant bool
Keyboard_IsPressingAnyKey(
	Keyboard *keyboard
) {
	Assert(keyboard);

	bool result = false;

	FOR(KEYBOARD_KEYCOUNT, it) {
		if (keyboard->pressing[it]) {
			result = true;
			break;
		}
	}

	return result;
}

instant void
Keyboard_WaitForIdle(
	Keyboard *keyboard_out,
	bool *pending_event,
	u32 delay_in_ms = 400
) {
	Assert(keyboard_out);
	Assert(pending_event);

	if (*pending_event) {
		static Timer timer_input_delay;

		if (Time_HasElapsed(&timer_input_delay, delay_in_ms, true)) {
			if (!Keyboard_IsPressingAnyKey(keyboard_out)) {
				*pending_event = false;
			}
		}

		Keyboard_Reset(keyboard_out);
	}
}
