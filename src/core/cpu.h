#pragma once

struct CPU_ID {
	u32 EAX;
	u32 EBX;
	u32 ECX;
	u32 EDX;
};

struct CPU_Type {
	u16 familty_ext;
	u16 model_ext;
	u16 type;
	u16 family;
	u16 model;
	u16 stepping;
};

struct CPU_Features {
	bool mmx;
	bool mmx_ext;
	bool sse;
	bool sse2;
	bool sse3;
	bool sse4_1;
	bool sse4_2;
	bool _3dnow;
	bool _3dnow_ext;
};

instant void
CPU_GetID(
	u32 id,
	CPU_ID *cpu_id_out
) {
	Assert(cpu_id_out);

	asm volatile
		("cpuid" :
		 "=a" (cpu_id_out->EAX), "=b" (cpu_id_out->EBX),
		 "=c" (cpu_id_out->ECX), "=d" (cpu_id_out->EDX)
		: "a" (id), "c" (0));
}

instant char *
CPU_GetVendor() {
	CPU_ID cpu_id;
	CPU_GetID(0, &cpu_id);

	char *vendor = Memory_Create(char, 13);
	Memory_Copy(vendor + 0, (char*)&cpu_id.EBX, 4);
	Memory_Copy(vendor + 4, (char*)&cpu_id.EDX, 4);
	Memory_Copy(vendor + 8, (char*)&cpu_id.ECX, 4);

	return vendor;
}

instant CPU_Type
CPU_GetInfo() {
	CPU_ID cpu_id;
	CPU_GetID(1, &cpu_id);

	CPU_Type cpu_type = {0};

	cpu_type.stepping    = (cpu_id.EAX >>  0) & 0xF;
	cpu_type.model       = (cpu_id.EAX >>  4) & 0xF;
	cpu_type.family      = (cpu_id.EAX >>  8) & 0xF;
	cpu_type.type        = (cpu_id.EAX >> 12) & 0x3;
	cpu_type.model_ext   = (cpu_id.EAX >> 16) & 0xF;
	cpu_type.familty_ext = (cpu_id.EAX >> 20) & 0xFF;

    return (cpu_type);
}

instant CPU_Features
CPU_GetFeatures() {
	char *vendor_name = CPU_GetVendor();

	bool is_Intel 	= String_IsEqual(vendor_name, "GenuineIntel");
	bool is_AMD 	= String_IsEqual(vendor_name, "AuthenticAMD");

	CPU_ID cpu_id;
	CPU_GetID(1, &cpu_id);

	CPU_Features cpu_features = {0};

	if (is_Intel OR is_AMD) {
		cpu_features.mmx_ext    = (cpu_id.EDX >> 22) & 0x1;
		cpu_features.mmx        = (cpu_id.EDX >> 23) & 0x1;
		cpu_features._3dnow_ext = (cpu_id.EDX >> 30) & 0x1;
		cpu_features._3dnow     = (cpu_id.EDX >> 31) & 0x1;

		if (is_Intel) {
			cpu_features.sse    = (cpu_id.EDX >> 25) & 0x1;
			cpu_features.sse2   = (cpu_id.EDX >> 26) & 0x1;
			cpu_features.sse3   = (cpu_id.ECX >>  0) & 0x1;
			cpu_features.sse4_1 = (cpu_id.ECX >> 19) & 0x1;
			cpu_features.sse4_2 = (cpu_id.ECX >> 20) & 0x1;
		}
	}

	Memory_Free(vendor_name);

	return (cpu_features);
}

instant Array<const char *>
CPU_GetFeaturesName(
) {
	Array<const char *> a_features_out;

	CPU_Features cpu_features = CPU_GetFeatures();

	if (cpu_features.mmx)     Array_Add(&a_features_out, "MMX");
	if (cpu_features.mmx_ext) Array_Add(&a_features_out, "MMX Ext");

	if (cpu_features.sse)     Array_Add(&a_features_out, "SSE");
	if (cpu_features.sse2)    Array_Add(&a_features_out, "SSE2");
	if (cpu_features.sse3)    Array_Add(&a_features_out, "SSE3");
	if (cpu_features.sse4_1)  Array_Add(&a_features_out, "SSE4.1");
	if (cpu_features.sse4_2)  Array_Add(&a_features_out, "SSE4.2");

	if (cpu_features._3dnow)     Array_Add(&a_features_out, "3DNow");
	if (cpu_features._3dnow_ext) Array_Add(&a_features_out, "3DNow Ext");

	return a_features_out;
}
