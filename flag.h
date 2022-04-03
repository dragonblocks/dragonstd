/*
	Flag
	----

	A flag is a data structure that can be used to syncronize a boolean across multiple
		threads.

	The flag's state can be set, read and waited for in a thread safe manner.
*/

#ifndef _DRAGONSTD_FLAG_H_ // include guard
#define _DRAGONSTD_FLAG_H_

#include <pthread.h>   // for pthread_mutex_t, pthread_cond_t
#include <stdatomic.h> // for atomic_bool
#include <stdbool.h>   // for bool

typedef struct {
	/* protected */
	atomic_bool set;     // whether the flag is set
	/* private */
	pthread_cond_t cnd;  // condition variable used for waiting
	pthread_mutex_t mtx; // mutex to protect the condition variable
} Flag;

void flag_ini(Flag *flag);
/*
	Initializes the flag.

	The flag should be uninitialized or deleted before passed to this function.
	This function should be called before any other function is called on the flag.
*/

void flag_dst(Flag *flag);
/*
	Destroy the flag.

	The refcount is unusable until reinitialized afterwards.
*/

void flag_set(Flag *flag);
/*
	[Thread Safe]
	Sets the flag.

	This changes the flag state to be true and wakes up any threads waiting for it.
	Afterwards, **the state cannot be changed back**.

	This function can be called multiple times.
*/

void flag_slp(Flag *flag);
/*
	[Thread Safe]
	Waits for the flag to be true.

	This will sleep until the flag's state is changed to true, unless it is already set to
		true.
*/

#endif // _DRAGONSTD_FLAG_H_
