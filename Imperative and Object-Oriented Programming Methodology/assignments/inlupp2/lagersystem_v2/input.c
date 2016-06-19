#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>

#include "input.h"

#ifdef WINDOWS
#define STR_TERM -2
#else
#define STR_TERM '\0'
#endif

//////////////// Implementations //////////////


/**
 *  \brief Copycat version of POSIX getline
 *
 *  A copycat version of the POSIX-function `getline`.
 *  Reads up until a newline (`'\n`) symbol to a string
 *  allocated on the heap.
 *
 *  Usage is like `getline` except for the need to
 *  free the string when not needed any longer.
 *
 *  With `len` supplied, `myGetLine` will read at most
 *  one less than `len` and add a terminating null byte
 *  (`'\0'`).
 *
 *  Ex:
 *  // Reading a string of undefined length
 *  char* line = NULL;
 *  size_t lineLength = myGetLine(&line, 0, stdin);
 *
 *  // Reading a string of length 10
 *  char* line = NULL;
 *  size_t length = 11; 
 *  myGetLine(&line, &length, stdin);
 *
 *  \bug Not really a bug. Not profiled or optimized, 
 *       most likely not optimally written
 *  
 *  \param  lineptr  Where to store the pointer to the string
 *  \param  len      Pointer to size_t-variable of max-length to read
 *  \param  stream   Stream to read from (eg. `stdin` or a `FILE` pointer)
 *
 *  \return size_t   Length of line (number of characters read, including newline)
 */
size_t input_myGetLine(char **lineptr, size_t *len, FILE *stream) {
  char* line = NULL;
  size_t read = 0;

  if(len && *len > 0) {
    line = (char*) malloc((*len) * sizeof(char));
    if(*len == 1) {
      read = 0;
      line[0] = '\0';
    } else {
      fgets(line, (*len), stream);
      read = strlen(line);
      if(line[(*len)-2] != '\n') {
        // Clear rest if exists
        while(fgetc(stream) != '\n');
      }
    }
  } else {
    char buf[2];
    line = (char*) malloc(sizeof(char));
    do{
      fgets(buf,2,stream);
      if(buf[0] != EOF) {
        line[read++] = buf[0];
        line = (char*)realloc(line, ((1+read) * sizeof(char)));
      } else {
        break;
      }
    } while (buf[0] != '\n');
 
    line[read] = '\0';
  }
  *lineptr = line;
  return strlen(line);
}

char* input_readStringBuf(FILE * restrict stream, size_t len, char* buf,  bool stripNewLine) {
  int read = getline(&buf, &len, stream);
  if(read > 0 && stripNewLine) {
    if(read > 0 && buf[read - 1] == '\n') {
      buf[read - 1] = STR_TERM;      
    }
  }
  return buf;
}

char* input_readString(FILE * restrict stream, bool stripNewLine) {
  char* str = NULL;
  return input_readStringBuf(stream, 0, str, stripNewLine);
}

bool input_validInt(char* str, bool allowNegative) {
  
  if(str == NULL) {
    return false;
  }
  
  if(str[0] == '-' && allowNegative) {
    ++str;
  }

  bool isInt = false;
  size_t i = 0;
  while(str[i] != STR_TERM){
    isInt = isdigit(str[i]);
    if(isInt == false) {
      break;
    }
    ++i;
  }
  
  return isInt;
}

int input_readIntWithMessage(FILE * restrict stream, bool verifyInt, char* errorMessage) {
  char* in = NULL;
  do{
    
    if(in != NULL) {
      free(in);
      in = NULL;

      if(errorMessage != NULL) {
        puts(errorMessage);
      }      
    }
    in = input_readString(stream, STRIP_NEWLINE);
  } while(verifyInt && input_validInt(in, false) == false);
  int out = atol(in);
  free(in);
  return out;
}

int input_readInt(FILE * restrict stream, bool verifyInt) {
  return input_readIntWithMessage(stream, verifyInt, NULL);
}
