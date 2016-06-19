#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>

#include "../src/gc.h"
#include "../src/gc.c"
#include "../src/h_init.c"

void allocate_on_heap_and_drop_ptr(heap_t *h, int count) {  
  for(int i = 0; i<count; i++) {
    o_alloc_raw(h, sizeof(void *));
  }
}

int attempt_clear_registers(int i, size_t st, long l)
{
  i = i * 2;
  for (size_t q = 0; q < 1000; q++) {
    l = (long)(i * (int)st);
  }
  char c[256] = "";
  sprintf(c, "long: %ld, int: %d, size_t: %zu\n",l,i,st);
  return (int)st;
}

void test_find_top() {

  // TODO: FIX BETTER TEST
  #if STACK_GROWTH == UP
  void *bottom = stack_find_bottom();
  void *current = stack_find_top();
  CU_ASSERT_TRUE((intptr_t)bottom < (intptr_t)current);
  
  #else
  void *bottom = stack_find_bottom();
  void *current = stack_find_top();
  CU_ASSERT_TRUE((intptr_t)bottom > (intptr_t)current);
  #endif
}
  
void test_find_bottom() {
  // TODO: FIX ACTUAL TEST
  void *bottom = stack_find_bottom();
  CU_ASSERT_TRUE(bottom != NULL);
}


// TODO: RENAME
void test_check_pointer() {
  
  heap_t *h = h_init(16*1024,false,0.5);
  void *on_heap = o_alloc_raw(h,sizeof(void *));
    
  bool c = is_heap_pointer(h,on_heap);
  
  CU_ASSERT_TRUE(c == true);
  h_delete(h);
}


void test_delete_dbg() {
  heap_t *h = h_init(16*1024,false,0.5);
  void *on_heap = o_alloc_raw(h,sizeof(void *));
  *(intptr_t*)on_heap = 666;
  void *change_to = o_alloc_raw(h,sizeof(void *));
  *(intptr_t*)change_to = 789;
    
  CU_ASSERT_TRUE(is_heap_pointer(h,on_heap) == true);  
  CU_ASSERT_TRUE(*(intptr_t*)on_heap == 666);

  h_delete_dbg(h, change_to);
  
  CU_ASSERT_TRUE(*(intptr_t*)on_heap == *(intptr_t*)change_to);
  //h_delete(h);
} 

void test_trace_one_variable_on_stack() {
  
  #if STACK_GROWTH == UP
  void *bottom = stack_find_bottom();
  void *current = stack_find_top();
  #else
  void *bottom = stack_find_top();
  void *current = stack_find_bottom();
  #endif
   
  heap_t *h = h_init(16*1024,false,0.5);
  void *on_heap = o_alloc_raw(h,sizeof(void *));
  /* void *a = o_alloc_raw(h,sizeof(void *)); */
  /* void *b = o_alloc_raw(h,sizeof(void *)); */
  /* printf("on_heap: %ld\n", (intptr_t)on_heap); */
  /* printf("a: %ld\n", (intptr_t)a); */
  /* printf("b: %ld\n", (intptr_t)b); */
  
  CU_ASSERT_TRUE(is_heap_pointer(h,on_heap) == true);
  /* int stop = 0; */
  while(current != NULL) {
    /* printf("currentA: %ld\n",(intptr_t)current); */
    /* printf("currentA innehåll: %ld\n",*(intptr_t*)current); */
    current = stack_trace(h,current,bottom);
  
    /* printf("efter stacktrace currentA: %ld\n",(intptr_t)current); */
    /* printf("efter stacktracecurrentA innehåll: %ld\n",*(intptr_t*)current); */
    /* printf("on_heap: %ld\n", (intptr_t)&on_heap); */
    /* printf("a: %ld\n", (intptr_t)&a); */

    if(&on_heap == current) {
      CU_ASSERT_TRUE(&on_heap == current);
      break;      
      //continue;
    }
  
    if(current == NULL){
      CU_ASSERT_TRUE(on_heap == current);
      break;
    }
    current = current-sizeof(void *);
    /* ++stop; */
    /* if(stop == 5){assert(false);} */
  } 

  h_delete(h);
}


void test_trace_multi_variable_on_stack() {
  
  #if STACK_GROWTH == UP
  void *bottom = stack_find_bottom();
  void *current = stack_find_top();
  #else
  void *bottom = stack_find_top();
  void *current = stack_find_bottom();
  #endif
      
  heap_t *h = h_init(16*1024,false,0.5);
  
  void *a = o_alloc_raw(h,sizeof(void *));
  CU_ASSERT_TRUE(is_heap_pointer(h,a) == true);
  void *b = o_alloc_raw(h,sizeof(void *));
  CU_ASSERT_TRUE(is_heap_pointer(h,b) == true);
  void *c = o_alloc_raw(h,sizeof(void *));
  CU_ASSERT_TRUE(is_heap_pointer(h,c) == true);  
    
  int array_size = 3;
  intptr_t arr[array_size - 1];
  
  arr[0] = (intptr_t)&a;
  arr[1] = (intptr_t)&b;
  arr[2] = (intptr_t)&c;

  /* printf("a:    %ld\n", &a); */
  /* printf("arr0: %ld\n", arr[0]); */
  /* printf("b:    %ld\n", &b); */
  /* printf("arr1: %ld\n", arr[1]); */
  /* printf("c:    %ld\n", &c); */
  /* printf("arr2: %ld\n", arr[2]); */
  
  intptr_t i, k, temp;
  for (i = 0; i < (array_size - 1); ++i) {
    for (k = 0; k < array_size - 1 - i; ++k ) {
      if (arr[k] < arr[k+1]) {
        temp = arr[k+1];
        arr[k+1] = arr[k];
        arr[k] = temp;
      }
    }
  }   
  
  int count = 0;

  while(current != NULL) {
    current = stack_trace(h,current,bottom);
    if(current == NULL) {
      break;
    }      
    else if(*(intptr_t *)&current == arr[count]) {
      count++;
    }       
    current = current-sizeof(void *);
  }  
  
  CU_ASSERT_TRUE(count == 3);
  h_delete(h);
}

void test_gc_safe_drop_ptr()
{
  heap_t *h = h_init(16*1024,false,0.5);
  CU_ASSERT_TRUE(h_gc_safe(h) == 0);

  void *ptr = o_alloc_raw(h, sizeof(void *));
  ptr = NULL;
  size_t allo_bytes = sizeof(void *)*2;
  attempt_clear_registers(1234, 34, 12345); // ATTEMPT CLEAR REGISTERS
  size_t collected_bytes = h_gc_safe(h);
  CU_ASSERT_TRUE(collected_bytes == allo_bytes);

  allo_bytes = 2*sizeof(void *)*2;
  ptr = o_alloc_raw(h, sizeof(void *));
  ptr = o_alloc_raw(h, sizeof(void *));
  ptr = NULL;
  attempt_clear_registers(1234, 34, 12345); // ATTEMPT CLEAR REGISTERS
  collected_bytes = h_gc_safe(h);
  CU_ASSERT_TRUE(collected_bytes == allo_bytes);
}


void test_gc_safe_drop_all_ptrs() {
  heap_t *h = h_init(16*1024,false,0.5);
  int count = 10;
  size_t allo_bytes = count*sizeof(void *)*2;
  
  allocate_on_heap_and_drop_ptr(h,count);
  attempt_clear_registers(1234, 34, 12345); // ATTEMPT CLEAR REGISTERS
  size_t gc_collected_bytes = h_gc_safe(h);

  CU_ASSERT_TRUE(gc_collected_bytes == allo_bytes);
  h_delete(h);
}

void test_gc_safe_drop_keep_one_ptr() {
  heap_t *h = h_init(16*1024,false,0.5);
  int count = 10;
  size_t allo_bytes = count*sizeof(void *)*2;
  
  allocate_on_heap_and_drop_ptr(h,count);
  
  void *a = o_alloc_raw(h,sizeof(void *));
  char str[100] = "";
  sprintf(str, "%p", a);
  
  attempt_clear_registers(1234, 34, 12345); // ATTEMPT CLEAR REGISTERS
  size_t gc_collected_bytes = h_gc_safe(h);
  CU_ASSERT_TRUE(gc_collected_bytes == allo_bytes);
  h_delete(h);
}

void test_gc_unsafe_drop_all_ptrs() {
  heap_t *h = h_init(16*1024,true,0.5);
  int count = 10;
  size_t allo_bytes = count*sizeof(void *)*2;
  
  allocate_on_heap_and_drop_ptr(h,count);
  attempt_clear_registers(1234, 34, 12345); // ATTEMPT CLEAR REGISTERS
  size_t gc_collected_bytes = h_gc_unsafe(h);
  
  CU_ASSERT_TRUE(gc_collected_bytes == allo_bytes);
  
  h_delete(h);
}


void test_gc_unsafe_drop_keep_one_ptr() {
  heap_t *h = h_init(16*1024,true,0.5);
  int count = 10;
  // size_t allo_bytes = count*sizeof(void *)*2;
  
  allocate_on_heap_and_drop_ptr(h,count);
  
  void *a = o_alloc_raw(h,sizeof(void *));
  char str[100] = "";
  sprintf(str, "%p", a);
  
  attempt_clear_registers(1234, 34, 12345); // ATTEMPT CLEAR REGISTERS
  size_t gc_collected_bytes = h_gc_unsafe(h);
  CU_ASSERT_TRUE(gc_collected_bytes == 0);
  h_delete(h);
}

void test_gc_safe_get_upd_addr_of_ptr() {
  heap_t *h = h_init(16*1024,false,0.5); 
  void *on_heap = o_alloc_struct(h,"*");
  *(intptr_t *)on_heap = 555;

  int first_addr = (intptr_t)on_heap +5000;

  h_gc_safe(h);
  
  first_addr = first_addr - 5000;

  CU_ASSERT_TRUE(first_addr != (intptr_t)on_heap);
  
  h_delete(h);
}

void test_gc_unsafe_get_upd_addr_of_ptr() {

  heap_t *h = h_init(16*1024,true,0.5); 
  void *on_heap = o_alloc_struct(h,"*");
  *(intptr_t *)on_heap = 555;
  
  intptr_t first_addr = (intptr_t)on_heap +5000;
  attempt_clear_registers(1234, 34, 12345); // ATTEMPT CLEAR REGISTERS
  h_gc_unsafe(h);
  
  first_addr = first_addr - 5000;
  
  CU_ASSERT_TRUE(first_addr == (intptr_t)on_heap);
  
  h_delete(h);

}

struct testdata {
  void * p1;
  int i1;
  void *p2;
};

void test_pointer_in_object() {
  heap_t *h = h_init(16*1024,false,0.5);
  struct testdata *test1 = o_alloc_struct(h,"*i*");
  struct testdata *test2 = o_alloc_struct(h,"*i*");

  char *str1 = "First";
  char *str2 = "Second"; 
  
  test1->p1 = test2;
  test1->i1 = 4545;
  test1->p2 = str1; 
  
  test2->p1 = str2;
  test2->i1 = 666;
  test2->p2 = NULL;
  test2 = NULL;
  
  //h_print_pages(h);

  
  size_t gc_collected_bytes = h_gc_safe(h);

  //puts("==================================");
  //h_print_pages(h);
    
  CU_ASSERT_TRUE(gc_collected_bytes == 0);
  CU_ASSERT_EQUAL(test1->i1, 4545);
  CU_ASSERT_EQUAL(test1->p2, str1);

  test2 = test1->p1;
  
  CU_ASSERT_PTR_NOT_NULL_FATAL(test2);
  CU_ASSERT_EQUAL(test2->p1, str2);
  CU_ASSERT_EQUAL(test2->i1, 666);
  CU_ASSERT_PTR_NULL(test2->p2);
  
  h_delete(h);
}

void test_pointer_deep_lost() {
  heap_t *h = h_init(16*1024,false,0.5);
  struct testdata *test1 = o_alloc_struct(h,"*i*");
  struct testdata *test2 = o_alloc_struct(h,"*i*");

  char *str1 = "First";
  char *str2 = "Second";
  
  
  test1->p1 = test2;
  test1->i1 = 4545;
  test1->p2 = str1;
  
  test2->p1 = str2;
  test2->i1 = 666;
  test2->p2 = NULL;
  test1 = NULL;
  test2 = NULL;
  
  attempt_clear_registers(1234, 34, 12345); // ATTEMPT CLEAR REGISTERS
  size_t gc_collected_bytes = h_gc_safe(h);
  CU_ASSERT_TRUE(gc_collected_bytes == (sizeof(struct testdata)*2+sizeof(void *)*2));
  
  h_delete(h); 
}




void test_h_gc_unsafe_helper()
{
  // TODO
}


void test_is_useless_ptr()
{
  int o1 = 1;
  int o2 = 2;

  void *p_o1 = &o1;
  void *p_o2 = &o2;

  CU_ASSERT_TRUE(is_useless_ptr(NULL, NULL));
  CU_ASSERT_TRUE(!is_useless_ptr(p_o1, p_o2));
  CU_ASSERT_TRUE(!is_useless_ptr(p_o2, p_o1));

  CU_ASSERT_TRUE(is_useless_ptr(p_o1, NULL));
  CU_ASSERT_TRUE(is_useless_ptr(p_o2, NULL));
  CU_ASSERT_TRUE(is_useless_ptr(NULL, p_o1));
  CU_ASSERT_TRUE(is_useless_ptr(NULL, p_o2));
}

void test_is_useless_ptr2()
{
  // testing null ptrs and ptrs to null objs
  void *p_null = NULL;
  void *p_o_null = &p_null;

  int o1 = 1;
  void *p_o1 = &o1;

  CU_ASSERT_TRUE(is_useless_ptr(p_null, p_null));
  CU_ASSERT_TRUE(is_useless_ptr(p_o_null, p_o_null));

  CU_ASSERT_TRUE(is_useless_ptr(p_null, p_o_null));
  CU_ASSERT_TRUE(is_useless_ptr(p_o_null, p_null));

  CU_ASSERT_TRUE(is_useless_ptr(p_null, NULL));
  CU_ASSERT_TRUE(is_useless_ptr(p_o_null, NULL));

  CU_ASSERT_TRUE(is_useless_ptr(NULL, p_null));
  CU_ASSERT_TRUE(is_useless_ptr(NULL, p_o_null));

  CU_ASSERT_TRUE(is_useless_ptr(p_null, p_o1) && " pointer to a null_pointer was accepted.");
  CU_ASSERT_FALSE(is_useless_ptr(p_o_null, p_o1) && " pointer to a null pointer was accepted");

  CU_ASSERT_TRUE(is_useless_ptr(p_o1, p_null));
  CU_ASSERT_FALSE(is_useless_ptr(p_o1, p_o_null));
}

int main() {
 
  CU_initialize_registry();

  CU_pSuite stack_trace = CU_add_suite("Stack trace", NULL, NULL);
  CU_add_test(stack_trace, "Find top of stack:", test_find_top);
  CU_add_test(stack_trace, "Find bottom of stack:", test_find_bottom);
  CU_add_test(stack_trace, "Check pointer on stack:", test_check_pointer);
  CU_add_test(stack_trace, "One variable on stack:", test_trace_one_variable_on_stack);
  CU_add_test(stack_trace, "Multi variable on stack:", test_trace_multi_variable_on_stack);
  CU_add_test(stack_trace, "Delete dbg:",test_delete_dbg);
  CU_add_test(stack_trace, "GC safe drop ptr:",test_gc_safe_drop_ptr);
  CU_add_test(stack_trace, "GC safe drop all ptrs:",test_gc_safe_drop_all_ptrs);
  CU_add_test(stack_trace, "GC unsafe drop all ptrs:",test_gc_unsafe_drop_all_ptrs);

  CU_add_test(stack_trace, "GC safe get upd addr of ptr:",test_gc_safe_get_upd_addr_of_ptr);
  CU_add_test(stack_trace, "GC unsafe get upd addr of ptr:",test_gc_unsafe_get_upd_addr_of_ptr);

  CU_add_test(stack_trace, "Testing helper of h_gc_unsafe", test_h_gc_unsafe_helper);

  CU_add_test(stack_trace, "Test pointer in object", test_pointer_in_object);
  CU_add_test(stack_trace, "Test deep lost", test_pointer_deep_lost);
  CU_add_test(stack_trace, "Test keep one pointer safe", test_gc_safe_drop_keep_one_ptr);
  CU_add_test(stack_trace, "Test keep one pointer unsafe", test_gc_unsafe_drop_keep_one_ptr);

  CU_add_test(stack_trace, "test is_useless_ptr", test_is_useless_ptr);
  CU_add_test(stack_trace, "test 2 of is_useless_ptr", test_is_useless_ptr2);

  CU_basic_run_tests();

  CU_cleanup_registry();
  return 0;
}

/*
test
  void h_delete_dbg(heap_t *h, void *dbg_value)
  void gc_update_str_header(heap_t *h, void *p)
  void **current_in_retro_list(list_t *l, void *p )
  list_t *gc_traverse(heap_t *h, void fn(heap_t *h, list_t *l, void *ptr))


don't test
  void *stack_find_bottom()
  void *stack_find_top()
  bool is_heap_pointer(heap_t *h, void *p)
  void *stack_trace(heap_t *h, void *current, void *bottom)
  bool is_useless_ptr(void *p, void *current_object)
  bool obj_in_list(list_t *l, void *p)

don't know
  void gc_depth(heap_t *h, void **p, list_t *seen, list_t *retroactive_ptr_upd)
  void gc_copy_collect(heap_t *h, list_t *root_set)
  void h_gc_safe_helper(heap_t *h, list_t *l, void *ptr)
  size_t h_gc_safe(heap_t *h)
  void h_gc_unsafe_helper(heap_t *h, list_t *l, void *ptr)
  size_t h_gc_unsafe(heap_t *h)
  size_t h_gc(heap_t *h)
  size_t h_gc_dbg(heap_t *h, bool unsafe_stack)
  void *h_alloc_struct(heap_t *h, char *layout)
  void *h_alloc_data(heap_t *h, size_t bytes)
*/
