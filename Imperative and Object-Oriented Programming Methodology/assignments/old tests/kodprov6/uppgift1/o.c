#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int(*cmp_f)(const void*,const void*);
typedef struct ordered_set ordered_set_t;
typedef struct node        node_t;

ordered_set_t *ordered_set_mk(cmp_f cmp);
void ordered_set_add(ordered_set_t *set, void *e);
void* ordered_set_get(ordered_set_t *set, int index);
void *ordered_set_del(ordered_set_t *set, int index);
int ordered_set_size(ordered_set_t *set);
void ordered_set_print(ordered_set_t *set);

struct ordered_set
{
  node_t *first;
  int size;
  cmp_f cmp; // Compare function
};

struct node
{
  void *element;
  node_t *next;
};


ordered_set_t *ordered_set_mk(cmp_f cmp)
{
  ordered_set_t *set = calloc(1, sizeof(ordered_set_t));

  if (set)
    {
      set->cmp = cmp;
    }

  return set;
}

node_t *node_mk(void *e, node_t *next)
{
  node_t *node = malloc(sizeof(node_t));

  if (node)
    {
      node->next = next;
      node->element = e;
    }

  return node;
}

int ordered_set_index_of(ordered_set_t *set, void *e)
{
  int index = 0;
  for (node_t *cursor = set->first; cursor; cursor = cursor->next, ++index)
    {
      int r = set->cmp(e, cursor->element);
      if (r == 0) return index;
      if (r  < 0) break;
    }
      
  return -1;
}

void ordered_set_add(ordered_set_t *set, void *e)
{
  node_t **cursor = &set->first;
  
  while (*cursor) 
    {
      int r = set->cmp((*cursor)->element, e);
      if (r == 0) return;
      if (r  > 0) break;
      
      cursor = &(*cursor)->next;
    }
      
  *cursor = node_mk(e, *cursor);
  ++set->size;
}

void* ordered_set_get(ordered_set_t *set, int index)
{
  if (ordered_set_size(set) <= index) return NULL;

  node_t *cursor = set->first;
  for (int i = 0; i < index; cursor = cursor->next, ++i)
    ;

  return cursor->element;
}

void *ordered_set_del(ordered_set_t *set, int index)
{
  // TODO
  if(set == NULL){
    return NULL;
  }
  int size = ordered_set_size(set);
  if(size == 1){
    free(set->first);
    set->first = NULL;
    
  }
  else if(index < size){
    node_t *pre = ordered_set_get(set,index-1);
    node_t *remove = ordered_set_get(set,index); 
    pre->next = remove->next;
    free(remove); 
  }
  --set->size;
  return NULL;
}

void ordered_set_rm(ordered_set_t *set)
{
  node_t *n = set->first;
  
  do{
    node_t *tmp = n->next;
    free(n);
    n=tmp;
    
  }while(n != NULL);
  free(set);
  // TODO
}

int ordered_set_size(ordered_set_t *set)
{
  return set->size;
}

void ordered_set_print(ordered_set_t *set)
{
  printf("{ ");
  for (node_t *cursor = set->first; cursor; cursor = cursor->next)
    {
      printf("%s", cursor->element);
      if (cursor->next) printf(", ");
    }
  printf(" }\n");
}

int main(int argc, char *argv[])
{
  ordered_set_t *set = ordered_set_mk((cmp_f) strcmp);

  for (int i = 1; i < argc; ++i)
    {
      if (argv[i][0] == '-')
	{
	  int index = ordered_set_index_of(set, argv[i]+1);
	  ordered_set_del(set, index);
	}
      else
	{
	  ordered_set_add(set, argv[i]);
	}
    }


  ordered_set_print(set);

  printf("Set size: %d\n", ordered_set_size(set));

  ordered_set_rm(set);

  return 0;
}
