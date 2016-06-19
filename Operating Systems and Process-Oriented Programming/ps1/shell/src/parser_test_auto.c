/*****************************************************************************
 * DESCRIPTION
 *
 * A collection of automatic tests for  command line parser.
 *****************************************************************************/
#include "parser.h"  // next_command(), enum cmd_pos, pos2str()

#include <stdio.h>  // printf()
#include <string.h> // strcpy()


#define MAX_ARGV_SIZE 10
#define COMMAND_LINE_BUFFER_SIZE 1024

void parse_and_print(char* line) {
  
  // This array will be populated with command data after each call to
  // the next_command() parser function.
  
  char* argv[MAX_ARGV_SIZE];  
  
  
  // Count the commands in the command line.
  
  int   cmd_nr     = 0;
 
  
  // Position of each command in a command line. By passing a pointer
  // to this variable to the next_command() parser function, the
  // parser can alter this variable.
  
  enum cmd_pos pos;  // {single, first, middle, last}
  
  
  printf("\n\n==> Command line @ [0x%p] = \"%s\"\n", line, line);

  // Call the next_command() parser until no more commands can be
  // found in the command line. 
  
  do {  

    pos = next_command(line, argv);
    
    
    // After the call to the next_command() parser function, the
    // command possition within the command line is now available in the pos
    // varialble.
    
    printf("\n%6s command #%d:\n", pos2str(pos, 1), cmd_nr++);
    
    // Loop through the argv and print the command data. 
    
    int i = 0;
    
    while (argv[i] != NULL) {
      printf("         argv[%d] @ [0x%p] = \"%s\"\n", i, argv[i], argv[i]);
      i++;
    }
    
    
  } while (pos != single && pos != last);

}

int main() {
  
  // A collection of command lines. 
  
  const char* line[] = {
    "ls",
    "   ls",
    "ls    ",
    "    ls    ",
    "    ls   -l  -i  ",
    "    ls | grep shell  ",
    "    ls | grep shell | wc -l ",
    "    ls | grep -i -V ShELl | sort |  wc -l  ",
    NULL};
  
  // The above array of command lines are declared const, hence the
  // strings cannot be modified. The next_command() parser will modify the
  // command line (insert new terminating nulls at various places), hence we need to copy
  // each command line to a modifyable buffer.
  
  char command_line_buffer[COMMAND_LINE_BUFFER_SIZE];
  
  // Loop throug all command lines and and print the command data obtained by calling the 
  // next_command() parser.
  
  int i = 0;
  
  while(line[i]) {
    strcpy(command_line_buffer, strdup(line[i]));
    parse_and_print(command_line_buffer);
    i++;
  }
  
}
