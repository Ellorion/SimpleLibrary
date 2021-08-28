#pragma once

#define MEMORY_INFO MEMORYSTATUSEX

instant MEMORY_INFO
Memory_GetInfoRAM(
) {
	MEMORY_INFO memInfo;
	memInfo.dwLength = sizeof(MEMORY_INFO);
	GlobalMemoryStatusEx(&memInfo);

	return memInfo;
}

instant unsigned long long
Memory_GetRAMVirtualTotal(
) {
	return Memory_GetInfoRAM().ullTotalPageFile;
}

instant unsigned long long
Memory_GetRAMVirtualCurrent(
) {
	auto memInfo = Memory_GetInfoRAM();
	return (memInfo.ullTotalPageFile - memInfo.ullAvailPageFile);
}

instant unsigned long long
Memory_GetRAMPhysicalTotal(
) {
	return Memory_GetInfoRAM().ullTotalPhys;
}

instant unsigned long long
Memory_GetRAMPhysicalCurrent(
) {
	auto memInfo = Memory_GetInfoRAM();
	return (memInfo.ullTotalPhys - memInfo.ullAvailPhys);
}
