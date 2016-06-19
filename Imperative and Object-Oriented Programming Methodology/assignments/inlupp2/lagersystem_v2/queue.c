#include "queue.h"

//////////////////////////////// Datatypes /////////////////////////////////

typedef enum {FIFO, FILO} queue_type_t;

struct queue {
  queue_type_t type;
  list_t *list;
};


/////////////////// Hidden (internal) function prototypes //////////////////

/**
 *  \brief Create a new queue
 *
 *  Creates a new queue of supplied type on the
 *  heap and returns pointer to the queue.
 *
 *  \param type Type of queue (FIFO/FILO)
 *  \return queue_t* Pointer to queue, NULL if not able to create queue
 */
queue_t* queue_new(queue_type_t type);


///////////////////////// Function Implementations /////////////////////////

queue_t* queue_newFIFO() {
  return queue_new(FIFO);
}

queue_t* queue_newFILO() {
  return queue_new(FILO);
}

void *queue_top(queue_t* queue) {
  if(queue != NULL && list_first(queue->list) != NULL){
    return list_first(queue->list);
  }
  else{
    return NULL;
  }
}

void* queue_pop(queue_t* queue) {
  void* data = NULL;
  list_t* list = queue->list;
  data = list_first(list);
  list_remove(list, 0);
  return data;
}

void* queue_popLast(queue_t* queue) {
  void* data = NULL;
  list_t* list = queue->list;
  data = list_last(list);
  list_remove(list, -1);
  return data;
}

bool queue_push(queue_t* queue, void* data) {
  list_t* list = queue->list;
  if(queue->type == FIFO) {
    return list_append(list, data);
  } else {
    return list_prepend(list, data);
  }
}

int queue_length(queue_t* queue) {
  return list_length(queue->list);
}

void** queue_getAll(queue_t* queue) {
  return list_getAll(queue->list);
}


void queue_empty(queue_t* queue) {
  list_empty(queue->list);
}

void queue_destroy(queue_t* queue) {
  list_destroy(queue->list);
  free(queue);
}

//////////////// Hidden (internal) function implementations ////////////////

queue_t* queue_new(queue_type_t type) {
  queue_t* queue = malloc(sizeof(queue_t));
  if(queue != NULL) {
    queue->type = type;
    queue->list = list_new();
    if(queue->list == NULL) {
      free(queue);
      queue = NULL;
    }
  }
  return queue;
}
