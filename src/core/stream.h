#pragma once

struct Stream {
	File file;
};

instant void
Stream_Clear(
	Stream *stream
) {
	Assert(stream);
}

instant void
Stream_Close(
	Stream *stream
) {
	Assert(stream);

	File_Close(&stream->file);

	Stream_Clear(stream);
}

instant bool
Stream_Open(
	Stream *stream,
	const File &file
) {
	Assert(stream);

	Stream_Close(stream);

	stream->file = file;

	return File_IsOpen(&stream->file);
}

Stream &operator<<(Stream &out, const String &s_data) {
	if (String_IsEmpty(&s_data)) {
		LOG_DEBUG("No data available to write in Stream Operator")
	}
	else
	if (!File_Write(&out.file, s_data)) {
		AssertMessage(false, "File could not be written to with File-Stream Operator.");
	}

	return out;
}
