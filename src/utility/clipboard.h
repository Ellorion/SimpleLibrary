#pragma once

instant String
Clipboard_GetText(
) {
	String s_result;

	if (OpenClipboard(0)) {
        HANDLE handle_clipboard = GetClipboardData(CF_TEXT);

		AssertMessage(handle_clipboard, "[Clipboard] Could not get clipboard data.");

		char *c_data = (char*)GlobalLock(handle_clipboard);

		s_result = String_Copy(c_data);

		GlobalUnlock(handle_clipboard);

		CloseClipboard();
	}

	return s_result;
}

instant void
Clipboard_SetText(
	String *s_data
) {
	Assert(s_data);

	if (OpenClipboard(0)) {
		EmptyClipboard();

		HGLOBAL handle_global = GlobalAlloc(GHND, s_data->length + 1 * sizeof(char));

		AssertMessage(handle_global, "[Clipboard] Could not allocate clipboard data.");

		char *c_data = (char*)GlobalLock(handle_global);
		Memory_Copy(c_data, s_data->value, s_data->length);
		GlobalUnlock(handle_global);

		SetClipboardData(CF_TEXT, handle_global);

		GlobalFree(handle_global);

		CloseClipboard();
	}
}
