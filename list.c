#include <assert.h>
#include <stdlib.h>

#include "list.h"

#define GROWTH_FACTOR 2

/* A List holds arbitrary objects and grows dynamically. */
struct List
{
	int length;
	int size;
	const void **elements;
};

/* Returns 1 if list is valid, otherwise 0. */
static int List_is_valid(List_T list)
{
	if (list->length < 0) return 0;
	if (list->length > list->size) return 0;
	if (list->elements == NULL) return 0;
	
	return 1;
}

/* Expand list by GROWTH_FACTOR + 2.  Returns 1 if successful, or 0 if
   insufficient memory. */
static int List_grow(List_T list)
{
	int newLength;
	const void **newElements;
	
	assert(list != NULL);
	
	newLength = (list->size + 1) * GROWTH_FACTOR;
	
	newElements = (const void**) realloc(list->elements,
					     sizeof(void*) * newLength);
	if (newElements == NULL) return 0;
	
	list->size = newLength;
	list->elements = newElements;
	
	return 1;
}

/* Return a new List_T of length len, or NULL if insufficient memory. */
List_T List_new(int len)
{
	List_T newList;
	
	assert(len >= 0);
	
	newList = (struct List*) malloc(sizeof(struct List));
	if (newList == NULL) { return NULL; } /* out of memory */
	
	newList->length = len;
	newList->size = len;
	newList->elements = (const void**) calloc((size_t) newList->size,
						  sizeof(void *));
	if (newList->elements == NULL) { free(newList); return NULL; }
	
	return newList;
}

/* Free list. */
void List_free(List_T list)
{
	assert(list != NULL);
	assert(List_is_valid(list));
	
	free(list->elements);
	free(list);
}

/* Return the number of elements in list. */
int List_get_length(List_T list)
{
	assert(list != NULL);
	
	return list->length;
}

/* Add element to the end of list, expanding list if required.
   Returns 1 if successful, or 0 if insufficient memory. */
int List_add(List_T list, const void *newElement)
{
	assert(list != NULL);
	assert(List_is_valid(list));
	
	if (list->length == list->size) {
		if (! List_grow(list))
			return 0;
	}
	
	list->elements[list->length] = newElement;
	list->length++;
	
	assert(List_is_valid(list));
	
	return 1;
}

/* Return the index'th element of list. */
void *List_get(List_T list, int index)
{
	assert(list != NULL);
	assert(index >= 0);
	assert(index < list->length);
	assert(List_is_valid(list));
	
	return (void *) list->elements[index];
}
