/**
 *   \file object.h
 *   \brief Object allocation and object headers
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "settings.h"
#include "h_init.h"

#ifndef __object__
#define __object__

////////////////////////// OBJECT MODULE SETTINGS //////////////////////////////
/**
 * \def O_MASK_TYPE
 *  The bitmask for extracting the header type bits from the header.
 */
#ifndef O_MASK_TYPE
#define O_MASK_TYPE 3UL
#endif

/**
 *  \def O_TYPE_BITS
 *  Indicates how many bits are used for the header type.
 */
#ifndef O_TYPE_BITS
#define O_TYPE_BITS 2
#endif

/**
 *  \def O_SMALLEST_SIZE
 *  Indicates how many bytes smallest allocation size will be
 */
#ifndef O_SMALLEST_SIZE
#define O_SMALLEST_SIZE 16
#endif

/**
 *  \def O_LARGEST_SIZE
 *  Indicates how many bytes largest allocation size will be
 */
#ifndef O_LARGEST_SIZE
#define O_LARGEST_SIZE 16 * 1024 // 16 kB
#endif

///////////////////////// GENERAL BITWISE MACROS ///////////////////////////////

/**
 *  \def O_SET_BIT(h,n)
 *  Sets bit \a n of bitarray \a h to 1.
 *  Indexing (\a n) starts at 0.
 */
#define O_SET_BIT(h,n)          (h | (1UL << n))

/**
 *  \def O_UNSET_BIT(h,n)
 *  Sets bit \a n of bitarray \a h to 0.
 *  Indexing (\a n) starts at 0.
 */
#define O_UNSET_BIT(h,n)        (h & ~(1UL << n))

/**
 *  \def O_READ_BIT(h,n)
 *  Reads bit \a n of bitarray \a h, resulting in 1 or 0.
 *  Indexing starts at 0.
 */
#define O_READ_BIT(h,n)         ((h & (1UL << n)) >> n)


//////////////////////////// OBJECT HEADER MACROS //////////////////////////////
/*
 *  COMPACT HEADER DESIGN/DOCUMENTATION  (For internal use)
 *  ============================================================================
 *
 * There are 4 headertypes.
 * - 00 (0) pointer to formatstring.
 * - 01 (1) compact layout representation.
 * - 10 (2) custom garbage collection function.
 * - 11 (3) forwarding adress / copy indicator
 *
 *
 *   COMPACT HEADER
 *   =================================================
 *   ```
 *   Size indicator, headertype (01)
 *   31/63                        0
 *   ______________________________
 *   |D|D|D|D|D|D|D|D|D|D|D|A|A|T|T| <- A represents size or bitvector (00 or 01)
 *   ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 *   ```
 *   (T) The 2 last bits, is the header type.
 *   (A=00) represents raw size of object (in bytes)
 *   (A=01) represents a bit vector mapping data pointers within the object.
 *
 *   ## RAW SIZE
 *   Raw size is simply represented by a size_t value stored in the data bits
 *   of the header. A number of MSB (depending on header type bits etc.) will
 *   always be set to 0.
 *
 *   ## BIT VECTOR
 *   The bit vector represents data types within the object using two bits
 *   per data type, resulting in three data type indicators and one stop
 *   indicator. The bit vector grows from LSB to MSB, thus index 0 is the
 *   two least significant bits in the bitvector.
 *
 *   | VALUE |  DESCRIPTION               |
 *   |-------|----------------------------|
 *   | 0b00  | STOP (end ov bitvector)    |
 *   | 0b01  | 4 byte datatype            |
 *   | 0b10  | 8 byte datatype            |
 *   | 0b11  | Pointer                    |
 *
 *
 *   FORWARDING-, STRING REPRESENATION- and CUSTOM TRACING FUNCTION- POINTER
 *   ===========================================================================
 *   Custom garbage collection function / Forwarding adress / Copy indicator
 *   31/63                     0
 *   ___________________________
 *   |D|D|D|D|D|D|D|D|D|D|D|T|T|
 *   ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 *   (D)  -  represents pointer
 *   (T)  -  represents headertype. (00, 10 or 11)
 *
 */

/**
 *  \def O_HEADER_GET_TYPE(h)
 *  Extracts header type from header \a h and "returns" it.
 *
 *  Possible header types are:
 *
 *  DEC | HEX | DESCRIPTION
 *  ----|-----|-------------------------------------
 *  0   | 00  | Pointer to format-string.
 *  1   | 01  | Compact layout representation.
 *  2   | 10  | Custom garbage collection function.
 *  3   | 11  | Forwarding adress / copy indicator
 *
 *  \a h should be of type `intptr_t`
 */
#define O_HEADER_GET_TYPE(h)    (h & O_MASK_TYPE)

/**
 *  \def O_HEADER_SET_TYPE(h,t)
 *  Sets header type bits \a t of header \a h and "returns" the resulting
 *  header bits. It bitmasks the header value from \a t to not overwrite
 *  other header data.
 *
 *  \a h should be of type `intptr_t`
 *
 *  \sa O_HEADER_GET_TYPE for valid header types
 */
#define O_HEADER_SET_TYPE(h,t)  ((h & ~(O_MASK_TYPE)) | (t & O_MASK_TYPE))

/**
 *  \def O_HEADER_GET_PTR(h)
 *  Extracts the pointer data from heder \a h, with the header type bits set
 *  to 0.
 *
 *  \a h should be of type `intptr_t`
 */
#define O_HEADER_GET_PTR(h)     (h & ~(O_MASK_TYPE))

/**
 *  \def O_HEADER_SET_PTR(h,p)
 *  Updates the data field of header \a h with supplied pointer \a p,
 *  excluding the header type bits.
 *
 *  \a h and \a p should be of type `intptr_t`
 */
#define O_HEADER_SET_PTR(h,p)   ((h & O_MASK_TYPE) | (p & ~(O_MASK_TYPE)))

/**
 *  \def O_HEADER_GET_DATA(h)
 *  Extracts the data from header \a h, shifting the data to start at bit 0.
 *
 *  \a h should be of type `intptr_t`
 */
#define O_HEADER_GET_DATA(h)    (h >> O_TYPE_BITS)

/**
 *  \def O_HEADER_SET_DATA(h,d)
 *  Sets data bits of header \a h with data bits \a d. Data bits will be
 *  shifted to start after header type bits and not at bit 0.
 *
 *  \a h and \a d should be of type `intptr_t`
 */
#define O_HEADER_SET_DATA(h,d)  ((h & O_MASK_TYPE) | (d << O_TYPE_BITS))

/////////////////////////   CONSTANTS  ///////////////////////////////

#define O_ZERO_BYTES      0

#if WORD_SIZE == 8
#define O_VECTOR_PADDING 2
#else
#define O_VECTOR_PADDING 1
#endif

///////////////////////// FUNCTION PROTOTYPES ///////////////////////////////


/**
 *  Allocate a new object on a heap with a given format string.
 *
 *  Valid characters in format strings are:
 *  | CHAR  |  REPRESENTS
 *  |-------|-----------------------------------------------
 *  |   i   |  for sizeof(int) bytes 'raw' data
 *  |   l   |  for sizeof(long) bytes 'raw' data
 *  |   f   |  for sizeof(float) bytes 'raw' data
 *  |   c   |  for sizeof(char) bytes 'raw' data
 *  |   d   |  for sizeof(double) bytes 'raw' data
 *  |   *   |  for a sizeof(void *) bytes pointer value
 *  |   \0  |  null-character terminates the format string
 *
 *  \param   h       the heap
 *  \param   layout  the format string
 *  \return  the newly allocated object
 *
 *  Note: the heap does *not* retain an alias to layout.
 */


void *o_alloc_struct(heap_t *h, char *layout);

/**
 *  Allocate a new object on a heap with a given size.
 *
 *  Objects allocated with this function will *not* be
 *  further traced for pointers.
 *
 *  \param   h      the heap
 *  \param   bytes  the size in bytes
 *  \return  the newly allocated object
*/
void *o_alloc_raw(heap_t *h, size_t bytes);

/**
 *  Returns amount (count) of pointers within object. Only works
 *  with objects that have been allocated using `o_alloc_struct`,
 *  other object types will default to 0 pointers.
 *
 *  \param   ptr  Pointer to object of interest.
 *  \return  Counted pointers within object.
 */
size_t o_pointers_in_object(void *ptr);

/**
 *  Returns a pointer to the \a n:th pointer within an object.
 *
 *  \sa o_pointers_in_object for
 *
 *  \param   ptr  Pointer to object
 *  \param   n    Index of pointer to get pointer to (starts with 0)
 *  \return  Pointer to pointer within object, NULL if out of range
 */
void **o_get_pointer_in_object(void *ptr, size_t n);

/**
 *  Returns header of object.
 *
 *  \param   ptr  Pointer to object from which to extract header
 *  \return  Header from object
 */
intptr_t o_get_header(void *ptr);

/**
 *
 *
 *  \param   obj
 *  \return  Header from object
 */
void o_set_header(void *obj, intptr_t header);

/**
 *  Returns size of object in bytes. Does not work
 *  for objects allocated using o_alloc_union.
 *
 *  \warning May segfault if sent pointer not pointing
 *           to actual object on heap.
 *
 *  \bug     May segfault if sent pointer not pointing
 *           to actual object on heap.
 *
 *  \param   ptr  Pointer to object
 *  \return  Size of object in bytes
 */
size_t o_get_object_size(void *ptr);

/**
 *  Re-allocates object and sets forwarding header in old copy,
 *  only works in garbage collection phase.
 *
 *  \warning Use only in garbage collection phase, as it uses
 *           a garbage collection phase specific allocation.
 *
 *  \param   obj  Object to copy
 *  \return  Pointer to new copy.
 */
void *o_move_object(heap_t *h, void *obj);



#endif
