#ifndef __LIST__
#define __LIST__

#include <stdbool.h>
#include <stdlib.h>

#define USE_STANDARD_FREE  NULL
#define LIST_FREE_DATA     false
#define LIST_LEAVE_DATA    true

typedef struct list list_t;

typedef struct list_iterator list_iterator_t;

typedef bool (*lc_f)(void* data, void* criteria);



/// \file list.h
///
/// \author Tobias Wrigstad
/// \author Karl Johansson 
/// \author Simon Lövgren
/// \version 1.0
/// Creates a new list
///
/// \returns: empty list
list_t *list_new();

/**
 *  \brief Destroys a list and frees associated data
 *
 *  \note Data must be `free`d separately before freeing
 *
 *  \param list       Function to list to destroy
 *  \return bool      True if OK, False otherwise
 */
bool list_destroy(list_t *list);

/// Inserts a new element at the end of the list
///
/// \param list pointer to the list
/// \param elem the integer to be appended
bool list_append(list_t *list, void *data);

/// Inserts a new element at the end of the list
///
/// \param list pointer to the list
/// \param elem the integer to be prepended
bool list_prepend(list_t *list, void *data);

/// Inserts a new element at a given index. 
///
/// Valid indexes are [0..size]. 
///
/// Example:
///
/// list_t *l = list_new(); // l == []
/// list_insert(l, 0, 42);  // l == [42]
/// list_insert(l, 0, 43);  // l == [43, 42]
/// list_insert(l, 1, 44);  // l == [43, 44, 42]
/// list_insert(l, 5, 45);  // l == [43, 44, 42]
///
/// The last case fails (and returns false) because
/// size is 3, meaning 5 is not a valid index. 
///
/// Note that insert at index 0 is the same as prepend 
/// and insert index size is the same as append. 
///
/// Negative indexes should be supported:
///
/// list_insert(l, 1, -45);       // l == [43, 44, 42, 45]
///
/// A positive index can be calculated from a 
/// negative like this: pos_i = size + 1 - neg_i.
/// 
/// \param list  pointer to the list
/// \param index the index for elem to be inserted at
/// \param elem  the integer to be prepended
/// \returns true if succeeded, else false
bool list_insert(list_t *list, int index, void *data ); //int elem

/// Removes an element from a list.
///
/// Example: (assume l == [43, 44, 42, 45])
///
/// int elem;
/// list_remove(l, 1, &elem);  // l = [43, 42, 45], elem == 44
/// list_remove(l, -1, &elem); // l = [43, 42], elem == 45
///
/// \param list  pointer to the list
/// \param index the index to be removed
/// \param elem a pointer to where the element can be stored
/// \returns true if succeeded, else 
bool list_remove(list_t *list, int index);

/**
 *  \brief Empty a list
 *
 *  Removes every element of a list and
 *  `free`s memory of associated data 
 *  but leaves the list.
 *
 *  \warning All associated data will be
 *           `free`d from memory.
 *
 *  \param list List to empty
 *  \return void
 */
void list_empty(list_t* list);

/// Returns the element at a given index
/// \param list  pointer to the list
/// \param index the index to be returns
/// \returns a pointer to the element at index index
void *list_get(list_t *list, int index);

/**
 *  \brief Returns the element at a given index
 *
 *  \param list  pointer to the list
 *  \param index the index to be returns
 *  \returns a pointer to the element at index index
 */
void *list_getRec(list_t *list, int index);

/**
 *  \brief Get pointers to all items in list
 *
 *  Gets an array of data pointers to all items
 *  in supplied list.
 *
 *  \note Use `list_length` to get length of returned array.
 *
 *  \param  list    Pointer to list to get items from
 *  \return void**  Array of data pointers, NULL if empty
 *
 *  \bug Untested and potentially dangerous
 */
void** list_getAll(list_t* list);

/// A convenience for list_get(list, 0)
void *list_first(list_t *list);

/// A convenience for list_get(list, -1)
void *list_last(list_t *list);

/// Returns the length of the list. It is undefined
/// whether the length is calculated in O(n) time or
/// whether there is a size counter in the list object
/// that is manipulated by insert, remove, etc. 
/// \param list the list
/// \returns the length of list
int list_length(list_t *list);


/**
 *  \brief Search for list item
 *
 *  Search for list item using custom function for verifying
 *  find. The compare-function is to read 2 void pointers, the
 *  first is examined list data and the second is supplied
 *  search-criteria, and return bool True if a match is found
 *  (false otherwise).
 *
 *  The compare function can be summarized:
 *  bool compareFunction(void* data, void* criteria);
 *
 *  \param list_t   Pointer to list to search for item in
 *  \param criteria Search criteria passed to compare-function
 *  \param compare  Function pointer to compare-function for comparing list data and search criteria
 *  \return void* Pointer to first matching data
 */
void *list_find(list_t* list, void* criteria, lc_f compare);

/**
 *  \brief Search for all matching items in list
 *
 *  Search for all matching items in list using custom 
 *  function for verifying find. The compare-function is
 *  to read 2 void pointers, the first is examined list
 *  data and the second is supplied search-criteria, and
 *  return bool True if a match is found (false otherwise).
 *
 *  The compare function can be summarized:
 *  bool compareFunction(void* data, void* criteria);
 *
 *  \warning  The returned array must be `free`d after use as it's
 *            stored on the heap and has no connection to the list
 *            other than pointers to list data.
 *
 *  \warning  Data pointed to from returned array is also pointed
 *            to from the supplied lisd. If the list is destroyed
 *            or an item removed, one or all pointers in returned
 *            array will be unusable/inaccurate.
 *
 *  \warning  Do not `free` the data pointed to from the array. It is
 *            still pointed to from the linked list and should be
 *            `free`d until `list_remove` or `list_destroy` is called.
 *
 *
 *  \param  list_t       Pointer to list to search for item in
 *  \param  criteria     Search criteria passed to compare-function
 *  \param  compare      Function pointer to compare-function for comparing list data and search criteria
 *  \param  resultCount  Pointer to where length of result array will be stored
 *  \return void**       Pointer to result array
 *
 *
 *  \bug Untested, highly dangerous and volitile function. Revise before use.
 */
void** list_findAll(list_t* list, void* criteria, lc_f compare, int* resultCount);




// Iterator

/**
 *  \brief Creates an iterator for given list
 *
 *  \param list List to create iterator from
 *  \return list_iterator_t*  Pointer to list iterator
 */
list_iterator_t* list_iterator(list_t* s);

/**
 *  \brief `Free`s an iterator from memory
 *
 *  \param it  Iterator to free
 */
void list_freeIterator(list_iterator_t* it);

/**
 *  \brief Gets next element from iterator
 *
 *  \param it Pointer to iterator
 *  \return void* Pointer to data
 */
void* list_next(list_iterator_t* it);

/**
 *  \brief Checks if iterator has more elements
 *
 *  \param it Pointer to iterator
 *  \return bool True if more elements exist, False otherwise
 */
bool list_more(list_iterator_t* it);


#endif
