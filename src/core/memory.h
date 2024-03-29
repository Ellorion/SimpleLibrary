#pragma once

#define Memory_Create(type, length) \
		((type *)_Memory_Alloc_Empty(sizeof(type) * length))

#define Memory_Resize(buffer, type, length) \
		(buffer \
			? (type *) _Memory_Resize(buffer, sizeof(type) * length) \
			: (type *) Memory_Create(type, length));

#define MEMORY_SIGNATURE 123456

struct Memory_Header {
	u32 sig;
};

constexpr
instant void *
_Memory_Alloc_Empty(
	u64 size
) {
    void *mem = calloc(1, size + sizeof(Memory_Header));

    AssertMessage(mem, "Memory could not be allocated.");

    ((Memory_Header *)mem)->sig = MEMORY_SIGNATURE;
    mem = (char *)mem + sizeof(Memory_Header);

    return mem;
}

instant void
Memory_GetHeader(
	Memory_Header *header_out,
	void *mem
) {
	Assert(header_out);
	Assert(mem);

	mem = (char *)mem - sizeof(Memory_Header);

	*header_out = (*(Memory_Header *)mem);
}

/// will set the free'd pointer to 0
/// - extra security
#define Memory_Free(_data) \
	_data = _Memory_Free(_data);

template <typename T>
constexpr
instant T
_Memory_Free(
	T data
) {
	Assert(std::is_pointer<T>::value);

	if (data == 0)
		return 0;

	Memory_Header mem_header;
	Memory_GetHeader(&mem_header, data);

	if (mem_header.sig == MEMORY_SIGNATURE) {
		void *mem = (char *)data - sizeof(Memory_Header);
		free(mem);

		Assert(mem != data);
	}
	else {
		LOG_WARNING("Trying to free heap pointer(?).")
	}

	return 0;
}

constexpr
instant void
Memory_Copy(
	const void *dest_out,
	const void *src,
	u64 length
) {
	if (!dest_out OR !src OR !length)	return;
	if (dest_out == src)				return;

    char *c_dest = (char *)dest_out;
    char *c_src  = (char *)src;

    if (dest_out > src) {
		while(length-- > 0)
			c_dest[length] = c_src[length];
    }
    else {
		FOR(length, it) {
			c_dest[it] = c_src[it];
		}
    }
}


instant void *
_Memory_Resize(
	void *mem,
	u64 size
) {
	if (!mem)
		return _Memory_Alloc_Empty(size);

	Memory_Header mem_header;
	Memory_GetHeader(&mem_header, mem);

	if (mem_header.sig != MEMORY_SIGNATURE)
		return _Memory_Alloc_Empty(size);

	mem = (char *)mem - sizeof(Memory_Header);

	void *mem_old = mem;

	///@Info: - will NOT keep the same (virtual) memory address!!!
	///       - does NOT init to 0 (zero)
	///       - will also once in a blue moon return 0 when there
	///         is still enouth memory available
	mem = realloc(mem, size + sizeof(Memory_Header));

	AssertMessage(mem, "Memory could not be reallocated.");

	if (mem != mem_old)
		LOG_INFO("Memory resizing resulted in a new base pointer.");

	((Memory_Header *)mem)->sig = MEMORY_SIGNATURE;

	mem = (char *)mem + sizeof(Memory_Header);

	return mem;
}

constexpr
instant void
Memory_Set(
	void *dest_out,
	int data,
	u64 length
) {
	if (dest_out == 0)
		return;

	u8 *c_dest = (u8*)dest_out;
	u8  c_data = data;

	while (length-- > 0)
		*c_dest++ = c_data;
}


/// @Inmportant: remember checking alignment buffer
///
/// true, if equal
instant bool
Memory_Compare(
	void *data_1,
	void *data_2,
	u64   length
) {
	FOR(length, it) {
		if (((char *)data_1)[it] != ((char *)data_2)[it])
			return false;
	}

	return true;
}

/// to bytes
constexpr u64 Kilobyte(u64 value) {
    return value * 1024;
}

constexpr u64 Megabyte(u64 value) {
    return Kilobyte(1) * 1024;
}
constexpr u64 Gigabyte(u64 value) {
    return Megabyte(1) * 1024;
}
