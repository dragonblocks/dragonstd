#include "callback.h" // for Transformer, Callback

#define WRAP_NODE_FUNCTIONS(Type, prefix) \
	bool prefix ## add(Type *self, void *key, void *dat, void *cmp, void *trans) \
	{ \
		Type ## Node **node = prefix ## nfd(self, key, cmp); \
 \
		if (*node) \
			return false; \
 \
		prefix ## nmk(self, node, trans ? ((Transformer) trans)(dat) : dat); \
		return true; \
	} \
 \
	void *prefix ## get(Type *self, void *key, void *cmp, void *trans) \
	{ \
		Type ## Node **node = prefix ## nfd(self, key, cmp); \
 \
		if (!*node) \
			return NULL; \
 \
		return trans ? ((Transformer) trans)((*node)->dat) : (*node)->dat; \
	} \
 \
	bool prefix ## del(Type *self, void *key, void *cmp, void *call, void *arg, void *trans) \
	{ \
		Type ## Node **node = prefix ## nfd(self, key, cmp); \
 \
		if (!*node) \
			return false; \
 \
		if (call) \
			((Callback) call)(trans ? ((Transformer) trans)((*node)->dat) : (*node)->dat, arg); \
 \
		prefix ## nrm(self, node); \
		return true; \
	}
