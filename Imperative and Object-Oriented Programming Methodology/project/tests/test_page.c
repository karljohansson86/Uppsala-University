// Many of these tests can fail if memory is full

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <stdbool.h>
#include <string.h>

#define N_PERMS 1*2*3*4*5*6*7
#define STRING_PERMS "0123456"
#define LEN_PERMS_PLUS_1 8

#include "../src/h_init.c"

void test_pages()
{
  size_t bytes = 100*1024;
  heap_t *h = h_init(bytes, true, 0.9);
  page_t *p = h->pages_start;
  size_t expected = (bytes - h->header_size)/PAGESIZE;
  int i = 0;

  while((void *)p < h->heap_end) {
    p = p->end;
    i++;
  }

  CU_ASSERT_TRUE(i == expected);
  h_delete(h);
}


void test_pages_newspace()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  page_t *p = h->pages_start;

  bool expected = false;
  while((void *)p < h->heap_end) {
    CU_ASSERT_TRUE(p->new_space == expected);
    p->new_space = true;
    p = p->end;
  }  
  p = h->pages_start;
  expected = true;
  while((void *)p < h->heap_end) {
    CU_ASSERT_TRUE(p->new_space == expected);
    p = p->end;
  }
  h_delete(h);
}

void test_pages_promoted()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  page_t *p = h->pages_start;

  bool expected = false;
  while((void *)p < h->heap_end) {
    CU_ASSERT_TRUE(p->promoted == expected);
    p->promoted = true;
    p = p->end;
  }
  p = h->pages_start;
  expected = true;
  while((void *)p < h->heap_end) {
    CU_ASSERT_TRUE(p->promoted == expected);
    p = p->end;
  }  

  h_delete(h);
}

void test_pages_bump()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  page_t *p = h->pages_start;
  void *expected;

  while((void *)p < h->heap_end) {
    expected = p + 1;
    CU_ASSERT_TRUE(p->bump == expected);
    p = p->end;
  }
  h_delete(h);
}

void test_pages_end()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  page_t *p = h->pages_start;
  void *expected;

  while((void *)p < h->heap_end) {
    expected = (void *)p + h->pagesize;
    CU_ASSERT_TRUE(p->end == expected);
    p = p->end;
  }
  h_delete(h);
}

void test_pages_freememory ()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  page_t *p = h->pages_start;

  while((void *)p < h->heap_end) {
    char *current_addr = (char *)(p+1);
    char *fin_addr = (char *)p + (h->pagesize);
    
    for(; current_addr < fin_addr; current_addr++){
      CU_ASSERT_TRUE(*current_addr == 0);
    }
    p = p->end;
  }  
  
  h_delete(h);
}

void test_is_space_newspace()
{
  heap_t *h = h_init(256*1024, true, 0.9);
  bool expected;
  page_t *p;

  expected = false;
  p = h->pages_start;
  while((void *)p < h->heap_end) {
    char *current = (char *)(p+1);
    while(current < (char *)(p->end)){
      CU_ASSERT_TRUE(is_space_newspace(h, current) == expected);
      current++;
    }
    p->new_space = true;
    p = p->end;
  }

  expected = true;
  p = h->pages_start;
  while((void *)p < h->heap_end) {
    char *current = (char *)(p+1);
    while(current < (char *)(p->end)){
      CU_ASSERT_TRUE(is_space_newspace(h, current) == expected);
      current++;
    }
    p = p->end;
  }

  h_delete(h);
}


void test_is_addr_promoted()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  bool expected;
  page_t *p;

  expected = false;
  p = h->pages_start;
  while((void *)p < h->heap_end) {
    char *current = (char *)(p+1);
    while(current < (char *)(p->end)){
      CU_ASSERT_TRUE(is_addr_promoted(h, current) == expected);
      current++;
    }
    p->promoted = true;
    p = p->end;
  }

  expected = true;
  p = h->pages_start;
  while((void *)p < h->heap_end) {
    char *current = (char *)(p+1);
    while(current < (char *)(p->end)){
      CU_ASSERT_TRUE(is_addr_promoted(h, current) == expected);
      current++;
    }
    p = p->end;
  }

  h_delete(h);
}

void test_promote_addr(){
  heap_t *h = h_init(16*1024, true, 0.9);
  bool expected;
  page_t *p;

  expected = true;
  p = h->pages_start;

  while((void *)p < h->heap_end) {
    char *current = (char *)(p+1);
    promote_addr(h,(char *)p+1);
    while(current < (char *)(p->end)){
      CU_ASSERT_TRUE(is_addr_promoted(h, current) == expected);
      current++;
    }
    p = p->end;
  }

  h_delete(h);  

}


void test_h_purge()
{
  heap_t *h = h_init(16*1024, true, 0.9);
  page_t *p;
  int i = 0;
  
  // Setting values for pages (4 possible combinations).
  //   [0] = !promoted, !newspace,  empty  ==purge==> !promoted, !newspace,  empty
  //   [1] =  promoted, !newspace, !empty  ==purge==> !promoted,  newspace, !empty
  //   [2] = !promoted,  newspace, !empty  ==purge==> !promoted, !newspace,  empty
  //   [3] =  promoted,  newspace, !empty  ==purge==> !promoted,  newspace, !empty
  p = h->pages_start;

  size_t bump_add = 1;

  i = 0;
  while((void *)p < h->heap_end) {
    void *bump_not_empty = ((char *)(p+1)) + bump_add;
    void *bump_empty= p+1;
    switch(i % 4) {
    case 0:
      p->promoted = false;
      p->new_space = false;
      p->bump = bump_empty;
      break;
    case 1:
      p->promoted = true;
      p->new_space = false;
      p->bump = bump_not_empty;
      break;
    case 2:
      p->promoted = false;
      p->new_space = true;
      p->bump = bump_not_empty;
      break;
    case 3:
      p->promoted = true;
      p->new_space = true;
      p->bump = bump_not_empty;
      break;
    default:
      assert(false && "Something went wrong with the test. Make sure to use modulo 4 on the switch variable.");
      break;
    }
    p = p->end;
    i++;
  }

  // Resetting pages and checking if values as expected
  //   [0] = !promoted, !newspace,  empty  ==purge==> !promoted, !newspace,  empty
  //   [1] =  promoted, !newspace, !empty  ==purge==> !promoted,  newspace, !empty
  //   [2] = !promoted,  newspace, !empty  ==purge==> !promoted, !newspace,  empty
  //   [3] =  promoted,  newspace, !empty  ==purge==> !promoted,  newspace, !empty
  h_purge(h);
  p = h->pages_start;
  i = 0;
  while((void *)p < h->heap_end) {
    void *bump_empty = p+1;
    void *bump_not_empty = ((char *)(p+1))+bump_add;
    CU_ASSERT_TRUE(p->promoted == false);
    switch(i % 4) {
    case 0:
      CU_ASSERT_TRUE(p->new_space == false);
      CU_ASSERT_TRUE(p->bump == bump_empty);
      break;
    case 1:
      CU_ASSERT_TRUE(p->new_space == true);
      CU_ASSERT_TRUE(p->bump == bump_not_empty);
      break;
    case 2:
      CU_ASSERT_TRUE(p->new_space == false);
      CU_ASSERT_TRUE(p->bump == bump_empty);
      break;
    case 3:
      CU_ASSERT_TRUE(p->new_space == true);
      CU_ASSERT_TRUE(p->bump == bump_not_empty);
      break;
    default:
      assert(false && "Something went wrong with the test. Make sure to use modulo 4 on the switch variable.");
      break;
    }
    p = p->end;
    i++;
  }
  h_delete(h);
}


/**************************************************************/
// The below is a modification of
// http://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string

/* Function to swap values at two pointers */
void swap(char *x, char *y)
{
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
 
/* Function to print permutations of string
   This function takes three parameters:
   1. String
   2. Starting index of the string
   3. Ending index of the string. */
void permute(char *a, int l, int r, char result[][LEN_PERMS_PLUS_1], int *i_res)
{
   int i;
   if (l == r)
   {
     char *current = (result)[*i_res];
     strcpy(current, a);
     *i_res = *i_res + 1;
   }
   else
   {
       for (i = l; i <= r; i++)
       {
          swap((a+l), (a+i));
          permute(a, l+1, r, result, i_res);
          swap((a+l), (a+i)); //backtrack
       }
   }
}

int digit_to_int(char d)
{
 char str[2];

 str[0] = d;
 str[1] = '\0';
 return (int) strtol(str, NULL, 10);
}

/**************************************************************/

/**
 * Warning: this test is messy.
 */
void test_h_alloc_gc()
{
  /********************/
  /*  PREPARING TEST  */
  /********************/

  bool  prom, newspc, empty, fits, skip; // prom = promoted, newspc = new_space
  prom = true, newspc = true, empty = true, fits = true, skip = true;

  // equivalence classes for pages: 0-3 when pages can   fit asked size
  //                                4-6 when pages can't fit asked size
  bool classes[7][8] = {
    {!prom, !newspc,  empty,  fits, !skip /*=case2     =>*/,  prom, !newspc, !empty}, // class0
    { prom, !newspc, !empty,  fits, !skip /*=case1     =>*/,  prom, !newspc, !empty}, // class1
    {!prom,  newspc, !empty,  fits,  skip /*=skip      =>*/, !prom,  newspc, !empty}, // class2   skip instantly
    { prom,  newspc, !empty,  fits, !skip /*=case1     =>*/,  prom,  newspc, !empty}, // class3
    { prom, !newspc, !empty, !fits, !skip /*=case1+skip=>*/,  prom, !newspc, !empty}, // class4   skip
    {!prom,  newspc, !empty, !fits,  skip /*=skip      =>*/, !prom,  newspc, !empty}, // class5   skip instantly
    { prom,  newspc, !empty, !fits, !skip /*=case1+skip=>*/,  prom,  newspc, !empty}  // class6   skip
  };

  // Generating possible permutations with one instance of each class = 7! or 1*2*3*4*5*6*7 permutations.
  char str[]       = STRING_PERMS;
  int  perms_len   = strlen(str);
  int  perms_count = N_PERMS;
  int  buffer_i    = 0;
  char perms[perms_count][LEN_PERMS_PLUS_1];
  permute(str, 0, perms_len-1, perms, &buffer_i);

  size_t  bytes             = 15*1024;
  heap_t *help_heap         = h_init(bytes, true, 0.9);
  int     page_count        = (help_heap->heap_end - help_heap->pages_start)/help_heap->pagesize;
  assert(help_heap->mem_per_page > 3 && "pages have too little memory for this test to run successfully");
  assert(page_count == 7 && " bad amount of pages, should be 7");

  size_t  incr_empty_fits   = 0;
  size_t  incr_nempty_nfits = help_heap->mem_per_page-2;
  size_t  incr_nempty_fits  = 1;
  size_t  alloc_size        = 2000; 
  h_delete(help_heap);

  /********************/
  /*   RUNNING TEST   */
  /********************/
  int perms_i;
  for(perms_i = 0; perms_i < perms_count; perms_i++) {
    heap_t *h    = h_init(bytes, true, 0.9);
    page_t *p    = h->pages_start;
    char   *perm = perms[perms_i];
    int     perm_i;
    int     modified_i = -1;

    // WRITING PERMS TO HEAP BEFORE h_alloc_gc
    p = h->pages_start;
    for(perm_i = 0; perm_i < perms_len; perm_i++) {
      int    class_i = digit_to_int(perm[perm_i]);
      p->promoted    = classes[class_i][0];
      p->new_space   = classes[class_i][1];
      bool   empty   = classes[class_i][2];
      bool   fits    = classes[class_i][3];
      //bool   skip    = classes[class_i][4];
      size_t bump_incr;
      if(!empty) {
        bump_incr = fits ? incr_nempty_fits : incr_nempty_nfits;
      } else {
        bump_incr = incr_empty_fits;
      }
      p->bump = ((char *)(p->bump)) + bump_incr;
      bool should_be_modified = (modified_i == -1) && fits && (p->promoted || !p->new_space);

      if(should_be_modified) {
        modified_i = perm_i;
      }
      p = p->end;
    }

    // RUNNING h_alloc_gc
    void *alloc_addr = h_alloc_gc(h, alloc_size);

    // READING HEAP AND VALIDATING AFTER h_alloc_gc
    p = h->pages_start;
    for(perm_i = 0; perm_i < perms_len; perm_i++) {
      int  class_i   = digit_to_int(perm[perm_i]);
      bool promoted  = classes[class_i][0];
      bool new_space = classes[class_i][1];
      bool empty     = classes[class_i][2];
      bool fits      = classes[class_i][3];
      //bool   skip   = classes[class_i][4];
      bool promoted_after  = classes[class_i][5];
      bool new_space_after = classes[class_i][6];
      size_t bump_incr;

      if(!empty) {
        bump_incr = fits ? incr_nempty_fits : incr_nempty_nfits;
      } else {
        bump_incr = incr_empty_fits;
      }
      void *bump_before = ((char *)(p+1)) + bump_incr;
      if(perm_i == modified_i) {
        CU_ASSERT_TRUE(p->promoted  == promoted_after);
        CU_ASSERT_TRUE(p->new_space == new_space_after);
        CU_ASSERT_TRUE(alloc_addr   == ((char *)(p->bump) - alloc_size));
        CU_ASSERT_TRUE(p->bump      == bump_before + alloc_size);
      } else {
        CU_ASSERT_TRUE(p->promoted  == promoted);
        CU_ASSERT_TRUE(p->new_space == new_space);
        CU_ASSERT_TRUE(p->bump      == bump_before);
      }
      p = p->end;
    }
    h_delete(h);
  }
}

void test_h_alloc(){
  heap_t *h = h_init(16*1024, true, 0.9);
  page_t *p = h->pages_start + h->pageheader_size;
  size_t bytes = 8;
  void *addr = h_alloc(h, bytes);
  void *old_bump;
  while((void*)p < p->end){
    CU_ASSERT_TRUE(addr == old_bump);
    old_bump = p->bump;
    addr = h_alloc(h, bytes);
  }

  p = h->pages_start;
  CU_ASSERT_TRUE(p->new_space == true);

  p = p->end;
  CU_ASSERT_TRUE(p->new_space == false);
}



int main()
{
  CU_initialize_registry();

  // Set up suits register tests
  ////////////////////////////////////////////////////////////////////////////////

  CU_pSuite pages = CU_add_suite("Check if the right amount of pages are created", NULL, NULL);
  CU_add_test(pages, "Check the page amount", test_pages);

  CU_pSuite page_header = CU_add_suite("Check if page values are as expected", NULL, NULL);
  CU_add_test(page_header, "Check initial newspace", test_pages_newspace);
  CU_add_test(page_header, "Check intial promoted", test_pages_promoted);
  CU_add_test(page_header, "Check initial bump pointer", test_pages_bump);
  CU_add_test(page_header, "Check initial end pointer", test_pages_end);

  CU_pSuite page_memory = CU_add_suite("Check if page contents are as expected", NULL, NULL);
  CU_add_test(page_memory, "Check if all pages' bump pointer is correct", test_pages_freememory);

  CU_pSuite gc_algorithms = CU_add_suite("Tests of the gc algorithms", NULL, NULL);
  CU_add_test(gc_algorithms, "Testing h_purge algorithm", test_h_purge);

  CU_pSuite address_accessors_mutators = CU_add_suite("Testing address basded page mutators and accessors", NULL, NULL);
  CU_add_test(address_accessors_mutators, "Testing is_space_newspace", test_is_space_newspace);
  CU_add_test(address_accessors_mutators, "Testing is_addr_promoted", test_is_addr_promoted);
  CU_add_test(address_accessors_mutators, "Testing promote_addr", test_promote_addr);


  CU_pSuite heap_allocation = CU_add_suite("Testing whether memory is allocated correctly", NULL, NULL);
  CU_add_test(heap_allocation, "Tests h_alloc_gc", test_h_alloc_gc);
  CU_add_test(heap_allocation, "Tests h_alloc", test_h_alloc);

  // Actually run tests
  //////////////////////////////////////////////////////////////////////////////
  CU_basic_run_tests();
  
  CU_cleanup_registry();
  /* heap_t *h = h_init(16*1024, true, 0.9); */
  /* h_print_pages(h); */
  return 0;
}
