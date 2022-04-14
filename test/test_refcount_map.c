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

int rand_id()
{
	return rand() % 1000;
}

void delete_obj(DataObject *obj)
{
	refcount_dst(&obj->rc);
	free(obj);
}

int cmp_obj(const Refcount *rc, const int *id)
{
	return ((DataObject *) rc->obj)->id - *id;
}

int cmp_obj_sym(const Refcount *rc1, const Refcount *rc2)
{
	return cmp_obj(rc1, &((DataObject *) rc2->obj)->id);
}

static void *thread_create(unsigned int *result)
{
	while (!cancel) {
		DataObject *obj = malloc(sizeof *obj);
		obj->id = rand_id();

		refcount_ini(&obj->rc, obj, &delete_obj);

		if (map_add(&map, &obj->rc, &cmp_obj_sym, &refcount_inc))
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
			objs[i] = map_get(&map, &id, &cmp_obj, &refcount_grb);
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

		if (map_del(&map, &id, &cmp_obj, &refcount_drp, NULL, NULL))
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

	map_cnl(&map, &refcount_drp, NULL, NULL, 0);
	map_dst(&map);

	printf("Time: 1 second\n");
	printf("Created objects: %u\n", results[0][0]);
	printf("Accessed objects: %u\n", results[1][0]);
	printf("Deleted objects: %u\n", results[2][0]);
}
