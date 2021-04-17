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

	String s_filepath;
	String_Overwrite(s_filepath, s_path);
	String_Append(   s_filepath, s_filename);
	String_Append(   s_filepath, S("\0", 1));

	String s_file_data;
	File_ReadAll(&s_file_data, s_filepath);

	Parser parser = Parser_Load(s_file_data, S(""));
	String s_token;

	while (Parser_IsRunning(&parser)) {
		Parser_SkipUntil(&parser, S("#include"), true);
		Parser_Token_Get(&parser, &s_token, true);

		/// in case a file is parsed, where #include is converted into a string
		/// (like this file, a few lines above)
		if (s_token != " ")
			continue;

		Parser_Token_Get(&parser, &s_token, true);

		Dependency dep_find;

		if (String_StartWith(s_token, S("\""), true)) {
			Dependency *dep_entry;

			String_AddOffset(s_token, 1);
			s_token.length -= 1;

			dep_find.s_file = String_Copy(s_path);
			String_Append(dep_find.s_file, s_token);

			bool entry_existed = Array_FindOrAdd(*a_dependencies_io, dep_find, &dep_entry,
				[](auto element, auto find) {
					return (element.s_file == find.s_file);
				}
			);

			/// would become invalid in case more than one file is being analyzed
			{
				if (!entry_existed)
					dep_entry->s_file = String_Copy(dep_entry->s_file);

				String *added_element;
				if (Array_AddUnique(dep_entry->as_depencency, s_filepath, &added_element)) {
					*added_element = String_Copy(*added_element);
				}
			}

			if (!entry_existed) {
				String s_subpath = S(s_path);

				s64 index_pos = String_IndexOfRev(s_token, S("//", 1), true);

				if (index_pos > 0) {
					s_subpath = String_Copy(s_subpath);
					/// include "//"
					String_Append(s_subpath, S(s_token, index_pos + 1));

					String_AddOffset(s_token, index_pos + 1);
				}

				Profiler_AnalyseDependencies(s_subpath, s_token, a_dependencies_io);

				String_Destroy(s_subpath);
			}
		}

		String_Destroy(dep_find.s_file);
	}

	String_Destroy(s_filepath);
	String_Destroy(s_file_data);
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

instant void
Array_Destroy(
	Array<Dependency> *a_dependencies
) {
	FOR_ARRAY(*a_dependencies, it) {
		Dependency *dep = &ARRAY_IT(*a_dependencies, it);
		String_Destroy(dep->s_file);

		FOR_ARRAY(dep->as_depencency, it_dep) {
			String *s_dependency = &ARRAY_IT(dep->as_depencency, it_dep);
			String_Destroy(*s_dependency);
		}

		Array_DestroyContainer(dep->as_depencency);
	}

	Array_DestroyContainer(*a_dependencies);
}
