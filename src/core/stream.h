#pragma once

enum class StreamFileMode {
	Clear = 0, 	/// create new file or erase  existing (read & write)
	Append,		/// create new file or append existing (read & write)
};

struct Stream {
	StreamFileMode mode;
	File file;
	String s_data;
};

instant void
Stream_Close(
	Stream *stream
) {
	Assert(stream);

	File_Close(&stream->file);
	String_Destroy(&stream->s_data);

	*stream = {};
}

instant bool
Stream_OpenFile(
	Stream *stream,
	String s_data,
	StreamFileMode mode
) {
	Assert(stream);

	Stream_Close(stream);

	stream->mode   = mode;
	stream->s_data = String_Copy(s_data);

	switch (stream->mode) {
		case StreamFileMode::Clear: {
			stream->file = File_Open(stream->s_data, "wb+");
		} break;

		case StreamFileMode::Append: {
			stream->file = File_Open(stream->s_data, "ab+");
		} break;

		default: {
			AssertMessage(false, "Unhandled StreamFileMode");
		} break;
	}

	return true;
}

Stream &operator<<(Stream &out, const String &s_data) {
	if (!File_Write(&out.file, s_data)) {
		AssertMessage(false, "File could not be written to with stream operator.");
	}

	return out;
}
