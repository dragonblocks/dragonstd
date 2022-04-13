#define WRAP_NODE_FUNCTIONS(Type, prefix) \
	bool prefix ## add(Type *self, void *dat, Comparator cmp, Transformer trans) \
	{ \
		Type ## Node **node = prefix ## nfd(self, dat, cmp); \
 \
		if (*node) \
			return false; \
 \
		prefix ## nmk(self, node, trans ? trans(dat) : dat); \
		return true; \
	} \
 \
	void *prefix ## get(Type *self, void *key, Comparator cmp, Transformer trans) \
	{ \
		Type ## Node **node = prefix ## nfd(self, key, cmp); \
 \
		if (! *node) \
			return NULL; \
 \
		return trans ? trans((*node)->dat) : (*node)->dat; \
	} \
 \
	bool prefix ## del(Type *self, void *key, Comparator cmp, Callback call) \
	{ \
		Type ## Node **node = prefix ## nfd(self, key, cmp); \
 \
		if (! *node) \
			return false; \
 \
 		if (call) \
 			call((*node)->dat); \
 \
		prefix ## nrm(self, node); \
		return true; \
	}
