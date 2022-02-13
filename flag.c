#include <stdlib.h>
#include "flag.h"

Flag *flag_create()
{
	Flag *flag = malloc(sizeof *flag);
	flag->done = false;
	pthread_cond_init(&flag->cv, NULL);
	pthread_mutex_init(&flag->mtx, NULL);
	return flag;
}

void flag_delete(Flag *flag)
{
	pthread_cond_destroy(&flag->cv);	
	pthread_mutex_destroy(&flag->mtx);
	free(flag);
}

void flag_wait(Flag *flag)
{
	pthread_mutex_lock(&flag->mtx);
	if (! flag->done)
		pthread_cond_wait(&flag->cv, &flag->mtx);
	pthread_mutex_unlock(&flag->mtx);
}

void flag_set(Flag *flag)
{
	pthread_mutex_lock(&flag->mtx);
	flag->done = true;
	pthread_cond_broadcast(&flag->cv);
	pthread_mutex_unlock(&flag->mtx);
}
