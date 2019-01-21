#include "src/SLib.h"
#include "src/test/core.h"

int main() {
	Tree<int>  myTree;
	Tree<int> *myBranch;
	Tree<int> *myLeaf;

	Tree_AddBranch(&myTree, &myBranch);
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 1;
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 2;
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 3;

	Tree_AddBranch(&myTree, &myBranch);
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 4;
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 5;
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 6;

	Tree_AddBranch(myBranch, &myBranch);
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 7;
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 8;
	Tree_AddLeaf(myBranch, &myLeaf);
	myLeaf->data = 9;

	Tree_Print(&myTree);

	return 0;
}
