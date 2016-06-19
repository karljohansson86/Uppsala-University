#include <stdbool.h>

/**
 *   \file input.h
 *   \brief Contains functions for reading user input
 *   \author Karl Johansson
 *   \author Simon Lövgren
 *
 *  Functions for reading user input
 */


// Defines

#define STRIP_NEWLINE true
#define LEAVE_NEWLINE false

#define VERIFY_INT true
#define NO_VERIFY_INT false

// Functions

/**
 *  \brief Read a line from given stream to the heap
 *
 *  Reads a line (up until line termination) and
 *  returns at pointer to the string on the heap.
 *
 *  \note   The string will be stored on the stack
 *          and will need to be freed at 'end of life'.
 *
 *  \param stream        Stream to read from (eg. stdin)
 *  \param stripNewLine  Whether to strip newline from line or not 
 *  \return char*        Pointer to the string on the stack
 */
char* input_readString(FILE * restrict stream, bool stripNewLine);

/**
 *  \brief Read a iven length of a line to given buffer
 *
 *  Reads a given length of a line from given stream to 
 *  given buffer. If the line on the stream is longer than
 *  the given length, te remainder of the string will be
 *   discarded.
 *
 *  \param stream        Stream to read from (eg. stdin)
 *  \param len           Number of characters to read (incl. string terminator)
 *  \param buf           Buffer to write string to
 *  \param stripNewLine  Whether to strip newline from line or not 
 *  \return char*        Pointer to given buffer
 */
char* input_readStringBuf(FILE * restrict stream, size_t len, char* buf,  bool stripNewLine);

/**
 *  \brief Read an integer from given stream
 *
 *  Reads a string from given input stream and
 *  converts it to an integer. If `verifyInt` is
 *  set, a verification of that the input consists
 *  of numbers only will be used.
 *
 *  In case the verification fails (and is enabled)
 *  the funcion will wait for the user to enter a
 *  new number.
 *
 *  \param stream     Stream to read from (eg. stdin)
 *  \param verifyInt  Whether to verify that only a 
 *                    number has been entered or not
 *
 *  \return int       Input converted to integer
 */
int input_readInt(FILE * restrict stream, bool verifyInt);

/**
 *  \brief Read an integer from given stream with
 *         settable error message on verification
 *         fail.
 *
 *  Reads a string from given input stream and
 *  converts it to an integer. If `verifyInt` is
 *  set, a verification of that the input consists
 *  of numbers only will be used.
 *
 *  In case the verification fails (and is enabled)
 *  the funcion will wait for the user to enter a
 *  new number.
 *
 *  If an error message is supplied, it will be
 *  printed before awaiting new input. If 
 *  `errorMessage` is set to `NULL`, no messge
 *  will be printed.
 *
 *  \param stream        Stream to read from (eg. stdin)
 *  \param verifyInt     Whether to verify that only a 
 *                       number has been entered or not
 *  \param errorMessage  Message to display on wrong input
 *
 *  \return int       Input converted to integer
 */
int input_readIntWithMessage(FILE * restrict stream, bool verifyInt, char* errorMessage);
