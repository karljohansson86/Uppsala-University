// TODO: move this to debug.c

#include "debug.h"
#include "parser.h"

#include <stdlib.h>    // EXIT_FAILURE, EXIT_SUCCESS
#include <stdio.h>  // fprintf()
#include <sys/stat.h> // stat()

void print_pipe_info(char *prefix, enum cmd_pos pos, int left_pipe[], int right_pipe[], char* msg) {

  struct stat left_stat , right_stat;
  
  char left_str[32]; 

  char right_str[32];
  
  char padd[] = "         ";

  
  if (pos == first  || pos == middle) {
    if (fstat(right_pipe[0], &right_stat) == -1) {
      perror("fstat ...");
      exit(EXIT_FAILURE);
    }
    

    sprintf(right_str, "[%ld]", (long) right_stat.st_ino);
    
  } else {
    sprintf(right_str, "%s", padd);
  }

  
  
  if (pos == middle || pos == last) {
    if (fstat(left_pipe[0], &left_stat) == -1) {
      perror("fstat ...");
      exit(EXIT_FAILURE);
    }
    sprintf(left_str, "[%ld]", (long) left_stat.st_ino);
  } else {
    sprintf(left_str, "%s", padd);
  }
  
  
  fprintf(stderr, "%s %s (%s) %s\n",
          prefix, 
          left_str,          
          pos2str(pos, 0), 
          right_str);
  

  fflush(NULL);
}

void print_info(char *prefix, enum cmd_pos pos, int left_pipe[], int right_pipe[], char* msg) {

  struct stat s;
  
  if (pos == first || pos == middle) {
    if (fstat(right_pipe[0], &s) == -1) {
      perror("fstat ...");
      exit(EXIT_FAILURE);
    }
    printf("[%ld][%d,%d]\n", (long) s.st_ino, right_pipe[0], right_pipe[1]);
  }
  
  printf("%s [%2d, %2d] (%s) [%2d, %2d] %s\n",
         prefix, 
         left_pipe[0], left_pipe[1], 
         pos2str(pos, 0),
         right_pipe[0], right_pipe[1], msg);

  fflush(NULL);
}





