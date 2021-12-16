// Simplified approach

#pragma once

#define INPUT_KEY_COUNT 0xFFFF

/// Notes:
/// no accents support

struct InputState {
    bool key_repeating[INPUT_KEY_COUNT];
    bool key_pressing[INPUT_KEY_COUNT];
    u8 keys_pressed = 0;
    u8 last_key_repeated = 0;
    u8 last_key_pressed = 0;
};

instant void
Input_UpdateKeyboard(InputState &state) {
    u8 key_states[0xFF];
    GetKeyboardState(key_states);

    state.keys_pressed = 0;
    state.last_key_pressed  = 0;
    state.last_key_repeated = 0;

    /// virtual keys
    FOR(0xFF, it) {
        wchar_t wch[5];

        s32 result = ToUnicode(
                        it,
                        MapVirtualKey(it, MAPVK_VK_TO_VSC),
                        key_states, wch, 4, 0
                     );

        if (result > 0) {
            auto vkey = GetAsyncKeyState(it);

            /// pressing (ignoring repeat)
            bool is_repeating = (vkey & 0x8000);
            bool is_pressing  = (vkey & 0x0001);

            auto ch = wch[result-1];
            state.key_pressing[ch]  = is_pressing;
            state.key_repeating[ch] = is_repeating;
            state.keys_pressed += is_repeating;

            if (is_pressing) {
                state.last_key_pressed  = ch;
            }

            if (is_repeating) {
                state.last_key_repeated = ch;
            }
        }
    }

    if (state.keys_pressed > 1) {
        state.last_key_pressed = 0;
        state.last_key_repeated = 0;
    }
}
