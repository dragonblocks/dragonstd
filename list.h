#ifndef _DRAGONSTD_LIST_H_
#define _DRAGONSTD_LIST_H_

#include <stdbool.h>

#define ITERATE_LIST(list, pair) for (ListPair *pair = (list)->first; pair != NULL; pair = pair->next)

typedef struct ListPair
{
	struct ListPair *next;
	void *key;
	void *value;
} ListPair;

typedef bool (*ListComparator)(void *v1, void *v2);

typedef struct
{
	ListComparator cmp;
	ListPair *first;
} List;

bool list_compare_default(void *v1, void *v2);
bool list_compare_string(void *v1, void *v2);

List list_create(ListComparator cmp);
void list_clear(List *list);
void list_clear_func(List *list, void (*func)(void *key, void *value, void *arg), void *arg);

bool list_put(List *list, void *key, void *value);
void *list_get_cached(List *list, void *key, void *(*provider)(void *key));
void list_set(List *list, void *key, void *value);
void *list_get(List *list, void *key);
void *list_delete(List *list, void *key);

bool list_serialize(int fd, List *list); // ToDo
bool list_deserialize(int fd, List *list); // ToDo

#endif
