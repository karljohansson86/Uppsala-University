
/**
 *   \file gc.c
 *   \brief 
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <setjmp.h>
#include <assert.h>


#include "settings.h"
#include "gc.h"
#include "list.h"
#include "object.h"
#include "h_init.h"


/////////////////////// GLOBAL ///////////////////////////////
//TODO comments?
extern char **environ;


///////////////////////// MACROS ///////////////////////////////


/**
 *  \def Dump_registers()
 *  Dumps register content on to the stack
 *
 */
#define Dump_registers()                        \
  jmp_buf env;                                  \
  if (setjmp(env)) abort();                     \




///////////////////////// INTERNAL PROTOTYPES /////////////////////////

/**
 * If an objects header contains a pointer to a string representation
 * of the object, that string is stored on the heap and this function
 * moves the string to a new location(if possible) on the heap during 
 * garbage collection
 * 
 * \param h   The heap
 * \param p   Pointer to the object which header to check/update
 */
void gc_update_str_header(heap_t *h, void *p);

/**
 * Compare if argument pointers are equal or if any of them are NULL
 *
 * Used for handling special case when an object on the heap that
 * contains a pointer in the first address of the object, meaning the pointer
 * to the object will be the same pointer as the first pointer in the object.
 *
 * \param p             Pointer in object 
 * \param current_obj   Pointer to object
 * \return              true if they are equal or any of them NULL
 *                      otherwise false 
 */
bool is_useless_ptr(void *p, void *current_object);

/**
 * Check if a list contains a specific pointer
 *
 * \param l   List to check 
 * \param p   Pointer to look for
 * \return    true if the list contains the pointer
 *            otherwise false 
 */
bool obj_in_list(list_t *l, void *p); 

/**
 * Checks if a pointer to the object that has been moved needs to be retroactively 
 * updated in other objects.
 *
 * Used to be able to handle circular references
 *
 * \param l   List containing
 * \param p   Pointer to old address of the object 
 * \return    NULL if not in the list, otherwise 
 *            Pointer to the Pointer that needs to be updated 
 */
void **current_in_retro_list(list_t *l, void *p);

/**
 * Creates a list of pointers (to pointers) from stack to heap
 * List will contain the root set
 * 
 *
 * \param h   The heap 
 * \param l   List to add new pointer to.
 * \param fn  Function used on each found root. Different function depending on if heap 
 *            treates pointers from stack as safe or unsafe.
 *            h_gc_safe_helper used when safe
 *            h_gc_unsafe_helper used when unsafe
 *
 * \return    A list containing the pointers if any
 *
 * \see h_gc_safe_helper
 * \see h_gc_unsafe_helper
 */
list_t *gc_traverse(heap_t *h, void fn(heap_t *h, list_t *l, void *ptr));


/**
 * Function that uses the list of roots and does a "Depth First Search" on each root by 
 * calling the gc_depth function with the root as a parameter 
 * 
 *
 * \param h          The heap 
 * \param root_set   List of roots, pointers (to pointers) from stack to heap.
 */
void gc_copy_collect(heap_t *h, list_t *root_set);


/**
 * Helper for creating a list of pointers (to pointers) from stack to heap
 * 
 * This function is used when the pointers from the stack are treated as 
 * safe pointers.
 *
 * \param h   The heap 
 * \param l   List to add new pointer to.
 * \return    A list containing the pointers if any
 */
void h_gc_safe_helper(heap_t *h, list_t *l, void *ptr);

/**
 * Garbage collection on heap, when pointers from stack are treated as safe
 *
 * \param h The heap 
 * \return  the number of bytes collected
 */
size_t h_gc_safe(heap_t *h);


/**
 * Helper for creating a list of pointers (to pointers) from stack to heap
 * and promotes the page on the heap 
 *
 * This function is used when the pointers from the stack are treated as 
 * unsafe pointers.
 *
 * \param h   The heap 
 * \param l   List to add new pointer to.
 * \return    A list containing the pointers if any
 */
void h_gc_unsafe_helper(heap_t *h, list_t *l, void *ptr);

/**
 * Garbage collection on heap, when pointers from stack are treated as unsafe
 *
 * \param h The heap 
 * \return  the number of bytes collected
 */
size_t h_gc_unsafe(heap_t *h);


/**
 * Garbage collection using depth search algorithm 
 * 
 * Short description of function and algorithm used
 * Given the heap and a pointer to pointer towards an object on that heap, first check if 
 * the the pointer is towards the heap, if not (it can for example be  an objects pointer 
 * pointing towards the stack and should not be handled) return. Then check if the the 
 * object has already been garbage collected otherwise the function checks if the object 
 * contains pointer and using recursive calls to itself with each of the objects pointers 
 * and in the end copying itself to a new location on the heap to get a less fragmented heap.
 *
 * NOTE this function does not free any memory,
 *
 * \param h                    The heap
 * \param p                    Pointer to the pointer to be garbage collected
 * \param seen                 List with objects already looked at
 * \param retroactive_ptr_upd  List with pointers to be retroactively updated
 *
 */
void gc_depth(heap_t *h, void **p, list_t *seen, list_t *retroactive_ptr_upd);

/**
 *  Create and return a pointer to bottom of stack
 *
 *  \return         Pointer to the bottom of the stack
 */
void *stack_find_bottom();


/**
 *  Create and return a pointer to top of stack
 *
 *  \return         Pointer to the top of the stack
 */
void *stack_find_top();


/**
 *  Check if a pointer points to an address on the heap and 
 *  that address is within an used memory area on the heap.
 *
 *  \param h        Heap to check within
 *  \param p        Pointer to be checked 
 *  \return         true if pointer points to an address in the heap used memory area,
 *                  false otherwise
 */
bool is_heap_pointer(heap_t *h, void *p);


/**
 *  Traverse stack from given address and towards the bottom argument of the stack
 *  and check if address on the stack contains pointer to the heap.
 *
 *  \param h        The heap
 *  \param current  Address to start with
 *  \param bottom   End address, stop argument for the loop(bottom of the stack)
 *  \return         Pointer to an address on stack which contains a pointer to the heap h
 *                  NULL if reached the bottom of the stack
 */
void *stack_trace(heap_t *h, void *current, void *bottom);


////////////////// FUNCTION IMPLEMENTATIONS //////////////////

void *stack_find_bottom()
{
  void *bottom = environ;
  return bottom;
}


void *stack_find_top()
{
  void *top = __builtin_frame_address(0);
  return top;
}


bool is_heap_pointer(heap_t *h, void *p)
{
  return (is_page_addr(h, p)) && (is_mem_addr(h, p));
}

void *stack_trace(heap_t *h, void *current, void *bottom)
{
  while(current > bottom) {
    intptr_t i = *(intptr_t *)current;
    void *p = (void *)i;  
    
    if (is_heap_pointer(h, p)) {
      return current;
    }
    else {      
      current = current-sizeof(void *);
    }
  }
  return NULL;
}


void gc_update_str_header(heap_t *h, void *p)
{
  intptr_t header = o_get_header(p);
  void *str_in_header = (void *)O_HEADER_GET_PTR(header);

  if(is_addr_promoted(h, str_in_header)) {
    return;
  }
  
  void *new_str_in_header = o_move_object(h, str_in_header);
  if(new_str_in_header == NULL) {
    promote_addr(h, str_in_header);
    return;
  }
  
  o_set_header(p, O_HEADER_SET_PTR(header, (intptr_t)new_str_in_header));
  
}


bool is_useless_ptr(void *p, void *current_object)
{
  return (current_object == NULL || current_object == p || p == NULL);
}


bool obj_in_list(list_t *l, void *p)
{
  return (list_get_index(l, p) >= 0);
    
}


void **current_in_retro_list(list_t *l, void *p )
{
  int len =  list_len(l);
  
  for(int i = 0 ;i < len; i++) {  
    void **current = (void **)list_nth(l, i);
    if(current != NULL && *current == p) {
      list_remove_nth(l, i);
      return current;
    } 
  }
  return NULL;
}

//TODO comment
bool gc_depth_first_check(heap_t *h, void **p, void *current)
{
  if(!is_heap_pointer(h, current)) {
      return false;
  }
     
  intptr_t header = o_get_header(current); 
  
  if (O_HEADER_GET_TYPE(header)==3) {
    intptr_t fwd_addr = O_HEADER_GET_PTR(header);    
    *(intptr_t *)p = fwd_addr;
    return false;
  }    
  return true; 
}

//TODO comments
void gc_depth_pointer_in_obj(heap_t *h, void *current, list_t * ptr_offset_to_update,list_t *seen,list_t *retroactive_ptr_upd)
{
  int n_ptrs_in_object = (int)o_pointers_in_object(current);
  
  for(size_t i = 0; i < n_ptrs_in_object; i++) {
    
    void **obj = o_get_pointer_in_object(current, i);
    
    if(is_useless_ptr(*obj, current)) {
      continue;
    }
    
    if(!is_heap_pointer(h, *obj)) {
      continue;
    }
    
    if(obj_in_list(seen, *obj)) {
      list_add(ptr_offset_to_update, (void *)i);
      continue;
    }
    
    gc_depth(h, obj, seen, retroactive_ptr_upd);
   
  }
}

//TODO comments
void gc_depth_add_pointers_to_retroactively_upd(void *new_addr, list_t *ptr_offset_to_update,list_t *retroactive_ptr_upd)
{
  while(list_len(ptr_offset_to_update) > 0) {
    size_t offset = (size_t)list_nth(ptr_offset_to_update, 0);
    list_remove_nth(ptr_offset_to_update, 0);    
    list_add(retroactive_ptr_upd, o_get_pointer_in_object(new_addr, offset));
  }
}

//TODO comments
void gc_depth_upd_retroactively(void *new_addr, void *current, list_t *retroactive_ptr_upd) 
{
  void *upd_me =  NULL;

  while((upd_me = current_in_retro_list(retroactive_ptr_upd, current)) != NULL) {
    *(intptr_t *)upd_me = (intptr_t)new_addr;
  }
}

void gc_depth(heap_t *h, void **p, list_t *seen, list_t *retroactive_ptr_upd)
{ 
  void *current = *p;
  
  if(!gc_depth_first_check(h,p,current)) {
    return;
  }
  intptr_t header = o_get_header(current);
  
  list_add(seen,current);

  list_t *ptr_offset_to_update = list_new();

  gc_depth_pointer_in_obj(h,current,ptr_offset_to_update,seen,retroactive_ptr_upd);
  
  if(O_HEADER_GET_TYPE(header) == 0) {
    gc_update_str_header(h, current);
  }

  void *new_addr = current;
  if(!is_addr_promoted(h, current)) {
    new_addr = o_move_object(h, current);
  }

  gc_depth_add_pointers_to_retroactively_upd(new_addr, ptr_offset_to_update,retroactive_ptr_upd);
  
  gc_depth_upd_retroactively(new_addr,current,retroactive_ptr_upd);
  
  *(intptr_t *)p = (intptr_t)new_addr;
  list_remove(seen, current);

  list_free(ptr_offset_to_update);
}

list_t *gc_traverse(heap_t *h, void fn(heap_t *h, list_t *l, void *ptr))
{
  Dump_registers();
  
  list_t *l = list_new();

  #if STACK_GROWTH == UP 
  void *bottom = stack_find_bottom();
  void *current = stack_find_top();
  #else 
  void *bottom = stack_find_top(); 
  void *current = stack_find_bottom();
  #endif

  while(current != NULL) {
    current = stack_trace(h, current, bottom);
    if (current == NULL) {
      break;
    }    
    fn(h, l, current);
    current = current-sizeof(void *);
  }
  return l;
}


void gc_copy_collect(heap_t *h, list_t *root_set) 
{
  
  list_t *seen = list_new();
  list_t *retroactive_ptr_upd = list_new();
  
  iter_t *it;
  for (it = iter(root_set); !iter_done(it); iter_next(it)) {
    void **current = (void **)iter_get(it);
    gc_depth(h, current, seen, retroactive_ptr_upd);
  }
  iter_free(it);
  list_free(seen);
  list_free(retroactive_ptr_upd);
}


void h_gc_safe_helper(heap_t *h, list_t *l, void *ptr)
{
  list_add(l, ptr);                                
}


size_t h_gc_safe(heap_t *h)
{
  size_t start_bytes = h_used(h);
  list_t *root_set = gc_traverse(h, h_gc_safe_helper);
  
  gc_copy_collect(h, root_set);
  
  h_purge(h);
  size_t end_bytes = h_used(h);
  //assert(end_bytes <= start_bytes && "end bytes should be less or equal to start bytes");
  size_t collected_bytes = start_bytes - end_bytes;
  
  list_free(root_set);
  
  return collected_bytes;      
}



void h_gc_unsafe_helper(heap_t *h, list_t *l, void *ptr)
{
  list_add(l, ptr);

  intptr_t i = *(intptr_t *)ptr;
  void *p = (void *)i; 
    
  promote_addr(h, p);
}

size_t h_gc_unsafe(heap_t *h)
{
  size_t start_bytes = h_used(h);
  list_t *root_set = gc_traverse(h, h_gc_unsafe_helper);

  gc_copy_collect(h, root_set);
  
  h_purge(h);
  size_t end_bytes = h_used(h);
  assert(end_bytes <= start_bytes && "end bytes should be less or equal to start bytes");
  size_t collected_bytes = start_bytes - end_bytes;
  
  list_free(root_set);
  
  return collected_bytes; 

}


size_t h_gc(heap_t *h) 
{
  if (!h_is_unsafe(h)) { 
    size_t collected_bytes = h_gc_safe(h);
    return collected_bytes;
  }
  else {
    size_t collected_bytes = h_gc_unsafe(h);
    return collected_bytes;
  }	
}


size_t h_gc_dbg(heap_t *h, bool unsafe_stack) 
{
  if(unsafe_stack) { 
    size_t collected_bytes = h_gc_unsafe(h);
    return collected_bytes;
  }
  else {
    size_t collected_bytes = h_gc_safe(h);
    return collected_bytes;
  }	
}


void *h_alloc_struct(heap_t *h, char *layout)
{  
  if(h_is_threshold_met(h)) {
    h_gc(h);
  }
  return o_alloc_struct(h, layout);
}


void *h_alloc_data(heap_t *h, size_t bytes)
{ 
  if(h_is_threshold_met(h)) {
    h_gc(h);
  }
  return o_alloc_raw(h, bytes);
}

void h_delete_dbg(heap_t *h, void *dbg_value)
{  
  #if STACK_GROWTH == UP
  void *bottom = stack_find_bottom();
  void *current = stack_find_top();
  #else
  void *bottom = stack_find_top();
  void *current = stack_find_bottom();
  #endif

  while(current > bottom) {
    intptr_t i = *(intptr_t *)current;
    void *p = (void *)i;  
    
    if (is_heap_pointer(h, p)) {
      *(intptr_t *)p = (intptr_t)dbg_value;
    }    
    
    current = current-sizeof(void *);
  }
  
  h_delete(h);
}
