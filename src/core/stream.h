#pragma once

enum class StreamType {
	File = 0,
	Buffer
};

struct Stream {
	StreamType type;
	File file;
	String s_buffer;
};

instant void
Stream_Clear(
	Stream *stream
) {
	Assert(stream);

	String_Clear(&stream->s_buffer);
}

instant void
Stream_Close(
	Stream *stream
) {
	Assert(stream);

	switch (stream->type) {
		case StreamType::File: {
			File_Close(&stream->file);
		} break;

		case StreamType::Buffer: {
			String_Destroy(&stream->s_buffer);
		} break;
	}

	Stream_Clear(stream);
}

instant bool
Stream_Open(
	Stream *stream,
	const File &file
) {
	Assert(stream);

	Stream_Close(stream);

	stream->type = StreamType::File;
	stream->file = file;

	return File_IsOpen(&stream->file);
}

instant String *
Stream_Open(
	Stream *stream
) {
	Assert(stream);

	Stream_Close(stream);

	stream->type = StreamType::Buffer;

	return &stream->s_buffer;
}

Stream &operator<<(Stream &out, const String &s_data) {
	if (String_IsEmpty(&s_data)) {
		LOG_DEBUG("No data available to write in Stream Operator")
	}
	else {
		switch(out.type) {
			case StreamType::File: {
				if (!File_Write(&out.file, s_data)) {
					AssertMessage(false, "File could not be written to with File-Stream Operator.");
				}
			} break;

			case StreamType::Buffer: {
				String_Append(&out.s_buffer, s_data, s_data.length);
			} break;
		}

	}

	return out;
}
