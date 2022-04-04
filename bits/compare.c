#include "compare.h"

int cmp_ref(const void *va, const void *vb)
{
	return
		va < vb ? -1 :
		va > vb ? +1 :
		0;
}
