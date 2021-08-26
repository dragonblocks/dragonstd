#ifndef _DRAGONTYPE_BINTREE_H_
#define _DRAGONTYPE_BINTREE_H_

#include <stddef.h>

struct Bintree;

typedef int (*BintreeComparator)(void *v1, void *v2, struct Bintree *tree);
typedef void (*BintreeFreeFunction)(void *value, void *arg);

typedef struct BintreeNode
{
	void *key;
	void *value;
	struct BintreeNode *left;
	struct BintreeNode *right;
} BintreeNode;

typedef struct Bintree
{
	BintreeNode *root;
	size_t key_size;
	BintreeComparator cmp;
} Bintree;

Bintree bintree_create(size_t key_size, BintreeComparator cmp);
BintreeNode **bintree_search(Bintree *tree, void *key);
void bintree_add_node(Bintree *tree, BintreeNode **nodeptr, void *key, void *value);
void bintree_clear(Bintree *tree, BintreeFreeFunction func, void *arg);

#endif
