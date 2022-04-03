/*
	List
	----

	A linked list.
*/

#ifndef _DRAGONSTD_LIST_H_ // include guard
#define _DRAGONSTD_LIST_H_

#include "bits/callback.h" // for Iterator, Comparator

#define LIST_ITERATE(list, node) for (ListNode *node = (list)->fst; node != NULL; node = node->nxt)

typedef struct ListNode {
	/* public */
	void *dat;            // pointer to data
	/* private */
	struct ListNode *nxt; // next node
} ListNode;

typedef struct {
	/* private */
	ListNode *fst;  // first element
	ListNode **end; // where address of next node will be stored
} List;

void list_ini(List *list);
/*
	Initializes the list.

	The list should be uninitialized or empty before passed to this function.
	This function should be called before any other function is called on the list.
*/

void *list_add(List *list, void *dat, Comparator cmp, Transformer trans);
/*
	Add an element to the list.

	If an equal element is already on the list, return it and don't add anything.
	Otherwise, return added element.
*/

void *list_get(List *list, void *key, Comparator cmp, Transformer trans);
/*
	Get an element from the list.

	The first matching element is returned, or NULL if none found.
*/

void *list_del(List *list, void *key, Comparator cmp, Transformer trans);
/*
	Delete an element from the list.

	The first matching element is returned (after being removed from the list), or NULL
		if none found.
*/

void list_apd(List *list, void *dat);
/*
	Append an element at the end of the list.
*/

ListNode **list_nfd(List *list, void *key, Comparator cmp);
/*
	Find the location of the first node matching key.

	This can be either an existing node, or the location the next new node would need to
		be stored.
*/

void list_nmk(List *list, ListNode **node, void *dat);
/*
	Create a node with dat as data and store it's pointer at the given location.
*/

void list_nrm(List *list, ListNode **node);
/*
	Remove the node at the given location.
*/

void list_itr(List *list, Iterator iter, void *arg, Transformer trans);
/*
	Iterate over the list.
	Calls iter on every element, with the extra argument arg.

	Note: the LIST_ITERATE macro can be used to do this without function calls.
*/

void list_clr(List *list, Iterator iter, void *arg, Transformer trans);
/*
	Iterates over the list and deletes all elements.
	Calls iter on every element, with the extra argument arg.

	The list is empty afterwards.
*/

#endif // _DRAGONSTD_LIST_H_
