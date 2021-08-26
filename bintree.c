#include <string.h>
#include <stdlib.h>
#include "bintree.h"

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

static BintreeNode **search_recursive(Bintree *tree, BintreeNode **nodeptr, void *key)
{
	if (*nodeptr) {
		int cond;

		if ((cond = tree->cmp((*nodeptr)->key, key, tree)) == 0)
			return nodeptr;
		else if (cond > 0)
			return search_recursive(tree, &(*nodeptr)->left, key);
		else
			return search_recursive(tree, &(*nodeptr)->right, key);
	} else {
		return nodeptr;
	}
}

BintreeNode **bintree_search(Bintree *tree, void *key)
{
	return search_recursive(tree, &tree->root, key);
}

void bintree_add_node(Bintree *tree, BintreeNode **nodeptr, void *key, void *value)
{
	*nodeptr = malloc(sizeof(BintreeNode));
	(*nodeptr)->key = malloc(tree->key_size);
	memcpy((*nodeptr)->key, key, tree->key_size);
	(*nodeptr)->value = value;
	(*nodeptr)->left = (*nodeptr)->right = NULL;
}

static void free_recursive(BintreeNode *node, BintreeFreeFunction func, void *arg)
{
	if (node) {
		free_recursive(node->left, func, arg);
		free_recursive(node->right, func, arg);
		free(node->key);
		if (func)
			func(node->value, arg);
		free(node);
	}
}

void bintree_clear(Bintree *tree, BintreeFreeFunction func, void *arg)
{
	if (tree) {
		free_recursive(tree->root, func, arg);
		tree->root = NULL;
	}
}
