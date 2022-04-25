#include "bits/callback.h" // for SingleCallback
#include "refcount.h"

void refcount_ini(Refcount *refcount, void *obj, void *del)
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

void *refcount_inc(Refcount *refcount)
{
	pthread_mutex_lock(&refcount->mtx);
	refcount->cnt++;
	pthread_mutex_unlock(&refcount->mtx);
	return refcount;
}

void *refcount_grb(Refcount *refcount)
{
	return refcount_obj(refcount_inc(refcount));
}

void refcount_drp(Refcount *refcount)
{
	pthread_mutex_lock(&refcount->mtx);
	unsigned short count = --refcount->cnt;
	pthread_mutex_unlock(&refcount->mtx);

	if (!count)
		((SingleCallback) refcount->del)(refcount->obj);
}

void *refcount_obj(Refcount *refcount)
{
	return refcount->obj;
}
