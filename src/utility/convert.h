#pragma once

#include <stdio.h>

instant String
Convert_SizeToString(
	double size
) {
	u32 index = 0;

	const char *units_size[] = {"bytes", "KB", "MB", "GB", "TB"};

	while(size >= 1024 AND index < ARRAY_COUNT(units_size) ) {
		size /= 1024;
		++index;
	}

	char buffer[32];
	snprintf(buffer, 32, "%.2f %s", size, units_size[index]);

	return To_String(buffer);
}
