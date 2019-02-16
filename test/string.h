#pragma once

instant void
Test_Strings(
) {
	char buffer[100];
	String s_data;

	AssertMessage(s_data.length == 0, "[Test] Initial string length not zero");

	String_Destroy(&s_data);
	AssertMessage(s_data.length == 0, "[Test] Empty string destruction failed");

	String_Destroy(&s_data);
	AssertMessage(s_data.length == 0, "[Test] Multiple empty string destruction failed");

	String_Append(&s_data, "Hello");
	To_CString(buffer, 100, &s_data);
	AssertMessage(String_IsEqual(buffer,  "Hello", 5), "[Test] C_String data does not match");
	AssertMessage(String_IsEqual(&s_data, "Hello", 5), "[Test] String data does not match (truncated)");
	AssertMessage(String_IsEqual(&s_data, "Hello")   , "[Test] String data does not match (autosize)");
	AssertMessage(String_IsEqual(&s_data, "Hel", 3)  , "[Test] String data does not match (substring)");
	AssertMessage(s_data.length == 5, "[Test] Appending c_string to empty string failed");

	String_Destroy(&s_data);
	AssertMessage(s_data.length == 0, "[Test] Clearing existing data failed");

	String_Append(&s_data, "Hello");
	String_Append(&s_data, " World");
	AssertMessage(String_IsEqual(&s_data, "Hello World"), "[Test] String data does not match (append c_string)");
	AssertMessage(s_data.length == 11, "[Test] Appending on existing data failed");

	To_CString(buffer, 6, &s_data);
	AssertMessage(String_GetLength(buffer) == 5, "[Test] Buffer overflow when converting string to c_string");

	String_Destroy(&s_data);

	String s_hello;
	String s_world;

	String_Append(&s_hello, "Hello");
	String_Append(&s_world, " World!");

	String_Append(&s_hello, s_world.value, s_world.length);
	AssertMessage(String_IsEqual(&s_hello, "Hello World!"), "[Test] String data does not match (append string)");
	AssertMessage(s_hello.length == 12, "[Test] Length error on appending two strings.");

	String_Destroy(&s_data);

	/// store string in buffer (f.e. for filling dynamic string arrays)
	String s_buffer;
	To_StringBuffer(&s_buffer, "buffer");
	AssertMessage(String_IsEqual(&s_buffer, "buffer"), "[Test] String data does not match (buffer)");

	String s_buffer_copy;
	s_buffer_copy = String_Copy(s_buffer.value, s_buffer.length);

	AssertMessage(String_IsEqual(&s_buffer_copy, "buffer"), "[Test] String copy failed (autosize");
	String_Destroy(&s_buffer_copy);

	s_buffer_copy = String_Copy(s_buffer.value, 3);
	AssertMessage(String_IsEqual(&s_buffer_copy, "buf"), "[Test] String copy failed (substring)");
	String_Destroy(&s_buffer_copy);

	char *c_buffer_temp_copy = String_CreateCBufferCopy(s_buffer.value, s_buffer.length);
	AssertMessage(String_IsEqual(c_buffer_temp_copy, "buffer"), "[Test] String copy to C buffer failed");
	Memory_Free(c_buffer_temp_copy);

	String_Destroy(&s_buffer);

	Memory_Set(buffer, 0, 100);

	String_Destroy(&s_buffer);

	String_Append(&s_buffer, "xxx_test_xxx");
	AssertMessage(String_IndexOf(&s_buffer, "test") == 4, "[Test] String indexof failed.");
	AssertMessage(String_StartWith(&s_buffer, "xxx_"), "[Test] String startwidth failed.");

	long pos_found = 0;
	if (String_Find(&s_buffer, "_xxx", 0, &pos_found)) {
		AssertMessage(pos_found == 8, "[Test] Data in string not found at correct index.");
	}
	else {
		AssertMessage(false, "[Test] Could not find data in string.");
	}

	if (String_FindRev(&s_buffer, "_xxx", &pos_found)) {
		AssertMessage(pos_found == 8, "[Test] Data in string not found at correct index.");
	}
	else {
		AssertMessage(false, "[Test] Could not find data in string.");
	}

	AssertMessage(String_EndWith(&s_buffer, "_xxx"), "[Test] End not found in string.");

	String_Destroy(&s_buffer);

	String_Append(&s_buffer, "aaa_test_bbb");
	AssertMessage(String_IndexOfRev(&s_buffer, "bbb") == 9, "[Test] String indexofrev failed.");

	String_Destroy(&s_buffer);

	String_Append(&s_buffer, "AbCDeFG");
	String_ToLower(&s_buffer);
	AssertMessage(String_IsEqual(&s_buffer, "abcdefg"), "[Test] String_ToLower failed.");

	String_ToUpper(&s_buffer);
	AssertMessage(String_IsEqual(&s_buffer, "ABCDEFG"), "[Test] String_ToUpper failed.");

	String_Reverse(&s_buffer);
	AssertMessage(String_IsEqual(&s_buffer, "GFEDCBA"), "String_Reverse failed.");

	String_Destroy(&s_buffer);

	String_Append(&s_buffer, "   test   ");
	String ts_buffer = s_buffer;
	String_TrimLeft(&ts_buffer);
	String_TrimRight(&ts_buffer);
	AssertMessage(ts_buffer.length == 4, "[Test] Trimming test failed.");

	String_Destroy(&s_buffer);

	String_Append(&s_buffer, "aaa__ccc");
    String_Insert(&s_buffer, 4, "bbb");
    AssertMessage(String_IsEqual(&s_buffer, "aaa_bbb_ccc"), "[Test] String insertion failed.");

	String_Remove(&s_buffer, 4, 7);
	AssertMessage(String_IsEqual(&s_buffer, "aaa__ccc"), "[Test] String removal failed.");

	String s_replace;
	String_Append(&s_replace, "_bbb_");
	String_Replace(&s_buffer, "__", &s_replace);
	AssertMessage(String_IsEqual(&s_buffer, "aaa_bbb_ccc"), "[Test] String replacing failed.");

    String_Destroy(&s_replace);
    String_Destroy(&s_buffer);

    {
		String s_data;
		String_Append(&s_data, "\r\na\n\r");
		String_Insert(&s_data, s_data.length, 'c');
		AssertMessage(String_IsEqual(&s_data, "\r\na\n\rc"), "[Test] Insert String failed (1).");

		String_Insert(&s_data, s_data.length, '\b');
		AssertMessage(String_IsEqual(&s_data, "\r\na\n\r" ), "[Test] Insert String failed (2).");

		String_Insert(&s_data, s_data.length, '\b');
		AssertMessage(String_IsEqual(&s_data, "\r\na\n"   ), "[Test] Insert String failed (3).");

		String_Insert(&s_data, s_data.length, '\b');
		AssertMessage(String_IsEqual(&s_data, "\r\na"     ), "[Test] Insert String failed (4).");

		String_Insert(&s_data, s_data.length, '\b');
		AssertMessage(String_IsEqual(&s_data, "\r\n"      ), "[Test] Insert String failed (5).");

		String_Insert(&s_data, s_data.length, '\b');
		AssertMessage(String_IsEqual(&s_data, ""          ), "[Test] Insert String failed (6).");

		String_Destroy(&s_data);
    }
}
