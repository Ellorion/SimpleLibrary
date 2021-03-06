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

instant CPU_ID
CPU_GetID(
	u32 id
) {
	CPU_ID cpu_id_out;

	asm volatile
		("cpuid" :
		 "=a" (cpu_id_out.EAX), "=b" (cpu_id_out.EBX),
		 "=c" (cpu_id_out.ECX), "=d" (cpu_id_out.EDX)
		: "a" (id), "c" (0));

    return cpu_id_out;
}

instant String
CPU_GetVendor() {
	auto cpu_id = CPU_GetID(0);

	String s_vendor = String_CreateBuffer(12);

	Memory_Copy(s_vendor.value + 0, (char*)&cpu_id.EBX, 4);
	Memory_Copy(s_vendor.value + 4, (char*)&cpu_id.EDX, 4);
	Memory_Copy(s_vendor.value + 8, (char*)&cpu_id.ECX, 4);

	return s_vendor;
}

instant CPU_Type
CPU_GetInfo() {
	auto cpu_id = CPU_GetID(1);

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
	String s_vendor = CPU_GetVendor();

	bool is_Intel = (s_vendor == "GenuineIntel");
	bool is_AMD   = (s_vendor == "AuthenticAMD");

	auto cpu_id = CPU_GetID(1);

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

	String_Destroy(s_vendor);

	return (cpu_features);
}

instant Array<String>
CPU_GetFeaturesList(
) {
	Array<String> a_features_out;

	CPU_Features cpu_features = CPU_GetFeatures();

	if (cpu_features.mmx)     Array_Add(a_features_out, S("MMX"));
	if (cpu_features.mmx_ext) Array_Add(a_features_out, S("MMX Ext"));

	if (cpu_features.sse)     Array_Add(a_features_out, S("SSE"));
	if (cpu_features.sse2)    Array_Add(a_features_out, S("SSE2"));
	if (cpu_features.sse3)    Array_Add(a_features_out, S("SSE3"));
	if (cpu_features.sse4_1)  Array_Add(a_features_out, S("SSE4.1"));
	if (cpu_features.sse4_2)  Array_Add(a_features_out, S("SSE4.2"));

	if (cpu_features._3dnow)     Array_Add(a_features_out, S("3DNow"));
	if (cpu_features._3dnow_ext) Array_Add(a_features_out, S("3DNow Ext"));

	return a_features_out;
}
