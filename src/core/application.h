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

instant bool
Application_OpenURL(
	String s_url
) {
	if (String_IsEmpty(&s_url, true))
		return false;

	String s_open_url   = S(s_url);
	String s_terminator = S("\0", 1);

	if (!String_EndWith(&s_url, s_terminator, true)) {
		s_open_url = String_Copy(s_open_url);
		String_Append(&s_open_url, s_terminator);
	}

	ShellExecute(0, "open", s_open_url.value, 0, 0, SW_SHOWNORMAL);

	String_Destroy(&s_open_url);

	return true;
}

instant bool
Application_OpenDirectory(
	String s_directory
) {
	if (String_IsEmpty(&s_directory, true))
		return false;

	String s_open_directory = S(s_directory);
	String s_terminator = S("\0", 1);

	if (!String_EndWith(&s_directory, s_terminator, true)) {
		s_open_directory = String_Copy(s_open_directory);
		String_Append(&s_open_directory, s_terminator);
	}

	bool result = PathFileExists(s_open_directory.value);

	if (result)
		ShellExecute(0, "open", s_open_directory.value, 0, 0, SW_SHOWNORMAL);

	String_Destroy(&s_open_directory);

	return result;

}

instant bool
Application_Execute(
	String s_command
) {
	if (String_IsEmpty(&s_command, true))
		return false;

	String ts_command;
	String_Append(&ts_command, S("\""));
	String_Append(&ts_command, s_command);
	String_Append(&ts_command, S("\""));

	/// length would calc. to 0 otherwise
	String_Append(&ts_command, S("\0", 1));

	STARTUPINFO info_startup = {};
	PROCESS_INFORMATION info_proc = {};

	info_startup.dwFlags = STARTF_USESTDHANDLES;

	bool result = CreateProcess(
		0,
		ts_command.value,
		0,
		0,
		false,
		CREATE_NEW_CONSOLE,
		0,
		0,
		&info_startup,
		&info_proc
	);

	if (!result)
		LOG_WARNING("could not open file: " << ts_command.value);

	String_Destroy(&ts_command);

	return result;
}

instant void
Application_Destroy(
	Application *app
) {
	Assert(app);

	CloseHandle(app->mutex_single_instance);
}
