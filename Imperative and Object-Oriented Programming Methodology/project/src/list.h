/**
 *   \file list.h
 *   \brief This module is a part of Johan Windahl/Tor Berglinds second assignment reseplaneraren. It contains a brief implementation of a linked list and a simple iterator.
 */ 

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>


typedef struct _list_t list_t;
typedef struct _iter_t iter_t;

/**
  Create an empty list.
*/
list_t *list_new();

/**
  Add an item to a list (at the end);
*/
void list_add(list_t *, void *);

/**
  Calculate the length of the list.

  \returns The list's length.
*/
int list_len(list_t *);

/**
  Access the nth member of a list. Zero-based.

 
  \returns value of nth element, otherwise NULL
*/
void *list_nth(list_t *, int n);


/**
  Create an iterator to a list. Needs to be freed using \ref
  iter_free.

  \sa http://en.wikipedia.org/wiki/Iterator
*/
iter_t *iter(list_t *);

/**
  Advance the iterator by one step.

  \warning Must only be called if \ref iter_done returns `false`.

  \sa iter_done
*/
void iter_next(iter_t *);

/**
  Test whether the iterator has additional elements.

  \returns `true` if the iterator has additional elements, `false`
  otherwise.
*/
bool iter_done(iter_t *);

/**
  Access the current element.

  \returns The current element.
*/
void *iter_get(iter_t *);

/**
  Free the memory allocated by the iterator.
*/
void iter_free(iter_t *);

/**
  Make a shallow clone of a list.

  This operation is *shallow*, it will *not* clone the elements, just
  the list's spine.

  \returns A shallow clone of the list.
*/
list_t *list_clone(list_t *);

/**
  Frees all memory that the list allocated.
*/
void list_free();


/**
  Removes nth element in the list
*/
void list_remove_nth(list_t *l, int n);

/**
   Finds index in list of specific pointer
 */
int list_get_index(list_t *l, void *p);

/**
   Remove a specific pointer from the list, if in the list
 */
void list_remove(list_t *l, void *p);


#endif // LIST_H
