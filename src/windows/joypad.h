#pragma once

#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000

#define XINPUT_ERROR_DEVICE_NOT_CONNECTED 0x048F

struct Joypad_State {
	u64     packet_number = 0;

    u16		button = 0;
    u8		trigger_left = 0;
    u8		trigger_right = 0;
    s16		thumb_left_x = 0;
    s16		thumb_left_y = 0;
    s16		thumb_right_x = 0;
    s16		thumb_right_y = 0;
};

typedef DWORD(WINAPI* proc_XInputGetState)(u64 joy_index, Joypad_State *state);

struct Joypad {
	HINSTANCE dll = 0;
	Joypad_State state = {};
	Joypad_State prev_state = {};

	proc_XInputGetState GetState = 0;
};

instant bool
Joypad_Init_XInput(
	Joypad *joypad_out
) {
	Assert(joypad_out);

    char sys_dir[MAX_PATH];

    u64 sys_dir_len = GetSystemDirectory(sys_dir, sizeof(sys_dir));
    if (!sys_dir_len OR sys_dir_len > sizeof(sys_dir))  return false;

    strcat(sys_dir, "\\xinput1_3.dll");

    joypad_out->dll = LoadLibrary(sys_dir);
    if (!joypad_out->dll)  return false;

    joypad_out->GetState = (proc_XInputGetState)GetProcAddress(joypad_out->dll, "XInputGetState");

	return true;
}

instant bool
Joypad_GetInput(
	Joypad *joypad,
	u64 joy_index
) {
	Assert(joypad);

	if (!joypad->dll)  return false;

	joypad->prev_state = joypad->state;

	u64 result = joypad->GetState(joy_index, &joypad->state);

	return (result != XINPUT_ERROR_DEVICE_NOT_CONNECTED);
}

///@Hint: test for != 0
///@Hint: "pressing" states

/// DPad
#define Joypad_IsPadUp(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_DPAD_UP)
#define Joypad_IsPadDown(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_DPAD_DOWN)
#define Joypad_IsPadLeft(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_DPAD_LEFT)
#define Joypad_IsPadRight(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_DPAD_RIGHT)

/// Center buttons
#define Joypad_IsStart(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_START)
#define Joypad_IsSelect(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_BACK)

/// Thumb button
#define Joypad_IsThumbLeft(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_LEFT_THUMB)
#define Joypad_IsThumbRight(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_RIGHT_THUMB)

/// Shoulder buttons
#define Joypad_IsShoulderLeft(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_LEFT_SHOULDER)
#define Joypad_IsShoulderRight(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_RIGHT_SHOULDER)

/// Control buttons
#define Joypad_IsA(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_A)
#define Joypad_IsB(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_B)
#define Joypad_IsX(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_X)
#define Joypad_IsY(ptr_joypad) \
	((*ptr_joypad).state.button & XINPUT_GAMEPAD_Y)

///@Hint: "button up" states

/// Center buttons
#define Joypad_IsStart_UP(ptr_joypad) \
	(   (  (*ptr_joypad).prev_state.button & XINPUT_GAMEPAD_START) \
	 && (!((*ptr_joypad).state.button      & XINPUT_GAMEPAD_START)))

#define Joypad_IsSelect_UP(ptr_joypad) \
	(   (  (*ptr_joypad).prev_state.button & XINPUT_GAMEPAD_BACK) \
	 && (!((*ptr_joypad).state.button      & XINPUT_GAMEPAD_BACK)))

/// Control buttons
#define Joypad_IsA_Up(ptr_joypad) \
	(   (  (*ptr_joypad).prev_state.button & XINPUT_GAMEPAD_A) \
	 && (!((*ptr_joypad).state.button      & XINPUT_GAMEPAD_A)))

#define Joypad_IsB_Up(ptr_joypad) \
	(   (  (*ptr_joypad).prev_state.button & XINPUT_GAMEPAD_B) \
	 && (!((*ptr_joypad).state.button      & XINPUT_GAMEPAD_B)))

#define Joypad_IsX_Up(ptr_joypad) \
	(   (  (*ptr_joypad).prev_state.button & XINPUT_GAMEPAD_X) \
	 && (!((*ptr_joypad).state.button      & XINPUT_GAMEPAD_X)))

#define Joypad_IsY_Up(ptr_joypad) \
	(   (  (*ptr_joypad).prev_state.button & XINPUT_GAMEPAD_Y) \
	 && (!((*ptr_joypad).state.button      & XINPUT_GAMEPAD_Y)))

/// for axis
/// Returns: current section

/// @Suggestion: deadzone = 8000
instant s32
Joypad_GetSection(
	s16 axis_value,
	u16 deadzone,
	u16 section_max
) {
	if (axis_value <= deadzone AND axis_value >= -deadzone)  return 0;

	if (section_max > 0) {
		constexpr s16 limit = 32767;

		if (axis_value < 0) axis_value += deadzone;
		else                axis_value -= deadzone;

		float steps = (float)((limit - deadzone) / section_max);

		return (axis_value / steps);
	}

	return 0;
}

/// for triggers
/// Returns: current section
instant u8
Joypad_GetSection(
	u8  axis_value,
	u16 deadzone,
	u16 section_max
) {
	if (axis_value <= deadzone AND axis_value >= -deadzone)  return 0;

	if (section_max > 0) {
		constexpr s64 limit = 255;

		if (axis_value < 0) axis_value += deadzone;
		else                axis_value -= deadzone;

		float steps = (float)((limit - deadzone) / section_max);
		return axis_value / steps;
	}

	return 0;
}
