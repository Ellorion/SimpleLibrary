#pragma once

struct Dependency {
	String s_file;
	Array<String> as_depencency;
};

/// will not include system libraries
instant void
Profiler_AnalyseDependencies(
	String s_path,
	String s_filename,
	Array<Dependency> *a_dependencies_io
) {
	Assert(a_dependencies_io);

	static String s_filepath;
	String_Overwrite(&s_filepath, s_path);
	String_Append(   &s_filepath, s_filename);

	String s_file_data = File_ReadAll(s_filepath);

	Parser parser = Parser_Load(s_file_data, false);
	String s_token;

	while (Parser_IsRunning(&parser)) {
		Parser_SkipUntil(&parser, S("#include"), true);
		Parser_SkipUntilToken(&parser);
		Parser_Token_Get(&parser, &s_token, true);

		if (String_StartWith(&s_token, S("\""), true)) {
			Dependency *dep_entry;
			Dependency  dep_find;

			String_AddOffset(&s_token, 1);
			s_token.length -= 1;

			dep_find.s_file = s_token;

			bool entry_existed = Array_FindOrAdd(a_dependencies_io, dep_find, &dep_entry,
				[](auto element, auto find) {
					return (element.s_file == find.s_file);
				}
			);

			Array_AddUnique(&dep_entry->as_depencency, s_filename);

			if (!entry_existed)
				Profiler_AnalyseDependencies(s_path, s_token, a_dependencies_io);
		}
	}
}

instant void
Profiler_PrintDependencies(
	Array<Dependency> *a_dependencies
) {
	FOR_ARRAY(*a_dependencies, it) {
		Dependency *dep = &ARRAY_IT(*a_dependencies, it);
		String_PrintLine(dep->s_file);

		FOR_ARRAY(dep->as_depencency, it_dep) {
			String *s_dependency = &ARRAY_IT(dep->as_depencency, it_dep);
			String_Print(S("\t"));
			String_PrintLine(*s_dependency);
		}
	}
}
