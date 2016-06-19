/*****************************************************************************
 * DESCRIPTION
 *
 * Interactive test of the command line parser.
 *****************************************************************************/

#include "shell.h"
#include "parser.h"     // next_command(), enum cmd_pos, pos2str()
#include "debug.h"		// DBG(), DBG_ONLY()

#include <stdio.h>		// fgets()
#include <stdlib.h>		// EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>		// strcmp() 


/*****************************************************************************
 * DESCRIPTION
 *
 * 1) Prints a promt to the screen. The prompt shows the number of
 *    executed commands.
 * 
 * 2) Reads a command line from the user. 
 * 
 * 3) Parses the command line.
 *
 * 4) For each of the commands in the command line, print the contents
 *     of the argument vector argv.
 *
 * 5) Go back to step 1. 
 *
 *****************************************************************************/
int main(void) {
  
  // Allocate a buffer for the command line read from the user. 

  char line_buffer[COMMAND_LINE_BUFFER_SIZE];
  
  // We parse the command line using the next_command() parser. The
  // parser will populate the following array with the result of each call
  // to the parser.
  
  char* argv[MAX_ARGV_SIZE];  
    
  // Count the number of non empty command lines.
  
  int command_line_nr = 0;
 
  // Position of each command in a command line.
  
  enum cmd_pos pos;  // {single, first, middle, last}
  

  while(1) {

    do {
      
      // Print the command prompt including the command line counter. 
      printf("SHELL %d> ", command_line_nr);
      fflush(NULL);
      
      // Read input from the user. 
      if ( fgets(line_buffer, COMMAND_LINE_BUFFER_SIZE, stdin) == NULL) {
		perror("====== ERROR ====> READING COMMAND LINE FAILED :(");
		exit(EXIT_FAILURE);
      }

      // Exit if the user types "exit" and presses enter. 
      if (strcmp(line_buffer, "exit\n") == 0) {
		printf("Goodbye!\n");
		exit(EXIT_FAILURE);
      }
      
      // If the user presses enter without typing anything else, don't
      // update the command line counter, just start over again.
      
    } while (empty_line(line_buffer));
    
    // We got some input from the user.
    command_line_nr++;
    
    // Parse the command line
    do {
      pos = next_command(line_buffer, argv);
	  
      // After the call to the next_command() parser function, the
      // command possition within the command line is now available in the pos
      // varialble.
      
      printf("\n%6s command %s\n", pos2str(pos, 1), argv[0]);
      
      // Loop through the argv and print the command data. 
          
      int i = 0;
      
      while (argv[i] != NULL) {
		printf("         argv[%d] = \"%s\"\n", i, argv[i]);
		i++;
      }
	  
      // When are we done?
    } while (pos != single && pos != last);
    
 
  } // end while(1)

} // end of main()







