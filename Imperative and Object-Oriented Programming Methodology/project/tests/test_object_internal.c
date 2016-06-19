#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>

#include "../src/object.h"
#include "../src/object.c"

typedef struct {
  void *p1;
  int  i1;
  void *p2;
  char c1;
  char c2;
  void *p3;
  long l1;
  void *p4;
} testdata;


void test_size_from_char() { 
  CU_ASSERT_TRUE( o_size_from_char('*') == sizeof(void *));
  CU_ASSERT_TRUE( o_size_from_char('i') == sizeof(int));
  CU_ASSERT_TRUE( o_size_from_char('f') == sizeof(float));
  CU_ASSERT_TRUE( o_size_from_char('c') == sizeof(char));
  CU_ASSERT_TRUE( o_size_from_char('l') == sizeof(long));
  CU_ASSERT_TRUE( o_size_from_char('d') == sizeof(double));
  CU_ASSERT_TRUE( o_size_from_char(' ') == 0);
  CU_ASSERT_TRUE( o_size_from_char('6') == 0);
  CU_ASSERT_TRUE( o_size_from_char('x') == 0);
}

void test_size_from_bits() {
  CU_ASSERT_TRUE( o_size_from_bits(3) == sizeof(void *));
  CU_ASSERT_TRUE( o_size_from_bits(2) == 8);
  CU_ASSERT_TRUE( o_size_from_bits(1) == 4);
  CU_ASSERT_TRUE( o_size_from_bits('c') == 0);
  CU_ASSERT_TRUE( o_size_from_bits(' ') == 0);
  CU_ASSERT_TRUE( o_size_from_bits(6) == 0);
}




void test_pointers_in_bitvector() {
  
  CU_ASSERT_TRUE(o_pointers_in_bitvector(0b00000000000000000000000000000000) == 0);
  CU_ASSERT_TRUE(o_pointers_in_bitvector(0b00001111111111111111111111111111) == 14);
  CU_ASSERT_TRUE(o_pointers_in_bitvector(0b11000000000000000000000000000011) == 1);
  CU_ASSERT_TRUE(o_pointers_in_bitvector(0b01100000000000000000000000000110) == 0);
  CU_ASSERT_TRUE(o_pointers_in_bitvector(0b01110000000000000000000000001110) == 1);
  CU_ASSERT_TRUE(o_pointers_in_bitvector(0b00000000000011100000000000000000) == 0);
  CU_ASSERT_TRUE(o_pointers_in_bitvector(0b01111000000000000000000000011110) == 1);
  CU_ASSERT_TRUE(o_pointers_in_bitvector(0b0001111111111111111111111111110) == 13);
    
}

void test_ctoi() {
  
  CU_ASSERT_TRUE(o_ctoi('0') == 0);
  CU_ASSERT_TRUE(o_ctoi('1') == 1);
  CU_ASSERT_TRUE(o_ctoi('2') == 2);
  CU_ASSERT_TRUE(o_ctoi('3') == 3);
  CU_ASSERT_TRUE(o_ctoi('4') == 4);
  CU_ASSERT_TRUE(o_ctoi('5') == 5);
  CU_ASSERT_TRUE(o_ctoi('6') == 6);
  CU_ASSERT_TRUE(o_ctoi('7') == 7);
  CU_ASSERT_TRUE(o_ctoi('8') == 8);
  CU_ASSERT_TRUE(o_ctoi('9') == 9);
  CU_ASSERT_TRUE(o_ctoi('t') == 0);
}

void test_is_numeric() {
  
  CU_ASSERT_TRUE(o_is_numeric('0') == true);
  CU_ASSERT_TRUE(o_is_numeric('1') == true);
  CU_ASSERT_TRUE(o_is_numeric('2') == true);
  CU_ASSERT_TRUE(o_is_numeric('3') == true);
  CU_ASSERT_TRUE(o_is_numeric('4') == true);
  CU_ASSERT_TRUE(o_is_numeric('5') == true);
  CU_ASSERT_TRUE(o_is_numeric('6') == true);
  CU_ASSERT_TRUE(o_is_numeric('7') == true);
  CU_ASSERT_TRUE(o_is_numeric('8') == true);
  CU_ASSERT_TRUE(o_is_numeric('9') == true);
  CU_ASSERT_TRUE(o_is_numeric('t') == false);
}


void test_advance_pointer() {
  
  intptr_t i = 456;
  void *ptr = &i;
  
  #if HEAP_GROWTH == UP
  CU_ASSERT_TRUE(o_advance_pointer(ptr) == ptr + sizeof(void *));
  #else
  CU_ASSERT_TRUE(o_advance_pointer(ptr) == ptr - sizeof(void *));  
  #endif
}

void test_reverse_pointer() {
  
  intptr_t i = 456;
  void *ptr = &i;
   
  #if HEAP_GROWTH == UP
  CU_ASSERT_TRUE(o_reverse_pointer(ptr) == ptr - sizeof(void *));
  #else
  CU_ASSERT_TRUE(o_reverse_pointer(ptr) == ptr + sizeof(void *));  
  #endif
}

void test_alignment_from_char() {
  
  CU_ASSERT_TRUE(o_alignment_from_char('*') == ALIGNMENT_POINTER);
  CU_ASSERT_TRUE(o_alignment_from_char('i') == ALIGNMENT_INT);
  CU_ASSERT_TRUE(o_alignment_from_char('f') == ALIGNMENT_FLOAT);
  CU_ASSERT_TRUE(o_alignment_from_char('c') == ALIGNMENT_CHAR);
  CU_ASSERT_TRUE(o_alignment_from_char('l') == ALIGNMENT_LONG);
  CU_ASSERT_TRUE(o_alignment_from_char('d') == ALIGNMENT_DOUBLE);
  CU_ASSERT_TRUE(o_alignment_from_char('x') == 0);
  CU_ASSERT_TRUE(o_alignment_from_char('1') == 0);

}

void test_pointers_in_string_rep() {

  //TODO
  CU_ASSERT_TRUE(true);

}

void test_size_from_string_rep() {

  //TODO
  CU_ASSERT_TRUE(true);

}

void test_size_from_bitvector() {

  //TODO
  CU_ASSERT_TRUE(true);

}

void test_get_pointer_from_string_rep() {

  //TODO
  CU_ASSERT_TRUE(true);

}


void test_get_pointer_from_bitvector() {

  //TODO
  CU_ASSERT_TRUE(true);

}


void test_get_new_offset() {

  //TODO
  CU_ASSERT_TRUE(true);

}


void test_string_format_to_bitvector() {
  
  intptr_t bitvector = o_string_format_to_bitvector("*i*cc*l*");

  #if WORDSIZE == 8
  CU_ASSERT_EQUAL(bitvector, (intptr_t)0b11101110111011);
  #else
  CU_ASSERT_EQUAL(bitvector, (intptr_t)0b11011101110111);
  #endif
}


void test_write_zeros() {

  //TODO
  CU_ASSERT_TRUE(true);
  
}




int main() {
  CU_initialize_registry();

  CU_pSuite internal_functions = CU_add_suite("Internal objects functions", NULL, NULL);
  CU_add_test(internal_functions, "Get correct size from char:", test_size_from_char);
  CU_add_test(internal_functions, "Get correct size from bits:", test_size_from_bits);
  CU_add_test(internal_functions, "Get pointers in bitvector:", test_pointers_in_bitvector);
  CU_add_test(internal_functions, "Char to int:", test_ctoi);
  CU_add_test(internal_functions, "Char is numeric:", test_is_numeric);
  CU_add_test(internal_functions, "Advance pointer:", test_advance_pointer);
  CU_add_test(internal_functions, "Reverse pointer:", test_reverse_pointer);
  CU_add_test(internal_functions, "Alignment from char:", test_alignment_from_char);
  
  //TODO
  CU_add_test(internal_functions, "Pointers in string rep:", test_pointers_in_string_rep);
  CU_add_test(internal_functions, "Size from string rep:", test_size_from_string_rep);
  CU_add_test(internal_functions, "Size from bitvector:", test_size_from_bitvector);
  CU_add_test(internal_functions, "Get ptrs from string rep:", test_get_pointer_from_string_rep);
  CU_add_test(internal_functions, "Get ptrs from bitvector:", test_get_pointer_from_bitvector);
  CU_add_test(internal_functions, "Get new offset:", test_get_new_offset);
  CU_add_test(internal_functions, "String format to bitvector:", test_string_format_to_bitvector);
  CU_add_test(internal_functions, "Write zeros:", test_write_zeros);
  


  CU_basic_run_tests();

  CU_cleanup_registry();
  return 0;
}
