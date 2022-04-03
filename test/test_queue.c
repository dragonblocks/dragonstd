#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../queue.h"

#define CHUNK_SIZE 1e5

Queue queue;

void *consumer(__attribute__((unused)) void *arg)
{
	char *data;

	while ((data = queue_deq(&queue, NULL)) != NULL) {
		// In a real world scenario, this would be something CPU intensive
		char c = 0;

		for (int i = 0; i < CHUNK_SIZE; i++)
			c += data[i];

		// How to convert char to int with zero-extend??? (MOVZX instruction on x86)
		// I literally have no idea why cast/coersion doesn't work, union seems to work.
		// But is it undefined behavior? And is there a better way to do it?
		// I can't access StackOverflow rn so I'm leaving it like this, for now.
		// I feel extremely stupid, feel free to make fun of me.
		// Also, some cute stuff I found: 376825 (You can thank me later シ)
		union {
			int i;
			char c;
		} u;
		u.i = 0;
		u.c = c;

		printf("%02x", u.i);
		fflush(stdout);

		free(data);
	}

	return NULL;
}

void *producer(__attribute__((unused)) void *arg)
{
	// In a real world scenario, this would be something with actual latency,
	// e.g. a network connection
	// In this example, I'm creating the latency by allocating huge chunks of memory.
	// Sometimes I kinda get the feeling I've fallen pretty low, but then I remember
	// JavaScript programmers exist
	// (DERZOMBIIIE IF YOU READ THIS IM SORRY NOTHING PERSONAL LUV U <3)
	FILE *file = fopen("/dev/random", "r");
	void *buffer;

	do
		fread(buffer = malloc(CHUNK_SIZE), 1, CHUNK_SIZE, file);
	while (queue_enq(&queue, buffer));

	free(buffer);
	fclose(file);

	return NULL;
}

int main()
{
	printf("-------------\n");
	printf("Testing Queue\n");
	printf("-------------\n");

	printf("Random bullshit go: ");
	queue_ini(&queue);

	pthread_t threads[17];

	for (int i = 0; i < 17; i++)
		pthread_create(&threads[i], NULL, i ? &consumer : &producer, NULL);

	sleep(1);

	queue_fin(&queue);
	queue_cnl(&queue);

	for (int i = 0; i < 17; i++)
		pthread_join(threads[i], NULL);

	queue_del(&queue);
	printf("\n");
}

// Btw I figured out my MOVZX problem from earlier, but it's funny so I'm leaving it
// I even figured it out without StackOverflow pls tell me you're proud of me
// The reason why char -> int sometimes extends to 0 and sometimes to F is this:
// char is signed by default, so if the first bit (sign bit) is on, it will extend that
// to the first bit on the int (MOVSX instruction)
// just use unsigned char to avoid it

// ngl was even tempted to use inline assembly this is what happens to your brain when
// you work offline

// for real tho, why tf is char signed by default
// its probably machine dependent (since there is the explicit signed qualifier for char)

// This file is probably the biggest bogus I've ever published anyway
