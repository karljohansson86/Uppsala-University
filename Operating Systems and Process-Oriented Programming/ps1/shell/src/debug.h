#ifndef DEBUG_H_
#define DEBUG_H_

#include "parser.h"

void print_pipe_info(char *prefix, enum cmd_pos pos, int left_pipe[], int right_pipe[], char* msg);


#define DO_DEBUG 0		// 1 <==>  enable debug printouts.
						// 0 <==>  disable debug printouts.

// Use the DBG macro for debug printouts. 

#if DO_DEBUG == 1
#define DBG(format, ...) fprintf(stderr, format, __VA_ARGS__);fflush(stderr)
#define DBG_ONLY(statement) statement

#else
#define DBG(format, ...) 
#define DBG_ONLY(statement)
#endif

#endif // DEBUG_H_
