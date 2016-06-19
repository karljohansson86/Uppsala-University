#include <stdlib.h>
#include "list.h"

struct list {
  struct node *first;
  struct node *last;
};

struct node {
  void *value;
  struct node *next;
};

struct iterator {
  struct node *current;
};

list *mkList() {
  return calloc(1, sizeof(list));
}

struct node *mkLink(struct node *n, void *v) {
  struct node *result = malloc(sizeof(struct node));
  if (result) {
    result->next = n;
    result->value = v;
  }
  return result;
}

iterator *mkIterator(list *list){
  iterator *it = calloc(1,sizeof(iterator));
  it->current = list->first;
  return it;
}

void append(list *l, void *v) {
  if (empty(l)) {
    l->first = l->last = mkLink(NULL, v);
  } else {
    l->last = l->last->next = mkLink(NULL, v);
  }
}

void delete(list *l, int idx) {
  struct node **cursor = &(l->first);
  for (int i = 0; i < idx && *cursor; ++i) {
    cursor = &((*cursor)->next);
  }

  if (*cursor) {
    struct node *temp = *cursor;
    *cursor = (*cursor)->next;
    free(temp);
  }
}

int empty(list *l) {
  return l->last == NULL;
}

int length(list *l) {
  int length = 0;
  iterator *iter = mkIterator(l);
  while (hasMore(iter)) {
    ++length;
    next(iter);
  }
  rmIterator(iter);
  return length;
}

int hasMore(struct iterator *i){
  if(i->current->next != NULL){
    return 1;
  } else{
    return 0;
  }
}

void *next(struct iterator *i){
  i->current = i->current->next;
  return i;
}

void rmIterator(struct iterator *i){
  free(i);
}


void rmList(struct list *l){
  //struct node *n = l->first;
  iterator *it = mkIterator(l);
  while(hasMore(it)){
    struct node *tmp = it->current->next;
    free(it->current->value);
    free(it->current);
    it->current = tmp;
    
  }
  free(it->current->value);
  free(it->current);
  rmIterator(it);
  
  free(l);
}
