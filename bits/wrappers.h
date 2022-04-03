#define WRAP_NODE_FUNCTIONS(Type, prefix) \
	void *prefix ## add(Type *self, void *dat, Comparator cmp, Transformer trans) \
	{ \
		Type ## Node **node = prefix ## nfd(self, dat, cmp); \
 \
		if (! *node) \
			prefix ## nmk(self, node, trans ? trans(dat) : dat); \
 \
		return (*node)->dat; \
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
	void *prefix ## del(Type *self, void *key, Comparator cmp, Transformer trans) \
	{ \
		Type ## Node **node = prefix ## nfd(self, key, cmp); \
 \
		if (! *node) \
			return NULL; \
 \
		void *dat = trans ? trans((*node)->dat) : (*node)->dat; \
		prefix ## nrm(self, node); \
		return dat; \
	}
