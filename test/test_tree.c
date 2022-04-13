#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../tree.h"

#define NUM_ELEMENTS 1e5

int cmp_int(const void *ia, const void *ib)
{
	return *(const int *) ia - *(const int *) ib;
}

void clear_callback(int *ia, int *ib)
{
	assert(*ia >= *ib);
	*ib = *ia;
	free(ia);
}

int main()
{
	srand(time(NULL));

	printf("------------\n");
	printf("Testing Tree\n");
	printf("------------\n");

	Tree tree;

	printf("testing ini\n");
	tree_ini(&tree);

	int a = 0;
	int b = 1;
	int c = 2;
	int d = 2;
	int e = 3;

	printf("testing add\n");
	assert(tree_add(&tree, &a, &cmp_int, NULL));
	assert(tree_add(&tree, &b, &cmp_int, NULL));
	assert(tree_add(&tree, &c, &cmp_int, NULL));
	assert(!tree_add(&tree, &d, &cmp_int, NULL));

	printf("testing get\n");
	assert(tree_get(&tree, &a, &cmp_int, NULL) == &a);
	assert(tree_get(&tree, &b, &cmp_int, NULL) == &b);
	assert(tree_get(&tree, &c, &cmp_int, NULL) == &c);
	assert(tree_get(&tree, &d, &cmp_int, NULL) == &c);
	assert(tree_get(&tree, &e, &cmp_int, NULL) == NULL);

	printf("testing del\n");
	assert(tree_del(&tree, &a, &cmp_int, NULL));
	assert(tree_get(&tree, &a, &cmp_int, NULL) == NULL);

	printf("testing clr\n");
	tree_clr(&tree, NULL, NULL, NULL, 0);
	assert(tree_get(&tree, &b, &cmp_int, NULL) == NULL);

	printf("testing order and speed with %d elements\n", (int) NUM_ELEMENTS);
	for (int i = 0; i < NUM_ELEMENTS; i++) {
		int *n = malloc(sizeof *n);
		*n = rand();

		if (!tree_add(&tree, n, &cmp_int, NULL))
			free(n);
	}

	int last = -1;
	tree_clr(&tree, (void *) &clear_callback, &last, NULL, TRAVERSION_INORDER);
}
