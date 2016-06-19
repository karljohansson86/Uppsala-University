#include "list.h"

#ifndef __queue__
#define __queue__

/**
 *  \file queue.h
 *  \brief FIFO/FILO queue
 *  \author Karl Johansson
 *  \author Simon Lövgren
 *
 *  Extension of list.h in form of
 *  FIFO/FILO queue.
 *
 */

/**
 *  \typedef queue_t
 *
 *  Queue data type containing FIFO- or FILO queue
 */
typedef struct queue queue_t;

/**
 *  \brief Creates a new FIFO queue
 *
 *  Creates a new FIFO (First In First Out)
 *  queue.
 *
 *  \warning Each queue must be passed to `queue_destroy()`
 *           at the end of it's life. **Otherwise memory 
 *           leaks may occur.**
 *
 *  \return queue_t* Pointer to queue, NULL if not able to create queue
 */
queue_t* queue_newFIFO();

/**
 *  \brief Creates a new FILO queue
 *
 *  Creates a new FIFO (First In Last Out)
 *  queue.
 *
 *  \warning Each queue must be passed to `queue_destroy()`
 *           at the end of it's life. **Otherwise memory 
 *           leaks may occur.**
 *
 *  \return queue_t* Pointer to queue, NULL if not able to create queue
 */
queue_t* queue_newFILO();

/**
 *  \brief Check if there is an element to pop
 *
 *  \param queue Queue to top
 *  \return bool True if data exists, False otherwise
 */
void *queue_top(queue_t* queue);

/**
 *  \brief Pop element off queue
 *
 *  Pops first element off supplied queue. The element
 *  is removed from the queue and returned from function.
 *
 *  \warning Data will **not** be automatically
 *           freed from memory. A manual call to
 *           `free` must be made for the data after
 *           it is of no further use to prevent
 *           memory leaks.
 *
 *  \param queue Queue to pop element off
 *  \return void* pointer to data, NULL if list is empty
 */
void* queue_pop(queue_t* queue);


/**
 *  \brief Pop last element off queue
 *
 *  Pops last element off supplied queue. The element
 *  is removed from the queue and returned from function.
 *
 *  \warning Data will **not** be automatically
 *           freed from memory. A manual call to
 *           `free` must be made for the data after
 *           it is of no further use to prevent
 *           memory leaks.
 *
 *  \param queue Queue to pop element off
 *  \return void* pointer to data, NULL if list is empty
 */
void* queue_popLast(queue_t* queue);

/**
 *  \brief Push an element onto the queue
 *
 *  Pushes element onto supplied queue in correct
 *  order regarding type of queue.
 *
 *  **FIFO:** Element is appended to the queue.
 *
 *  **FILO:** Element is prepended onto the queue.
 *
 *  \param queue Queue to push element to
 *  \param data Pointer to data
 *  \return bool True if succesful, False otherwise
 */
bool queue_push(queue_t* queue, void* data);


/**
 *  \brief Get length of queue
 *
 *  Get length of queue
 *
 *  \param queue Queue to get length of
 *  \return int Length of queue
 */
int queue_length(queue_t* queue);

/**
 *  \brief Get pointers to all elements in queue
 *
 *  Get an array of pointers to all queue-element data.
 *  To get length of queue, use `queue_length`.
 *
 *  \param queue Queue to get all elements of
 *  \return void** Array of all elements in queue
 */
void** queue_getAll(queue_t* queue);

/**
 *  \brief Empty queue
 *
 *  Empty queue, removing all elements as well
 *  as `free`ing data associated with elements
 *  from memory.
 *
 *  \warning This will free the memory of all
 *           data associated with elements of
 *           the queue.
 *
 *  \param queue Queue to empty
 *  \return void
 */
void queue_empty(queue_t* queue);

/**
 *  \brief Destroy queue
 *
 *  Destroy a queue, removing- and `free`ing
 *  each element and associated data from
 *  memory as well as removing- and `free`ing
 *  the queue itself from memory.
 *
 *  \warning This will free the memory of all
 *           data associated with elements of
 *           the queue.
 *
 *  \param queue Queue to be destroyed
 *  \return void
 */
void queue_destroy(queue_t* queue);

#endif
