#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../flag.h"

#define NUM_FLAGS 5
#define ITER_FLAGS for (int i = 0; i < NUM_FLAGS; i++)

Flag flags[NUM_FLAGS];

void *start_thread(__attribute__((unused)) void *val)
{
	ITER_FLAGS {
		printf("waiting for flag %d\n", i);
		flag_slp(&flags[i]);
	}

	return NULL;
}

bool is_finished()
{
	ITER_FLAGS if (! flags[i].set)
		return false;

	return true;
}

int main()
{
	printf("------------\n");
	printf("Testing Flag\n");
	printf("------------\n");

	srand(time(NULL));
	ITER_FLAGS flag_ini(&flags[i]);

	pthread_t thread;
	pthread_create(&thread, NULL, &start_thread, NULL);

	while (! is_finished()) {
		int i = rand() % NUM_FLAGS;

		printf("setting flag %d\n", i);
		flag_set(&flags[i]);

		nanosleep(&(struct timespec) {
			.tv_sec = 0,
			.tv_nsec = 1e7,
		}, NULL);
	}

	pthread_join(thread, NULL);

	ITER_FLAGS flag_dst(&flags[i]);
}
