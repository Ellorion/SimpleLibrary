// Simplified approach

#pragma once

#define INPUT_KEY_COUNT 0xFF

/// --- Work in progress
/// - crashes with alt gr
/// - some keys do not work like [

struct InputState {
    bool key_pressing[INPUT_KEY_COUNT];
    u8 keys_pressed = 0;
    u8 last_key_pressed = 0;
};

instant void
Input_UpdateKeyboard(InputState &state) {
    u8 key_states[0xFF];
    GetKeyboardState(key_states);

    state.keys_pressed = 0;
    state.last_key_pressed = 0;

    FOR(INPUT_KEY_COUNT, it) {
        auto vkey = GetAsyncKeyState(it);
        wchar_t ch = 0;

        s32 result = ToUnicode(
                        it,
                        MapVirtualKey(it, 0),
                        key_states, &ch, 1, 0
                     );

        if (result > 0) {
            /// pressing (ignoring repeat)
            bool is_pressing = (vkey & 0x8000);

            state.key_pressing[ch] = is_pressing;
            state.keys_pressed += is_pressing;

            if (is_pressing) {
                state.last_key_pressed = ch;
            }
        }
    }

    if (state.keys_pressed > 1) {
        state.last_key_pressed = 0;
    }
}
