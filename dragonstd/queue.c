#include <sched.h>         // for sched_yield
#include "bits/callback.h" // for Transformer
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

void queue_clr(Queue *queue, void *iter, void *arg, void *trans)
{
	list_clr(&queue->lst, iter, arg, trans);
}

#define ENQUEUE(queue_fun, list_fun) \
	bool queue_fun(Queue *queue, void *dat) \
	{ \
		bool success = false; \
 \
		pthread_mutex_lock(&queue->mtx); \
		if (!queue->fin) { \
			success = true; \
			list_fun(&queue->lst, dat); \
			pthread_cond_signal(&queue->cnd); \
		} \
		pthread_mutex_unlock(&queue->mtx); \
 \
		return success; \
	}

ENQUEUE(queue_enq, list_apd)
ENQUEUE(queue_ppd, list_ppd)

#undef ENQUEUE

void *queue_deq(Queue *queue, void *trans)
{
	void *dat = NULL;

	pthread_mutex_lock(&queue->mtx);
	while (!queue->cnl && !dat) {
		ListNode **node = &queue->lst.fst;
		if (*node) {
			dat = (*node)->dat;
			list_nrm(&queue->lst, node);

			if (trans)
				dat = ((Transformer) trans)(dat);
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
