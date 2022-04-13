#include "map.h"

static bool get_lock(Map *map, bool write)
{
	bool success;

	pthread_rwlock_rdlock(&map->clk);

	if ((success = ! map->cnl)) {
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

void map_cnl(Map *map, Iterator iter, void *arg, Transformer trans, TreeTraversionOrder order)
{
	pthread_rwlock_wrlock(&map->clk);
	map->cnl = true;

	pthread_rwlock_wrlock(&map->tlk);
	pthread_rwlock_unlock(&map->clk);

	tree_clr(&map->tre, iter, arg, trans, order);

	pthread_rwlock_unlock(&map->tlk);
}

#define WRAP_TREE_FUNC(type, name, write, CallbackType, null) \
	type map_ ## name(Map *map, void *dat, Comparator cmp, CallbackType func) \
	{ \
		if (! get_lock(map, write)) \
			return null; \
 \
		type ret = tree_ ## name(&map->tre, dat, cmp, func); \
		pthread_rwlock_unlock(&map->tlk); \
		return ret; \
	}

WRAP_TREE_FUNC(bool,   add, true,  Transformer, false)
WRAP_TREE_FUNC(void *, get, false, Transformer, NULL)
WRAP_TREE_FUNC(bool,   del, true,  Callback,    false)

void map_trv(Map *map, Iterator iter, void *arg, Transformer trans, TreeTraversionOrder order)
{
	if (! get_lock(map, false))
		return;

	tree_trv(&map->tre, iter, arg, trans, order);
	pthread_rwlock_unlock(&map->tlk);
}
