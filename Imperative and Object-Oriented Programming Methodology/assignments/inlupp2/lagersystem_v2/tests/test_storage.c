#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>

#include "../storage.h"

void createStorage() {
  storage_t* s = storage_new();
  CU_ASSERT_PTR_NOT_NULL_FATAL(s);
  storage_free(s);
}





int main(int argc, char *argv[])
{
  puts("Storage Test!\n\n");

  CU_initialize_registry();

  // Set up suits register tests
  ////////////////////////////////////////////////////////////////////////////////
  CU_pSuite creation = CU_add_suite("Creation and height", NULL, NULL);
  CU_add_test(creation, "Create and Free a storage", createStorage);

  //  CU_pSuite read  = CU_add_suite("Test reading elements from list", NULL, NULL);
  
  //CU_pSuite inserts = CU_add_suite("Inserts of elements", NULL, NULL);

  // Actually run tests
  //////////////////////////////////////////////////////////////////////////////
  CU_basic_run_tests();
  
  CU_cleanup_registry();
  return 0;
}
