#include "bits/callback.h" // for Transformer
#include "map.h"

static bool get_lock(Map *map, bool write)
{
	bool success;

	pthread_rwlock_rdlock(&map->clk);

	if ((success = !map->cnl)) {
		if (write)
			pthread_rwlock_wrlock(&map->tlk);
		else
			pthread_rwlock_rdlock(&map->tlk);
	}

	pthread_rwlock_unlock(&map->clk);

	return success;
}

void map_ini(Map *map)
{
	tree_ini(&map->tre);
	pthread_rwlock_init(&map->tlk, NULL);
	map->cnl = false;
	pthread_rwlock_init(&map->clk, NULL);
}

void map_dst(Map *map)
{
	pthread_rwlock_destroy(&map->tlk);
	pthread_rwlock_destroy(&map->clk);
}

void map_cnl(Map *map, void *iter, void *arg, void *trans, TreeTraversionOrder order)
{
	pthread_rwlock_wrlock(&map->clk);
	map->cnl = true;

	pthread_rwlock_wrlock(&map->tlk);
	pthread_rwlock_unlock(&map->clk);

	tree_clr(&map->tre, iter, arg, trans, order);

	pthread_rwlock_unlock(&map->tlk);
}

bool map_add(Map *map, void *key, void *dat, void *cmp, void *trans)
{
	if (!get_lock(map, true))
		return false;

	bool ret = tree_add(&map->tre, key, dat, cmp, trans);
	pthread_rwlock_unlock(&map->tlk);
	return ret;
}

void *map_get(Map *map, void *key, void *cmp, void *trans)
{
	if (!get_lock(map, false))
		return NULL;

	void *ret = tree_get(&map->tre, key, cmp, trans);
	pthread_rwlock_unlock(&map->tlk);
	return ret;
}

bool map_del(Map *map, void *key, void *cmp, void *call, void *arg, void *trans)
{
	if (!get_lock(map, true))
		return false;

	bool ret = tree_del(&map->tre, key, cmp, call, arg, trans);
	pthread_rwlock_unlock(&map->tlk);
	return ret;
}

void map_trv(Map *map, void *iter, void *arg, void *trans, TreeTraversionOrder order)
{
	if (!get_lock(map, false))
		return;

	tree_trv(&map->tre, iter, arg, trans, order);
	pthread_rwlock_unlock(&map->tlk);
}
