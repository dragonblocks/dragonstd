#include <assert.h>
#include <stdio.h>
#include "../list.h"

int cmp_int(const int *ia, const int *ib)
{
	return *ia - *ib;
}

int main()
{
	printf("------------\n");
	printf("Testing List\n");
	printf("------------\n");

	List list;

	printf("testing ini\n");
	list_ini(&list);

	int a = 0;
	int b = 1;
	int c = 2;
	int d = 2;
	int e = 3;

	printf("testing add\n");
	assert(list_add(&list, &a, &a, &cmp_int, NULL));
	assert(list_add(&list, &b, &b, &cmp_int, NULL));
	assert(list_add(&list, &c, &c, &cmp_int, NULL));
	assert(!list_add(&list, &d, &d, &cmp_int, NULL));

	printf("testing get\n");
	assert(list_get(&list, &a, &cmp_int, NULL) == &a);
	assert(list_get(&list, &b, &cmp_int, NULL) == &b);
	assert(list_get(&list, &c, &cmp_int, NULL) == &c);
	assert(list_get(&list, &d, &cmp_int, NULL) == &c);
	assert(list_get(&list, &e, &cmp_int, NULL) == NULL);

	printf("testing del\n");
	assert(list_del(&list, &a, &cmp_int, NULL, NULL, NULL));
	assert(list_get(&list, &a, &cmp_int, NULL) == NULL);

	printf("testing clr\n");
	list_clr(&list, NULL, NULL, NULL);
	assert(list_get(&list, &b, &cmp_int, NULL) == NULL);
}
