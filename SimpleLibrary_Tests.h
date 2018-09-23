#pragma once

/// Usage: include after SimpleLibrary.h and run the desired tests.

instant void
Test_Strings(
) {
	char buffer[100];
	String s_data;

	AssertMessage(s_data.length == 0, "Initial string length not zero");

	String_Destroy(&s_data);
	AssertMessage(s_data.length == 0, "Empty string destruction failed");

	String_Destroy(&s_data);
	AssertMessage(s_data.length == 0, "Multiple empty string destruction failed");

	String_Append(&s_data, "Hello");
	To_CString(buffer, 100, &s_data);
	AssertMessage(String_IsEqual(buffer,  "Hello", 5), "C_String data does not match");
	AssertMessage(String_IsEqual(&s_data, "Hello", 5), "String data does not match (truncated)");
	AssertMessage(String_IsEqual(&s_data, "Hello")   , "String data does not match (autosize)");
	AssertMessage(String_IsEqual(&s_data, "Hel", 3)  , "String data does not match (substring)");
	AssertMessage(s_data.length == 5, "Appending c_string to empty string failed");

	String_Destroy(&s_data);
	AssertMessage(s_data.length == 0, "Clearing existing data failed");

	String_Append(&s_data, "Hello");
	String_Append(&s_data, " World");
	AssertMessage(String_IsEqual(&s_data, "Hello World"), "String data does not match (append c_string)");
	AssertMessage(s_data.length == 11, "Appending on existing data failed");

	To_CString(buffer, 6, &s_data);
	AssertMessage(String_Length(buffer) == 5, "Buffer overflow when converting string to c_string");

	String_Destroy(&s_data);

	String s_hello;
	String s_world;

	String_Append(&s_hello, "Hello");
	String_Append(&s_world, " World!");

	String_Append(&s_hello, s_world.value, s_world.length);
	AssertMessage(String_IsEqual(&s_hello, "Hello World!"), "String data does not match (append string)");
	AssertMessage(s_hello.length == 12, "Length error on appending two strings.");

	String_Destroy(&s_data);

	/// store string in buffer (f.e. for filling dynamic string arrays)
	String s_buffer;
	To_StringBuffer(&s_buffer, "buffer");
	AssertMessage(String_IsEqual(&s_buffer, "buffer"), "String data does not match (buffer)");

	String s_buffer_copy;
	s_buffer_copy = String_Copy(s_buffer.value, s_buffer.length);

	AssertMessage(String_IsEqual(&s_buffer_copy, "buffer"), "String copy failed (autosize");
	String_Destroy(&s_buffer_copy);

	s_buffer_copy = String_Copy(s_buffer.value, 3);
	AssertMessage(String_IsEqual(&s_buffer_copy, "buf"), "String copy failed (substring)");
	String_Destroy(&s_buffer_copy);

	String_Destroy(&s_buffer);

	Memory_Set(buffer, 0, 100);

	String_Append(&s_buffer, "copy_test");
	To_CString(buffer, 100, &s_buffer);
	String_Copy(buffer + 9, buffer);
	To_StringBuffer(&s_buffer, buffer);
	AssertMessage(String_IsEqual(&s_buffer, "copy_testcopy_test"), "C_string copy_test failed.");

	String_Destroy(&s_buffer);

	String_Append(&s_buffer, "xxx_test_xxx");
	AssertMessage(String_IndexOf(&s_buffer, "test") == 4, "String indexof failed.");
	AssertMessage(String_StartWith(&s_buffer, "xxx_"), "String startwidth failed.");

	long pos_found = 0;
	if (String_Find(&s_buffer, "_xxx", 0, &pos_found)) {
		AssertMessage(pos_found == 8, "Data in string not found at correct index.");
	}
	else {
		AssertMessage(false, "Could not find data in string.");
	}

	if (String_FindRev(&s_buffer, "_xxx", &pos_found)) {
		AssertMessage(pos_found == 8, "Data in string not found at correct index.");
	}
	else {
		AssertMessage(false, "Could not find data in string.");
	}

	AssertMessage(String_EndWith(&s_buffer, "_xxx"), "End not found in string.");

	String_Destroy(&s_buffer);


	String_Append(&s_buffer, "aaa_test_bbb");
	AssertMessage(String_IndexOfRev(&s_buffer, "bbb") == 9, "String indexofrev failed.");

	String_Destroy(&s_buffer);

	String_Append(&s_buffer, "AbCDeFG");
	String_ToLower(&s_buffer);
	AssertMessage(String_IsEqual(&s_buffer, "abcdefg"), "String_ToLower failed.");

	String_ToUpper(&s_buffer);
	AssertMessage(String_IsEqual(&s_buffer, "ABCDEFG"), "String_ToUpper failed.");

	String_Reverse(&s_buffer);
	AssertMessage(String_IsEqual(&s_buffer, "GFEDCBA"), "String_Reverse failed.");

	String_Destroy(&s_buffer);

	String_Append(&s_buffer, "   test   ");
	String ts_buffer = s_buffer;
	String_TrimLeft(&ts_buffer);
	String_TrimRight(&ts_buffer);
	AssertMessage(ts_buffer.length == 4, "Trimming test failed.");

	String_Destroy(&s_buffer);

	String_Append(&s_buffer, "aaa__ccc");
    String_Insert(&s_buffer, 4, "bbb");
    AssertMessage(String_IsEqual(&s_buffer, "aaa_bbb_ccc"), "String insertion failed.");

	String_Remove(&s_buffer, 4, 7);
	AssertMessage(String_IsEqual(&s_buffer, "aaa__ccc"), "String removal failed.");

	String s_replace;
	String_Append(&s_replace, "_bbb_");
	String_Replace(&s_buffer, "__", &s_replace);
	AssertMessage(String_IsEqual(&s_buffer, "aaa_bbb_ccc"), "String replacing failed.");

    String_Destroy(&s_replace);
    String_Destroy(&s_buffer);

    {
		String s_data;
		String_Append(&s_data, "\r\na\n\r");
		String_Insert(&s_data, s_data.length, 'c');
		AssertMessage(String_IsEqual(&s_data, "\r\na\n\rc"), "Insert String failed (1).");

		String_Insert(&s_data, s_data.length, '\b');
		AssertMessage(String_IsEqual(&s_data, "\r\na\n\r" ), "Insert String failed (2).");

		String_Insert(&s_data, s_data.length, '\b');
		AssertMessage(String_IsEqual(&s_data, "\r\na\n"   ), "Insert String failed (3).");

		String_Insert(&s_data, s_data.length, '\b');
		AssertMessage(String_IsEqual(&s_data, "\r\na"     ), "Insert String failed (4).");

		String_Insert(&s_data, s_data.length, '\b');
		AssertMessage(String_IsEqual(&s_data, "\r\n"      ), "Insert String failed (5).");

		String_Insert(&s_data, s_data.length, '\b');
		AssertMessage(String_IsEqual(&s_data, ""          ), "Insert String failed (6).");

		String_Destroy(&s_data);
    }
}

void
Test_Arrays(
) {
	{
		String s_data;
		String_Append(&s_data, "test");

		Array<String> as_test;
		Array_Add(&as_test, s_data);

		FOR_ARRAY(as_test, it) {
			String s_data_it = ARRAY_IT(as_test, it);
			AssertMessage(String_IsEqual(&s_data_it, "test"), "Array_Add failed.");
		}

		Array_ClearContainer(&as_test);
		AssertMessage(		as_test.size  == 0
						AND as_test.count == 0
						AND as_test.limit == sizeof(String)
						AND as_test.memory > 0, "Clearing array failed.");

		/// Memory ownership in local scope and not in array
		String_Destroy(&s_data);
	}

	{
		Array<String> as_test;
		String *s_item = 0;

		Array_AddEmpty(&as_test, &s_item);
		String_Append(s_item, "bla");

		FOR_ARRAY(as_test, it) {
			String s_data_it = ARRAY_IT(as_test, it);
			AssertMessage(String_IsEqual(&s_data_it, "bla"), "Array_AddEmpty failed.");
		}

		Array_AddEmpty(&as_test, &s_item);
		String_Append(s_item, "blub");

		u64 index_found = 0;
		Array_Find(&as_test, *s_item, &index_found);
		AssertMessage(index_found == 1, "Array_Find failed.");

		while(as_test.count) {
			String s_data_it = Array_Remove(&as_test, 0);
			String_Destroy(&s_data_it);
		}
		AssertMessage(as_test.count == 0, "Array_Remove failed.");
		Array_Destroy(&as_test);
	}

	{
		String s_split;
		String_Append(&s_split, "aaa");

		Array<String> as_split;
		as_split = String_Split(&s_split, "\n");

		AssertMessage(as_split.count == 1, "Incorrect amount of items split from string (1).");
		AssertMessage(String_IsEqual(&ARRAY_IT(as_split, 0), "aaa"), "First Array item does not match.");

		Array_Destroy(&as_split);
	}

	{
		Array<String> as_data;
		String s_data;
		String_Append(&s_data, "demo");
		Array_Add(&as_data, s_data);
		AssertMessage(as_data.limit == sizeof(String), "Data size missmatch.");
		/// Will result in 1 (existing) + 3 (new empty) slots
		Array_ReserveAdd(&as_data, 3, true);
		AssertMessage(as_data.limit == (sizeof(String) * 4) AND as_data.count == 1, "Array reservation failed.");

		/// non-generic array_destroy will free the string
		Array_Destroy(&as_data);
	}

	{
		String s_split;
		String_Append(&s_split, "aaa\nbbb");

		Array<String> as_split;
		as_split = String_Split(&s_split, "\n");

		AssertMessage(as_split.count == 2, "Incorrect amount of items split from string (2).");
		AssertMessage(String_IsEqual(&ARRAY_IT(as_split, 0), "aaa"), "First Array item does not match.");
		AssertMessage(String_IsEqual(&ARRAY_IT(as_split, 1), "bbb"), "Second Array item does not match.");

		Array_Destroy(&as_split);
		String_Destroy(&s_split);
	}

	{
		Array<String> as_data;
		String s_data1;
		String s_data2;
		String s_data3;
		String s_data4;

		String_Append(&s_data3, "3");
		String_Append(&s_data2, "2");
		String_Append(&s_data4, "4");
		String_Append(&s_data1, "1");

		Array_Add(&as_data, s_data3);
		Array_Add(&as_data, s_data2);
		Array_Add(&as_data, s_data4);
		Array_Add(&as_data, s_data1);

		Array_Sort_Ascending(&as_data);

		AssertMessage(		String_IsEqual(&ARRAY_IT(as_data, 0), "1")
						AND String_IsEqual(&ARRAY_IT(as_data, 1), "2")
						AND String_IsEqual(&ARRAY_IT(as_data, 2), "3")
						AND String_IsEqual(&ARRAY_IT(as_data, 3), "4")
							, "String array sorting failed (ascending).");

		Array_Sort_Descending(&as_data);

		AssertMessage(		String_IsEqual(&ARRAY_IT(as_data, 0), "4")
						AND String_IsEqual(&ARRAY_IT(as_data, 1), "3")
						AND String_IsEqual(&ARRAY_IT(as_data, 2), "2")
						AND String_IsEqual(&ARRAY_IT(as_data, 3), "1")
							, "String array sorting failed (descending).");

		Array_Destroy(&as_data);
	}

	{
		Array<s32> a_sort;

		Array_Add(&a_sort, 3);
		Array_Add(&a_sort, 1);
		Array_Add(&a_sort, 4);
		Array_Add(&a_sort, 1);
		Array_Add(&a_sort, 5);
		Array_Add(&a_sort, 9);
		Array_Add(&a_sort, 2);
		Array_Add(&a_sort, 6);
		Array_Add(&a_sort, 5);
		Array_Add(&a_sort, 2);
		Array_Add(&a_sort, 12);

		Array_Sort_Ascending(&a_sort);

		AssertMessage(		ARRAY_IT(a_sort, 0)  == 1
						AND ARRAY_IT(a_sort, 1)  == 1
						AND ARRAY_IT(a_sort, 2)  == 2
						AND ARRAY_IT(a_sort, 3)  == 2
						AND ARRAY_IT(a_sort, 4)  == 3
						AND ARRAY_IT(a_sort, 5)  == 4
						AND ARRAY_IT(a_sort, 6)  == 5
						AND ARRAY_IT(a_sort, 7)  == 5
						AND ARRAY_IT(a_sort, 8)  == 6
						AND ARRAY_IT(a_sort, 9)  == 9
						AND ARRAY_IT(a_sort, 10) == 12
							, "Integer array sorting failed (ascending).");

		Array_Sort_Descending(&a_sort);

		AssertMessage(		ARRAY_IT(a_sort, 0)  == 12
						AND ARRAY_IT(a_sort, 1)  == 9
						AND ARRAY_IT(a_sort, 2)  == 6
						AND ARRAY_IT(a_sort, 3)  == 5
						AND ARRAY_IT(a_sort, 4)  == 5
						AND ARRAY_IT(a_sort, 5)  == 4
						AND ARRAY_IT(a_sort, 6)  == 3
						AND ARRAY_IT(a_sort, 7)  == 2
						AND ARRAY_IT(a_sort, 8)  == 2
						AND ARRAY_IT(a_sort, 9)  == 1
						AND ARRAY_IT(a_sort, 10) == 1
							, "Integer array sorting failed (descending).");
	}

	{
		String s_data;
		String_Append(&s_data, "Lorem ipsum dolor\r\n\r\nLorem ipsum dolor sit amet.");

		///@Performance: text could be processed without having to split
		///              data multiple times
		Array<String> as_lines = Array_Split(&s_data, "\r\n", 2, DELIMITER_ADD_BACK);
		String_Destroy(&s_data);

		Array<String> as_words;

		FOR_ARRAY(as_lines, it_lines) {
			String *ts_line = &ARRAY_IT(as_lines, it_lines);

			Array<String> tas_words = Array_Split(ts_line, " ", 1, DELIMITER_ADD_FRONT);

			FOR_ARRAY(tas_words, it_words) {
				String *ts_word = &ARRAY_IT(tas_words, it_words);
				Array_Add(&as_words, *ts_word);
			}

			/// keep string values in as_words valid
			Array_DestroyContainer(&tas_words);
		}

///		"Lorem ipsum dolor\r\n",
///		"\r\n",
///		"Lorem ipsum dolor sit amet."
		AssertMessage(as_lines.count == 3, "Splitting lines failed.");

///		"Lorem",
///		" ipsum",
///		" dolor\r\n",
///		"\r\n",
///		"Lorem",
///		" ipsum",
///		" dolor",
///		" sit",
///		" amet."
		AssertMessage(as_words.count == 9, "Splitting words failed.");

		Array_Destroy(&as_lines);
		Array_Destroy(&as_words);
	}
}

instant void
Test_Files(
) {
    {
    	String s_file;
    	String s_extension;
    	String_Append(&s_file, "C:/test.cpp");
    	String_Append(&s_extension, ".cpp|.h");

        bool has_ext = File_HasExtension(&s_file, &s_extension);
        AssertMessage(has_ext, "File extension finding failed (1).");

        String_Destroy(&s_file);
        String_Destroy(&s_extension);
    }

    {
    	String s_file;
    	String s_extension;
    	String_Append(&s_file, "C:/test.cpp");
    	String_Append(&s_extension, ".cp|.h");

        bool has_ext = File_HasExtension(&s_file, &s_extension);
        AssertMessage(!has_ext, "File extension finding failed (2).");

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
			AssertMessage(file_exists, "File existence check failed.");
		}

		String_Destroy(&s_pathfile);
    }

    {
    	String s_filename;
    	String_Append(&s_filename, __FILE__);

		File file = File_Open(s_filename.value, "r", s_filename.length);
		AssertMessage(file.fp          , "File could not be opened.");
		AssertMessage(File_Size(&file) , "Could not read file size.");

		String s_data = File_Read(&file);
		AssertMessage(s_data.length    , "Could not read file content.");

		String_Destroy(&s_data);

		AssertMessage(File_Close(&file), "File could not be closed.");
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
//			AssertMessage(as_files.count, "Reading directory listing failed.");
//			Array_Destroy(&as_files);
//		}
//
//		String_Destroy(&s_pathfile);
//    }
}

instant void
Test_Windows(
) {
}
