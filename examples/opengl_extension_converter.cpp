#include "src/SLib.h"

int main() {
	String s_ogl_ext = File_ReadAll(S("oglext.txt"));
	File file_ogl_ext = File_Open(S("extensions.h"), "wb");

    Array<String> as_extension = Array_Split(&s_ogl_ext, S("\r\n"), DELIMITER_IGNORE, false);
    if (!as_extension.count)  return 0;

    File_Write(&file_ogl_ext, S("#pragma once\n\n"));
    File_Write(&file_ogl_ext, S("#include <gl/glext.h>\n"));
    File_Write(&file_ogl_ext, S("#include <gl/wglext.h>\n"));
    File_Write(&file_ogl_ext, S("\n"));

    FOR_ARRAY(as_extension, it) {
		String *ts_name = &ARRAY_IT(as_extension, it);

		if (String_StartWith(ts_name, S("#"), true))
			continue;

		File_Write(&file_ogl_ext, S("PFN"));

		static String ts_name_upper;
		String_Clear(&ts_name_upper);
		String_Append(&ts_name_upper, *ts_name);
		String_ToUpper(&ts_name_upper);

		File_Write(&file_ogl_ext, ts_name_upper);

		File_Write(&file_ogl_ext, S("PROC "));
		File_Write(&file_ogl_ext, *ts_name);
		File_Write(&file_ogl_ext, S(" = 0;\n"));
    }

    File_Write(&file_ogl_ext, S("\nvoid\n"));
    File_Write(&file_ogl_ext, S("Window_InitOpenGL_Ext() {\n"));

    FOR_ARRAY(as_extension, it) {
		String *ts_name = &ARRAY_IT(as_extension, it);

		if (String_StartWith(ts_name, S("#"), true))
			continue;

		File_Write(&file_ogl_ext, S("\t"));
		File_Write(&file_ogl_ext, *ts_name);
		File_Write(&file_ogl_ext, S(" = (PFN"));

		static String ts_name_upper;
		String_Clear(&ts_name_upper);
		String_Append(&ts_name_upper, *ts_name);
		String_ToUpper(&ts_name_upper);

		File_Write(&file_ogl_ext, ts_name_upper);

		File_Write(&file_ogl_ext, S("PROC)wglGetProcAddress(\""));
		File_Write(&file_ogl_ext, *ts_name);
		File_Write(&file_ogl_ext, S("\");\n"));
    }

    File_Write(&file_ogl_ext, S("}"));

    File_Close(&file_ogl_ext);
}
