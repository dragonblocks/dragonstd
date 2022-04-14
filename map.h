/*
	Map
	---

	Thread safe map/set, using a Tree.
	Useful for managing big amounts of objects with add, get and del access from multiple			threads.
*/

#ifndef _DRAGONSTD_MAP_H_ // include guard
#define _DRAGONSTD_MAP_H_

#include <stdbool.h>       // for bool
#include <pthread.h>       // for pthread_rwlock_t
#include "tree.h"          // for Tree

typedef struct {
	/* private */
	Tree tre;             // search tree to manage data
	pthread_rwlock_t tlk; // lock to protect tree
	bool cnl;             // cancel state
	pthread_rwlock_t clk; // lock to protect cancel state
} Map;

void map_ini(Map *map);
/*
	Initializes the map.

	The map should be uninitialized before passed to this function.
	This function should be called before any other function is called on the tree.
*/

void map_dst(Map *map);
/*
	Destroys the map.

	The map should not be used afterwards.
	Make sure to cancel the map before destroying it, to avoid memory leaks.
*/

void map_cnl(Map *map, void *iter, void *arg, void *trans, TreeTraversionOrder order);
/*
	[Thread Safe]
	Cancels and clears the map.

	All subsequent calls to add, get and del have no effect and return NULL.

	Traverses the map and deletes all elements.
	Calls func on every element, with the extra argument arg.

	The map is empty afterwards.
	If no callback is given, the traversion order is irrelevant.
*/

bool map_add(Map *map, void *dat, void *cmp, void *trans);
/*
	[Thread Safe]
	Add an element to the map.

	If an equal element is already in the map, don't add anything.
	Return whether an element has been added.
*/

void *map_get(Map *map, void *key, void *cmp, void *trans);
/*
	[Thread Safe]
	Get an element from the map, or return NULL if none found.
*/

bool map_del(Map *map, void *key, void *cmp, void *call, void *arg, void *trans);
/*
	[Thread Safe]
	Delete an element from the map if it is found.
	Return whether an element has been deleted.
*/

void map_trv(Map *map, void *iter, void *arg, void *trans, TreeTraversionOrder order);
/*
	[Thread Safe]
	Traverse the map.
	Calls iter on every element, with the extra argument arg.
*/


#endif
