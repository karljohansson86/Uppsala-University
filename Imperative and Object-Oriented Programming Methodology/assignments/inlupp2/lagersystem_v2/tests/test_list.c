#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>

#include "../list.h"

// Helper functions
list_t* _test_populateList(int length, int *elements) {
  list_t* l = list_new();
  for (int i = 0; i < length; ++i) {
    list_append(l, elements[i]);
  }
  return l;
}

void test_list_creation(){
  list_t *l = list_new();
  CU_ASSERT_PTR_NOT_NULL(l);
  CU_ASSERT_TRUE(list_length(l) == 0);
  list_destroy(l);
}

void test_list_height(){
  int i[10] = {1,2,3,4,5,6,7,8,9,10};
  list_t* l = list_new();
  CU_ASSERT_TRUE(list_length(l) == 0);
  list_append(l,&i[0]);
  CU_ASSERT_TRUE(list_length(l) == 1);
  list_append(l,&i[1]);
  CU_ASSERT_TRUE(list_length(l) == 2);
  list_append(l,&i[2]);
  list_append(l,&i[3]);
  list_append(l,&i[4]);
  list_append(l,&i[5]);
  list_append(l,&i[6]);
  list_append(l,&i[7]);
  list_append(l,&i[8]);
  list_append(l,&i[9]);
  CU_ASSERT_TRUE(list_length(l) == 10);
  list_destroy(l);
}

void test_list_append(){
  int el[10] = {1,2,3,4,5,6,7,8,9,10};
  list_t* l = _test_populateList(10, el);
  int i1 = 567;
  int i2 = 988;
  
  list_append(l, &i1);
  int* g1 = list_last(l);
  CU_ASSERT_PTR_NOT_NULL(g1);
  CU_ASSERT_TRUE(*g1 == i1);

  list_append(l, &i2);
  int* g2 = list_last(l);
  CU_ASSERT_PTR_NOT_NULL(g2);
  CU_ASSERT_TRUE(*g2 == i2);

  g1 = list_get(l,10);
  CU_ASSERT_PTR_NOT_NULL(g1);
  CU_ASSERT_TRUE(*g1 == i1);
  list_destroy(l);
}

void test_list_append_empty(){
  list_t* l = list_new();
  int i = 346;
  list_append(l,&i);
  CU_ASSERT_TRUE(list_length(l) == 1);
  int* g1 = list_first(l);
  int* g2 = list_last(l);
  CU_ASSERT_PTR_NOT_NULL(g1);
  CU_ASSERT_TRUE(*g1 == i && g1 == &i);
  CU_ASSERT_TRUE(*g2 == i && g2 == &i);
  list_destroy(l);
}

void test_list_prepend(){
  int el[10] = {1,2,3,4,5,6,7,8,9,10};
  list_t* l = _test_populateList(10, el);
  int i1 = 567;
  int i2 = 988;
  
  list_prepend(l, &i1);
  int* g1 = list_first(l);
  CU_ASSERT_PTR_NOT_NULL(g1);
  CU_ASSERT_TRUE(*g1 == i1);

  list_prepend(l, &i2);
  int* g2 = list_first(l);
  CU_ASSERT_PTR_NOT_NULL(g2);
  CU_ASSERT_TRUE(*g2 == i2);

  g1 = list_get(l,1);
  CU_ASSERT_PTR_NOT_NULL(g1);
  CU_ASSERT_TRUE(*g1 == i1);
  list_destroy(l);
}

void test_list_prepend_empty(){
  list_t* l = list_new();
  int i = 346;
  list_prepend(l,&i);
  CU_ASSERT_TRUE(list_length(l) == 1);
  int* g1 = list_first(l);
  int* g2 = list_last(l);
  CU_ASSERT_PTR_NOT_NULL(g1);
  CU_ASSERT_TRUE(*g1 == i && g1 == &i);
  CU_ASSERT_TRUE(*g2 == i && g2 == &i);
  list_destroy(l);
}


void test_list_insert() {
  
}

void test_list_insert_empty() {
  list_t* l = list_new();
  int i = 346;
  CU_ASSERT_FALSE(list_insert(l, 40, &i));
  CU_ASSERT_TRUE(list_insert(l,0,&i));
  CU_ASSERT_TRUE(list_length(l) == 1);
  int* g1 = list_first(l);
  int* g2 = list_last(l);
  CU_ASSERT_PTR_NOT_NULL(g1);
  CU_ASSERT_TRUE(*g1 == i && g1 == &i);
  CU_ASSERT_TRUE(*g2 == i && g2 == &i);
  list_destroy(l);
}






int main(int argc, char *argv[])
{
  CU_initialize_registry();
  // Set up suits register tests
  ////////////////////////////////////////////////////////////////////////////////
  CU_pSuite creation = CU_add_suite("Creation and height", NULL, NULL);
  CU_add_test(creation, "Create a linked list", test_list_creation);
  CU_add_test(creation, "Measure list growth", test_list_height);

  //  CU_pSuite read  = CU_add_suite("Test reading elements from list", NULL, NULL);
  
  CU_pSuite inserts = CU_add_suite("Inserts of elements", NULL, NULL);
  CU_add_test(inserts, "Appending element", test_list_append);
  CU_add_test(inserts, "Appending element", test_list_append_empty);
  CU_add_test(inserts, "Prepending element", test_list_prepend);
  CU_add_test(inserts, "Prepending element", test_list_prepend_empty);
  CU_add_test(inserts, "Insert at specified index", test_list_insert);

  // Actually run tests
  //////////////////////////////////////////////////////////////////////////////
  CU_basic_run_tests();
  
  CU_cleanup_registry();
  return 0;
}
