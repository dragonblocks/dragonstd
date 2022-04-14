#include "flag.h"

void flag_ini(Flag *flag)
{
	flag->set = false;
	pthread_cond_init(&flag->cnd, NULL);
	pthread_mutex_init(&flag->mtx, NULL);
	list_ini(&flag->cvs);
	flag_sub(flag, &flag->cnd);
}

void flag_dst(Flag *flag)
{
	pthread_cond_destroy(&flag->cnd);
	pthread_mutex_destroy(&flag->mtx);
	list_clr(&flag->cvs, NULL, NULL, NULL);
}

void flag_sub(Flag *flag, pthread_cond_t *cnd)
{
	pthread_mutex_lock(&flag->mtx);
	list_add(&flag->cvs, cnd, cnd, &cmp_ref, NULL);
	pthread_mutex_unlock(&flag->mtx);
}

void flag_uns(Flag *flag, pthread_cond_t *cnd)
{
	pthread_mutex_lock(&flag->mtx);
	list_del(&flag->cvs, cnd, &cmp_ref, NULL, NULL, NULL);
	pthread_mutex_unlock(&flag->mtx);
}

void flag_set(Flag *flag)
{
	pthread_mutex_lock(&flag->mtx);
	flag->set = true;

	LIST_ITERATE(&flag->cvs, node)
		pthread_cond_broadcast(node->dat);

	pthread_mutex_unlock(&flag->mtx);
}

void flag_slp(Flag *flag)
{
	pthread_mutex_lock(&flag->mtx);
	if (!flag->set)
		pthread_cond_wait(&flag->cnd, &flag->mtx);
	pthread_mutex_unlock(&flag->mtx);
}
