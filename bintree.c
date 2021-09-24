#include <string.h>
#include <stdlib.h>
#include "bintree.h"

typedef struct
{
	BintreeTraversionFunction func;
	void *arg;
} BintreeFreeData;

static int bintree_compare_mem(void *v1, void *v2, Bintree *tree)
{
	return memcmp(v1, v2, tree->key_size);
}

Bintree bintree_create(size_t key_size, BintreeComparator cmp)
{
	return (Bintree) {
		.root = NULL,
		.key_size = key_size,
		.cmp = cmp ? cmp : &bintree_compare_mem,
	};
}

static BintreeNode **search_recursive(Bintree *tree, BintreeNode **nodeptr, void *key, bool return_existing)
{
	if (*nodeptr) {
		int cond;

		if ((cond = tree->cmp((*nodeptr)->key, key, tree)) == 0 && return_existing)
			return nodeptr;
		else if (cond > 0)
			return search_recursive(tree, &(*nodeptr)->left, key, return_existing);
		else
			return search_recursive(tree, &(*nodeptr)->right, key, return_existing);
	} else {
		return nodeptr;
	}
}

BintreeNode **bintree_search(Bintree *tree, void *key)
{
	return search_recursive(tree, &tree->root, key, true);
}

void bintree_add_node(Bintree *tree, BintreeNode **nodeptr, void *key, void *value)
{
	*nodeptr = malloc(sizeof(BintreeNode));
	(*nodeptr)->key = malloc(tree->key_size);
	memcpy((*nodeptr)->key, key, tree->key_size);
	(*nodeptr)->value = value;
	(*nodeptr)->left = (*nodeptr)->right = NULL;
}

void bintree_insert(Bintree *tree, void *key, void *value)
{
	bintree_add_node(tree, search_recursive(tree, &tree->root, key, false), key, value);
}

static void bintree_free(BintreeNode *node, void *arg)
{
	BintreeFreeData *fdata = arg;

	if (fdata->func)
		fdata->func(node, fdata->arg);

	free(node->key);
	free(node);
}

void bintree_clear(Bintree *tree, BintreeTraversionFunction func, void *arg)
{
	if (tree) {
		BintreeFreeData fdata = {
			.func = func,
			.arg = arg,
		};

		bintree_traverse(tree, BTT_POSTORDER, &bintree_free, &fdata);
		tree->root = NULL;
	}
}

static void traverse_recursive(BintreeNode *node, BintreeTraversion traversion, BintreeTraversionFunction func, void *arg)
{
	if (node) {
		if (traversion == BTT_PREORDER)
			func(node, arg);

		traverse_recursive(node->left, traversion, func, arg);

		if (traversion == BTT_INORDER)
			func(node, arg);

		traverse_recursive(node->right, traversion, func, arg);

		if (traversion == BTT_POSTORDER)
			func(node, arg);
	}
}

void bintree_traverse(Bintree *tree, BintreeTraversion traversion, BintreeTraversionFunction func, void *arg)
{
	traverse_recursive(tree->root, traversion, func, arg);
}
