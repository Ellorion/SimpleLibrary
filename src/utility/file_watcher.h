#pragma once

struct File_Watcher {
	bool exists            = false;
	HANDLE file_handle     = 0;
	FILETIME lastWriteTime = {};
	String s_filename      = {};
};

instant bool
File_HasChanged(
	File_Watcher *file_watcher_io
) {
	Assert(file_watcher_io);

	bool has_changed = false;

	if (!file_watcher_io->exists) {
		file_watcher_io->file_handle = CreateFile(
			file_watcher_io->s_filename.value,
			0,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL
		);

		file_watcher_io->exists = (file_watcher_io->file_handle != INVALID_HANDLE_VALUE);

		return file_watcher_io->exists;
	}

	FILETIME *lwt = &file_watcher_io->lastWriteTime;

	FILETIME prevWriteTime = *lwt;
	GetFileTime(file_watcher_io->file_handle, 0, 0, lwt);

	if (lwt->dwHighDateTime != prevWriteTime.dwHighDateTime)
		has_changed = true;
	else
	if (lwt->dwLowDateTime  != prevWriteTime.dwLowDateTime )
		has_changed = true;

	return has_changed;
}

instant void
File_Watch(
	File_Watcher *file_watcher_out,
	String s_filename
) {
	Assert(file_watcher_out);

	*file_watcher_out = {};

	file_watcher_out->s_filename = String_Copy(s_filename);
	String_Append(&file_watcher_out->s_filename, S("\0", 1));

	File_HasChanged(file_watcher_out);
	file_watcher_out->lastWriteTime = {};
}
