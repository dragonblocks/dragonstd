#ifndef _DRAGONSTD_BINTREE_H_
#define _DRAGONSTD_BINTREE_H_

#include <stddef.h>

struct Bintree;

typedef struct BintreeNode
{
	void *key;
	void *value;
	struct BintreeNode *left;
	struct BintreeNode *right;
} BintreeNode;

typedef int (*BintreeComparator)(void *v1, void *v2, struct Bintree *tree);
typedef void (*BintreeTraversionFunction)(BintreeNode *node, void *arg);

typedef struct Bintree
{
	BintreeNode *root;
	size_t key_size;
	BintreeComparator cmp;
} Bintree;

typedef enum
{
	BTT_PREORDER,
	BTT_INORDER,
	BTT_POSTORDER,
} BintreeTraversion;

Bintree bintree_create(size_t key_size, BintreeComparator cmp);
BintreeNode **bintree_search(Bintree *tree, void *key);
void bintree_add_node(Bintree *tree, BintreeNode **nodeptr, void *key, void *value);
void bintree_insert(Bintree *tree, void *key, void *value);
void bintree_traverse(Bintree *tree, BintreeTraversion traversion, BintreeTraversionFunction func, void *arg);
void bintree_clear(Bintree *tree, BintreeTraversionFunction func, void *arg);

#endif
