#pragma once

/// Usage: include after SimpleLibrary.h and run the desired tests.

instant void
Test_Strings(
) {
	char buffer[100];
	String s_data;

	AssertMessage(String_Length(&s_data) == 0, "Initial string length not zero");

	String_Destroy(&s_data);
	AssertMessage(String_Length(&s_data) == 0, "Empty string destruction failed");

	String_Destroy(&s_data);
	AssertMessage(String_Length(&s_data) == 0, "Multiple mpty string destruction failed");

	s_data << "Hello";
	To_CString(buffer, 100, &s_data);
	AssertMessage(String_IsEqual(buffer,  "Hello", 5), "C_String data does not match");
	AssertMessage(String_IsEqual(&s_data, "Hello", 5), "String data does not match (truncated)");
	AssertMessage(String_IsEqual(&s_data, "Hello")   , "String data does not match (autosize)");
	AssertMessage(String_IsEqual(&s_data, "Hel", 3)  , "String data does not match (substring)");
	AssertMessage(String_Length(&s_data) == 5, "Appending c_string to empty string failed");

	String_Destroy(&s_data);
	AssertMessage(String_Length(&s_data) == 0, "Clearing existing data failed");

	s_data << "Hello";
	s_data << " World";
	AssertMessage(String_IsEqual(&s_data, "Hello World"), "String data does not match (append c_string)");
	AssertMessage(String_Length(&s_data) == 11, "Appending on existing data failed");

	s_data << '!';
	AssertMessage(String_IsEqual(&s_data, "Hello World!"), "String data does not match (append char)");
	AssertMessage(String_Length(&s_data) == 12, "Appending single character on existing data failed");

	To_CString(buffer, 6, &s_data);
	AssertMessage(String_Length(buffer) == 5, "Buffer overflow when converting string to c_string");

	String_Destroy(&s_data);

	String s_hello;
	String s_world;

	s_hello << "Hello";
	s_world << " World!";

	s_hello << s_world;
	AssertMessage(String_IsEqual(&s_hello, "Hello World!"), "String data does not match (append string)");
	AssertMessage(String_Length(&s_hello) == 12, "Length error on appending two strings.");

	String_Destroy(&s_data);

	/// store string in buffer (f.e. for filling dynamic string arrays)
	String s_buffer;
	To_String(&s_buffer, "buffer");
	AssertMessage(String_IsEqual(&s_buffer, "buffer"), "String data does not match (buffer)");

	String s_buffer_copy;
	String_Copy(&s_buffer_copy, &s_buffer);
	AssertMessage(String_IsEqual(&s_buffer_copy, "buffer"), "String copy failed (autosize");
	String_Destroy(&s_buffer_copy);

	String_Copy(&s_buffer_copy, &s_buffer, 3);
	AssertMessage(String_IsEqual(&s_buffer_copy, "buf"), "String copy failed (substring)");
	String_Destroy(&s_buffer_copy);

	String_Destroy(&s_buffer);

	Memory_Set(buffer, 0, 100);

	s_buffer << "copy_test";
	To_CString(buffer, 100, &s_buffer);
	String_Copy(buffer + 9, buffer);
	To_String(&s_buffer, buffer);
	AssertMessage(String_IsEqual(&s_buffer, "copy_testcopy_test"), "C_string copy_test failed.");

	String_Destroy(&s_buffer);

	s_buffer << "xxx_test_xxx";
	AssertMessage(String_IndexOf(&s_buffer, "test") == 4, "String indexof failed.");
	AssertMessage(String_StartWith(&s_buffer, "xxx_"), "String startwidth failed.");

	long pos_found = 0;
	if (String_Find(&s_buffer, "_xxx", &pos_found, 0, true)) {
		AssertMessage(pos_found == 12, "Data in string not found at correct index.");
	}
	else {
		AssertMessage(false, "Could not find data in string.");
	}

	if (String_FindRev(&s_buffer, "_xxx", &pos_found, 0, true)) {
		AssertMessage(pos_found == 12, "Data in string not found at correct index.");
	}
	else {
		AssertMessage(false, "Could not find data in string.");
	}

	AssertMessage(String_EndWith(&s_buffer, "_xxx"), "End not found in string.");

	String_Destroy(&s_buffer);

	s_buffer << "aaa_test_bbb";
	AssertMessage(String_IndexOfRev(&s_buffer, "bbb") == 9, "String indexofrev failed.");

	String_Destroy(&s_buffer);

	s_buffer << "AbCDeFG";
	String_ToLower(&s_buffer);
	AssertMessage(String_IsEqual(&s_buffer, "abcdefg"), "String_ToLower failed.");

	String_ToUpper(&s_buffer);
	AssertMessage(String_IsEqual(&s_buffer, "ABCDEFG"), "String_ToUpper failed.");

	String_Reverse(&s_buffer);
	AssertMessage(String_IsEqual(&s_buffer, "GFEDCBA"), "String_Reverse failed.");

	String_Destroy(&s_buffer);

	s_buffer << "   test   ";
	String ts_buffer = s_buffer;
	String_TrimLeft(&ts_buffer);
	String_TrimRight(&ts_buffer);
	AssertMessage(ts_buffer.len == 4, "Trimming test failed.");

	String_Destroy(&s_buffer);

	s_buffer << "aaa__ccc";
    String_Insert(&s_buffer, "bbb", 4);
    AssertMessage(String_IsEqual(&s_buffer, "aaa_bbb_ccc"), "String insertion failed.");

	String_Remove(&s_buffer, 4, 7);
	AssertMessage(String_IsEqual(&s_buffer, "aaa__ccc"), "String removal failed.");

	String s_replace;
	s_replace << "_bbb_";
	String_Replace(&s_buffer, "__", &s_replace);
	AssertMessage(String_IsEqual(&s_buffer, "aaa_bbb_ccc"), "String replacing failed.");

    String_Destroy(&s_replace);
    String_Destroy(&s_buffer);
}

void
Test_Arrays(
) {
	{
		String s_data;
		s_data << "test";

		Array<String> as_test;
		Array_Add(&as_test, s_data);

		FOR_ARRAY(as_test, it) {
			String s_data_it = ARRAY_IT(as_test, it);
			AssertMessage(String_IsEqual(&s_data_it, "test"), "Array_Add failed.");
		}

		Array_Clear(&as_test);
		AssertMessage(		as_test.size  == 0
						AND as_test.count == 0
						AND as_test.limit == 8
						AND as_test.memory > 0, "Clearing array failed.");

		/// Memory ownership in local scope and not in array
		String_Destroy(&s_data);
	}

	{
		Array<String> as_test;
		String *s_item = 0;

		s_item = Array_AddEmpty(&as_test);
		*s_item << "bla";

		FOR_ARRAY(as_test, it) {
			String s_data_it = ARRAY_IT(as_test, it);
			AssertMessage(String_IsEqual(&s_data_it, "bla"), "Array_AddEmpty failed.");
		}

		s_item = Array_AddEmpty(&as_test);
		*s_item << "blub";

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
		s_split << "aaa";

		Array<String> as_split;
		as_split = String_Split(&s_split, "\n", true);

		AssertMessage(as_split.count == 1, "Incorrect amount of items split from string (1).");
		AssertMessage(String_IsEqual(&ARRAY_IT(as_split, 0), "aaa"), "First Array item does not match.");

		while(as_split.count) {
			String s_data_it = Array_Remove(&as_split, 0);
			String_Destroy(&s_data_it);
		}
		Array_Destroy(&as_split);
	}

	{
		String s_split;
		s_split << "aaa\nbbb";

		Array<String> as_split;
		as_split = String_Split(&s_split, "\n", true);

		AssertMessage(as_split.count == 2, "Incorrect amount of items split from string (2).");
		AssertMessage(String_IsEqual(&ARRAY_IT(as_split, 0), "aaa"), "First Array item does not match.");
		AssertMessage(String_IsEqual(&ARRAY_IT(as_split, 1), "bbb"), "Second Array item does not match.");

		while(as_split.count) {
			String s_data_it = Array_Remove(&as_split, 0);
			String_Destroy(&s_data_it);
		}
		Array_Destroy(&as_split);
	}
}
