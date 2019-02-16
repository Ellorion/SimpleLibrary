#include "src/SLib.h"
#include "test/core.h"

int main() {
	Tree<int>  myTree;
	Tree<int> *myBranch;
	Tree<int> *myLeaf;

	myTree.is_expanded = true;

	Tree_AddBranch(&myTree, &myBranch, "- branch 01 -");
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 1;
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 2;
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 3;

	myBranch->is_expanded = true;

	Tree_AddBranch(&myTree, &myBranch, "- branch 02 -");
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 4;
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 5;
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 6;

	myBranch->is_expanded = true;

	Tree_AddBranch(&myTree, &myBranch, "- branch 03 -");
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 10;
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 11;
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 12;

	if (Tree_Find(&myTree, &myBranch, "- branch 02 -")) {
		Tree_AddBranch(myBranch, &myBranch, "- branch 02.1 -");
		Tree_AddLeaf(myBranch, &myLeaf);
		myLeaf->data = 7;
		Tree_AddLeaf(myBranch, &myLeaf);
		myLeaf->data = 8;
		Tree_AddLeaf(myBranch, &myLeaf);
		myLeaf->data = 9;

		myBranch->is_expanded = true;
	}

	Tree_Print(&myTree);

	return 0;
}
