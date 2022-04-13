#include "refcount.h"

void refcount_ini(Refcount *refcount, void *obj, Callback del)
{
	refcount->obj = obj;
	refcount->del = del;
	refcount->cnt = 1;
	pthread_mutex_init(&refcount->mtx, NULL);
}

void refcount_dst(Refcount *refcount)
{
	pthread_mutex_destroy(&refcount->mtx);
}

void *refcount_inc(void *refcount)
{
	Refcount *rc = refcount;

	pthread_mutex_lock(&rc->mtx);
	rc->cnt++;
	pthread_mutex_unlock(&rc->mtx);
	return rc;
}

void *refcount_grb(void *refcount)
{
	return refcount_obj(refcount_inc(refcount));
}

void refcount_drp(void *refcount)
{
	Refcount *rc = refcount;

	pthread_mutex_lock(&rc->mtx);
	unsigned short count = --rc->cnt;
	pthread_mutex_unlock(&rc->mtx);

	if (! count)
		rc->del(rc->obj);
}

void *refcount_obj(void *refcount)
{
	return ((Refcount *) refcount)->obj;
}
