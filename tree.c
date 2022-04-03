#include <stdlib.h>  // for malloc, free
#include "bits/wrappers.h"
#include "tree.h"

static inline TreeNode **search(TreeNode **node, void *key, Comparator cmp)
{
	if (! *node)
		return node;

	int rel = cmp((*node)->dat, key);

	if (rel == 0)
		return node;
	else if (rel > 0)
		return search(&(*node)->lft, key, cmp);
	else // (rel < 0)
		return search(&(*node)->rgt, key, cmp);
}

static inline void traverse(TreeNode *node, Iterator func, void *arg, TreeTraversionOrder order, int delete)
{
	if (! node)
		return;

	if (func && order == TRAVERSION_PREORDER ) func(node->dat, arg);
	traverse(node->lft, func, arg, order, delete);
	if (func && order == TRAVERSION_INORDER  ) func(node->dat, arg);
	traverse(node->rgt, func, arg, order, delete);
	if (func && order == TRAVERSION_POSTORDER) func(node->dat, arg);

	if (delete)
		free(node);
}

void tree_ini(Tree *tree)
{
	tree->rot = NULL;
}

WRAP_NODE_FUNCTIONS(Tree, tree_)

TreeNode **tree_nfd(Tree *tree, void *key, Comparator cmp)
{
	return search(&tree->rot, key, cmp);
}

void tree_nmk(__attribute__((unused)) Tree *tree, TreeNode **node, void *dat)
{
	*node = malloc(sizeof **node);
	(*node)->dat = dat;
	(*node)->lft = (*node)->rgt = NULL;
}

void tree_nrm(__attribute__((unused)) Tree *tree, TreeNode **node)
{
	TreeNode *old = *node;
	*node = old->lft;

	if (old->rgt) {
		TreeNode **rgt = node;

		while (*rgt)
			rgt = &(*rgt)->rgt;

		*rgt = old->rgt;
	}

	free(old);
}

void tree_trv(Tree *tree, Iterator func, void *arg, TreeTraversionOrder order)
{
	traverse(tree->rot, func, arg, order, 0);
}

void tree_clr(Tree *tree, Iterator func, void *arg, TreeTraversionOrder order)
{
	traverse(tree->rot, func, arg, order, 1);
	tree_ini(tree);
}
