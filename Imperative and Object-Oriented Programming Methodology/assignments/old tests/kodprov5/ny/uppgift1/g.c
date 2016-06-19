#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#define RANDOM_LIMIT 100000
#define RANDOM (random() % RANDOM_LIMIT)
#define NO_LISTS 5

#define MallocNode(s) malloc(s)
#define MallocList(s) malloc(s)
#define Hinit
#define Hexit
#define RUN_GC /*Only valid when used with memory management*/
#define list_contains list_contains_rec

typedef struct _node list_node_t;
typedef struct _list list_t;

struct _node
{
  uint64_t     value;
  list_node_t *next;
};

struct _list
{
  list_node_t *first;
  list_node_t *last;
};

/// OBS! Det är tillåtet att skapa egna funktioner

/// Uppgift #1
bool list_contains_iter(list_t *l, uint64_t v)
{
  if(l == NULL){
    return false;
  }
  list_node_t * n = l->first;
  do{
    if(n->value == v){
      return true;
    }
    n = n->next;
  }while(n->next != NULL);
  

  return false;
}

/// Uppgift #2

bool list_contains_rec_aux(list_node_t *n , uint64_t v){
  if(n == NULL){
    return false;
  }
  else if(n->value == v){
    return true;
  }
  else{
    return list_contains_rec_aux(n->next,v);
  }
}

bool list_contains_rec(list_t *l, uint64_t v)
{
  
  return list_contains_rec_aux(l->first,v);
}

list_t *list_create()
{
  list_t *l = MallocList(sizeof(list_t));
  l->first = NULL;
  l->last  = NULL;

  return l;
}

void list_append(list_t *l, uint64_t v)
{
  list_node_t *n = MallocNode(sizeof(list_node_t));
  n->value = v;
  n->next  = NULL;

  if (l->first)
    {
      l->last->next = n;
      l->last = n;
    }
  else
    {
      l->first = n;
      l->last = n;
    }
}

void list_print(list_t *l)
{
  printf("[");
  for (list_node_t *n = l->first; n; n = n->next)
    {
      printf("%lli, ", n->value);
    }
  printf("\b\b]\n");
}

void build_lists(list_t **lists, int no_lists, int M, int N)
{
  for (int i = 0; i < no_lists; ++i)
    {
      lists[i] = list_create();
    }

  for (int i = 0; i < M; ++i)
    {
      uint64_t r = RANDOM;
      list_append(lists[r % no_lists], r);
    }
}

int test(int M, int N)
{
  Hinit;

  list_t *lists[NO_LISTS];

  build_lists(lists, NO_LISTS, N, M);

  RUN_GC;

  int hit_ratio = 0;
  for (int i = 0; i < N; ++i)
    {
      uint64_t r = RANDOM;
      if (list_contains(lists[r % NO_LISTS], r))
        {
          ++hit_ratio;
        }
    }

  Hexit;
  
  for(int i = 0; i< NO_LISTS; ++i){
    list_t *l = lists[i];
    list_node_t *n = l->first;
    do{
      list_node_t *tmp = n->next;
      free(n);
      n = tmp;

    }while(n->next != NULL);
    free(n);
    free(l);
  }


  return hit_ratio;
}

int main(int argc, char *argv[])
{
  int M = argc > 1 ? atoi(argv[1]) : 4;
  int N = argc > 2 ? atoi(argv[2]) : 2;

  int H = test(M, N);

  printf("Hit ratio: %d of %d\n\n", H, N);

  return 0;
}
