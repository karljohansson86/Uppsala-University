#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "h_init.h"
#include "object.h"


////////////////// INTERNAL PROTOTYPES //////////////////
/**
 *  Returns size of satatype represented by character.
 *  Valid characters are:
 *
 *  | CHAR  |  REPRESENTS
 *  |-------|----------------
 *  |   i   |  int
 *  |   l   |  long
 *  |   f   |  float
 *  |   c   |  char
 *  |   d   |  double
 *  |   *   |  void *
 *
 *  \param   c  Char to get size of
 *  \return  sizeof() datatype represented by char
 */
size_t o_size_from_char(char c);

/**
 *  Returns size of satatype represented by character.
 *  Valid characters are:
 *
 *   | VALUE |  DESCRIPTION               |
 *   |-------|----------------------------|
 *   | 0b00  | N/A                        |
 *   | 0b01  | 4 byte datatype            |
 *   | 0b10  | 8 byte datatype            |
 *   | 0b11  | sizeof(vois *)             |
 *
 *  \param   bits Bit-representation to get size of
 *  \return  sizeof() datatype represented by bits
 */
size_t o_size_from_bits(int bits);

/**
 *  Counts pointers notet in a format-string
 *
 *  \param   format  Format string describing object
 *  \return  Number of pointers in format string
 */
size_t o_pointers_in_string_rep(char *format);

/**
 *  Counts pointers in compact header bit-vector
 *
 *  \param   intptr_t  Bit-vector to use
 *  \return  Number of pointers in bit-vector
 */
size_t o_pointers_in_bitvector(intptr_t header_data);

/**
 *  Calculates size of object described by bit-vector
 *
 *  \param   intptr_t  Bit-vector to use
 *  \return  Size of object described by bit-vector
 */
size_t o_size_from_bitvector(intptr_t header_data);

/**
 *  Calculates size of object described by string format
 *
 *  \param   format  String format describing object
 *  \return  Number of pointers in bit-vector
 */
size_t o_size_from_string_rep(char *format);

/**
 *  Returns pointer at given index if exists
 *
 *  \param   header_data  bitvector from header
 *  \param   object Object from which to extract pointer
 *  \param   index  index of pointer
 *  \return  Number of pointers in bit-vector
 */
void **o_get_pointer_from_bitvector(intptr_t header_data, void *object, size_t index);

/**
 *  Returns pointer at given index if exists
 *
 *  \param   format string representation of object
 *  \param   object Object from whih to extract pointer
 *  \param   index  index of pointer
 *  \return  Number of pointers in bit-vector
 */
void **o_get_pointer_from_string_rep(char *format, void *object, size_t index);

/**
 *  Get alignment of datatype from char representation
 *
 *  \param  c  Char representation of data type
 *  \return Alignment of datatype
 */
long o_alignment_from_char(char c);

/**
 *  Get new offset of data structure when datatype (represented
 *  by char c) is added. Calculates padding and adds padding to
 *  current offset (bytes from starting address, also viable as
 *  size of object) and byte size of data type if padding_only
 *  is set to false.
 *
 *  \param  bytes         Byte size of datatype
 *  \param  alignment     Alignment (in bytes) in memory of datatype
 *  \param  offset        Current offset of data structure (starts on 0)
 *  \param  padding_only  If only padding should be added to offset
 *  \return New offset of data structure
 */
long o_get_new_offset(long bytes, long alignment, long offset, bool padding_only);

/**
 *  Cinvert numerical character (0-9) to integer
 *
 *  \param  c  Character to convert
 *  \return Integer representation of char (0-9)
 */
unsigned int o_ctoi(char c);

/**
 *  Test if char is numerical (0-9)
 *
 *  \param  c  Character to test
 *  \return True if numerical, False otherwise
 */
bool o_is_numeric(char c);

/**
 *  Converts string format to bitvector notation
 *
 *  \param  format String format of object
 *  \return Bit vector format of object
 */
intptr_t o_string_format_to_bitvector(char *format);

/**
 *  Test if object is small enough to be represented as a bitvector
 *
 *  \param  format String format of object
 *  \return True if it can be represented as bit vector, False otherwise
 */
bool o_fits_in_vector(char *format);

/**
 * Writing zeros
 *
 * \param p pointer for writing zeros
 * \param bytes the size (in bytes) to write
 */
void o_write_zeros(void *p, size_t bytes);

/**
 * Advance pointer to beyond object header
 *
 * \param ptr Pointer to advance
 * \param Pointer advanced beyond object header
 */
void *o_advance_pointer(void *ptr);

/**
 * Reverse pointer to point at object header
 *
 * \param ptr Pointer to reverse
 * \param Pointer to object header
 */
void *o_reverse_pointer(void *ptr);

////////////////// FUNCTION IMPLEMENTATIONS //////////////////

intptr_t o_get_header(void *ptr)
{
  if (ptr == NULL) {
    return 0;
  }

  #if HEAP_GROWTH == UP
  return *((intptr_t *)o_reverse_pointer(ptr));
  #else
  return *((intptr_t *)o_reverse_pointer(ptr);
  #endif
}

void o_set_header(void *obj, intptr_t header)
{
  *(intptr_t *)o_reverse_pointer(obj) = header;
}

void *o_advance_pointer(void *ptr)
{
  if (ptr == NULL) {
    return NULL;
  }

  #if HEAP_GROWTH == UP
  return (void *)((intptr_t)ptr + sizeof(void *));
  #else
  return (void *)((intptr_t)ptr - sizeof(void *));
  #endif
}

void *o_reverse_pointer(void *ptr)
{
  if (ptr == NULL) {
    return NULL;
  }

  #if HEAP_GROWTH == UP
  return (void *)((intptr_t)ptr - sizeof(void *));
  #else
  return (void *)((intptr_t)ptr + sizeof(void *));
  #endif
}

void *o_alloc_struct(heap_t *h, char *layout)
{
  size_t struct_size = o_size_from_string_rep(layout);

  // TODO: IMPORTANT: Check that size of object is greater than 0 and less than MAX_OBJ_SIZE
  // If so, return obj-pointer as is without copy
  if(struct_size == 0 || (struct_size + sizeof(void *)) > get_page_mem(h)) {
    return NULL;
  }

  void *location = h_alloc(h, struct_size + sizeof(void *));

  if (location == NULL) {
    return NULL;
  }

  intptr_t header = 0;
  if (o_fits_in_vector(layout)) {
    header = o_string_format_to_bitvector(layout);
    header <<= 2;
    header = O_HEADER_SET_TYPE(header, 1);
    header <<= 2;
    header = O_HEADER_SET_TYPE(header, 1);
  }
  else {
    // Save string to heap and use string-rep header
    size_t layout_length = strlen(layout);
    char *stored_string = o_alloc_raw(h, layout_length + 1);
    if(stored_string == NULL) {
      // Unable to allocate space
      return NULL;
    }
    // Copy string to newly allocated space
    strncpy(stored_string, layout, layout_length);

    header = O_HEADER_SET_TYPE(header, 0);
    header = O_HEADER_SET_PTR(header, (intptr_t)stored_string);
  }

  // Write header
  *(intptr_t *)location = header;
  // Advance pointer
  location = o_advance_pointer(location);
  // Zero-out memory
  o_write_zeros(location, struct_size);

  return location;
}

void  o_write_zeros(void *p, size_t bytes)
{
  char *cursor = p;
  for (size_t i = 0; i<bytes; ++i) {
    *cursor = '\0';
    ++cursor;
  }
}

void *o_alloc_raw(heap_t *h, size_t bytes)
{
  // TODO: IMPORTANT: Check that size of object is greater than 0 and less than MAX_OBJ_SIZE
  // If so, return obj-pointer as is without copy
  if(bytes == 0 || (bytes + sizeof(void *)) > get_page_mem(h)) {
    return NULL;
  }

  // Calculate bytes with header and correct padding
  size_t bytes_total = o_get_new_offset(O_ZERO_BYTES, WORDSIZE, bytes + sizeof(void *), true);

  // Allocate on heap + size of header
  void *location = h_alloc(h, bytes_total);
  if(location == NULL) {
    return NULL;
  }

  // Create header
  intptr_t header = 0;
  intptr_t data = ((intptr_t)bytes << 2);
  header = O_HEADER_SET_TYPE(header, 1);
  header = O_HEADER_SET_DATA(header, data);

   // Write header
  *(intptr_t *)location = header;
  // Advance pointer
  location = o_advance_pointer(location);

  o_write_zeros(location, bytes);

  // Return pointer
  return location;
}

void *o_move_object(heap_t *h, void *obj)
{
  intptr_t header = o_get_header(obj);
  short type = (short)O_HEADER_GET_TYPE(header);
  void *new_location = obj;
  size_t bytes = 0;

  if(type == 1) {
    // Compact header
    intptr_t data = O_HEADER_GET_DATA(header);
    if(O_HEADER_GET_TYPE(data) == 0) {
      // Raw size
      bytes = (size_t)O_HEADER_GET_DATA(data);
      bytes = o_get_new_offset(O_ZERO_BYTES, WORDSIZE, bytes, true);
    } else {
      // Bitvector
      bytes = o_size_from_bitvector(O_HEADER_GET_DATA(data));
    }
  } else if(type == 0) {
    // String rep
    bytes = o_size_from_string_rep((char *)O_HEADER_GET_PTR(header));
  } else if(type == 3) {
    // Forward header
    void *fwd = (void *)O_HEADER_GET_PTR(header);
    // Check if valid heap-pointer
    if(is_page_addr(h, fwd) && is_mem_addr(h, fwd)) {
      return fwd;
    }
    return obj;
  }

  // TODO: IMPORTANT: Check that size of object is greater than 0 and less than MAX_OBJ_SIZE
  // If so, return obj-pointer as is without copy
  if(bytes == 0 || (bytes + sizeof(void *)) > get_page_mem(h)) {
    return obj;
  }

  new_location = h_alloc_gc(h, bytes + sizeof(void *));
  if(new_location != NULL) {
    memcpy(new_location, o_reverse_pointer(obj), sizeof(void *) + bytes);
    // Advance new copy pointer
    new_location = o_advance_pointer(new_location);

    // Write fwd header
    void *old = o_reverse_pointer(obj);
    intptr_t header = 0;
    header = O_HEADER_SET_TYPE(header, 3);
    header = O_HEADER_SET_PTR(header, (intptr_t)new_location);
    // Write header
    *(intptr_t *)old = header;
  }

  return new_location;
}

size_t o_size_from_char(char c)
{
  switch(c)
    {
    case '*':
      return sizeof(void *);
      break;
    case 'i':
      return sizeof(int);
      break;
    case 'f':
      return sizeof(float);
      break;
    case 'c':
      return sizeof(char);
      break;
    case 'l':
      return sizeof(long);
      break;
    case 'd':
      return sizeof(double);
      break;
    default:
      return 0; // TODO: How detect this state of 'unknown' type?
      break;
    }
}

size_t o_size_from_bits(int bits)
{
  if (bits == 3) {
    return sizeof(void *);
  }
  else if(bits == 2) {
    return 8;
  }
  else if (bits == 1) {
    return  4;
  }
  return 0;
}

void **o_get_pointer_in_object(void *ptr, size_t index)
{
  if (ptr == NULL) {
    return NULL;
  }

  // TODO: Same code as o_pointers_in_object - merge functions
  intptr_t header = o_get_header(ptr);
  int header_type = (int) O_HEADER_GET_TYPE(header);

  // Compact header
  if (header_type == 1) {
    intptr_t data = O_HEADER_GET_DATA(header);
    int compact_type = (int) (data & 3UL); // TODO: Remove 'magic' constant OR make into MACRO
    data = data >> 2; // TODO: remove 'magic' constant OR make into MACRO
    if (compact_type == 1) {
      return o_get_pointer_from_bitvector(data, ptr, index);
    }
  }
  // String format
  else if (header_type == 0) {
    return o_get_pointer_from_string_rep((char *)O_HEADER_GET_PTR(header), ptr, index);
  }
  // Forwarding pointer
  else if (header_type == 3) {
    return o_get_pointer_in_object((void *)O_HEADER_GET_PTR(header), index);
  }
  // Union or unknown
  return NULL;
}

void **o_get_pointer_from_bitvector(intptr_t header_data, void *object, size_t index)
{
  if (object == NULL) {
    return NULL;
  }

  int t = (int) (header_data & 3UL);
  size_t offset = 0;
  bool found = false;
  while(t != 0) {
    // Check if pointer
    if(t == 3) {
      if(index == 0) {
        found = true;
        break;
      }
      --index;
    }
    offset += o_size_from_bits(t);
    // Advance to next bytes in vector
    header_data = header_data >> 2; // TODO: Remove 'magic' constant OR make into MACRO
    t = (int) (header_data & 3UL); // TODO: Remove 'magic' constant OR make into MACRO
  }

  if(found) {
    return (void *) ((char *)object) + offset;
  }

  return NULL;
}

void **o_get_pointer_from_string_rep(char *format, void *object, size_t index)
{
  if (format == NULL || object == NULL) {
    return NULL;
  }

  char *cursor = format;
  size_t offset = 0;
  bool found = false;
  unsigned int multiplier = 0;

  while((*cursor != '\0' || multiplier > 0) && !found) {

    char c = *cursor;
    if(c == '\0') {
      c = 'c';
    }

    // Check multiplier
    if(o_is_numeric(*cursor)) {
      multiplier = (multiplier * 10) + o_ctoi(*cursor);
      ++cursor;
      continue;
    } else if(multiplier == 0) {
      multiplier = 1;
    }

    for(;multiplier > 0; --multiplier) {
      // Add preceding offset
      offset = o_get_new_offset(O_ZERO_BYTES, o_alignment_from_char(*cursor), offset, true);

      // Check if pointer
      if(*cursor == '*') {
        if(index == 0) {
          found = true;
          break;
        }
        --index;
      }
      offset += o_size_from_char(*cursor);
    }

    if(*cursor != '\0') {
      ++cursor;
    }
  }

  if(found) {
    return object + offset;
  }

  return NULL;
}


size_t o_get_object_size(void *ptr)
{
  if (ptr == NULL) {
    return 0;
  }

  // TODO: Same code as o_pointers_in_object - merge functions
  intptr_t header = o_get_header(ptr);
  int header_type = (int) O_HEADER_GET_TYPE(header);

  // Compact header
  if (header_type == 1) {
    intptr_t data = O_HEADER_GET_DATA(header);
    int compact_type = (int) (data & 3UL); // TODO: Remove 'magic' constant OR make into MACRO
    data = data >> 2; // TODO: remove 'magic' constant OR make into MACRO
    if (compact_type == 1) {
      return o_size_from_bitvector(data);
    }
    return (size_t)data;
  }
  // String format
  else if (header_type == 0) {
    return o_size_from_string_rep((char *)O_HEADER_GET_PTR(header));
  }
  // Forwarding pointer
  else if (header_type == 3) {
    return o_get_object_size((void *)O_HEADER_GET_PTR(header));
  }
  // Union or unknown
  return 0;
}

size_t o_size_from_bitvector(intptr_t header_data)
{
  int t = (int) (header_data & 3UL);
  size_t bytes = 0;
  while(t != 00) {
    bytes += o_size_from_bits(t);
    // Advance to next bytes in vector
    header_data = header_data >> 2; // TODO: Remove 'magic' constant OR make into MACRO
    t = (int) (header_data & 3UL); // TODO: Remove 'magic' constant OR make into MACRO
  }
  return bytes;
}


bool o_is_numeric(char c)
{
  if(c == '0' ||
     c == '1' ||
     c == '2' ||
     c == '3' ||
     c == '4' ||
     c == '5' ||
     c == '6' ||
     c == '7' ||
     c == '8' ||
     c == '9') {
    return true;
  }
  return false;
}

unsigned int o_ctoi(char c)
{
  switch(c) {
  case '0':
    return 0;
  case '1':
    return 1;
  case '2':
    return 2;
  case '3':
    return 3;
  case '4':
    return 4;
  case '5':
    return 5;
  case '6':
    return 6;
  case '7':
    return 7;
  case '8':
    return 8;
  case '9':
    return 9;
  default:
    return 0;
  }
}

long o_get_new_offset(long bytes, long alignment, long offset, bool padding_only)
{
  long padding = (-offset) & (alignment - 1);
  return offset + padding + (padding_only ? 0 : bytes);
}

long o_alignment_from_char(char c)
{
  switch(c) {
  case '*':
    // 8 byte aligned (x64), 4 byte aligned (x86)
    return ALIGNMENT_POINTER;
  case 'i':
    // 4-byte aligned
    return ALIGNMENT_INT;
  case 'f':
    // 4-byte aligned
    return ALIGNMENT_FLOAT;
  case 'c':
    // 1-byte aligned
    return ALIGNMENT_CHAR;
  case 'l':
    // 8 byte aligned (x64), 4 byte aligned (x86)
    return ALIGNMENT_LONG;
  case 'd':
    // 8 byte aligned (x64), 4 byte aligned (x86, 8-byte aligned on Windows)
    return ALIGNMENT_DOUBLE;
  default:
    return 0;
  }
}

size_t o_size_from_string_rep(char *format)
{
  if (format == NULL) {
    return 0;
  }

  char *cursor = format;
  long bytes = 0;
  long offset = 0;
  long alignment = 1;
  int multiplier = 0;

  while(*cursor != '\0' || multiplier > 0) {
    char c = *cursor;
    if(c == '\0') {
      c = 'c';
    }

    // If numeric
    if(o_is_numeric(*cursor)) {
      multiplier = (multiplier * 10) + o_ctoi(*cursor);
      ++cursor;
      continue;
    } else if(multiplier == 0) {
      multiplier = 1;
    }

    for(;multiplier > 0; --multiplier) {
      bytes = o_size_from_char(c);
      // If unknown character - return 0 as it is an
      // invalid string representation
      if(bytes == 0) {
        return 0;
      }
      alignment = o_alignment_from_char(c);
      offset = o_get_new_offset(bytes, alignment, offset, false);
    }

    if(*cursor != '\0') {
      ++cursor;
    }

  }
  // Add last possible padding if not ended on word alignmnent
  offset = o_get_new_offset(O_ZERO_BYTES, WORDSIZE, offset, true);

  return offset;
}

size_t o_pointers_in_object(void *ptr)
{
  if (ptr == NULL) {
    return 0;
  }

  // TODO: Same code as o_get_object_size - merge functions
  intptr_t header = o_get_header(ptr);
  int header_type = (int) O_HEADER_GET_TYPE(header);

  // Compact header
  if (header_type == 1) {
    intptr_t data = O_HEADER_GET_DATA(header);
    int compact_type = (int) (data & 3UL); // TODO: Remove 'magic' constant OR make into MACRO
    if (compact_type == 1) {
      data = data >> 2; // TODO: remove 'magic' constant OR make into MACRO
      return o_pointers_in_bitvector(data);
    }
    return 0;
  }
  // String format
  else if (header_type == 0) {
    return o_pointers_in_string_rep((char *)O_HEADER_GET_PTR(header));
  }
  // Union or unknown
  return 0;
}

size_t o_pointers_in_string_rep(char *format)
{
  if (format == NULL) {
    return 0;
  }

  // TODO: Implement advanced notation (eg. 3* for ***)
  size_t count = 0;
  char *cursor = format;
  unsigned int multiplier = 0;

  while(*cursor != '\0' || multiplier > 0) {

    char c = *cursor;
    if(c == '\0') {
      c = 'c';
    }

    // Check multiplier
    if(o_is_numeric(*cursor)) {
      multiplier = (multiplier * 10) + o_ctoi(*cursor);
      ++cursor;
      continue;
    } else if(multiplier == 0) {
      multiplier = 1;
    }

    // Check cursor is valid char
    if(o_size_from_char(c) == 0) {
      return 0;
    }

    for(;multiplier > 0; --multiplier) {
      if(*cursor == '*') {
        ++count;
      }
    }

    if(*cursor != '\0') {
      ++cursor;
    }
  }

  return count;
}

size_t o_pointers_in_bitvector(intptr_t header_data)
{
  int t = (int) (header_data & 3UL);
  size_t count = 0;
  while(t != 00) {
    if (t == 3) {
      count++;
    }
    // TODO: Exchange for 'val = o_advance_bitvector(&bitvector);' which shifts the
    //       bitvector and returns extracted value.
    header_data = header_data >> 2; // TODO: Remove 'magic' constant OR make into MACRO
    t = (int) (header_data & 3UL); // TODO: Remove 'magic' constant OR make into MACRO
  }
  return count;
}


bool o_fits_in_vector(char *format) {
  // TODO: Check NOT NULL
  size_t vector_length = (sizeof(void *) * 4) - 2;
  return (vector_length * WORDSIZE) >= o_size_from_string_rep(format);
}

intptr_t o_string_format_to_bitvector(char *format) {

  // TODO: Check NOT NULL

  char *cursor = format;
  intptr_t vector = 0;
  long offset = 0;
  size_t pair = 0;
  size_t padding = 0;
  unsigned int multiplier = 0;

  #if WORDSIZE == 8
  intptr_t data_value = 2; // 1 on 32 bit systems
  #else
  intptr_t data_value = 1; // 1 on 32 bit systems
  #endif
  intptr_t ptr_value  = 3;

  while(*cursor != '\0' || multiplier > 0) {
    char c = *cursor;
    if(c == '\0') {
      c = 'c';
    }

    // Check multiplier
    if(o_is_numeric(*cursor)) {
      multiplier = (multiplier * 10) + o_ctoi(*cursor);
      ++cursor;
      continue;
    } else if(multiplier == 0) {
      multiplier = 1;
    }

    long bytes = o_size_from_char(c);
    long alignment = o_alignment_from_char(c);

    for(;multiplier > 0; --multiplier) {
      padding = (-offset) & (bytes - 1);

      if(padding > 0) {
	vector = vector | data_value << (pair * 2);
	++pair;
      }

      offset = o_get_new_offset(bytes, alignment, offset, false);

      if(c == '*') {
	vector = vector | ptr_value << (pair * 2);
	++pair;
      } else if (offset % WORDSIZE == 0) {
	vector = vector | data_value << (pair * 2);
	++pair;
      }
    }

    if(*cursor != '\0') {
      ++cursor;
    }

    padding = (-offset) & (WORDSIZE - 1);
    if(padding > 0) {
      vector = vector | data_value << (pair * 2);
    }
  }

  return vector;
}
