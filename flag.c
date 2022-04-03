#include "flag.h"

void flag_ini(Flag *flag)
{
	flag->set = false;
	pthread_cond_init(&flag->cnd, NULL);
	pthread_mutex_init(&flag->mtx, NULL);
}

void flag_dst(Flag *flag)
{
	pthread_cond_destroy(&flag->cnd);
	pthread_mutex_destroy(&flag->mtx);
}

void flag_set(Flag *flag)
{
	pthread_mutex_lock(&flag->mtx);
	flag->set = true;
	pthread_cond_broadcast(&flag->cnd);
	pthread_mutex_unlock(&flag->mtx);
}

void flag_slp(Flag *flag)
{
	pthread_mutex_lock(&flag->mtx);
	if (! flag->set)
		pthread_cond_wait(&flag->cnd, &flag->mtx);
	pthread_mutex_unlock(&flag->mtx);
}
