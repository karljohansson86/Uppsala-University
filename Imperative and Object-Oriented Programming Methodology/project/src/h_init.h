/**
 *   \file h_init.h
 *   \brief Heap allocation, headers, accessors and mutators.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef h_init_h
#define h_init_h

#ifndef __gc__
/**
 * The opaque data type holding all the heap data
 */
typedef struct heap heap_t;
#endif

/**
 * The opaque data type holding all the page.
 */
typedef struct page page_t;

/**
 * Create a new heap with bytes total size (including both spaces
 * and metadata), meaning strictly less than bytes will be
 * available for allocation.
 *
 * \param bytes         the total size of the heap in bytes
 * \param unsafe_stack  true if pointers on the stack are to be considered unsafe pointers
 * \param gc_threshold  the memory pressure at which gc should be triggered (1.0 = full memory)
 * \return              the new heap if allocation was successful,
 *                      `NULL` otherwise.
 * \see valid_bytes
 * \see valid_threshold
 */
heap_t *h_init(size_t bytes, bool unsafe_stack, float gc_threshold);

/**
 *
 * Delete a heap.
 * \param h             the heap
 * \see   h_delete_dbg
 */
void h_delete(heap_t *h);

/**
 * Accessor for safe "unsafe_stack" field. Indicates whether
 * the heap considers stack pointers to itself unsafe.
 *
 * \param h  Heap to check.
 *
 * \return   `true` if stack pointers are considered unsafe,
 *           `false` otherwise.
 *
 * \see      struct heap
 */
bool h_is_unsafe(heap_t *h);

/**
 * Checks if memory threshold is met for a heap.
 * If works based on what percentage of available memory
 * or pages are used. Ideally it should only look at used
 * memory, however pages may be used for development
 * purpose.s
 *
 * \param h  Heap to check.
 *
 * \return   `true` if threshold is met,
 *           `false` otherwise.
 */
bool h_is_threshold_met(heap_t *h);


/**
 * Get the total memory for a page in the heap. I.e.
 * a free page's amount of free memory.
 * 
 * \param h  The heap.
 * \return   The total memory of a single page's memory.
 */
size_t get_mem_per_page(heap_t *h);

/**
 * DEPRECATED
 *
 * \see get_mem_per_page
 */
size_t get_page_mem(heap_t *h);

/**
 * Checks if an address points to somewhere in a heap's
 * in page-area.
 *
 * This doesn't check if the pointer is in an actually
 * *used* memory area, and is thus meant to be used for
 * quick checks of stack pointers.
 *
 * a page's *used* memory area.
 * 
 * \param h     A heap with pages.
 *
 * \param addr  An address.
 *
 * \return      `true` if an address points to somewhere within
 *              the heap's pages, (even if not in an actual
 *              memory area!), `false` otherwise.
 *
 * \see is_mem_addr
 */
bool is_page_addr(heap_t *h, void *addr);

/**
 * Checks if an address points to somewhere in a heap's
 * used memory area. This is a more exact, but slower
 * way of checking if an address points to a heap.
 * 
 * \param h     A heap with pages.
 *
 * \param addr  An address.
 *
 * \return      `true` if an address points to a used memory
 *              area in the heap's pages, `false` otherwise.
 *
 * \see is_page_addr
 */
bool is_mem_addr(heap_t *h, char *addr);

/**
 * Check if an address points to a "new space" (a page that is set
 * to "new space").
 *
 * \param h      Heap to check within.
 *
 * \param addr   Address to check.
 *
 * \return       `True` if address pointed to a new space,
 *               `false` otherwise.
 *               
 * \invariant    is_mem_addr(h, addr) 
 *
 * \see is_mem_addr
 * \see is_page_addr
 */
bool is_space_newspace(heap_t *h, void *addr);

/**
 * Check if an address points to a "promoted space" (a page that is set
 * to "promoted").
 *
 * \param h      Heap to check within.
 *
 * \param addr   Address to check.
 *
 * \return       `True` if address pointed to a promoted space,
 *               `false` otherwise.
 *               
 * \invariant    is_mem_addr(h, addr) 
 *
 * \see is_mem_addr
 * \see is_page_addr
 */
bool is_addr_promoted(heap_t *h, void *addr);

/**
 * Returns the available free memory in a page.
 *
 * \param p  The page.
 * \return   The available free memory.
 * \see h_avail
 * \see p_used
 * \see h_used
 */
size_t p_avail(page_t *p);

/**
 * Returns the bytes currently in use by user structures. This
 * does not include the collector's own meta data. Notably,
 * this means that h_avail + h_used will not equal the size of
 * the heap passed to h_init.
 * 
 * \param p   The page
 * \return    The bytes currently in use by user structures.
 * \see h_used 
 * \see p_avail
 * \see h_avail
 */
size_t p_used(page_t *p);

/**
 * Returns the available free memory in the heap.
 *
 * \param h    The heap
 * \return     The available free memory.
 * \see p_avail
 * \see h_used
 * \see p_used
 */
size_t h_avail(heap_t *h);

/**
 * Returns the bytes currently in use by user structures. This
 * doesn't not include the collector's own meta data. Notably,
 * this means that h_avail + h_used will not equal the size of
 * the heap passed to h_init.
 * 
 * \param h   The heap
 * \return    The bytes currently in use by user structures.
 * \see p_used
 * \see h_avail
 * \see p_avail 
 */
size_t h_used(heap_t *h);

/**
 * Promotes the page whos space an address points. (Sets the page to
 * promoted).
 *
 * \param h      Heap to promote within.
 *
 * \param addr   Address to promote.
 *               
 * \invariant    is_mem_addr(h, addr) 
 *
 * \see is_mem_addr
 * \see is_page_addr
 */
void promote_addr(heap_t *h, void *addr);

/**
 * Allocates an amount of bytes in the heap.
 *
 * For allocation during gc use `h_alloc_gc`, this doesn't
 * take page parameters into consideration when allocating.
 *
 * Warning: Caller is responsible for only writing within
 * bounds of requested space!
 *
 * Among others, allocation can fail if there is insufficient
 * free space in the heap. 
 *
 * \param h      Heap to allocate in.
 *
 * \param bytes  Amount of bytes to allocate.
 *
 * \return       `NULL` if the allocation failed,
 *               address of allocated memory place otherwise.
 *
 * \see h_alloc_gc
 */
void *h_alloc(heap_t *h, size_t bytes);


/**
 * Allocates an amount of bytes in the heap for the gc.
 *
 * For normal allocation use `h_alloc`, this only uses
 * promoted or new pages (with `new_space` set to `false`). 
 *
 * Warning: Caller is responsible for only writing within
 * bounds of requested space!
 *
 * Among others, allocation can fail if there is insufficient
 * free space in the heap. 
 *
 * \param h      Heap to allocate in.
 *
 * \param bytes  Amount of bytes to allocate.
 *
 * \return       `NULL` if the allocation failed,
 *               address of allocated memory place otherwise.
 *
 * \see h_alloc
 */
void *h_alloc_gc(heap_t *h, size_t bytes);


/**
 * Prepares heap pages for next gc cycle. This is done by
 * appropriately modifying the newSpace and promoted fields
 * of pages and resetting the bump pointer if needed.
 * 
 * \param h  Heap to purge.
 *
 */
void h_purge(heap_t *h); // TODO move to gc.h/gc.g

#endif
