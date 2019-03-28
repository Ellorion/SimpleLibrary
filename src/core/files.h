#pragma once

struct File {
	FILE *fp;
};

instant bool
File_HasExtension(
	String *s_filename,
	String  s_extension
) {
	Assert(s_filename);

	bool result = false;

	if (String_IsEmpty(&s_extension, true))
		return true;

	Array<String> as_extentions = Array_Split(&s_extension, S("|"), DELIMITER_IGNORE, true);

    FOR_ARRAY(as_extentions, it) {
    	String ts_data = ARRAY_IT(as_extentions, it);

		if (String_EndWith(s_filename, ts_data, true)) {
			result = true;
			break;
		}
    }

    Array_Destroy(&as_extentions);

	return result;
}

instant bool
File_Exists(
	String s_filename
) {
	WIN32_FIND_DATA file_data;

	char *c_search_file = String_CreateCBufferCopy(s_filename);

	bool result = false;

	if (FindFirstFile(c_search_file, &file_data) != INVALID_HANDLE_VALUE)
		result = true;

	Memory_Free(c_search_file);

	return result;
}

instant bool
File_Exists(
	String s_path,
	String s_filename
) {
	WIN32_FIND_DATA file_data;

	String ts_filename;
	String_Append(&ts_filename, s_path);
	String_Append(&ts_filename, S("/"));
	String_Append(&ts_filename, s_filename);

	bool result = File_Exists(ts_filename);

	String_Destroy(&ts_filename);

	return result;
}

instant bool
File_CreateDirectory(
	String s_directory,
	bool make_path_relative = true
) {
	if (String_IsEmpty(&s_directory, true))
		return false;

	String s_path_relative;

	if (make_path_relative)
		String_Append(&s_path_relative, S("./"));

	String_Append(&s_path_relative, s_directory);
	String_Append(&s_path_relative, S("\0", 1));

	bool success = (CreateDirectory(s_path_relative.value, 0) != 0);

	if (!success) {
		/// you wanted it, you got it -> success
		if (GetLastError() == ERROR_ALREADY_EXISTS)
			success = true;
	}

	String_Destroy(&s_path_relative);

	return success;
}

instant File
File_Open(
	String s_filename,
	const char *mode
) {
	Assert(mode);

	File file = {};

	char *tc_filename = String_CreateCBufferCopy(s_filename);

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

instant void
File_Write(
	File *file,
	String s_data
) {
	Assert(file);

    fwrite(s_data.value, sizeof(char), sizeof(char) * s_data.length, file->fp);
}

///@TODO: pass return value as a buffer, to allow
///       file hot-reloading without destroying the string buffer
instant String
File_Read(
	File *file
) {
	Assert(file);

	u64 length = File_Size(file);

	String s_data;
	s_data.length = length;

	if (!length) {
		s_data.value = Memory_Resize(s_data.value, char, 1);
		s_data.value[0] = '\0';
	}
	else {
		s_data.value  = Memory_Resize(s_data.value, char, length);
		fread(s_data.value, sizeof(char), sizeof(char) * length, file->fp);
	}

	s_data.changed = true;

	return s_data;
}

instant String
File_ReadAll(
	String s_filename,
	bool as_binary = true
) {
	MEASURE_START();

    File file;

    if (as_binary)
		file = File_Open(s_filename, "rb");
	else
		file = File_Open(s_filename, "r");

	if (!file.fp)
		LOG_WARNING("File \"" << s_filename.value << "\" does not exists.");

	String s_data = File_Read(&file);

	File_Close(&file);

	s_data.changed = true;

	MEASURE_END("(" << c_filename << ") ");

	return s_data;
}

enum DIR_ENTRY_TYPE {
	DIR_ENTRY_DRIVE,
	DIR_ENTRY_DIR,
	DIR_ENTRY_FILE
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

bool
operator < (
	Directory_Entry &entry_1,
	Directory_Entry &entry_2
) {
	if(entry_1.type == entry_2.type) {
		/// do not move, should always be the first entry
		if (entry_1.s_name == "..") return false;
		if (entry_2.s_name == "..") return false;

		long index_1 = String_IndexOfRev(&entry_1.s_name, S("."), true);
		long index_2 = String_IndexOfRev(&entry_2.s_name, S("."), true);

		u64 length = 0;

		if (index_1 >= 0 AND index_2 >= 0)
			length = MIN(index_1, index_2);

		return (String_Compare(entry_1.s_name, entry_2.s_name, length, false) < 0);
	}

	return (entry_1.type < entry_2.type);
}
bool
operator > (
	Directory_Entry &entry_1,
	Directory_Entry &entry_2
) {
	if(entry_1.type == entry_2.type) {
		/// do not move, should always be the first entry
		if (entry_1.s_name == "..") return false;
		if (entry_2.s_name == "..") return false;

		long index_1 = String_IndexOfRev(&entry_1.s_name, S("."), true);
		long index_2 = String_IndexOfRev(&entry_2.s_name, S("."), true);

		u64 length = 0;

		if (index_1 >= 0 AND index_2 >= 0)
			length = MIN(index_1, index_2);

		return (String_Compare(entry_1.s_name, entry_2.s_name, length, false) > 0);
	}

	return (entry_1.type > entry_2.type);
}

instant bool
File_IsDirectory(
	String s_path
) {
	if (String_IsEmpty(&s_path, true))
		return false;

	char *c_path = String_CreateCBufferCopy(s_path);

	DWORD attrib = GetFileAttributes(c_path);

	bool result =     (attrib != INVALID_FILE_ATTRIBUTES
			      AND (attrib & FILE_ATTRIBUTE_DIRECTORY));

	Memory_Free(c_path);

	return result;
}

/// does not list or includes subdirectories
instant void
File_ReadDirectory(
	Array<Directory_Entry> *a_entries_io,
	String s_path,
	DIR_LIST_TYPE type = DIR_LIST_ALL,
	bool prefix_path = true,
	const char *extension_filter = 0,
	const char *name_filter = 0
) {
	Assert(a_entries_io);

	if (String_IsEmpty(&s_path, true))
		return;

	HANDLE id_directory;
	WIN32_FIND_DATA file_data;

	String s_search_path;
	String_Append(&s_search_path, s_path);
	String_Append(&s_search_path, S("/*"));
	String_Append(&s_search_path, S("\0", 1));

	String s_extension_filter = S(extension_filter);

	if ((id_directory = FindFirstFile(s_search_path.value, &file_data)) != INVALID_HANDLE_VALUE) {
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

			String s_filename = S(file_data.cFileName);

			bool has_extension = File_HasExtension(&s_filename, s_extension_filter);

			bool found_name = true;

			/// exclude if filter does not match
			if (name_filter)
				found_name = String_Find(&s_filename, S(name_filter));

			if (has_extension AND found_name) {
				String ts_filename;

				if (prefix_path) {
					String_Append(&ts_filename, s_path);
					if (    !String_EndWith(&ts_filename, S("/") , true)
						AND !String_EndWith(&ts_filename, S("\\"), true)
					) {
						String_Append(&ts_filename, S("\\"));
					}
				}

				String_Append(&ts_filename, S(file_data.cFileName));

				Directory_Entry dir_entry;
				String_Append(&dir_entry.s_name, ts_filename);

				if (is_directory)
					dir_entry.type = DIR_ENTRY_DIR;
				else
					dir_entry.type = DIR_ENTRY_FILE;

				Array_Add(a_entries_io, dir_entry);
			}
		} while (FindNextFile(id_directory, &file_data));

		FindClose(id_directory);
	}
	else {
		if (   type == DIR_LIST_ALL
			OR type == DIR_LIST_ONLY_DIR
		) {
			/// mouse support for going to parent directory
			/// when navigating to connected non-inserted drives
			/// like CD/DVD drives
			Directory_Entry dir_entry;
			String_Append(&dir_entry.s_name, S(".."));

			dir_entry.type = DIR_ENTRY_DIR;

			Array_Add(a_entries_io, dir_entry);
		}
	}

	String_Destroy(&s_search_path);
}

///@HINT: test this with backup data, before using
///       or it could leave a mess
instant void
File_Rename(
	String s_path,
	String s_filename_old,
	String s_filename_new
) {
	String s_file_old;
	String_Append(&s_file_old, s_path);
	String_Append(&s_file_old, S("/"));
	String_Append(&s_file_old, s_filename_old);
	String_Append(&s_file_old, S("\0", 1));

	String s_file_new;
	String_Append(&s_file_new, s_path);
	String_Append(&s_file_new, S("/"), 1);
	String_Append(&s_file_new, s_filename_new);
	String_Append(&s_file_new, S("\0", 1));

	MoveFile(s_file_old.value, s_file_new.value);

	String_Destroy(&s_file_old);
	String_Destroy(&s_file_new);
}

instant String
File_GetExtension(
	String *s_data
) {
	String s_result;

	s64 pos_ext = String_IndexOfRev(s_data, S("."), true);

	if (!pos_ext) {
		return s_result;
	}

	s_result = String_Copy(s_data->value + pos_ext, s_data->length - pos_ext);

	return s_result;
}

instant void
File_GetDrives(
	Array<Directory_Entry> *a_drives_out
) {
	Assert(a_drives_out);

	Array_DestroyContainer(a_drives_out);

	u64 buffer_size = GetLogicalDriveStrings(0, 0);

	static String s_buffer;
	String_CreateBuffer(&s_buffer, buffer_size, true);
	GetLogicalDriveStringsA(s_buffer.length, s_buffer.value);

	String s_buffer_it = S(s_buffer);

	s64 index_found;
	String s_find = S("\0", 1);

	while(!String_IsEmpty(&s_buffer_it)) {
		if (!String_Find(&s_buffer_it, s_find, &index_found))
			break;

		if (index_found) {
			Directory_Entry dir_entry;
			dir_entry.s_name = S(s_buffer_it, index_found);
			dir_entry.type   = DIR_ENTRY_DRIVE;

			Array_Add(a_drives_out, dir_entry);
		}

		String_AddOffset(&s_buffer_it, index_found + 1);
	}

	String_Destroy(&s_buffer);

}

instant void
File_ChangePath(
	String *s_dest_io,
	String  s_append
) {
	Assert(s_dest_io);

	if (!(   String_EndWith(&s_append, S("..")    , true)
		  OR String_EndWith(&s_append, S("\\..")  , true)
		  OR String_EndWith(&s_append, S("\\..\\"), true)
	)) {
		if (    !String_EndWith(s_dest_io, S("\\"), true)
			AND s_dest_io->length
		) {
			String_Append(s_dest_io, S("\\"));
		}

		String_Append(s_dest_io, s_append);
		return;
	}

	while(   String_EndWith(s_dest_io, S("\\"), true)
		  OR String_EndWith(s_dest_io, S("/") , true)
	) {
        String_Cut(s_dest_io, s_dest_io->length - 1);
	}

	s64 pos_found;
	bool found = String_FindRev(s_dest_io, S("\\"), &pos_found);

	if (found) {
		String_Cut(s_dest_io, pos_found);
		return;
	}
	else {
		String_Clear(s_dest_io);
		return;
	}
}
