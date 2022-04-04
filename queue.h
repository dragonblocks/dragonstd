/*
	Queue
	-----

	Thread safe FIFO data structure, using a List.
	Useful for working with producer/consumer threads.
*/

#ifndef _DRAGONSTD_QUEUE_H_ // include guard
#define _DRAGONSTD_QUEUE_H_

#include <pthread.h>       // for pthread_cond_t, pthread_mutex_t
#include <stdbool.h>       // for bool
#include "bits/callback.h" // for Transformer
#include "list.h"          // for List

typedef struct {
	/* private */
	List lst;            // list containing data
	unsigned int cnl: 1; // cancel state
	unsigned int fin: 1; // finish state
	pthread_cond_t cnd;  // condition variable for notifying consumer
	pthread_mutex_t mtx; // mutex to protect the condition variable
} Queue;

void queue_ini(Queue *queue);
/*
	Initializes the queue.

	The queue should be uninitialized or deleted when passed to this function.
*/

void queue_dst(Queue *queue);
/*
	Destroy the queue.

	Afterwards, the queue should no longer be used.

	This does not clear the underling list. Use queue_fin or queue_clr to make sure the
		list is cleared before calling this function.
*/

void queue_clr(Queue *queue, Iterator iter, void *arg, Transformer trans);
/*
	Clears the queue.

	Calls list_clr on the underling list. This is not thread safe. Use queue_fin instead
		to wait for consumer threads to finish processing the data.
*/

bool queue_enq(Queue *queue, void *dat);
/*
	[Thread Safe]
	Enqueues an element to the back of the queue.
	Returns true if the enqueueing was successful (this is always the case if queue_fin
		has not been called)

	Notifies waiting consumer threads.
*/

void *queue_deq(Queue *queue, Transformer trans);
/*
	[Thread Safe]
	Dequeue an element.

	If no element is in the queue, wait until there is one.
	The (optional) transformer is called once an element is available and has been
		dequeued and while the queue mutex is still locked.

	Note the the wait continues if the transformer returns NULL.
*/

void queue_cnl(Queue *queue);
/*
	[Thread Safe]
	Cancels dequeueing.
	Subsequent calls to queue_deq immediately return NULL.
*/

void queue_fin(Queue *queue); // disallow enqueing, wait until consumption finished
/*
	[Thread Safe]
	Cancels enqueueing and waits for the queue to finish.
	Subsequent calls to queue_enq have no effect and return false.

	Before returning, this function waits for the queue to be empty.
*/

#endif // _DRAGONSTD_QUEUE_H_
