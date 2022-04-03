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

static inline void traverse(TreeNode *node, Iterator iter, void *arg, Transformer trans, TreeTraversionOrder order, int delete)
{
	if (! node)
		return;

	if (iter && order == TRAVERSION_PREORDER ) iter(trans ? trans(node->dat) : node->dat, arg);
	traverse(node->lft, iter, arg, trans, order, delete);
	if (iter && order == TRAVERSION_INORDER  ) iter(trans ? trans(node->dat) : node->dat, arg);
	traverse(node->rgt, iter, arg, trans, order, delete);
	if (iter && order == TRAVERSION_POSTORDER) iter(trans ? trans(node->dat) : node->dat, arg);

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

void tree_trv(Tree *tree, Iterator iter, void *arg, Transformer trans, TreeTraversionOrder order)
{
	traverse(tree->rot, iter, arg, trans, order, 0);
}

void tree_clr(Tree *tree, Iterator iter, void *arg, Transformer trans, TreeTraversionOrder order)
{
	traverse(tree->rot, iter, arg, trans, order, 1);
	tree_ini(tree);
}
