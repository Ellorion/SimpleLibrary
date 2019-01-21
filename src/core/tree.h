#pragma once

template <typename T>
struct Tree {
	T data;
	bool is_leaf = false;

	Array<Tree<T>> a_branch;
	Array<Tree<T>> a_leaf;
};

template <typename T>
instant void
Tree_AddBranch(
	Tree<T> *tree,
	Tree<T> **branch
) {
	Assert(tree);
	Assert(!tree->is_leaf);

	Assert(branch);

	Array_AddEmpty(&tree->a_branch, branch);
}

template <typename T>
instant void
Tree_AddLeaf(
	Tree<T> *tree,
	Tree<T> **leaf
) {
	Assert(tree);
	Assert(leaf);

	Array_AddEmpty(&tree->a_leaf, leaf);
	(*leaf)->is_leaf = true;
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
		std::cout << "- branch -" << std::endl;
	else
		std::cout << "- tree -"   << std::endl;

	FOR_ARRAY(tree->a_leaf, it)
        Tree_Print(&ARRAY_IT(tree->a_leaf, it)  , level+1);

	FOR_ARRAY(tree->a_branch, it)
        Tree_Print(&ARRAY_IT(tree->a_branch, it), level+1);
}
