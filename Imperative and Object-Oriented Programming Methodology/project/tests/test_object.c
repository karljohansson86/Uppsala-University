#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <assert.h>

#include "../src/settings.h"
#include "../src/object.h"
#include "../src/h_init.h"


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

typedef struct {
  void *p1;
  int  i1;
  void *p2;
  char c1;
  char c2;
  void *p3;
  long l1;
  void *p4;
  void *p5;
  void *p6;
  void *p7;
  void *p8;
  void *p9;
  void *p10;
  void *p11;
  void *p12;
  #if WORDSIZE == 8
  void *p13;
  void *p14;
  void *p15;
  void *p16;
  void *p17;
  void *p18;
  void *p19;
  void *p20;
  void *p21;
  void *p22;
  void *p23;
  void *p24;
  int  *i2;
  void *p25;
  void *p26;
  void *p27;
  #endif
} testdata_large;


///////////////// HELPER FUNCTIONS //////////////////

/**
 *  Compare data byte-for-byte between object first and
 *  object second.
 *
 *  \param first   first/original object
 *  \param second  second/clone of object
 *  \param bytes   Total bytes to compare
 *  \return        True if all same, False otherwise
 */
bool verify_data_integrity(char *first, char *second, size_t bytes)
{
  for(size_t i = 0; i < bytes; ++i) {
    if(first[i] != second[i]) {
      return false;
    }
  }
  return true;
}

///////////////// MACRO TESTS /////////////////

void test_read_bit()
{
  unsigned long data = 0x55555555;

  CU_ASSERT_TRUE(O_READ_BIT(data,0) == 1);
  CU_ASSERT_TRUE(O_READ_BIT(data,1) == 0);
  CU_ASSERT_TRUE(O_READ_BIT(data,2) == 1);
  CU_ASSERT_TRUE(O_READ_BIT(data,3) == 0);
}


void test_set_bit()
{
  unsigned long data = 0x55555555;

  CU_ASSERT_TRUE(data == 0x55555555);
  data = O_SET_BIT(data, 1);
  CU_ASSERT_TRUE(data == 0x55555557);
}


void test_unset_bit()
{
  unsigned long data = 0x55555557;

  CU_ASSERT_TRUE(data == 0x55555557);
  data = O_UNSET_BIT(data, 1);
  CU_ASSERT_TRUE(data == 0x55555555);
}

void test_get_type()
{
  unsigned long type1   = 0x00000000;
  unsigned long type2   = 0x00000001;
  unsigned long type3   = 0x00000002;
  unsigned long type4   = 0x00000003;
  unsigned long type1_2 = 0x00000004;

  CU_ASSERT_TRUE(O_HEADER_GET_TYPE(type1)   == 0);
  CU_ASSERT_TRUE(O_HEADER_GET_TYPE(type2)   == 1);
  CU_ASSERT_TRUE(O_HEADER_GET_TYPE(type3)   == 2);
  CU_ASSERT_TRUE(O_HEADER_GET_TYPE(type4)   == 3);
  CU_ASSERT_TRUE(O_HEADER_GET_TYPE(type1_2) == 0);
}

void test_set_type()
{
  unsigned long type   = 0x00000000;

  CU_ASSERT_TRUE(O_HEADER_SET_TYPE(type, 0)   == 0);
  CU_ASSERT_TRUE(O_HEADER_SET_TYPE(type, 1)   == 1);
  CU_ASSERT_TRUE(O_HEADER_SET_TYPE(type, 2)   == 2);
  CU_ASSERT_TRUE(O_HEADER_SET_TYPE(type, 3)   == 3);
  CU_ASSERT_TRUE(O_HEADER_SET_TYPE(type, 4)   == 0);
}

void test_get_ptr()
{
  #if WORDSIZE == 8
  unsigned long header = 0xDEADBEEFDEADBEEF;
  CU_ASSERT_TRUE(O_HEADER_GET_PTR(header) == 0xDEADBEEFDEADBEEC);
  #else
  unsigned long header = 0xDEADBEEF;
  CU_ASSERT_TRUE(O_HEADER_GET_PTR(header) == 0xDEADBEEC);
  #endif
}

void test_set_ptr()
{
  #if WORDSIZE == 8
  unsigned long header = 0x0000000000000003;
  CU_ASSERT_TRUE(O_HEADER_SET_PTR(header, 0xDEADBEEFDEADBEEC) == 0xDEADBEEFDEADBEEF);
  #else
  unsigned long header = 0x00000003;
  CU_ASSERT_TRUE(O_HEADER_SET_PTR(header, 0xDEADBEEC) == 0xDEADBEEF);
  #endif
}


void test_get_data()
{
#if WORDSIZE == 8
  unsigned long header = 0x3FFFFFFFFFFFFFFC;
  CU_ASSERT_TRUE(O_HEADER_GET_DATA(header) == 0x0FFFFFFFFFFFFFFF);
#else
  unsigned long header = 0x3FFFFFFC;
  CU_ASSERT_TRUE(O_HEADER_GET_DATA(header) == 0x0FFFFFFF);
#endif
}

void test_set_data()
{
#if WORDSIZE == 8
  unsigned long header = 0x0000000000000001;
  header = O_HEADER_SET_DATA(header, 0x0FFFFFFFFFFFFFFF);
  unsigned long expected =           0x3FFFFFFFFFFFFFFD;
  CU_ASSERT_TRUE(header == expected);
#else
  unsigned long header = 0x00000001;
  CU_ASSERT_TRUE(O_HEADER_SET_DATA(header, 0x0FFFFFFF) == 0x3FFFFFFD);
#endif
}


// ALLOCATE OBJECT OF SIZE bytes WITH HEADER header
void *allocate_with_header(intptr_t header, size_t bytes)
{
  // Allocate memory
  void *obj = malloc(bytes + sizeof(void *));

  // Set header
  *((intptr_t *)obj) = header;

  // Move pointer to after header
  #if HEAP_GROWTH == UP
  obj = ((intptr_t *)(obj + sizeof(void *)));
  #else
  obj = ((intptr_t *)(obj - sizeof(void *)));
  #endif

  // Return object pointer
  return obj;
}

// FREE ALLOCATED OBJECT WITH HEADER
void free_with_header(void *obj)
{
  // Move pointer to after header
  #if HEAP_GROWTH == UP
  obj = ((intptr_t *)(obj - sizeof(void *)));
  #else
  obj = ((intptr_t *)(obj + sizeof(void *)));
  #endif

  free(obj);
}

///////////////////////////// BASE TESTS //////////////////////////////////
void test_set_object_header()
{
  #if WORDSIZE == 8
  intptr_t header = 0xadbeefdeadbeef01;
  intptr_t new_header = 0xadbeefdeadbeef99;
  #else
  intptr_t header = 0xadbeef01;
  intptr_t new_header = 0xadbeef99;
  #endif
  
  char *obj = allocate_with_header(header, sizeof(char));
  CU_ASSERT_EQUAL(header, o_get_header(obj));
  o_set_header(obj, new_header);
  CU_ASSERT_EQUAL(new_header, o_get_header(obj));
  free_with_header(obj);
}

void test_get_object_header()
{
  #if WORDSIZE == 8
  intptr_t header = 0xdeadbeefdeadbeef;
  #else
  intptr_t header = 0xdeadbeef;
  #endif
  
  char *obj = allocate_with_header(header, sizeof(char));
  CU_ASSERT_EQUAL(header, o_get_header(obj));
  free_with_header(obj);
}

void test_move_object()
{
  heap_t *heap = h_init(1024*10, true, 0.5);

  // Check fraudulent header that ramps up size beyond allocatable
  char *fraud_format = malloc(sizeof(char) * 7);
  strncpy(fraud_format, "99999*\0", 7);
  
  void *fraud_struct = allocate_with_header(O_HEADER_SET_PTR(0, (intptr_t)fraud_format), WORDSIZE);
  void *fraud_raw = allocate_with_header(0xAAAAAAA1, WORDSIZE);
  CU_ASSERT_EQUAL(o_move_object(heap, fraud_struct), fraud_struct);
  CU_ASSERT_EQUAL(o_move_object(heap, fraud_raw), fraud_raw);
  free_with_header(fraud_raw);
  free_with_header(fraud_struct);
  free(fraud_format);

  // Allocate one of each type
  char *obj_raw = o_alloc_raw(heap, sizeof(char) *12);
  obj_raw[0] = 'H';
  obj_raw[1] = 'e';
  obj_raw[2] = 'l';
  obj_raw[3] = 'l';
  obj_raw[4] = 'o';
  obj_raw[5] = ' ';
  obj_raw[6] = 'W';
  obj_raw[7] = 'o';
  obj_raw[8] = 'r';
  obj_raw[9] = 'l';
  obj_raw[10] = 'd';
  
  char *format = malloc(sizeof(char) * 9);
  format[0] = '*';
  format[1] = 'i';
  format[2] = '*';
  format[3] = 'c';
  format[4] = 'c';
  format[5] = '*';
  format[6] = 'l';
  format[7] = '*';
  format[8] = '\0';

  testdata *obj_bitvector = o_alloc_struct(heap, format);
  obj_bitvector->p1 = (void *)0xdead01;
  obj_bitvector->p4 = (void *)0xdead04;

  char *format_large = malloc(sizeof(char) * 32);
  format_large[0] = '*';
  format_large[1] = 'i';
  format_large[2] = '*';
  format_large[3] = 'c';
  format_large[4] = 'c';
  format_large[5] = '*';
  format_large[6] = 'l';
  format_large[7] = '9';
  format_large[8] = '*';
  format_large[9] = '\0';
  #if WORDSIZE == 8
  format_large[9] =  '1';
  format_large[10] = '2';
  format_large[11] = '*';
  format_large[12] = 'i';
  format_large[13] = '3';
  format_large[14] = '*';
  format_large[15] = '\0';
  #endif

  testdata_large *obj_str = o_alloc_struct(heap, format_large);
  obj_str->p1 = (void *)0xdead01;
  #if WORDSIZE == 8
  obj_str->p27 = (void *)0xdead27;
  #else
  obj_str->p9 = (void *)0xdead09;
  #endif

  char *obj_raw_new =          o_move_object(heap, obj_raw);
  testdata *obj_bitvector_new = o_move_object(heap, obj_bitvector);
  testdata_large *obj_str_new = o_move_object(heap, obj_str);

  // Check if actually moved someplace new
  CU_ASSERT_NOT_EQUAL_FATAL(obj_raw, obj_raw_new);
  CU_ASSERT_NOT_EQUAL_FATAL(obj_bitvector, obj_bitvector_new);
  CU_ASSERT_NOT_EQUAL_FATAL(obj_str, obj_str_new);

  // Check if fwd-headers set and if correct
  CU_ASSERT_EQUAL(3, O_HEADER_GET_TYPE(o_get_header(obj_raw)));
  CU_ASSERT_EQUAL(obj_raw_new, (void *)O_HEADER_GET_PTR(o_get_header(obj_raw)));

  CU_ASSERT_EQUAL(3, O_HEADER_GET_TYPE(o_get_header(obj_bitvector)));
  CU_ASSERT_EQUAL(obj_bitvector_new, (void *)O_HEADER_GET_PTR(o_get_header(obj_bitvector)));

  CU_ASSERT_EQUAL(3, O_HEADER_GET_TYPE(o_get_header(obj_str)));
  CU_ASSERT_EQUAL(obj_str_new, (void *)O_HEADER_GET_PTR(o_get_header(obj_str)));

  // Check data integrity
  CU_ASSERT_TRUE(verify_data_integrity((char *)obj_raw, (char *)obj_raw_new, 12));
  CU_ASSERT_TRUE(verify_data_integrity((char *)obj_bitvector, (char *)obj_bitvector_new, sizeof(testdata)));
  CU_ASSERT_TRUE(verify_data_integrity((char *)obj_str, (char *)obj_str_new, sizeof(testdata_large)));
  
  
  free(format);
  free(format_large);
  h_delete(heap);
}




////////////////////////////// STRING HEADER TESTS //////////////////////////////////
void test_calculate_size_from_string()
{
  // Real size
  size_t real = sizeof(testdata);
  
  // Create header
  char *format = malloc(sizeof(char)*9);
  format[0] = '*';
  format[1] = 'i';
  format[2] = '*';
  format[3] = 'c';
  format[4] = 'c';
  format[5] = '*';
  format[6] = 'l';
  format[7] = '*';
  format[8] = '\0';

  intptr_t header = 0;
  header = O_HEADER_SET_TYPE(header, 0);
  header = O_HEADER_SET_PTR(header, (intptr_t)format);

  // Allocate object
  void *obj = allocate_with_header(header, real);

  // Get size of object
  size_t calc = o_get_object_size(obj);

  // Test
  CU_ASSERT_EQUAL(real, calc);

  // Free test object
  free_with_header(obj);
  free(format);
}

void test_pointers_in_string_rep()
{
  char *format = malloc(sizeof(char)*9);

  intptr_t header = 0b00;
  header = O_HEADER_SET_PTR(header, (intptr_t)format);
  void *obj = allocate_with_header(header, 9);

  // No format
  format[0] = '\0';
  CU_ASSERT_EQUAL(o_pointers_in_object(obj), 0);
  
  // Multiple pointers
  format[0] = '*';
  format[1] = 'i';
  format[2] = '*';
  format[3] = 'c';
  format[4] = 'c';
  format[5] = '*';
  format[6] = 'l';
  format[7] = '*';
  format[8] = '\0';
  CU_ASSERT_EQUAL(o_pointers_in_object(obj), 4);

  // No pointers
  format[0] = 'i';
  format[1] = 'i';
  format[2] = 'i';
  format[3] = 'c';
  format[4] = 'c';
  format[5] = 'i';
  format[6] = 'l';
  format[7] = 'i';
  format[8] = '\0';
  CU_ASSERT_EQUAL(o_pointers_in_object(obj), 0);

  // Only pointers
  format[0] = '*';
  format[1] = '*';
  format[2] = '*';
  format[3] = '*';
  format[4] = '*';
  format[5] = '*';
  format[6] = '*';
  format[7] = '*';
  format[8] = '\0';
  CU_ASSERT_EQUAL(o_pointers_in_object(obj), 8);

  // Special format (eg. 3*)
  format[0] = '3';
  format[1] = '*';
  format[2] = '6';
  format[3] = 'i';
  format[4] = '2';
  format[5] = '*';
  format[6] = '*';
  format[7] = 'c';
  format[8] = '\0';
  CU_ASSERT_EQUAL(o_pointers_in_object(obj), 6);

  // Bad format
  format[0] = '*';
  format[1] = 'i';
  format[2] = '*';
  format[3] = 'c';
  format[4] = 'q';
  format[5] = '*';
  format[6] = 'l';
  format[7] = '*';
  format[8] = '\0';
  CU_ASSERT_EQUAL(o_pointers_in_object(obj), 0);

  free(format);
  free_with_header(obj);
}

void test_get_pointer_in_string_rep()
{

  heap_t *heap = h_init(1024*10, true, 0.5);
 
  char format_large[16] = "";
  format_large[0] = '*';
  format_large[1] = 'i';
  format_large[2] = '*';
  format_large[3] = 'c';
  format_large[4] = 'c';
  format_large[5] = '*';
  format_large[6] = 'l';
  format_large[7] = '9';
  format_large[8] = '*';
  format_large[9] = '\0';
  #if WORDSIZE == 8
  format_large[9] =  '1';
  format_large[10] = '2';
  format_large[11] = '*';
  format_large[12] = 'i';
  format_large[13] = '3';
  format_large[14] = '*';
  format_large[15] = '\0';
  #endif


  // Set data
  testdata_large *obj_str = o_alloc_struct(heap, format_large);
  obj_str->p1 = (void *)0xdead01;
  #if WORDSIZE == 8
  obj_str->p27 = (void *)0xdead27;
  #else
  obj_str->p9 = (void *)0xdead09;
  #endif

  CU_ASSERT_TRUE(((intptr_t)*o_get_pointer_in_object(obj_str, 0)) == (intptr_t)0xdead01);
  #if WORDSIZE == 8
  CU_ASSERT_TRUE(((intptr_t)*o_get_pointer_in_object(obj_str, 26)) == (intptr_t)0xdead27);
  #else
  CU_ASSERT_TRUE(((intptr_t)*o_get_pointer_in_object(obj_str, 8)) == (intptr_t)0xdead09);
  #endif
  CU_ASSERT_PTR_NULL(o_get_pointer_in_object(obj_str, 99));
  CU_ASSERT_PTR_NULL(o_get_pointer_in_object(obj_str, -2));

  h_delete(heap);
}

////////////////////////////// COMPACT HEADER //////////////////////////////////
void test_get_size_raw()
{
  heap_t *heap = h_init(1024*10, true, 0.5);
  void *small = o_alloc_raw(heap, sizeof(testdata));
  void *large = o_alloc_raw(heap, sizeof(testdata_large));
  void *dbl   = o_alloc_raw(heap, sizeof(testdata_large) + sizeof(testdata));

  CU_ASSERT_EQUAL(o_get_object_size(NULL), 0);
  CU_ASSERT_EQUAL(o_get_object_size(small), sizeof(testdata));
  CU_ASSERT_EQUAL(o_get_object_size(large), sizeof(testdata_large));
  CU_ASSERT_EQUAL(o_get_object_size(dbl), sizeof(testdata_large) + sizeof(testdata));

  h_delete(heap);
}

void test_get_size_bitvector()
{
  heap_t *heap = h_init(1024*10, true, 0.5);

  char *format = "*i*2c*l*";
  testdata *data = o_alloc_struct(heap, format);
  
  CU_ASSERT_EQUAL(NULL, 0);
  CU_ASSERT_EQUAL(o_get_object_size(data), sizeof(testdata));
  
  h_delete(heap);
}

void test_pointers_in_bitvector()
{
  heap_t *heap = h_init(1024*10, true, 0.5);

  char *format = "*i*2c*l*";
  testdata *data = o_alloc_struct(heap, format);
  
  CU_ASSERT_EQUAL(NULL, 0);
  CU_ASSERT_EQUAL(o_pointers_in_object(data), 4);
  
  h_delete(heap);
}

void test_get_pointer_in_bitvector() {

  heap_t *heap = h_init(1024*10, true, 0.5);

  char *format = malloc(sizeof(char)*9);
  format[0] = '*';
  format[1] = 'i';
  format[2] = '*';
  format[3] = 'c';
  format[4] = 'c';
  format[5] = '*';
  format[6] = 'l';
  format[7] = '*';
  format[8] = '\0';

  void *obj = o_alloc_struct(heap, format);
  CU_ASSERT_PTR_NOT_NULL_FATAL(obj);

  // Set data
  testdata *td = obj;
  #if WORDSIZE == 8
  td->p1 = (void *)0xADBEEFDEADBEEF01;
  td->p2 = (void *)0xADBEEFDEADBEEF02;
  td->p3 = (void *)0xADBEEFDEADBEEF03;
  td->p4 = (void *)0xADBEEFDEADBEEF04;
  #else
  td->p1 = (void *)0xADBEEF01;
  td->p2 = (void *)0xADBEEF02;
  td->p3 = (void *)0xADBEEF03;
  td->p4 = (void *)0xADBEEF04;
  #endif
  td->i1 = 12345;
  td->c1 = 'A';
  td->c2 = 'B';
  td->l1 = 1234567890;

  #if WORDSIZE == 8
  CU_ASSERT_TRUE(((intptr_t)*o_get_pointer_in_object(obj, 0)) == (intptr_t)0xADBEEFDEADBEEF01);
  CU_ASSERT_TRUE(((intptr_t)*o_get_pointer_in_object(obj, 1)) == (intptr_t)0xADBEEFDEADBEEF02);
  CU_ASSERT_TRUE(((intptr_t)*o_get_pointer_in_object(obj, 2)) == (intptr_t)0xADBEEFDEADBEEF03);
  CU_ASSERT_TRUE(((intptr_t)*o_get_pointer_in_object(obj, 3)) == (intptr_t)0xADBEEFDEADBEEF04);
  #else
  CU_ASSERT_TRUE(((intptr_t)*o_get_pointer_in_object(obj, 0)) == (intptr_t)0xADBEEF01);
  CU_ASSERT_TRUE(((intptr_t)*o_get_pointer_in_object(obj, 1)) == (intptr_t)0xADBEEF02);
  CU_ASSERT_TRUE(((intptr_t)*o_get_pointer_in_object(obj, 2)) == (intptr_t)0xADBEEF03);
  CU_ASSERT_TRUE(((intptr_t)*o_get_pointer_in_object(obj, 3)) == (intptr_t)0xADBEEF04);
  #endif
  CU_ASSERT_PTR_NULL(o_get_pointer_in_object(obj, 4));

  free(format);
  h_delete(heap);
}

////////////////////////////// ALLOCATION TESTS //////////////////////////////////

void test_alloc_raw()
{
  heap_t *heap = h_init(1024*10, true, 0.5);
  testdata *my_object = o_alloc_raw(heap, sizeof(testdata));
  CU_ASSERT_PTR_NOT_NULL_FATAL(my_object);

  // Test allocating a too large object
  void *too_large = o_alloc_raw(heap, 1024 * 1024);
  CU_ASSERT_PTR_NULL(too_large);

  my_object->p1 = (void *)0xDEAD01;
  my_object->p4 = (void *)0xDEAD04;

  // Check that object is allocated with correct header type
  intptr_t header = o_get_header(my_object);
  CU_ASSERT_EQUAL(O_HEADER_GET_TYPE(header), 1);

  // Check that the compact header type is correct
  intptr_t data = O_HEADER_GET_DATA(header);
  CU_ASSERT_EQUAL(O_HEADER_GET_TYPE(data), 0);

  // Extract size from header and verify integrity
  size_t stored_size = (size_t)(O_HEADER_GET_DATA(data));
  CU_ASSERT_EQUAL(stored_size, sizeof(testdata));

  // Test data integrity
  CU_ASSERT_EQUAL(my_object->p1, (void *)0xDEAD01);
  CU_ASSERT_EQUAL(my_object->p4, (void *)0xDEAD04);

  h_delete(heap);
}

void test_alloc_struct_bitvector()
{
  heap_t *heap = h_init(1024*10, true, 0.5);

  // Create header
  char *format = malloc(sizeof(char)*9);
  format[0] = '*';
  format[1] = 'i';
  format[2] = '*';
  format[3] = 'c';
  format[4] = 'c';
  format[5] = '*';
  format[6] = 'l';
  format[7] = '*';
  format[8] = '\0';
  
  testdata *my_struct = o_alloc_struct(heap, format);

  CU_ASSERT_PTR_NOT_NULL_FATAL(my_struct);
  
  my_struct->p1 = (void *)0xDEAD01;
  my_struct->p4 = (void *)0xDEAD04;

  // Check that object is allocated with correct header type
  intptr_t header = o_get_header(my_struct);
  CU_ASSERT_EQUAL(O_HEADER_GET_TYPE(header), 1);

  // Check that the compact header type is correct
  intptr_t data = O_HEADER_GET_DATA(header);
  CU_ASSERT_EQUAL_FATAL(O_HEADER_GET_TYPE(data), 1);

  // Extract compact type from header
  CU_ASSERT_EQUAL_FATAL(1, O_HEADER_GET_TYPE(data));

  // Extract bitvector and test if same as expected
  #if WORDSIZE == 8
  CU_ASSERT_EQUAL(0b11101110111011, O_HEADER_GET_DATA(data));
  #else
  CU_ASSERT_EQUAL(0b11011101110111, O_HEADER_GET_DATA(data));
  #endif
  
  // Test data integrity
  CU_ASSERT_EQUAL(my_struct->p1, (void *)0xDEAD01);
  CU_ASSERT_EQUAL(my_struct->p4, (void *)0xDEAD04);

  free(format);
  h_delete(heap);
}


void test_alloc_struct_stringrep_on_heap() {
  heap_t *heap = h_init(1024*10, true, 0.5);

  // Create header
  #if WORDSIZE == 4
  char *format = malloc(sizeof(char)*10);
  #else
  char *format = malloc(sizeof(char)*16);
  #endif

  format[0] = '*';
  format[1] = 'i';
  format[2] = '*';
  format[3] = 'c';
  format[4] = 'c';
  format[5] = '*';
  format[6] = 'l';
  format[7] = '9';
  format[8] = '*';
  format[9] = '\0';
#if WORDSIZE == 8
  format[9] =  '1';
  format[10] = '2';
  format[11] = '*';
  format[12] = 'i';
  format[13] = '3';
  format[14] = '*';
  format[15] = '\0';
#endif
  
  testdata_large *my_struct = o_alloc_struct(heap, format);

  CU_ASSERT_PTR_NOT_NULL_FATAL(my_struct);
  
  // Check that object is allocated with correct header type
  intptr_t header = o_get_header(my_struct);
  CU_ASSERT_EQUAL_FATAL(O_HEADER_GET_TYPE(header), 0);

  // Extract string from header and compare
  CU_ASSERT_TRUE(strncmp(format, (char *)O_HEADER_GET_PTR(header), strlen(format)) == 0);

  free(format);
  h_delete(heap);
}

void test_alloc_struct_stringrep()
{
  heap_t *heap = h_init(1024*10, true, 0.5);

  // Test allocating a too large object
  void *too_large = o_alloc_struct(heap, "66666*");
  CU_ASSERT_PTR_NULL(too_large);

  // Create header
  #if WORDSIZE == 4
  char *format = malloc(sizeof(char)*10);
  #else
  char *format = malloc(sizeof(char)*16);
  #endif

  format[0] = '*';
  format[1] = 'i';
  format[2] = '*';
  format[3] = 'c';
  format[4] = 'c';
  format[5] = '*';
  format[6] = 'l';
  format[7] = '9';
  format[8] = '*';
  format[9] = '\0';
#if WORDSIZE == 8
  format[9] =  '1';
  format[10] = '2';
  format[11] = '*';
  format[12] = 'i';
  format[13] = '3';
  format[14] = '*';
  format[15] = '\0';
#endif

  testdata_large *my_struct = o_alloc_struct(heap, format);

  CU_ASSERT_PTR_NOT_NULL_FATAL(my_struct);

  // Check that object is allocated with correct header type
  intptr_t header = o_get_header(my_struct);
  CU_ASSERT_EQUAL_FATAL(O_HEADER_GET_TYPE(header), 0);

  
  my_struct->p1 = (void *)0xDEAD01;
  #if WORDSIZE == 8
  my_struct->p27 = (void *)0xDEAD27;
  #else
  my_struct->p12 = (void *)0xDEAD12;
  #endif
  
  // Test data integrity
  CU_ASSERT_EQUAL(my_struct->p1, (void *)0xDEAD01);
  #if WORDSIZE == 8
  CU_ASSERT_EQUAL(my_struct->p27, (void *)0xDEAD27);
  #else
  CU_ASSERT_EQUAL(my_struct->p12, (void *)0xDEAD12);
  #endif

  free(format);
  h_delete(heap);
}


int main()
{  
  CU_initialize_registry();
  // Set up suits register tests
  ////////////////////////////////////////////////////////////////////////////////

  // MACROS
  CU_pSuite macros = CU_add_suite("Macros", NULL, NULL);
  CU_add_test(macros, "Read individual bits", test_read_bit);
  CU_add_test(macros, "Write individual bits", test_set_bit);
  CU_add_test(macros, "Get header type", test_get_type);
  CU_add_test(macros, "Set header type", test_set_type);
  CU_add_test(macros, "Get pointer in header", test_get_ptr);
  CU_add_test(macros, "Set pointer in header", test_set_ptr);
  CU_add_test(macros, "Get data in header", test_get_data);
  CU_add_test(macros, "Set data in header", test_set_data);

  // ALLOCATION OF OBJECTS
  CU_pSuite allocation = CU_add_suite("Allocation", NULL, NULL);
  CU_add_test(allocation, "o_alloc_raw", test_alloc_raw);
  CU_add_test(allocation, "o_alloc_struct_bitvector", test_alloc_struct_bitvector);
  CU_add_test(allocation, "layout string correctly stored on heap?", test_alloc_struct_stringrep_on_heap);
  CU_add_test(allocation, "o_alloc_struct_stringrep", test_alloc_struct_stringrep);

  // BASIC/GENERAL OBJECT HANDLING
  CU_pSuite base = CU_add_suite("Base tests", NULL, NULL);
  CU_add_test(base, "Get header from object", test_get_object_header);
  CU_add_test(base, "Set header of object", test_set_object_header);
  CU_add_test(base, "Move object in memory", test_move_object);

  // STRING REP BASED OBJECTS
  CU_pSuite string_header = CU_add_suite("String representation", NULL, NULL);
  CU_add_test(string_header,
    "Calculate size of string representation", test_calculate_size_from_string);
  CU_add_test(string_header,
    "Calculate pointers in object from string representation",
    test_pointers_in_string_rep);
  CU_add_test(string_header,
    "Get specific pointer from string rep oobject",
    test_get_pointer_in_string_rep);
    
  // COMPACT HEADER OBJECTS
  CU_pSuite compact_header = CU_add_suite("Compact header", NULL, NULL);
  CU_add_test(compact_header, "Get size of object by bitvector", test_get_size_bitvector);
  CU_add_test(compact_header, "Calculate pointers in object from bitvector", test_pointers_in_bitvector);
  CU_add_test(compact_header, "Get pointer in object from bitvector", test_get_pointer_in_bitvector);
  CU_add_test(compact_header, "Get size of raw object", test_get_size_raw);
  
  // Actually run tests
  //////////////////////////////////////////////////////////////////////////////
  CU_basic_run_tests();

  CU_cleanup_registry();
  return 0;
}
