#ifndef LIST_INCLUDED
#define LIST_INCLUDED

/* A List holds arbitrary objects and grows dynamically. */
typedef struct List *List_T;

/* Return a new List_T of length len, or NULL if insufficient memory. */
List_T List_new(int len);

/* Free list. */
void List_free(List_T list);

/* Return the number of elements in list. */
int List_get_length(List_T list);

/* Add element to the end of list, expanding list if required.  Returns
   1 if successful, or 0 if insufficient memory. */
int List_add(List_T list, const void *element);

/* Return the index'th element of list. */
void *List_get(List_T list, int index);

#endif
