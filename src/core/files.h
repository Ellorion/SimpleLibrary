#pragma once

struct File {
	FILE *fp;
};

instant bool
File_HasExtension(
	String *s_filename,
	String *s_extension
) {
	Assert(s_filename);
	Assert(s_extension);

	bool result = false;

	if (!s_extension->length)  return true;

	Array<String> as_extentions = Array_Split(s_extension, "|");

    FOR_ARRAY(as_extentions, it) {
    	String s_data_it = ARRAY_IT(as_extentions, it);

		if (String_EndWith(s_filename, s_data_it.value, s_data_it.length)) {
			result = true;
			break;
		}
    }

    Array_Destroy(&as_extentions);

	return result;
}

instant bool
File_HasExtension(
	const char *c_filename,
	const char *c_extension,
	u64 c_length_filename = 0,
	u64 c_length_extension = 0
) {
	Assert(c_filename);
	Assert(c_extension);

	bool result = false;

	if (!c_length_filename)
		c_length_filename = String_GetLength(c_filename);

	if (!c_length_extension)
		c_length_extension = String_GetLength(c_extension);

	String s_extension;
	s_extension.value  = (char *)c_extension;
	s_extension.length = c_length_extension;

	String s_filename;
	s_filename.value  = (char *)c_filename;
	s_filename.length = c_length_filename;

	Array<String> as_extentions = Array_Split(&s_extension, "|");

    FOR_ARRAY(as_extentions, it) {
    	String s_data_it = ARRAY_IT(as_extentions, it);

		if (String_EndWith(&s_filename, s_data_it.value, s_data_it.length)) {
			result = true;
			break;
		}
    }

    Array_Destroy(&as_extentions);

	return result;
}

instant bool
File_Exists(
	const char *c_filename,
	u64 c_length = 0
) {
	Assert(c_filename);

	WIN32_FIND_DATA file_data;

	char *c_search_file = String_CreateCBufferCopy(c_filename, c_length);

	bool result = false;

	if (FindFirstFile(c_search_file, &file_data) != INVALID_HANDLE_VALUE)
		result = true;

	Memory_Free(c_search_file);

	return result;
}

instant bool
File_Exists(
	String *s_path,
	String *s_filename
) {
	Assert(s_path);
	Assert(s_filename);

	WIN32_FIND_DATA file_data;

	String ts_filename;
	String_Append(&ts_filename, s_path->value, s_path->length);
	String_Append(&ts_filename, "/", 1);
	String_Append(&ts_filename, s_filename->value, s_filename->length);

	bool result = File_Exists(ts_filename.value, ts_filename.length);

	String_Destroy(&ts_filename);

	return result;
}

instant bool
File_CreateDirectory(
	String *s_directory,
	bool make_path_relative = true
) {
	Assert(s_directory);

	if (!s_directory->length)  return false;

	String s_path_relative;

	if (make_path_relative)
		String_Append(&s_path_relative, "./", 2);

	String_Append(&s_path_relative, s_directory->value, s_directory->length);

	char *c_path_relative = String_CreateCBufferCopy(s_path_relative.value,
													 s_path_relative.length);

	bool success = (CreateDirectory(c_path_relative, 0) != 0);

	if (!success) {
		/// you wanted it, you got it -> success
		if (GetLastError() == ERROR_ALREADY_EXISTS)
			success = true;
	}

	Memory_Free(c_path_relative);
	String_Destroy(&s_path_relative);

	return success;
}

instant File
File_Open(
	const char *c_filename,
	const char *mode,
	u64 c_length = 0
) {
	Assert(c_filename);
	Assert(mode);

	File file = {};

	char *tc_filename = String_CreateCBufferCopy(c_filename, c_length);

	file.fp = fopen(tc_filename, mode);

	Memory_Free(tc_filename);

	return file;
}

instant bool
File_Close(
	File *file_io
) {
	Assert(file_io);

	/// Returns 0 on success
	return (fclose(file_io->fp) == 0);
}

instant u64
File_Size(
	File *file
) {
	Assert(file);

	if (!file->fp)
		return 0;

	fseek (file->fp, 0, SEEK_END);
	u64 size = ftell(file->fp);
	rewind(file->fp);

	return size;
}

instant String
File_Execute(
	const char *c_command,
	u64 c_length = 0
) {
	String s_result;

	if (!c_length)
		c_length = String_GetLength(c_command);

	char *tc_command = String_CreateCBufferCopy(c_command, c_length);

	FILE *pipe = popen(tc_command, "r");

	/// without the delay, the pipe operations most likely will fail!
	/// and the assert is unable to check that...
	Sleep(30);

	if (!pipe)
		Assert(false);

	fseek (pipe, 0, SEEK_END);
	u64 length = ftell(pipe);
	rewind(pipe);

	s_result.value  = Memory_Resize(s_result.value, char, length);
	s_result.length = length;
	fread(s_result.value, sizeof(char), sizeof(char) * length, pipe);

	s_result.changed = true;

	pclose(pipe);
	Memory_Free(tc_command);

	return s_result;
}

instant void
File_Write(
	File *file,
	const char *data_out,
	u64 length = 0
) {
	Assert(file);

	if (!length)
		length = String_GetLength(data_out);

    fwrite(data_out, sizeof(char), sizeof(char) * length, file->fp);
}

instant String
File_Read(
	File *file
) {
	Assert(file);

	u64 length = File_Size(file);

	String s_data;

	s_data.value  = Memory_Resize(s_data.value, char, length);
	s_data.length = length;
	fread(s_data.value, sizeof(char), sizeof(char) * length, file->fp);

	s_data.changed = true;

	return s_data;
}

instant String
File_ReadAll(
	const char *c_filename,
	u64 c_length = 0,
	bool as_binary = true
) {
    Assert(c_filename);

	MEASURE_START();

    File file;

    if (as_binary)
		file = File_Open(c_filename, "rb", c_length);
	else
		file = File_Open(c_filename, "r" , c_length);

	if (!file.fp)
		LOG_WARNING("File \"" << c_filename << "\" does not exists.");

	String s_data = File_Read(&file);

	File_Close(&file);

	s_data.changed = true;

	MEASURE_END("(" << c_filename << ") ");

	return s_data;
}

struct File_Watcher {
	HANDLE file = 0;
	bool exists = false;
	FILETIME lastWriteTime = {};
};

instant void
File_Watch(
	File_Watcher *file_watcher_out,
	const char *c_filename,
	u64 c_length = 0
) {
	Assert(file_watcher_out);

	*file_watcher_out = {};

	char *tc_filename = String_CreateCBufferCopy(c_filename, c_length);

	file_watcher_out->file = CreateFile(
			tc_filename,
			0,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL
		);

	if (file_watcher_out->file)
		file_watcher_out->exists = true;

	Memory_Free(tc_filename);
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
	GetFileTime(file_watcher->file, 0, 0, lwt);

	if (lwt->dwHighDateTime != prevWriteTime.dwHighDateTime)
		has_changed = true;
	else
	if (lwt->dwLowDateTime  != prevWriteTime.dwLowDateTime )
		has_changed = true;

	return has_changed;
}

enum DIR_ENTRY_TYPE {
	DIR_ENTRY_FILE,
	DIR_ENTRY_DIR
};

struct Directory_Entry {
	String s_name;
	DIR_ENTRY_TYPE type;
};

enum DIR_LIST_TYPE {
	DIR_LIST_ONLY_FILES,
	DIR_LIST_ONLY_DIR,
	DIR_LIST_ALL
};

/// does not list or includes subdirectories
instant void
File_ReadDirectory(
	Array<Directory_Entry> *a_entries,
	const char *c_path,
	const char *extension_filter = 0,
	u64 c_length = 0,
	bool prefix_path = true,
	const char *name_filter = 0,
	DIR_LIST_TYPE type = DIR_LIST_ONLY_FILES
) {
	Assert(a_entries);

	if (!c_length)
		c_length = String_GetLength(c_path);

	if (!c_length)   return;

	HANDLE id_directory;
	WIN32_FIND_DATA file_data;

	String s_search_path;
	String_Append(&s_search_path, c_path, c_length);
	String_Append(&s_search_path, "/*");

	String s_extension_filter;
	String_Append(&s_extension_filter, extension_filter);

	char *c_search_path = String_CreateCBufferCopy(	s_search_path.value,
													s_search_path.length);

	if ((id_directory = FindFirstFile(c_search_path, &file_data)) != INVALID_HANDLE_VALUE) {
		do {
			const bool is_directory = (file_data.dwFileAttributes &
									   FILE_ATTRIBUTE_DIRECTORY)  != 0;

			if (is_directory) {
				if (type == DIR_LIST_ONLY_FILES)
					continue;

				if (String_IsEqual(file_data.cFileName, ".", 1) AND String_GetLength(file_data.cFileName) == 1) {
					continue;
				}
			}
			else {
				 if (type == DIR_LIST_ONLY_DIR)
					continue;
			}

			String s_filename;
			String_Append(&s_filename, file_data.cFileName);

			bool has_extension = File_HasExtension(&s_filename, &s_extension_filter);

			bool found_name = true;

			/// exclude if filter does not match
			if (name_filter)
				found_name = String_Find(&s_filename, name_filter);

			if (has_extension AND found_name) {
				String ts_filename;

				if (prefix_path) {
					String_Append(&ts_filename, c_path, c_length);
					if (!String_EndWith(&ts_filename, "/") AND !String_EndWith(&ts_filename, "\\"))
						String_Append(&ts_filename, "\\");
				}

				String_Append(&ts_filename, file_data.cFileName);

				Directory_Entry dir_entry;
				String_Append(&dir_entry.s_name, ts_filename.value, ts_filename.length);

				if (is_directory)
					dir_entry.type = DIR_ENTRY_DIR;
				else
					dir_entry.type = DIR_ENTRY_FILE;

				Array_Add(a_entries, dir_entry);
			}

			String_Destroy(&s_filename);
		} while (FindNextFile(id_directory, &file_data));

		FindClose(id_directory);
	}

	String_Destroy(&s_extension_filter);

	Memory_Free(c_search_path);
	String_Destroy(&s_search_path);
}

///@HINT: test this with backup data, before using
///       or it could leave a mess
instant void
File_Rename(
	const char *c_path,
	u64 c_length_path,
	const char *c_filename_old,
	u64 c_length_file_old,
	const char *c_filename_new,
	u64 c_length_file_new
) {
	if (!c_length_path)
		c_length_path = String_GetLength(c_path);

	if (!c_length_file_old)
		c_length_file_old = String_GetLength(c_filename_old);

	if (!c_length_file_new)
		c_length_file_new = String_GetLength(c_filename_new);

	String s_file_old;
	String_Append(&s_file_old, c_path, c_length_path);
	String_Append(&s_file_old, "/", 1);
	String_Append(&s_file_old, c_filename_old, c_length_file_old);

	String s_file_new;
	String_Append(&s_file_new, c_path, c_length_path);
	String_Append(&s_file_new, "/", 1);
	String_Append(&s_file_new, c_filename_new, c_length_file_new);

	String_Destroy(&s_file_old);
	String_Destroy(&s_file_new);

	char *tc_file_old = String_CreateCBufferCopy(s_file_old.value, s_file_old.length);
	char *tc_file_new = String_CreateCBufferCopy(s_file_new.value, s_file_new.length);

	MoveFile(tc_file_old, tc_file_new);

	Memory_Free(tc_file_old);
	Memory_Free(tc_file_new);
}

instant String
File_GetExtension(
	String *s_data
) {
	String s_result;

	s64 pos_ext = String_IndexOfRev(s_data, ".");

	if (!pos_ext) {
		return s_result;
	}

	s_result = String_Copy(s_data->value + pos_ext, s_data->length - pos_ext);

	return s_result;
}
