#pragma once

struct Application {
	HANDLE mutex_single_instance = 0;
};

instant bool
Application_IsSingleInstance(
	Application *app,
	String s_instance_id
) {
	Assert(app);

	app->mutex_single_instance = CreateMutex(0, false, s_instance_id.value);
	u64 last_error = GetLastError();

	return (last_error != ERROR_ALREADY_EXISTS);
}

instant String
Application_GetFilename(
	bool include_path
) {
	char c_filename[MAX_PATH + 1];

	GetModuleFileName(0, c_filename, MAX_PATH + 1);
	String s_filename;

	if (include_path) {
		s_filename = String_Copy(c_filename);
	}
	else {
		String s_key = S("\\");
		s_filename = S(c_filename);

		u64 index = String_IndexOfRev(&s_filename, s_key, true);
		String_AddOffset(&s_filename, index + s_key.length);
		s_filename = String_Copy(s_filename);
	}

	return s_filename;
}

instant String
Application_GetDirectory(
) {
	char c_filename[MAX_PATH + 1];

	GetModuleFileName(0, c_filename, MAX_PATH + 1);

	String s_key = S("\\");
	String s_filename = S(c_filename);

	u64 index = String_IndexOfRev(&s_filename, s_key, true);
	s_filename = String_Copy(s_filename.value, index);

	return s_filename;
}

instant String
Application_GetWorkingDirectory(
) {
	u64 buffer_size = GetCurrentDirectory(0, 0);

	String s_directory;

	if (buffer_size) {
		s_directory = String_CreateBuffer(buffer_size);
		GetCurrentDirectory(s_directory.length, s_directory.value);
	}

	return s_directory;
}

instant bool
Application_RegisterHotKey(
	s32 hotkey_id,
	u32 mod_flags,
	u32 keyboard_char
) {
	keyboard_char = String_ToUpper(keyboard_char);

	return RegisterHotKey(0, hotkey_id, mod_flags, keyboard_char);
}

instant void
Application_Destroy(
	Application *app
) {
	Assert(app);

	CloseHandle(app->mutex_single_instance);
}
