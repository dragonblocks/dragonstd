/*
	Refcount
	--------

	An object that knows how and when to delete itself, using a reference counter.

	Whenever a reference to an object like this is obtained/kept, it needs to be grabbed,
		whenever a reference is discarded, it needs to be dropped.

	Made for use with the List/Tree/Map types: refcount_inc, refcount_grb, refcount_drp
		can be used as add, get, del callbacks.
*/
#ifndef _DRAGONSTD_REFCOUNT_H_ // include guard
#define _DRAGONSTD_REFCOUNT_H_

#include <pthread.h>       // for pthread_mutex_t
#include "bits/callback.h" // for Transformer

typedef struct {
	/* private */
	void *obj;
	Transformer del;
	unsigned short cnt;  // counter
	pthread_mutex_t mtx; // lock to protect count
} Refcount;

void refcount_ini(Refcount *refcount, void *obj, Transformer del);
/*
	Initializes the refcount.

	The refcount should be uninitialized or deleted before passed to this function.
	This function should be called before any other function is called on the refcount.
*/

void refcount_dst(Refcount *refcount);
/*
	Destroy the refcount.

	This does NOT mean delete the object that the reference counter is referring to-
	This means delete the refcount structure itself (and is usually called from the
	delete callback of the referenced object).

	The refcount is unusable until reinitialized afterwards.
*/

void *refcount_inc(void *refcount);
/*
	[Thread Safe]
	Grab a reference to the refcount.
	This actually takes a Refcount * as argument, however void * is used to make it more
		convenient to use the function as callback.

	Returns the refcount.
*/

void *refcount_grb(void *refcount);
/*
	[Thread Safe]
	Does the same as refcount_inc, except it returns the referenced object instead of the
		refcount.
*/

void *refcount_drp(void *refcount);
/*
	[Thread Safe]
	Drop a reference to the object.
	This actually takes a Refcount * as argument, however void * is used to make it more
		convenient to use the function as callback.

	May delete the object using the del function if the counter gets down to zero.
	Returns the return value of the del function if it has been called; returns the object
		otherwise.
*/

void *refcount_obj(void *refcount)
/*
	[Thread Safe]
	Return referenced object.
	This actually takes a Refcount * as argument, however void * is used to make it more
		convenient to use the function as callback.
*/

#endif // _DRAGONSTD_REFCOUNT_H_
