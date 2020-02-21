/// Usage:
///	int main() {
///		Stream stream;
///
///		if (!Stream_Create(&stream, S("file.log"))) {
///			AssertMessage(false, "Error on Stream_Create");
///		}
///
///		stream << S("Hello, World");
///
///		String s_finishline = String_Copy("!");
///		stream << s_finishline;
///
///		stream << S('\r') << S('\n');
///
///		Stream_Close(&stream);
///
///		return 0;
///	}

struct Stream {
	File file;
};

instant void
Stream_Close(
	Stream *stream
) {
	Assert(stream);

	File_Close(&stream->file);

	*stream = {};
}

instant bool
Stream_Create(
	Stream *stream,
	String s_filename
) {
	Assert(stream);

	Stream_Close(stream);

	stream->file = File_Open(s_filename, "wb+");

	return true;
}

Stream &operator<<(Stream &out, const String &s_data) {
	if (!File_Write(&out.file, s_data)) {
		AssertMessage(false, "File could not be written to with stream operator.");
	}

	return out;
}