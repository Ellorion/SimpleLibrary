#pragma once

instant void
Test_Files(
) {
    {
    	String s_file;
    	String s_extension;
    	String_Append(&s_file, "C:/test.cpp");
    	String_Append(&s_extension, ".cpp|.h");

        bool has_ext = File_HasExtension(&s_file, &s_extension);
        AssertMessage(has_ext, "[Test] File extension finding failed (1).");

        String_Destroy(&s_file);
        String_Destroy(&s_extension);
    }

    {
    	String s_file;
    	String s_extension;
    	String_Append(&s_file, "C:/test.cpp");
    	String_Append(&s_extension, ".cp|.h");

        bool has_ext = File_HasExtension(&s_file, &s_extension);
        AssertMessage(!has_ext, "[Test] File extension finding failed (2).");

        String_Destroy(&s_file);
        String_Destroy(&s_extension);
    }

    {
		s64 pos_found;
		String s_pathfile;

		String_Append(&s_pathfile, __FILE__);
		String_Replace(&s_pathfile, "\\", "/");

		if (String_FindRev(&s_pathfile, "/", &pos_found)) {
			String s_path;
			s_path.value = s_pathfile.value;
			s_path.length   = pos_found;

			String s_file;
			s_file.value  = s_pathfile.value  + pos_found + 1;
			s_file.length = s_pathfile.length - pos_found - 1;

			bool file_exists = File_Exists(&s_path, &s_file);
			AssertMessage(file_exists, "[Test] File existence check failed.");
		}

		String_Destroy(&s_pathfile);
    }

    {
    	String s_filename;
    	String_Append(&s_filename, __FILE__);

		File file = File_Open(s_filename.value, "r", s_filename.length);
		AssertMessage(file.fp          , "[Test] File could not be opened.");
		AssertMessage(File_Size(&file) , "[Test] Could not read file size.");

		String s_data = File_Read(&file);
		AssertMessage(s_data.length    , "[Test] Could not read file content.");

		String_Destroy(&s_data);

		AssertMessage(File_Close(&file), "[Test] File could not be closed.");
    }

//    {
//    	Array<String> as_files;
//
//		s64 pos_found;
//		String s_pathfile;
//
//		String_Append(&s_pathfile, __FILE__);
//
//		if (String_FindRev(&s_pathfile, "\\", &pos_found)) {
//			File_ReadDirectory(&as_files, s_pathfile.value, 0, pos_found);
//			AssertMessage(as_files.count, "[Test] Reading directory listing failed.");
//			Array_Destroy(&as_files);
//		}
//
//		String_Destroy(&s_pathfile);
//    }
}
