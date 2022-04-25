/*
	Array
	-----

	Data structure where all elements (members) have the same size and are
		located consecutively in memory.

*/

#ifndef _DRAGONSTD_ARRAY_H_ // include guard
#define _DRAGONSTD_ARRAY_H_

#include <stddef.h>       // for size_t
#include <sys/types.h>    // for ssize_t
#include "bits/compare.h" // for cmp_ref (not used in file)

typedef struct {
	/* public */
	void *ptr;  // memory
	size_t ext; // extra space
	/* private */
	size_t mbs; // member size
	size_t siz; // used space
	size_t cap; // available space
} Array;

void array_ini(Array *array, size_t mmb, size_t ext);
/*
	Initializes the array.

	The array will have the member size of mmb and the extra space set to ext.
	mmb should be bigger than 0 and may not be changed after the initialization.
	ext can be 0 or bigger and may be changed any time.

	This function should be called before calling any other functions on the
		array.

	This function should not be called on an array that has been initialized before,
		unless the array has a capacity of 0. Otherwise a memory leak will occur.
*/

void array_rlc(Array *array);
/*
	Reallocates the array's memory to match it's capacity.

	This function should be called every time the capacity has changed.
*/

void array_grw(Array *array, size_t n);
/*
	Grows the array by n bytes.

	This function increases the arrays size by n bytes. If this exceeds the capacity of
		the array, the capacity set to the size and the extra space ext is added to it.

	If the capacity is changed, the array is reallocated.

	If n is zero, the array's capacity may still grow by extra space if it exactly
		matches the current size.
*/

void array_shr(Array *array, size_t n);
/*
	Shrinks the array by n bytes.

	This function decreases the arrays size by n bytes.

	If the array has additional capacity left after it has been shrunk, the capacity
		is set to the new size and the array is reallocated to fit the new capacity.
		For n > 0, this is always the case, for n = 0, this may be the case.

	Note that calling this function with n = 0 is useful to shrink the array's memory to
		exactly fit it's used size.
*/

void array_put(Array *array, const void *ptr, size_t n);
/*
	Grows the array by 1 and inserts ptr at the index n.

	This function inserts the memory pointed to by ptr before the array member at
		index n, moving all elements from that index to the end of the array.

	After this operation, the inserted element will be _at_ the index n.

	The memory that ptr points to, which's size is assumed to be at least as big as the
		array's member size is copied into the arrays memory.

	n should be in the range from 0 to the array's size.
*/

void array_apd(Array *array, const void *ptr);
/*
	Grows the array by 1 and appends ptr at the end of the array.

	This function's result is equivalent to calling array_put(array, ptr, array->siz),
		but it is slightly faster since it saves unnecessary calls.
*/

ssize_t array_idx(Array *array, const void *ptr);
/*
	Returns the index of the first element that equals ptr, or none if no matches.

	Uses memcmp to compare the elements.
*/

void array_cpy(Array *array, void **ptr, size_t *n);
/*
	Allocates a buffer big enough to fit the array's used size.
	Copies the array's contents into the allocated buffer.
	Returns the buffer in ptr and the size in n.

	Note that the returned size is the number of elements, not the number of bytes.
*/

void array_cln(Array *dst, Array *src);
/*
	Clones the array src to the array dst.

	dst is initialized to have the same configuration (member size, extra space) as src.

	After the operation, the contents of the array dst are be the same as those of src.
		The size of dst and src are the same, the capacity of dst however is the same as
		the size of dst and src (which might not equal the capacity of src).

	Since array_ini is called on dst, it should be uninitialized, empty or deleted.
*/

void array_rcy(Array *array);
/*
	Recycles the array.

	This function sets the used size of the array to 0 but leaves the capacity unchanged.
	The array's memory is not free'd and the array can be reused.
*/

void array_clr(Array *array);
/*
	Clears the array.

	This function frees the arrays memory. If this is not called when the array's
		reference is dropped, a memory leak occurs, unless the array is empty (capacity
		of 0), in which case the function does not need to be called. The function works
		fine on empty arrays however.

	After this, the array is empty and can be reused.
*/

void array_srt(Array *array, void *cmp);
/*
	Sorts the array using the quicksort algorithm.

	Comparator must not be NULL.
	Wraps the qsort C-library routine. Please refer to it's documentation.
*/

ssize_t array_fnd(Array *array, const void *ptr, size_t *idx, void *cmp);
/*
	Searches the sorted array for the element ptr.
	Returns the index of the element, or -1 if it wasn't found.

	If idx is not NULL, a pointer to the last searched index is saved to where it
		points to. This is the index ptr would need to be inserted at to keep the order.

	It is assumed that the array has been sorted by array_srt before (or was empty),
		and the order has been kept and the same comparator is used.
*/

size_t array_ins(Array *array, const void *ptr, void *cmp);
/*
	Inserts an element into a sorted array, keeping the order.
	Returns the index the element has been inserted at.

	Calls array_fnd and array_put.

	It is assumed that the array has been sorted by array_srt before (or was empty),
		and the order has been kept and the same comparator is used.
*/

#endif // _DRAGONSTD_ARRAY_H_
