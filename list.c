#include <stdlib.h> // for malloc, free
#include <string.h> // for strcmp
#include "bits/wrappers.h"
#include "list.h"

#define ITER_REFS node = &list->fst; *node != NULL; node = &(*node)->nxt

void list_ini(List *list)
{
	list->fst = NULL;
	list->end = &list->fst;
}

WRAP_NODE_FUNCTIONS(List, list_)

void list_apd(List *list, void *dat)
{
	list_nmk(list, list->end, dat);
}

void list_ppd(List *list, void *dat)
{
	ListNode *fst = list->fst;
	list_nmk(list, &list->fst, dat);
	list->fst->nxt = fst;
}

ListNode **list_nfd(List *list, void *key, Comparator cmp)
{
	ListNode **node;

	for (ITER_REFS)
		if (cmp((*node)->dat, key) == 0)
			return node;

	return node;
}

void list_nmk(List *list, ListNode **node, void *dat)
{
	*node = malloc(sizeof **node);
	(*node)->dat = dat;
	(*node)->nxt = NULL;

	if (list->end == node)
		list->end = &(*node)->nxt;
}

void list_nrm(List *list, ListNode **node)
{
	ListNode *old = *node;
	*node = old->nxt;

	if (list->end == &old->nxt)
		list->end = node;

	free(old);
}

void list_itr(List *list, Callback iter, void *arg, Transformer trans)
{
	LIST_ITERATE(list, node)
		iter(trans ? trans(node->dat) : node->dat, arg);
}

void list_clr(List *list, Callback iter, void *arg, Transformer trans)
{
	for (ListNode *node = list->fst; node != NULL;) {
		ListNode *next = node->nxt;

		if (iter)
			iter(trans ? trans(node->dat) : node->dat, arg);

		free(node);
		node = next;
	}

	list_ini(list);
}
