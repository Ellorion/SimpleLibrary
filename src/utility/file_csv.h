#pragma once

instant Array<Array<String>>
File_CSV_Load(
	String s_filename
) {
	String s_content = File_ReadAll(s_filename);
	Array<String> as_rows = Array_SplitLinesRef(&s_content, false);

	Array<Array<String>> a_csv;
	Array<String>        as_rowitem;

	FOR_ARRAY(as_rows, it_row) {
		String *s_row = &ARRAY_IT(as_rows, it_row);

		/// can get overwritten after the prev. split was copied
		as_rowitem = Array_Split(s_row, S(","), DELIMITER_IGNORE, true);
		/// will copy the content, so do not destroy it
		Array_Add(&a_csv, as_rowitem);
	}

	String_Destroy(&s_content);

	return a_csv;
}
