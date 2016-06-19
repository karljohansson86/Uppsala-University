#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#include "libgc.h"

typedef struct _perf_list_t perf_list_t;
typedef struct _perf_list_node_t perf_list_node_t;

/**
 *  Each node in the linked list, with an element and a pouint64_ter to the next node.
 */
struct _perf_list_node_t
{
  uint64_t value;
  struct _perf_list_node_t *next;
};

/**
 *  A Linked list, with pouint64_ters to first and last node.
 */
struct _perf_list_t
{
  perf_list_node_t *first;
  perf_list_node_t *last;
};

/**
 *  Creates a new node
 *  \returns  A new node
 */
perf_list_node_t *perf_list_node_new()
{
  return calloc(1, sizeof(struct _perf_list_node_t));
}

perf_list_node_t *perf_list_node_new_gc(heap_t *h)
{
  return h_alloc_struct(h, "2*");
}

/**
 *  Creates a new linked list
 *  \returns  A new linked list
 */

perf_list_t *perf_list_new()
{
  return calloc(1, sizeof(struct _perf_list_t));
}

perf_list_t *perf_list_new_gc(heap_t *h)
{
  return h_alloc_struct(h, "8c*");
}

void perf_list_add(perf_list_t *l, uint64_t value)
{
  assert(l);
  assert((l->first && l->last) || (!l->first && !l->last));

  perf_list_node_t *new_last = perf_list_node_new();
  new_last->value = value;

  if (l->first && l->last)
    {
      l->last->next = new_last;
      l->last = new_last;
    }
  else
    {
      // list is empty
      l->first = l->last = new_last;
    }
}

void perf_list_add_gc(heap_t *h, perf_list_t *l, uint64_t value)
{
  assert(l);
  assert((l->first && l->last) || (!l->first && !l->last));

  perf_list_node_t *new_last = perf_list_node_new_gc(h);
  new_last->value = value;

  if (l->first && l->last)
    {
      l->last->next = new_last;
      l->last = new_last;
    }
  else
    {
      // list is empty
      l->first = l->last = new_last;
    }
}

bool perf_list_contains(perf_list_t *l, uint64_t value)
{
  assert(l);
  perf_list_node_t *cur = l->first;
  while (cur)
    {
      if (cur->value==value)
        {
          return true;
        }
      cur = cur->next;
    }
  return false;
}

void perf_list_free(perf_list_t *l)
{
  assert(l);
  perf_list_node_t *cur = l->first;
  while (cur)
    {
      perf_list_node_t *to_delete = cur;
      cur = cur->next;
      free(to_delete);
    }

  free(l);
}

uint64_t perf_list_len(perf_list_t *l)
{
  uint64_t ret = 0;
  assert(l);
  perf_list_node_t *cur = l->first;
  while (cur)
    {
      cur = cur->next;
      ret++;
    }
  return ret;
}

/////////////////// MALLOC TEST ////////////////////

void perf_malloc(uint64_t scope1, uint64_t scope2, uint64_t scope3, uint64_t scope4, uint64_t n, uint64_t m)
{
  // Timer saves
  clock_t begin, create_lists, insert, search;
 
  // BEGIN TIMING
  begin = clock();
    
  perf_list_t *perf_list_1 = perf_list_new();
  perf_list_t *perf_list_2 = perf_list_new();
  perf_list_t *perf_list_3 = perf_list_new();
  perf_list_t *perf_list_4 = perf_list_new();

  // Stamp time
  create_lists = clock();
  
  srand(time(NULL));
  for(; m>0; --m) {
    uint64_t r = (uint64_t)((uint64_t)(rand() << 31) | (uint64_t)rand()) % (uint64_t)scope4;
    
    if(r < scope1) {
      perf_list_add(perf_list_1, r);
    }
    if(r >= scope1 && r < scope2) {
      perf_list_add(perf_list_2, r);
    }
    if(r >= scope2 && r < scope3) {
      perf_list_add(perf_list_3, r);
    }
    if(r >= scope3 && r < scope4) {
      perf_list_add(perf_list_4, r);
    }
  }

  // Stamp time
  insert = clock();  

  /*   uint64_t len1 = perf_list_len(perf_list_1); */
  /* uint64_t len2 = perf_list_len(perf_list_2); */
  /* uint64_t len3 = perf_list_len(perf_list_3); */
  /* uint64_t len4 = perf_list_len(perf_list_4); */
  
  /* printf("Length of lists\n=======================\nList 1; %ld\nList 2: %ld\nList 3: %ld\nList 4: %ld\nTotal: %ld\nExpected: %ld\nDiff: %ld\n----------------------\n", */
  /*        len1, */
  /*        len2, */
  /*        len3, */
  /*        len4, */
  /*        len1+len2+len3+len4, */
  /*        m, */
  /*        len1+len2+len3+len4 - m); */


  
  // Look for random elements
  srand(time(NULL));
  for(; n>0; --n) {
    uint64_t r = rand() % scope4;

    if(r < scope1) {
      perf_list_contains(perf_list_1, r);
    }
    if(r >= scope1 && r < scope2) {
      perf_list_contains(perf_list_2, r);
    }
    if(r >= scope2 && r < scope3) {
      perf_list_contains(perf_list_3, r);
    }
    if(r >= scope3 && r < scope4) {
      perf_list_contains(perf_list_4, r);
    }
  }

  // Stamp time
  search = clock();

  // free lists
  perf_list_free(perf_list_1);
  perf_list_free(perf_list_2);
  perf_list_free(perf_list_3);
  perf_list_free(perf_list_4);

  //////////// PRUINT64_T RESULT ///////////
  puts("");
  puts("MALLOC");
  puts("=====================================");
  printf("Create lists:    %03.10f s\n", (double)(create_lists - begin)/CLOCKS_PER_SEC);
  printf("Fill random:     %03.10f s\n", (double)(insert - create_lists)/CLOCKS_PER_SEC);
  printf("Search random:   %03.10f s\n", (double)(search - insert)/CLOCKS_PER_SEC);
  puts("-------------------------------------");
  printf("Total:           %03.10f s\n", (double)(search - begin)/CLOCKS_PER_SEC);
  puts("");

}  

////////////////////// GC TEST /////////////////////

void perf_gc(uint64_t scope1, uint64_t scope2, uint64_t scope3, uint64_t scope4, uint64_t n,  uint64_t m,
             bool unsafe_ptrs, float heap_x, float threshold, bool collect_before_search)
{
  // Create heap
  size_t heap_size = heap_x * m * sizeof(perf_list_node_t);
  heap_t *h = h_init(heap_size, unsafe_ptrs, threshold);
  
  // Timer saves
  clock_t begin, create_lists, insert, search, gc;
  
  // BEGIN TIMING
  begin = clock();
    
  perf_list_t *perf_list_1 = perf_list_new_gc(h);
  perf_list_t *perf_list_2 = perf_list_new_gc(h);
  perf_list_t *perf_list_3 = perf_list_new_gc(h);
  perf_list_t *perf_list_4 = perf_list_new_gc(h);

  // Stamp time
  create_lists = clock();
  
  srand(time(NULL));
  for(; m>0; --m) {
    uint64_t r = (uint64_t)((uint64_t)(rand() << 31) | (uint64_t)rand()) % (uint64_t)scope4;

    if(r < scope1) {
      perf_list_add_gc(h,perf_list_1, r);
    }
    if(r >= scope1 && r < scope2) {
      perf_list_add_gc(h,perf_list_2, r);
    }
    if(r >= scope2 && r < scope3) {
      perf_list_add_gc(h,perf_list_3, r);
    }
    if(r >= scope3 && r < scope4) {
      perf_list_add_gc(h,perf_list_4, r);
    }
  }

  /* uint64_t len1 = perf_list_len(perf_list_1); */
  /* uint64_t len2 = perf_list_len(perf_list_2); */
  /* uint64_t len3 = perf_list_len(perf_list_3); */
  /* uint64_t len4 = perf_list_len(perf_list_4); */
  
  /* printf("Length of lists\n=======================\nList 1; %ld\nList 2: %ld\nList 3: %ld\nList 4: %ld\nTotal: %ld\nExpected: %ld\nDiff: %ld\n----------------------\n", */
  /*        len1, */
  /*        len2, */
  /*        len3, */
  /*        len4, */
  /*        len1+len2+len3+len4, */
  /*        m, */
  /*        len1+len2+len3+len4 - m); */

  
  // Stamp time
  insert = clock();

  if(collect_before_search) {
    // Garbage collect
    h_gc(h);

    // Stamp time
    gc = clock();
  }

  
  /* len1 = perf_list_len(perf_list_1); */
  /* len2 = perf_list_len(perf_list_2); */
  /* len3 = perf_list_len(perf_list_3); */
  /* len4 = perf_list_len(perf_list_4); */
  
  /* printf("List 1; %ld\nList 2: %ld\nList 3: %ld\nList 4: %ld\nTotal: %ld\nExpected: %ld\nDiff: %ld\n----------------------\n", */
  /*        len1, */
  /*        len2, */
  /*        len3, */
  /*        len4, */
  /*        len1+len2+len3+len4, */
  /*        m, */
  /*        len1+len2+len3+len4 - m); */
  
  
  // Look for random elements
  srand(time(NULL));
  for(; n>0; --n) {
    uint64_t r = rand() % scope4;

    if(r < scope1) {
      perf_list_contains(perf_list_1, r);
    }
    if(r >= scope1 && r < scope2) {
      perf_list_contains(perf_list_2, r);
    }
    if(r >= scope2 && r < scope3) {
      perf_list_contains(perf_list_3, r);
    }
    if(r >= scope3 && r < scope4) {
      perf_list_contains(perf_list_4, r);
    }
  }
  
  // Stamp time
  search = clock();

  h_delete(h);
  
  //////////// PRUINT64_T RESULT ///////////
  puts("");
  puts("GC HEAP");
  printf("Size: %.2f x alloc, Unsafe stack: %s, Threshold: %.2f\n",
         heap_x, unsafe_ptrs ? "YES" : "NO", threshold);
  printf("Collect before search: %s\n", collect_before_search ? "YES" : "NO");
  puts("=======================================================");
  printf("Create lists:    %03.10f s\n", (double)(create_lists - begin)/CLOCKS_PER_SEC);
  printf("Fill random:     %03.10f s\n", (double)(insert - create_lists)/CLOCKS_PER_SEC);
  if(collect_before_search) {
    printf("Search random:   %03.10f s\n", (double)(search - gc)/CLOCKS_PER_SEC);
    printf("GC run:          %03.10f s\n", (double)(gc - insert)/CLOCKS_PER_SEC);
  } else {
      printf("Search random:   %03.10f s\n", (double)(search - insert)/CLOCKS_PER_SEC);
  }
  puts("-------------------------------------");
  printf("Total:           %03.10f s\n", (double)(search - begin)/CLOCKS_PER_SEC);
  puts("");

}

/////////////////////// MAIN ///////////////////////

int main(int argc, char const *argv[])
{
  
  // Scopes
  uint64_t scope1   = 1000000000;
  uint64_t scope2   = 2000000000;
  uint64_t scope3   = 3000000000;
  uint64_t scope4   = 4000000000;

  // Numbers to generate and search for
  uint64_t n = 10000;
  uint64_t m = 100000;

  puts("");
  puts("SETTINGS");
  puts("=============================================");
  printf("Insert random elements:   %llu elements\n", m);    
  printf("Random search for:        %llu elements\n", n);
  printf("Scope MAX:                %llu\n", scope4);
  printf("RAND_MAX:                 %d\n", RAND_MAX);
  puts("");

  // Run perf tests
  // MALLOC
  perf_malloc(scope1, scope2, scope3, scope4, n, m);
  // GC
  perf_gc(scope1, scope2, scope3, scope4, n, m, false, 4, 0.5, false);
  perf_gc(scope1, scope2, scope3, scope4, n, m, false, 4, 0.5, true);
  perf_gc(scope1, scope2, scope3, scope4, n, m, true, 4, 0.5, false);
  perf_gc(scope1, scope2, scope3, scope4, n, m, true, 4, 0.5, true);
}
