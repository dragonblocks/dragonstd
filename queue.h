#ifndef _DRAGONSTD_QUEUE_H_
#define _DRAGONSTD_QUEUE_H_

#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "list.h"

typedef struct
{
	atomic_bool cancel;
	bool finish;
	List list;
	pthread_cond_t cv;
	pthread_mutex_t mtx;
} Queue;

Queue *queue_create();
void queue_delete(Queue *queue);
void queue_enqueue(Queue *queue, void *elem);
void *queue_dequeue(Queue *queue);
void *queue_dequeue_callback(Queue *queue, void (*callback)(void *elem));
void queue_cancel(Queue *queue); // disallow dequeing
void queue_finish(Queue *queue); // disallow enqueing, wait until consumption finished

#endif
