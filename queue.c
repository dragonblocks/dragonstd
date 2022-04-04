#include <sched.h>
#include "queue.h"

void queue_ini(Queue *queue)
{
	list_ini(&queue->lst);
	queue->cnl = queue->fin = 0;
	pthread_cond_init(&queue->cnd, NULL);
	pthread_mutex_init(&queue->mtx, NULL);
}

void queue_dst(Queue *queue)
{
	pthread_cond_destroy(&queue->cnd);
	pthread_mutex_destroy(&queue->mtx);
}

void queue_clr(Queue *queue, Iterator iter, void *arg, Transformer trans)
{
	list_clr(&queue->lst, iter, arg, trans);
}

bool queue_enq(Queue *queue, void *dat)
{
	bool success = false;

	pthread_mutex_lock(&queue->mtx);
	if (! queue->fin) {
		success = true;
		list_apd(&queue->lst, dat);
		pthread_cond_signal(&queue->cnd);
	}
	pthread_mutex_unlock(&queue->mtx);

	return success;
}

void *queue_deq(Queue *queue, Transformer trans)
{
	void *dat = NULL;

	pthread_mutex_lock(&queue->mtx);
	while (! queue->cnl && ! dat) {
		ListNode **node = &queue->lst.fst;
		if (*node) {
			dat = (*node)->dat;
			list_nrm(&queue->lst, node);

			if (trans)
				dat = trans(dat);
		} else {
			pthread_cond_wait(&queue->cnd, &queue->mtx);
		}
	}
	pthread_mutex_unlock(&queue->mtx);

	return dat;
}

void queue_cnl(Queue *queue)
{
	pthread_mutex_lock(&queue->mtx);
	queue->cnl = 1;
	pthread_cond_broadcast(&queue->cnd);
	pthread_mutex_unlock(&queue->mtx);
}

void queue_fin(Queue *queue)
{
	pthread_mutex_lock(&queue->mtx);
	queue->fin = 1;
	pthread_mutex_unlock(&queue->mtx);

	for (;;) {
		pthread_mutex_lock(&queue->mtx);
		ListNode *node = queue->lst.fst;
		pthread_mutex_unlock(&queue->mtx);

		if (node)
			sched_yield();
		else
			break;
	}
}
