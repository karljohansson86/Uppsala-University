#include "list.h"

// datatypes
typedef struct link {
  void * data;
  struct link * next;
} link_t;

struct list {
  unsigned int count;
  link_t * first;
  link_t * last;
};

struct list_iterator {
  link_t* cursor;  
};

// hidden/private function prototypes

link_t *_list_get(list_t *list, int index);

link_t * _list_first(list_t *list);

link_t * _list_last(list_t *list);


// functions

list_t *list_new(){
  list_t *list = malloc(sizeof(list_t));
  list->count = 0;
  list->first = NULL;
  list->last = NULL;
  return list;
}

bool list_destroy(list_t *list){
  if(list == NULL){
    return false;
  }
  link_t *cursor =  list->first;
  link_t *next = NULL;
  while(cursor != NULL){
    next = cursor->next;
    free(cursor);
    cursor = next;
  }
  free(list);
  return true;
}

bool list_append(list_t *list, void *data){
  link_t *new = malloc(sizeof(link_t));
  if(new != NULL){
    if(list->last == NULL){
      list->first = new;
    } else {
      list->last->next = new;  
    }
    list->last = new;
    new->data = data;
    new->next = NULL;
    ++list->count;
    return true;
  }
  return false;
}

bool list_prepend(list_t *list, void *data){
  link_t * new = malloc(sizeof(link_t));
  if(new != NULL){
    new->next = list->first;
    new->data = data;
    list->first = new; 
    if(list->last == NULL){
      list->last = new;
    }
    ++list->count;
    return true;
  }
  return false;
}


bool list_insert(list_t *list, int index, void *data ){
  if(index == 0){
    return list_prepend(list, data);  
  }
  else if(index == list->count-1){
    return list_append(list, data);
  }  
  link_t *appendTo = _list_get(list, index-1);
  link_t *new = malloc(sizeof(link_t));
  if(new != NULL){
    new->next = appendTo->next;
    new->data = data;
    appendTo->next = new;
    ++list->count;
    return true;
  }
  return false;
}

bool list_remove(list_t *list, int index){
  if(index < 0){
    index = (list->count + index);
  }
  if(index > list->count || index < 0 ||  list->count == 0){
    return false;
  }
  
  if(list->count == 1) {
    free(list->first);
    list->first = NULL;
    list->last = NULL;
    list->count = 0;
    return true;
  }

  if(index == 0){
    link_t *remove = list->first;
    list->first = remove->next;
    free(remove);
    --list->count;
    return true;
  }

  link_t *previous = _list_get(list, index-1);
  link_t *remove = previous->next;
  previous->next = remove->next;
  if(remove->next == NULL){
    list->last = previous;
  }
  free(remove);
  --list->count;
  return true;
}

void list_empty(list_t* list) {
  while(list_remove(list,0));
}

void *list_get(list_t *list, int index){
  link_t * link = _list_get(list, index);
  if(link != NULL){
    return link->data;
  }
  return NULL;
}

// LIST GET RECURSIVE
link_t* _list_getRec(link_t* link, int index, int cursor) {
  if(link == NULL) {
    return NULL;
  }if(index == cursor) {
    return link;
  }
  return _list_getRec(link->next, index, ++cursor);
}

void* list_getRec(list_t* list, int index) {
  if(index < 0){
    index = (list->count + index);
  }
  if( index < 0 ||  index > list->count || list->count < 1){
    return NULL;
  }

  link_t* link = _list_getRec(list->first, index, 0);
  return link == NULL ? NULL : link->data;
}
// EOF LIST GET RECURSIVE

void** list_getAll(list_t* list) {
  if (list != NULL && list->count > 0) {
    void** items = calloc(list->count, sizeof(void*));
    int index = 0;
    void* data = NULL;
    while((data = list_get(list, index))) {
      items[index] = data;
      // *(items + index) = data
      ++index;
    }
    return items;
  }
  return NULL;
}

link_t *_list_get(list_t *list, int index){
  if(index < 0){
    index = (list->count + index);
  }
  if( index < 0 ||  index > list->count || list->count < 1){
    return NULL;
  }
  if(index == list->count-1){
    return _list_last(list);
  }
  link_t *link = list->first;
  for(int i = 1; i<= index; ++i){
    link = link->next;
  }
  return link;
}

void *list_first(list_t *list){
  if(list->first == NULL){
    return NULL;
  }
  return list->first->data;
}

link_t * _list_first(list_t *list){
  return list->first;
}

void *list_last(list_t *list){
  if(list->last == NULL){
    return NULL;
  }
  return list->last->data;
}

link_t * _list_last(list_t *list){
  return list->last;
}

int list_length(list_t *list){
  return list->count;
}

void *list_find(list_t* list, void* criteria, lc_f compare) {
  void* data = NULL;
  int index = 0;
  while((data = list_get(list, index++))) {
    if(compare(data, criteria)) {
      return data;
    }
  }
  return NULL;
}

void** list_findAll(list_t* list, void* criteria,  lc_f compare, int* resultCount) {
  list_t* results_tmp = list_new();
  void**  results = NULL;

  if(list != NULL) {

    void* data = NULL;
    int index = 0;
    
    while((data = list_get(list, index++))) {
      if(compare(data, criteria)) {
        list_append(results_tmp, data);
      }
    }

    *resultCount = list_length(results_tmp);
    if(*resultCount) {
       results = list_getAll(results_tmp);
    }
  }

  list_destroy(results_tmp);

  return results;
}


/// Iterator

list_iterator_t* list_iterator(list_t* l) {
  list_iterator_t* it =
    (list_iterator_t*) malloc(sizeof(list_iterator_t));
  if(it != NULL) {
    it->cursor = _list_first(l);
  }
  return it;
}

void list_freeIterator(list_iterator_t* it) {
  free(it);
}

void* list_next(list_iterator_t* it) {
  if(it != NULL && it->cursor != NULL) {
    void* data = it->cursor->data;
    it->cursor = it->cursor->next;
    return data;
  }
  return NULL;
}

bool list_more(list_iterator_t* it) {
  if(it != NULL && it->cursor != NULL) {
    if(it->cursor->data != NULL) {
      return true;
    }
  }
  return false;
}
