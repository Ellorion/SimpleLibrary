#pragma once

struct File_Watcher {
	bool exists            = false;
	HANDLE file_handle     = 0;
	FILETIME lastWriteTime = {};
	String s_filename      = {};
};

instant void
File_Watch(
	File_Watcher *file_watcher_out,
	String s_filename
) {
	Assert(file_watcher_out);

	*file_watcher_out = {};

	file_watcher_out->s_filename = String_Copy(s_filename);
	String_Append(&file_watcher_out->s_filename, S("\0", 1));

	file_watcher_out->file_handle = CreateFile(
			file_watcher_out->s_filename.value,
			0,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL
		);

	if (file_watcher_out->file_handle)
		file_watcher_out->exists = true;
}

instant bool
File_HasChanged(
	File_Watcher *file_watcher
) {
	Assert(file_watcher);

	bool has_changed = false;

	if (!file_watcher->exists)  return false;

	FILETIME *lwt = &file_watcher->lastWriteTime;

	FILETIME prevWriteTime = *lwt;
	GetFileTime(file_watcher->file_handle, 0, 0, lwt);

	if (lwt->dwHighDateTime != prevWriteTime.dwHighDateTime)
		has_changed = true;
	else
	if (lwt->dwLowDateTime  != prevWriteTime.dwLowDateTime )
		has_changed = true;

	return has_changed;
}
