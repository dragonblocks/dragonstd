#ifndef _DRAGONSTD_FLAG_H_
#define _DRAGONSTD_FLAG_H_

#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>

typedef struct {
	atomic_bool done;
	pthread_cond_t cv;
	pthread_mutex_t mtx;
} Flag;

Flag *flag_create();
void flag_delete(Flag *flag);
void flag_set(Flag *flag);
void flag_wait(Flag *flag);

#endif
