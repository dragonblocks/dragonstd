#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue *queue_create()
{
	Queue *queue = malloc(sizeof(Queue));
	queue->cancel = false;
	queue->list = list_create(NULL);
	pthread_cond_init(&queue->cv, NULL);
	pthread_mutex_init(&queue->mtx, NULL);
	return queue;
}

void queue_delete(Queue *queue)
{
	pthread_cond_destroy(&queue->cv);
	pthread_mutex_destroy(&queue->mtx);
	list_clear(&queue->list);
	free(queue);
}

void queue_enqueue(Queue *queue, void *elem)
{
	pthread_mutex_lock(&queue->mtx);
	list_put(&queue->list, elem, NULL);
	pthread_cond_signal(&queue->cv);
	pthread_mutex_unlock(&queue->mtx);
}

void *dequeue(Queue *queue)
{
	return queue_dequeue_callback(queue, NULL);
}

void *queue_dequeue_callback(Queue *queue, void (*callback)(void *elem))
{
	void *elem = NULL;

	while (! queue->cancel && ! elem) {
		pthread_mutex_lock(&queue->mtx);

		ListPair **lptr = &queue->list.first;
		if (*lptr) {
			elem = (*lptr)->key;
			ListPair *next = (*lptr)->next;
			free(*lptr);
			*lptr = next;

			if (callback)
				callback(elem);
		} else {
			pthread_cond_wait(&queue->cv, &queue->mtx);
		}

		pthread_mutex_unlock(&queue->mtx);
	}

	return elem;
}

void queue_cancel(Queue *queue)
{
	queue->cancel = true;

	pthread_mutex_lock(&queue->mtx);
	pthread_cond_broadcast(&queue->cv);
	pthread_mutex_unlock(&queue->mtx);
}
