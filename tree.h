/*
	Tree
	----

	A binary search tree.

	Note: This structure is included in <search.h> the C library as of POSIX.1-2001 and
		POSIX.1-2008. However, the standard does not contain functions to delete the tree
		and to traverse it with an additional argument. Glibc provides these functions,
		but dragonstd conforms to the POSIX standard and does not use GNU extensions.
		Therefore, this implementation is used to provide the missing features.
*/

#ifndef _DRAGONSTD_TREE_H_ // include guard
#define _DRAGONSTD_TREE_H_

#include <stdbool.h>       // for bool
#include "bits/callback.h" // for Callback, Comparator, Transformer, Callback
#include "bits/compare.h"  // for cmp_ref (not used in file)

typedef struct TreeNode {
	/* public */
	void *dat;            // pointer to data
	/* private */
	struct TreeNode *lft; // left branch (data is "smaller")
	struct TreeNode *rgt; // right branch (data is "bigger")
} TreeNode;

typedef struct {
	/* private */
	TreeNode *rot; // root node
} Tree;

typedef enum {
	TRAVERSION_PREORDER,
	TRAVERSION_INORDER,
	TRAVERSION_POSTORDER,
} TreeTraversionOrder;

void tree_ini(Tree *tree);
/*
	Initializes the tree.

	The tree should be uninitialized or empty before passed to this function.
	This function should be called before any other function is called on the tree.
*/

bool tree_add(Tree *tree, void *dat, Comparator cmp, Transformer trans);
/*
	Add an element to the tree.

	If an equal element is already in the tree, don't add anything.
	Return whether an element has been added.
*/

void *tree_get(Tree *tree, void *key, Comparator cmp, Transformer trans);
/*
	Get an element from the tree, or return NULL if none found.
*/

bool tree_del(Tree *tree, void *key, Comparator cmp, Callback call, void *arg, Transformer trans);
/*
	Delete an element from the tree if it is found.
	Return whether an element has been deleted.
*/

TreeNode **tree_nfd(Tree *tree, void *key, Comparator cmp);
/*
	Find the location of a node matching key.

	This can be the location of an an existing node, or the location where a node matching
		that key would need to be stored.
*/

void tree_nmk(Tree *tree, TreeNode **node, void *dat);
/*
	Create a node with dat as data and store it's pointer at the given location.
*/

void tree_nrm(Tree *tree, TreeNode **node);
/*
	Remove the node at the given location.
*/

void tree_trv(Tree *tree, Callback iter, void *arg, Transformer trans, TreeTraversionOrder order);
/*
	Traverse the tree.
	Calls iter on every element, with the extra argument arg.
*/

void tree_clr(Tree *tree, Callback iter, void *arg, Transformer trans, TreeTraversionOrder order);
/*
	Traverses the tree and deletes all elements.
	Calls iter on every element, with the extra argument arg.

	The tree is empty afterwards.
	If no callback is given, the traversion order is irrelevant.
*/

#endif // _DRAGONSTD_TREE_H_
