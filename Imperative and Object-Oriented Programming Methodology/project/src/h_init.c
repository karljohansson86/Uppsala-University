
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include "h_init.h"
#include "settings.h"

#ifndef h_init_c
#define h_init_c

/**
 * The datatype holding all the heap data
 *
 *
 * unsafe_stack     Whether to consider stack pointers to the
 *                  heap as unsafe (or safe).
 *
 * gc_threshold     The percentage of the heap that has to be
 *                  used to trigger a garbage collection cycle.
 *                  Must be over 0.5 and under 0.1.
 *
 * heapsize         Size of entire heap object (bytes),
 *                  includes everything associated with heap.
 *
 * heap_end         End of entire heap object.
 *
 * header_size      Size (bytes) of heap header.
 *
 * pagesize         Size (bytes) of each page in the heap.
 *
 * pageheader_size  Size (bytes) of each page's header.
 *
 * mem_per_page     Memory (bytes) of each page.
 *
 * pages_start      Start address of the first page.
 *
 * page_count       Total amount of pages in heap.
 */
struct heap {
  bool     unsafe_stack;
  float    gc_threshold;
  size_t   heapsize;
  void    *heap_end;
  size_t   header_size;
  size_t   pagesize;
  size_t   pageheader_size;
  size_t   mem_per_page;
  void    *pages_start;
  int      page_count;
};

/**
 * A datatype representing one page in the heap.
 *
 *
 * new_space       If `true`, page became active (was
 *                 used by an object) and has not been
 *                 through a gc-cycle yet.
 *
 * promoted        Indicates whether unsafe pointers were
 *                 found to be pointing toward the page.
 *
 * bump            The bump pointer points to the page's
 *                 lowest address with free memory. The
 *                 pointer is incremented any time a new
 *                 object is allocated on the page.
 *
 * end             End of the page and its memory.
 *
 */
struct page {
  bool new_space;
  bool promoted;
  void *bump;
  void *end;
};


bool valid_bytes(size_t, size_t, size_t);
bool valid_threshold(float);
void create_pages (void *, int, size_t);
size_t new_pageheader_size(size_t, size_t);

heap_t *h_init(size_t bytes, bool unsafe_stack, float gc_threshold)
{
  size_t heap_header_size   = sizeof(heap_t);
  size_t page_header_size   = sizeof(page_t);
  size_t mem_per_page       = PAGESIZE - page_header_size;
  assert(valid_bytes(bytes, heap_header_size, page_header_size));
  
  int    page_count         = (bytes-heap_header_size)/PAGESIZE;
  size_t total_size         = page_count*PAGESIZE + heap_header_size;

  void *h_temp = calloc(1, total_size);
  assert(h_temp != NULL && ". calloc failed, Exiting program.");

  heap_t *h = (heap_t *)h_temp;
  if(!h) {
    return NULL;
  }
  
  if(GC_THRESHOLD_OVERRIDE) {
    assert(valid_threshold(gc_threshold)); 
    h->gc_threshold = GC_THRESHOLD_OVERRIDE_VALUE;
  } else {
    h->gc_threshold = gc_threshold;
  }

  h->unsafe_stack = unsafe_stack;
  h->heapsize = total_size;
  h->heap_end = ((char *)h) + total_size; // char cast for byte increments
  h->header_size = heap_header_size;
  h->pagesize = PAGESIZE;
  h->pageheader_size = new_pageheader_size(page_header_size, mem_per_page);
  h->mem_per_page = PAGESIZE - h->pageheader_size;
  h->pages_start = (char *)h + heap_header_size;
  h->page_count = page_count;

  create_pages(h->pages_start, page_count, PAGESIZE);

  return h;
}

/**
 * Creates the pages of the heap.
 *
 * \param start     Address of the first page.
 * \param n_pages   Amount of pages to create.
 * \param pagesize  The total size of each page.
 */
void create_pages (void *start, int n_pages, size_t pagesize)
{
  page_t template;
  template.new_space = false;
  template.promoted = false;

  page_t *p = start;
  int i;
  for(i = 0; i < n_pages; i++){
    *p = template;
    p->bump = p+1;
    p->end = (char *)p + pagesize;
    p = p->end;
  }
}

/**
 * Validates a size argument for h_init.
 *
 * \param bytes        Amount of bytes to be allocated..
 * \param header_size  Expected size of the heaps header.
 * \param header_size  Total size of each page in heap.
 * \return             True if the size argument is valid,
 *                     false otherwise.
 * \see h_init
 * \see valid_threshold
 */
bool valid_bytes(size_t bytes, size_t header_size, size_t pagesize)
{
  return (bytes) >= (2*pagesize + header_size);
}

/**
 * Validates a threshold argument for h_init.
 * which should be less over 0 and under 1.
 *
 * \param gc_threshold  The threshold.
 * \return              True if the threshold argument is valid,
 *                      false otherwise.
 * \see h_init
 * \see valid_bytes
 */
bool valid_threshold (float gc_threshold)
{
  return gc_threshold > 0 && gc_threshold < 1;
}

void h_delete(heap_t *h)
{
  assert(h != NULL && "Heap is NULL");
  free(h);
}

bool h_is_unsafe(heap_t *h)
{
  return h->unsafe_stack;
}


bool h_is_threshold_met(heap_t *h)
{
  float current_ratio = ((float)h_used(h)) / (float)(h->page_count * h->mem_per_page);
  return h->gc_threshold <= current_ratio;
}


size_t get_mem_per_page(heap_t *h)
{
  return h->mem_per_page;
}

size_t get_page_mem(heap_t *h) // TODO: Remove (deprecated)
{
  return get_mem_per_page(h);
}


bool is_page_addr(heap_t *h, void *addr)
{
  return addr >= h->pages_start && addr < h->heap_end;
}

bool is_mem_addr_aux(page_t *p, char *addr)
{
  char *taken_start = (char *)(p + 1);
  if(addr >= taken_start && addr < (char *)p->bump) {
    return true;
  }
  return false;
}

bool is_mem_addr(heap_t *h, char *addr)
{
  page_t *p = h->pages_start;

  while((void *)p < h->heap_end) {
    if(is_mem_addr_aux(p, addr)) {
      return true;
    }
    p = p->end;
  }
  return false;
}

bool is_space_newspace(heap_t *h, void *addr)
{
  page_t *current_page = h->pages_start;
  
  assert(!(addr >= h->heap_end || addr < (void *)h));

  while((void *)(addr - (void *)current_page) > (void *) h->pagesize){
    current_page = current_page->end;
  }
  return current_page->new_space;   
}

page_t *get_addr_page(heap_t *h, void *addr)
{
  assert(!(addr >= h->heap_end || addr < (void *)h));

  page_t *p = h->pages_start;  
  void *distance_from_page = (void *)(addr - (void *)p);

  while(distance_from_page > (void *)h->pagesize){
    p = p->end;
    distance_from_page = (void *)(addr - (void *)p);
  }
  return p;
}

bool is_addr_promoted(heap_t *h, void *addr)
{
  page_t *p = get_addr_page(h, addr);
  return p->promoted;   
}

void promote_addr(heap_t *h, void *addr)
{
  page_t *p = get_addr_page(h, addr);
  p->promoted = true;
}

/**
 * Aux function that purges a single page.
 *
 * \param p  Page to purge.
 * \see      h_purge
 */
void h_purge_aux(page_t *p)
{
  if(p->promoted) {
    p->promoted = false;
    p->new_space = true;
  } else {
    p->new_space = false;
    p->bump = p+1;
  }
}


void h_purge(heap_t *h)
{
  page_t *p = h->pages_start;
  while((void *)p < h->heap_end) {
    h_purge_aux(p);
    p = p->end;
  }
}


size_t p_avail(page_t *p)
{
  return p->end - p->bump;
}

size_t p_used(page_t *p)
{
  return p->bump - (void *)(p+1);
}

size_t h_avail(heap_t *h)
{
  size_t result = 0;
  page_t *p = h->pages_start;
  while((void *)p < h->heap_end) {
    if(!p->promoted) {
      result += p_avail(p);
    }
    p = p->end;
  }
  return result;
}

size_t h_used(heap_t *h)
{
  size_t result = 0;
  page_t *p = h->pages_start;
  while((void *)p < h->heap_end) {
    result += p_used(p);
    p = p->end;
  }
  return result;
}



// PLACE NEW FUNCTIONS BELOW              
// TODO: TEST BELOW FUNCTIONS


size_t new_pageheader_size(size_t pageheader_size, size_t mem_per_page)
{
  size_t aligned;
  aligned = mem_per_page % WORDSIZE;

  if(WORDSIZE == 4) {
    return pageheader_size + aligned;
  }

  if(WORDSIZE == 8) { 
    return pageheader_size + aligned;
  }
}

/**
 * Attempt to allocates an amount of bytes in the page without
 * modifying any flags. Allocates in lowest possible adddress
 * and moves bump pointer as appropriate.
 *
 * If insufficient space nothing is allocated.
 *
 * \param p      Page to allocate/reserve in.
 *
 * \param bytes  Amount of bytes to allocate/reserve.
 *
 * \return       `NULL` if there was insufficient space in page,
 *               otherwise address of the allocated memory space.
 * \see h_alloc
 */
void *p_alloc(page_t *p, size_t bytes)
{
  if((size_t)(p->end - p->bump) >= bytes) {
    void *result = p->bump;
    p->bump = ((char *)p->bump) + bytes;
    return result;
  }
  return NULL;
}

void *h_alloc_gc(heap_t *h, size_t bytes)
{
  void *result;
  page_t *p = h->pages_start;

  while((void *)p < h->heap_end) {
    if(p->promoted) {
      result = p_alloc(p, bytes);
      if(result) {
        return result;
      }
    } else if(!p->new_space) {
      result = p_alloc(p, bytes);
      if(result) {
        p->promoted = true;
        return result;
      }
    }
    p = p->end;
  }
  return NULL;
}

void *h_alloc(heap_t *h, size_t bytes)
{
  void *result;
  page_t *p = h->pages_start;

  while((void *)p < h->heap_end) {
    result = p_alloc(p, bytes);
    if(result) {
      p->new_space = true;
      return result;
    }
    p = p->end;
  }

  return NULL;
}

// TESTS FOR DEVELOPMENT


void h_print_pages(heap_t *h)
{
  page_t *p = h->pages_start;
  int i = 0;
  while((void *)p < h->heap_end) {
    printf("%d Start: %p   End: %p   Bump: %p   Newspace: %d   Promoted: %d\n",
           i, p, p->end, p->bump, p->new_space, p->promoted);
    i++;
    p = p->end;
  }  
}


/* int main(int argc, char *argv[]) */
/* { */
/*   heap_t *h = h_init(16*1024, true, 0.9); */
/*   h_print_pages(h); */
/* } */

#endif

