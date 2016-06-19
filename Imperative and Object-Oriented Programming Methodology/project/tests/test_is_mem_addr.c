// Many of these tests can fail if memory is full

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <stdbool.h>

#include "../src/h_init.c"

void test_is_mem_addr_outside_heap()
{
  heap_t *h = h_init(16*1024, true, 0.9);

  void *before_heap1 = (char *)h-1;
  void *before_heap2 = h-1;
  void *after_heap1 = h->heap_end;
  void *after_heap2 = (char *)h->heap_end+1;

  CU_ASSERT_TRUE(!is_mem_addr(h, before_heap1));
  CU_ASSERT_TRUE(!is_mem_addr(h, before_heap2));
  CU_ASSERT_TRUE(!is_mem_addr(h, after_heap1));
  CU_ASSERT_TRUE(!is_mem_addr(h, after_heap2)); 
  h_delete(h);
}

void test_is_mem_addr_outside_pages()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  page_t *p = h->pages_start;
  char *pages_end = h->heap_end;
    
  void *before_pages1 = (char *)p-1;
  void *before_pages2 = p-1;
  void *after_pages1 = pages_end;
  void *after_pages2 = (char *)pages_end+1;

  CU_ASSERT_TRUE(!is_mem_addr(h, before_pages1));
  CU_ASSERT_TRUE(!is_mem_addr(h, before_pages2));
  CU_ASSERT_TRUE(!is_mem_addr(h, after_pages1));
  CU_ASSERT_TRUE(!is_mem_addr(h, after_pages2));
  h_delete(h); 
}

void test_is_mem_addr_page_headers()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  char *pages_start = h->pages_start;
  char *pages_end = h->heap_end;
  size_t pagesize = h->pagesize;

  char *header_n_start = pages_start;
  char *header_n_end = header_n_start + h->pageheader_size;
  while(header_n_end < pages_end) {
    char *header_n_left = header_n_start;
    char *header_n_right = header_n_end;
    while(header_n_left < header_n_right) {
      CU_ASSERT_TRUE(!is_mem_addr(h, header_n_left));
      header_n_left++;
    }
    header_n_start += pagesize;
    header_n_end += pagesize;  
  }
  h_delete(h);
}

void test_is_mem_addr_free_mem()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  char *pages_start = h->pages_start;
  char *pages_end = h->heap_end;

  page_t *p = (page_t *)pages_start;

  while((char *)p < pages_end) {
    char *free_mem_left  = p->bump;
    char *free_mem_right = ((char *)p) + h->pagesize;
    while(free_mem_left < free_mem_right) {
      CU_ASSERT_TRUE(!is_mem_addr(h, free_mem_left));
      free_mem_left++;
    }
    p = p->end;
  }
  h_delete(h);
}

// CALLER IS RESPONSIBLE FOR FREEING THE HEAP OBJECT
void test_is_mem_addr_taken_mem_aux(heap_t *h, page_t *p)
{
  char *current_addr = (char *)(p + 1);
  while(p->bump < p->end) {
    while(current_addr < (char *)p->bump) {
      CU_ASSERT_TRUE(is_mem_addr(h, current_addr));
      current_addr++;
    }
    p->bump = (char *)p->bump+1;
  }
  // DO NOT ADD A FREE STATEMENT
}

void test_is_mem_addr_taken_mem()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  page_t *p = (page_t *)(h->pages_start);

  while(p->end < h->heap_end) {
    test_is_mem_addr_taken_mem_aux(h, p);
    p = (page_t *)((char *)p + h->pagesize);
    p = p->end;
  }

  h_delete(h);
}

void test_is_page_addr_outside_heap()
{
  heap_t *h = h_init(16*1024, true, 0.9);

  void *before_heap1 = (char *)h-1;
  void *before_heap2 = h-1;
  void *after_heap1 = h->heap_end;
  void *after_heap2 = (char *)h->heap_end+1;

  CU_ASSERT_TRUE(!is_page_addr(h, before_heap1));
  CU_ASSERT_TRUE(!is_page_addr(h, before_heap2));
  CU_ASSERT_TRUE(!is_page_addr(h, after_heap1));
  CU_ASSERT_TRUE(!is_page_addr(h, after_heap2)); 

  h_delete(h);
}

void test_is_page_addr_outside_pages()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  page_t *p = h->pages_start;
  char *pages_end = h->heap_end;
    
  void *before_pages1 = (char *)p-1;
  void *before_pages2 = p-1;
  void *after_pages1 = pages_end;
  void *after_pages2 = (char *)pages_end+1;

  CU_ASSERT_TRUE(!is_page_addr(h, before_pages1));
  CU_ASSERT_TRUE(!is_page_addr(h, before_pages2));
  CU_ASSERT_TRUE(!is_page_addr(h, after_pages1));
  CU_ASSERT_TRUE(!is_page_addr(h, after_pages2));

  h_delete(h);
}

void test_is_page_addr_inside_pages()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  char *pages_left = h->pages_start;
  char* pages_right = h->heap_end;
  while(pages_left < pages_right) {
    CU_ASSERT_TRUE(is_page_addr(h, pages_left));
    pages_left++;
  }
  h_delete(h);
}



int main()
{
  CU_initialize_registry();
  // Set up suits register tests
  ////////////////////////////////////////////////////////////////////////////////
  CU_pSuite test_is_mem_addr = CU_add_suite("Tests for is_mem_addr", NULL, NULL);
  CU_add_test(test_is_mem_addr, "Check if addresses outside heap are treated correctly", test_is_mem_addr_outside_heap);
  CU_add_test(test_is_mem_addr, "Check if addresses outside pages are treated correctly", test_is_mem_addr_outside_pages);
  CU_add_test(test_is_mem_addr, "Check if addresses in page headers are treated correctly", test_is_mem_addr_page_headers);
  CU_add_test(test_is_mem_addr, "Check if addresses to free memory are treated correctly", test_is_mem_addr_free_mem);
  CU_add_test(test_is_mem_addr, "Check if addresses to taken memory are treated correctly", test_is_mem_addr_taken_mem);

  CU_pSuite test_is_page_addr = CU_add_suite("Tests for is_page_addr", NULL, NULL);
  CU_add_test(test_is_page_addr, "Check if addresses outside heap are treated correctly", test_is_page_addr_outside_heap);
  CU_add_test(test_is_page_addr, "Check if addresses outside pages are treated correctly", test_is_page_addr_outside_pages);
  CU_add_test(test_is_page_addr, "Check if addresses inside pages are treated correctly", test_is_page_addr_inside_pages);
  // Actually run tests
  //////////////////////////////////////////////////////////////////////////////
  CU_basic_run_tests();
  
  CU_cleanup_registry();
  return 0;
}
