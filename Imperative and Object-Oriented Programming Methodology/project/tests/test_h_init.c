// Many of these tests can fail if memory is full

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <stdbool.h>

#include "../src/h_init.c"



void test_create_destroy()
{
  heap_t *h = h_init(16*1024, false, 0.3);  
  h_delete(h); // TODO use valgrind or similar to see if everything was actually freed
}

void test_set_bytes()
{
  bool unsafe_stack = true;
  float gc_threshold = 0.9;

  heap_t *h = h_init(4*2048, unsafe_stack, gc_threshold);
  h_delete(h);

  h = h_init(1024*2048, true, gc_threshold);
  h_delete(h);
}

void test_set_unsafestack()
{
  size_t bytes = 16*1024;
  float gc_threshold = 0.9;

  heap_t *h = h_init(bytes, true, gc_threshold);
  h_delete(h);

  h = h_init(bytes, false, gc_threshold);
  h_delete(h);
}

void test_set_threshold()
{
  size_t bytes = 16*1024;
  bool unsafe_stack = true;

  heap_t *h = h_init(bytes, unsafe_stack, 0.9);
  h_delete(h);

  h = h_init(bytes, unsafe_stack, 0.1);
  h_delete(h);
}


void test_h_is_threshold_met()
{
  size_t bytes = 16*1024;
  bool unsafe_stack = true;
  float threshold = 0.4;
  float max = 0.5;
  float increment = 0.01;
  while(threshold < max)
  {
    heap_t *h = h_init(bytes, unsafe_stack, threshold);
    size_t total_mem = h->page_count * h->mem_per_page;
    size_t allocate_to_miss_threshold = (float)total_mem * ((float)threshold)/2.0;
    size_t allocate_to_then_surpass_threshold = total_mem - allocate_to_miss_threshold-10;

    // allocate enough to reach to just before the threshold
    page_t *p = h->pages_start;
    while(allocate_to_miss_threshold > 0) {
      size_t available_mem = p->end - p->bump;
      if(available_mem >= allocate_to_miss_threshold) {
        p->bump = (char *)p->bump + allocate_to_miss_threshold;
        allocate_to_miss_threshold = 0;
      } else {
        p->bump = p->end;
        allocate_to_miss_threshold -= available_mem;
      }
      CU_ASSERT_FALSE(h_is_threshold_met(h) && " threshold shouldn't be met yet, something went wrong.");
      p = p->end;
    }

    p = h->pages_start;
    while(allocate_to_then_surpass_threshold > 0) {
      size_t available_mem = p->end - p->bump;
      if(available_mem >= allocate_to_then_surpass_threshold) {
        p->bump = (char *)p->bump + allocate_to_then_surpass_threshold;
        allocate_to_then_surpass_threshold = 0;
      } else {
        p->bump = p->end;
        allocate_to_then_surpass_threshold -= available_mem;
      }
      /* printf("available_mem %lu   allocate_to_surpass: %lu   bump %p   is_threshold_met %d\n", */
      /*        available_mem, */
      /*        allocate_to_then_surpass_threshold, p->bump, h_is_threshold_met(h)); */
      p = p->end;
    }

    CU_ASSERT_TRUE(h_is_threshold_met(h) && " threshold should be met, something went wrong.");
    h_delete(h);
    threshold += increment;
  }
}

void test_check_unsafestack()
{
  size_t bytes = 16*1024;
  float threshold = 0.9;

  heap_t *h = h_init(bytes, true, threshold);
  CU_ASSERT_TRUE(h->unsafe_stack == true);

  heap_t *h2 = h_init(bytes, false, threshold);
  CU_ASSERT_TRUE(h2->unsafe_stack == false);
  h_delete(h);
}

void test_accessor_unsafestack()
{
  size_t bytes = 16*1024;
  float threshold = 0.9;
  heap_t *h;

  h = h_init(bytes, true, threshold);
  CU_ASSERT_TRUE(h_is_unsafe(h) == true);
  h->unsafe_stack = false;
  CU_ASSERT_TRUE(h_is_unsafe(h) == false);
  h_delete(h);

  h = h_init(bytes, false, threshold);
  CU_ASSERT_TRUE(h_is_unsafe(h) == false)
  h->unsafe_stack = true;
  CU_ASSERT_TRUE(h_is_unsafe(h) == true);
  h_delete(h);
}

void test_get_mem_per_page()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  size_t expected = h->pagesize - h->pageheader_size;
  CU_ASSERT_TRUE(get_mem_per_page(h) == expected);
}

void test_check_threshold()
{
  size_t bytes = 16*1024;
  bool unsafe_stack = true;

  float threshold = 0.9;
  heap_t *h = h_init(bytes, unsafe_stack, threshold);
  if(GC_THRESHOLD_OVERRIDE) {
    threshold = GC_THRESHOLD_OVERRIDE_VALUE;
  }
  CU_ASSERT_TRUE(h->gc_threshold == threshold);
  h_delete(h);

  threshold = 0.1;
  h = h_init(bytes, unsafe_stack, threshold);
  if(GC_THRESHOLD_OVERRIDE) {
    threshold = GC_THRESHOLD_OVERRIDE_VALUE;
  }
  CU_ASSERT_TRUE(h->gc_threshold == threshold);

  h_delete(h);
}

void test_check_headersize()
{
  heap_t *h = h_init(16*1024, true, 0.9);

  size_t actual_size = sizeof(heap_t);
  CU_ASSERT_TRUE(actual_size == h->header_size);

  h_delete(h);
}

void test_check_pages_start()
{
  heap_t *h = h_init(16*1024, true, 0.9);

  void *actual_start = h->pages_start;
  void *expected_start = h+1;
  CU_ASSERT_TRUE(actual_start == expected_start);

  h_delete(h);
}

void test_check_pageheadersize()
{
  heap_t *h = h_init(16*1024, true, 0.9);

  size_t pageheader_size = h->pageheader_size;
  CU_ASSERT_TRUE(pageheader_size == sizeof(page_t));

  h_delete(h);
}

void test_check_heapsize()
{
  size_t bytes = 16*1024;
  heap_t *h = h_init(bytes, true, 0.9);
  size_t max_bytes_pages = bytes - h->header_size;
  int    n_pages = max_bytes_pages / h->pagesize;
  size_t expected_bytes_pages = n_pages * h->pagesize;

  size_t expected = h->header_size + expected_bytes_pages;
  CU_ASSERT_TRUE(expected == h->heapsize);

  h_delete(h);
}

void test_check_end()
{
  size_t bytes = 16*1024;
  heap_t *h = h_init(bytes, true, 0.9);
  size_t max_bytes_pages = bytes - h->header_size;
  int    n_pages = max_bytes_pages / h->pagesize;
  size_t expected_bytes_pages = n_pages * h->pagesize;
  size_t expected_bytes = expected_bytes_pages + h->header_size;

  void *expected = (void *) ((char *)h + expected_bytes);
  CU_ASSERT_TRUE(expected == h->heap_end);

  h_delete(h);
}

void test_check_mem_per_page()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  page_t *p = h->pages_start;
  size_t start = (size_t)h->pages_start + h->pageheader_size;
  size_t expected = (size_t)p->end - start;

  size_t mem_per_page = h->mem_per_page;
  CU_ASSERT_TRUE(mem_per_page == expected);

  h_delete(h);
}

void test_p_used()
{
  //TODO
}

void test_p_avail()
{
  //TODO
}

void test_h_used()
{
  heap_t *h = h_init(64*1024, true, 0.9);
  page_t *p;
  int i = 0;
  size_t add_full = h->mem_per_page;
  size_t add_half = add_full/2;
  int n_classes = 3;
  size_t expected_used = 0;

  // Setting values for pages, equivalent classes:
  //   [0] = empty
  //   [1] = half_full
  //   [2] = full

  p = h->pages_start;
  i = 0;
  CU_ASSERT_TRUE(h_used(h) == expected_used);
  while((void *)p < h->heap_end) {    
    void *before = p->bump;
    void *bump_full = ((char *)(p+1)) + add_full;
    void *bump_half = ((char *)(p+1)) + add_half;
    void *bump_empty = p+1;

    switch(i % n_classes) {
    case 0:
      p->bump = bump_empty;
      break;
    case 1:
      p->bump = bump_half;
      break;
    case 2:
      p->bump = bump_full;
      break;
    default:
      assert(false && "Something went wrong with the test.");
      break;
    }
    size_t added = p->bump - before;
    expected_used += added;
    CU_ASSERT_TRUE(h_used(h) == expected_used);
    p = p->end;
    i++;
  }
  h_delete(h);
}

void test_h_avail()
{
  heap_t *h = h_init(64*1024, true, 0.9);
  page_t *p;
  int i = 0;
  size_t add_full = h->mem_per_page;
  size_t add_half = add_full/2;
  size_t old_avail;
  size_t accumulated = 0;
  int n_classes = 3;
  // Setting values for pages, equivalent classes:
  //   [0] = empty
  //   [1] = half_full
  //   [2] = full

  p = h->pages_start;
  i = 0;
  old_avail = h_avail(h);
  while((void *)p < h->heap_end) {    
    void *bump_full = ((char *)(p+1)) + add_full;
    void *bump_half = ((char *)(p+1)) + add_half;
    void *bump_empty = p+1;
    switch(i % n_classes) {
    case 0:
      p->bump = bump_empty;
      break;
    case 1:
      p->bump = bump_half;
      break;
    case 2:
      p->bump = bump_full;
      break;
    default:
      assert(false && "Something went wrong with the test.");
      break;
    }
    size_t expected_taken = (p->bump) - (void *)(p+1);
    size_t avail = h_avail(h);
    size_t taken = old_avail - avail;
    accumulated += p->end - p->bump;

    CU_ASSERT_TRUE(taken == expected_taken);
    old_avail = avail;
    p = p->end;
    i++;
  }
  CU_ASSERT_TRUE(h_avail(h) == accumulated);
  h_delete(h);
}



int main()
{
  CU_initialize_registry();
  // Set up suits register tests
  ////////////////////////////////////////////////////////////////////////////////
  CU_pSuite heap_creation = CU_add_suite("Heap creation and possible argument", NULL, NULL);
  CU_add_test(heap_creation, "h_init (unsafe_stack)", test_create_destroy);
  CU_add_test(heap_creation, "h_init (bytes)", test_set_bytes);
  CU_add_test(heap_creation, "h_init (unsafe_stack)", test_set_unsafestack);
  CU_add_test(heap_creation, "h_init (threshold)", test_set_threshold);
  
  CU_pSuite heap_check_values = CU_add_suite("Manually check if heap values are as expected", NULL, NULL);
  CU_add_test(heap_check_values, "Check if unsafestack is correct", test_check_unsafestack);
  CU_add_test(heap_check_values, "Check if threshold is correct", test_check_threshold);
  CU_add_test(heap_check_values, "Check if headersize is correct", test_check_headersize);
  CU_add_test(heap_check_values, "Check if pages_start is correct", test_check_pages_start);
  CU_add_test(heap_check_values, "Check if pageheader_size is correct", test_check_pageheadersize);
  CU_add_test(heap_check_values, "Check if heapsize is correct", test_check_heapsize);
  CU_add_test(heap_check_values, "Check if end address is correct", test_check_end);
  CU_add_test(heap_check_values, "Check if mem_per_page is correct", test_check_mem_per_page);

  CU_pSuite heap_accessor_values = CU_add_suite("Check if heap accessos work as expected", NULL, NULL);
  CU_add_test(heap_accessor_values, "Check if mem_per_page is correct",  test_get_mem_per_page);
  CU_add_test(heap_accessor_values, "Check if get_unsafestack is correct",  test_accessor_unsafestack);

CU_pSuite heap_calc_functions = CU_add_suite("Test functions that calculate values", NULL, NULL);
  CU_add_test(heap_calc_functions, "Check if p_used works correctly", test_p_used);
  CU_add_test(heap_calc_functions, "Check if p_avail works correctly", test_p_avail);
  CU_add_test(heap_calc_functions, "Check if h_used works correctly", test_h_used);
  CU_add_test(heap_calc_functions, "Check if h_avail works correctly", test_h_avail);
  CU_add_test(heap_calc_functions, "Check if h_is_threshold_met works correctly", test_h_is_threshold_met);

  // Actually run tests
  //////////////////////////////////////////////////////////////////////////////
  CU_basic_run_tests();
  
  CU_cleanup_registry();
  return 0;
}
