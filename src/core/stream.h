#pragma once

#define IN_STREAM_VERSION(_object, _min, _max_incl)                                   \
    if ((numcheck >= (_min)) && ((numcheck <= (_max_incl)) || ((_max_incl) == 0))) {  \
        in >> _object;                                                                \
    }

enum class StreamType {
	Buffer,
	File,
};

struct Stream {
	StreamType type = StreamType::Buffer;
	File file;
    String s_buffer;
};

constexpr
instant void
Stream_Clear(
	Stream *stream
) {
	Assert(stream);

	String_Clear(stream->s_buffer);
}

constexpr
instant void
Stream_Close(
	Stream *stream
) {
	Assert(stream);

	switch (stream->type) {
		case StreamType::File: {
			File_Close(stream->file);
		} break;

		case StreamType::Buffer: {
			String_Destroy(stream->s_buffer);
		} break;

		default: {
			AssertMessage(false, "Unhandled StreamType!");
		} break;
	}

	Stream_Clear(stream);
}

/// Usage: auto [stream, success] = Stream_Open(file);
constexpr
instant Tuple<Stream, bool>
Stream_Open(
	File &file
) {
    Stream stream;

	stream.type = StreamType::File;
	stream.file = file;

	return Tuple(stream, File_IsOpen(stream.file));
}

constexpr
instant Tuple<Stream, bool>
Stream_Open(
) {
	Stream stream;
	return Tuple(stream, true);
}

constexpr
instant String
Stream_GetBuffer(
	Stream *stream
) {
	Assert(stream);

	switch (stream->type) {
		case StreamType::File: {
			AssertMessage(false, "File-Stream does not use the internal buffer.");
		} break;

		case StreamType::Buffer: {
			return S(stream->s_buffer);
		} break;

		default: {
			AssertMessage(false, "Unhandled StreamType!");
		} break;
	}

	return {};
}

constexpr
Stream &
operator<<(Stream &out, const String &s_data) {
	if (String_IsEmpty(s_data)) {
		LOG_DEBUG("No data available to write in Stream Operator")
	}
	else {
		switch(out.type) {
			case StreamType::File: {
				if (!File_Write(out.file, s_data)) {
					AssertMessage(false, "File could not be written to with File-Stream Operator.");
				}
			} break;

			case StreamType::Buffer: {
				String_Append(out.s_buffer, s_data, s_data.length);
			} break;

			default:  {
				AssertMessage(false, "Unhandled StreamType!");
			} break;
		}

	}

	return out;
}

template <class T>
constexpr
Stream &
operator<<(Stream &out, const Array<T> a_data) {
    FOR_ARRAY_AUTO(a_data, it) {
        out << *it << S("\n");
    }

    out << S("\n");

    return out;
}

template <class T, int Count>
constexpr
Stream &
operator<<(Stream &out, const Array_Const<T, Count> a_data) {
    FOR_ARRAY_AUTO(a_data, it) {
        out << *it << S("\n");
    }

    out << S("\n");

    return out;
}
