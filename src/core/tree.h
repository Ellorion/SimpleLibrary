#pragma once

template <typename T>
struct Tree {
	T data;
	String s_name;

	bool is_leaf     = false;
	bool is_expanded = false;

	Array<Tree<T>> a_branch;
	Array<Tree<T>> a_leaf;
};

template <typename T>
instant void
Tree_AddBranch(
	Tree<T> *tree_io,
	Tree<T> **branch_out,
	const char *c_name,
	u64 c_length = 0
) {
	Assert(tree_io);
	Assert(!tree_io->is_leaf);

	Assert(branch_out);

	Array_AddEmpty(&tree_io->a_branch, branch_out);
	String_Append(&(*branch_out)->s_name, c_name, c_length);
}

template <typename T>
instant void
Tree_AddLeaf(
	Tree<T> *tree_io,
	Tree<T> **leaf_out
) {
	Assert(tree_io);
	Assert(leaf_out);

	Array_AddEmpty(&tree_io->a_leaf, leaf_out);
	(*leaf_out)->is_leaf = true;
}

template <typename T>
instant bool
Tree_Find(
	Tree<T> *tree_io,
	Tree<T> **branch_out,
	const char *c_name,
	u64 c_length = 0
) {
	Assert(tree_io);
	Assert(branch_out);

	FOR_ARRAY(tree_io->a_branch, it) {
        Tree<T> *t_branch = &ARRAY_IT(tree_io->a_branch, it);

        if (String_IsEqual(&t_branch->s_name, c_name, c_length)) {
			*branch_out = t_branch;
			return true;
        }
	}

	*branch_out = 0;
	return false;
}

template <typename T>
instant void
Tree_Print(
	Tree<T> *tree,
	u32 level = 0
) {
	FOR(level, it)
		std::cout << "\t";

	if (tree->is_leaf) {
		std::cout << tree->data << std::endl;
		return;
	}

	if (level != 0)
		String_Print(&tree->s_name);
	else
		std::cout << "- root -";

	std::cout << std::endl;

	if (!tree->is_expanded)
		return;

	FOR_ARRAY(tree->a_leaf, it)
        Tree_Print(&ARRAY_IT(tree->a_leaf, it)  , level+1);

	FOR_ARRAY(tree->a_branch, it)
        Tree_Print(&ARRAY_IT(tree->a_branch, it), level+1);
}
