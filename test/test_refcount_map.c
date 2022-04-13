#include <assert.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include "../map.h"
#include "../refcount.h"

Map map;
atomic_bool cancel;

typedef struct {
	int id;
	Refcount rc;
} DataObject;

void data_object_delete(DataObject *obj)
{
	refcount_dst(&obj->rc);
	free(obj);
}

int rand_id()
{
	return rand() % 1000;
}

int data_object_compare(const void *pa, const void *pb)
{
	return
		((DataObject *) ((const Refcount *) pa)->obj)->id -
		((DataObject *) ((const Refcount *) pb)->obj)->id;
}

int data_object_compare_id(const void *pa, const void *pb)
{
	return
		((DataObject *) ((const Refcount *) pa)->obj)->id -
		*(const int *) pb;
}

static void *thread_create(unsigned int *result)
{
	while (!cancel) {
		DataObject *obj = malloc(sizeof *obj);
		obj->id = rand_id();

		refcount_ini(&obj->rc, obj, (void *) &data_object_delete);

		if (map_add(&map, &obj->rc, &data_object_compare, (void *) &refcount_inc))
			(*result)++;

		refcount_drp(&obj->rc);
	}

	return NULL;
}

#define NUM_OBJS 100

static void *thread_access(unsigned int *result)
{
	DataObject *objs[NUM_OBJS] = {NULL};

	while (!cancel) {
		int i = rand() % NUM_OBJS;

		if (objs[i]) {
			refcount_drp(&objs[i]->rc);
			objs[i] = NULL;
		}

		while (!objs[i] && !cancel) {
			int id = rand_id();
			objs[i] = map_get(&map, &id, &data_object_compare_id, (void *) &refcount_grb);
		}

		if (objs[i])
			(*result)++;
	}

	for (int i = 0; i < NUM_OBJS; i++)
		if (objs[i])
			refcount_drp(&objs[i]->rc);

	return NULL;
}

#undef NUM_OBJS

static void *thread_delete(unsigned int *result)
{
	while (!cancel) {
		unsigned int id = rand_id();

		if (map_del(&map, &id, &data_object_compare_id, (void *) &refcount_drp, NULL, NULL))
			(*result)++;
	}

	return NULL;
}

int main()
{
	srand(time(NULL));

	printf("------------------------\n");
	printf("Testing Map and Refcount\n");
	printf("------------------------\n");

	map_ini(&map);

	void *(*funcs[3])(void *) = {
		(void *) &thread_create,
		(void *) &thread_access,
		(void *) &thread_delete,
	};

	unsigned int results[3][5] = {0};
	pthread_t threads[3][5] = {0};

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 5; j++)
			pthread_create(&threads[i][j], NULL, funcs[i], &results[i][j]);

	sleep(1);

	cancel = true;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 5; j++) {
			pthread_join(threads[i][j], NULL);

			if (j)
				results[i][0] += results[i][j];
		}

	map_cnl(&map, (void *) &refcount_drp, NULL, NULL, 0);
	map_dst(&map);

	printf("Time: 1 second\n");
	printf("Created objects: %u\n", results[0][0]);
	printf("Accessed objects: %u\n", results[1][0]);
	printf("Deleted objects: %u\n", results[2][0]);
}
