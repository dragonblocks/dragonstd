#include <string.h> // for memmove, memcpy
#include <stdlib.h> // for malloc, realloc, free, qsort
#include "array.h"

void array_ini(Array *array, size_t mbs, size_t ext, ArrayComparator cmp)
{
	array->mbs = mbs;
	array->ext = ext;
	array->cmp = cmp;
	array->siz = 0;
	array->cap = 0;
	array->ptr = NULL;
}

void array_rlc(Array *array)
{
	array->ptr = realloc(array->ptr, array->cap * array->mbs);
}

void array_grw(Array *array, size_t n)
{
	array->siz += n;

	if (array->siz > array->cap) {
		array->cap = array->siz + array->ext;
		array_rlc(array);
	}
}

void array_shr(Array *array, size_t n)
{
	array->siz -= n;

	if (array->cap > array->siz) {
		array->cap = array->siz;
		array_rlc(array);
	}
}

void array_put(Array *array, const void *ptr, size_t n)
{
	size_t oldsiz = array->siz;
	array_grw(array, 1);

	char *iptr = array->ptr + n * array->mbs;
	memmove(iptr + array->mbs, iptr, (oldsiz - n) * array->mbs);
	memcpy(iptr, ptr, array->mbs);
}

void array_add(Array *array, const void *ptr)
{
	size_t oldsiz = array->siz;
	array_grw(array, 1);

	memcpy(array->ptr + oldsiz * array->mbs, ptr, array->mbs);
}

void array_cpy(Array *array, void **ptr, size_t *n)
{
	*n = array->siz;
	size_t size = array->siz * array->mbs;
	*ptr = malloc(size);
	memcpy(*ptr, array->ptr, size);
}

void array_cln(Array *dst, Array *src)
{
	array_ini(dst, src->mbs, src->ext, src->cmp);
	array_cpy(src, &dst->ptr, &dst->siz);
	dst->cap = dst->siz;
}

void array_rcy(Array *array)
{
	array->siz = 0;
}

void array_del(Array *array)
{
	if (array->ptr)
		free(array->ptr);
}

void array_srt(Array *array)
{
	qsort(array->ptr, array->siz, array->mbs, array->cmp);
}

ssize_t array_fnd(Array *array, const void *ptr, size_t *idx)
{
	size_t low, high, mid;

	low = 0;
	high = array->siz;

	while (low < high) {
		mid = (low + high) / 2;

		int cmp = array->cmp(ptr, array->ptr + mid * array->mbs);

		if (cmp == 0)
			return idx ? (*idx = mid) : mid;
		else if (cmp < 0)
			high = mid;
		else // (cmp > 0)
			low = mid + 1;
	}

	if (idx)
		*idx = low;

	return -1;
}

size_t array_ins(Array *array, const void *ptr)
{
	size_t n;

	array_fnd(array, ptr, &n);
	array_put(array, ptr, n);

	return n;
}
