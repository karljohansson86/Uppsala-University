/*****************************************************************************
 * DESCRIPTION
 *
 * A simple shell for executing a single command (system program) or a
 * sequence of pipelined commands.
 * 
 * EXAMPLE USAGE
 * 
 * A single command: 
 *
 *         SHELL 0> date
 *         Fri Jan 29 22:14:07 CET 2016
 *         SHELL 1>
 * 
 * Two pipelined commands:
 * 
 *         SHELL 1> date | wc -w
 *         6
 *         SHELL 2>
 *
 * Type 'exit' <enter> to exit the shell. 
 * 
 *         SHELL 2> exit
 *         Goodbye!
 *
 * AUTHOR(S)
 *
 * Karl Marklund <karl.marklund@it.uu.se>
 * 
 * HISTORY
 *
 * 2010-11-09 (Karl)
 * 
 * First version.
 *
 * 2016-01-31 (Karl)
 *
 * Major re-write. Each logical step is now handled in a separate
 * function. Added shell.h header file with function prototypes
 * togehter with short descriptons of each function.
 * 
 *****************************************************************************/

#include "shell.h"

#include "parser.h"    // next_command(), enum cmd_pos, pos2str()
#include "debug.h"     // DBG(), DBG_ONLY()

#include <stdio.h>     // fgets()
#include <stdlib.h>    // EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>    // strcmp() 
#include <unistd.h>    // execvp(), fork(), getpid(), getppid(),
                       // STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO
#include <sys/wait.h>  // wait()
#include <signal.h>    // SIGPIPE
#include <sys/types.h> // pid_t


int main(void) {
  
  // Allocate a buffer for the command line read from the user. 
  char command_line_buffer[COMMAND_LINE_BUFFER_SIZE];
  
  // Parse the command line using the next_command() parser. The
  // parser will populate the following array with the result of each call
  // to the parser.
  char* argv[MAX_ARGV_SIZE];  
    
  // Count the number of non empty command lines.
  int command_line_nr = 0;
  
  // Count the number of children forked for each command line. 
  int num_of_children = 0; 
  
  while(1) {

    command_line_nr = read_command_line(command_line_nr, 
                                        command_line_buffer, 
                                        COMMAND_LINE_BUFFER_SIZE);
    
    num_of_children = execute_command_line(command_line_buffer, argv);
    
    pid_t child_pid;
    int *child_status = 0;
    //pid_t child_arr[num_of_children];
    for (int i = 0; i < num_of_children; i ++) {
      // TODO 1: Make the parent wait for all children. 
      child_pid = wait(child_status);
      //child_arr[i] = child_pid;
    }


    fflush(NULL);
 
  } // end while(1)
} // end of main()


int read_command_line(int n, char *buffer, int buffer_size) {
  do {
      
    // Print the command prompt including the command line counter. 
    printf("SHELL %d> ", n);
    fflush(NULL);
      
    // Read input from the user. 
    if ( fgets(buffer, buffer_size, stdin) == NULL) {
      perror("====== ERROR ====> READING COMMAND LINE FAILED :(");
      exit(EXIT_FAILURE);
    }

    // Exit if the user types "exit" and presses enter. 
    if (strcmp(buffer, "exit\n") == 0) {
      printf("Goodbye!\n");
      exit(EXIT_FAILURE);
    }
      
    // If the user presses enter without typing anything but white
    // space, don't update the command line counter, just start over
    // again.
      
  } while (empty_line(buffer));
    
  return n+1;
}


int execute_command_line(char *line, char *argv[]) {

  int  num_of_commands = 0;
  enum cmd_pos pos     = unknown;
  int  left_pipe[2]    = {-1, -1};
  int  right_pipe[2]   = {-1, -1};

  do {
    
    // Repeat this loop for each of the commands separated by '|' in
    // the command pipeline.

    // Use the next_command() parser to get the position and argument
    // vector of the next command in the command pipeline.
    pos = next_command(line, argv);
    
    create_pipe(pos, right_pipe);

    
    fork_child(pos, left_pipe, right_pipe, argv);

    // Only the parent will return here!!!

    num_of_commands++;

    // The previous rigth pipe now becomes the current left pipe.
    shift_pipes(left_pipe, right_pipe);

    
  } while (pos != single && pos != last);
  
  return num_of_commands;
}


void create_pipe(enum cmd_pos pos, int new_pipe[]) {

  // TODO 2: If there are more than one command in the pipeline,
  //         create a pipe for all but the last command.
  int status = 0;
  if(pos == last || pos == single) {
    //should not create pipe
    return;
  } else {
    
    status = pipe(new_pipe);
    if(status < 0) {
      perror("UNABLE TO CREATE PIPE");
    }
  }
    
}


void shift_pipes(int left_pipe[], int right_pipe[]) {

  left_pipe[0]  = right_pipe[0];
  left_pipe[1]  = right_pipe[1];
}


void fork_child(enum cmd_pos pos, int left_pipe[], int right_pipe[], char *argv[]) {

  pid_t pid = fork();
  
  switch(pid) {
    
  case 0: // == CHILD ==

    child_redirect_io(pos, left_pipe, right_pipe);

    // NOTE: The child doesn't need to close any pipe descritpors
    // since they are closed once the child terminates.

    execute_command(argv);
    
    // Child should never return from execute_command(). 
    exit(EXIT_FAILURE);
    
  case -1: // == ERROR ==
    
    perror("======= ERROR ====> Forking child process failed.");
    exit(EXIT_FAILURE);
    
  default: // == PARENT ==

    parent_close_pipes(pos, left_pipe,  right_pipe);

  }

}


void parent_close_pipes(enum cmd_pos pos, int left_pipe[], int right_pipe[]) {

  // TODO 3: The parent must close un-used pipe descriptors. You need
  // to figure out wich descriptors that must be closes when.
  if(pos == first) {
    close(right_pipe[1]);
  }
  else if(pos ==  middle) {
    close(left_pipe[0]);
    close(right_pipe[1]);
  }
  else if(pos == last) {
    close(left_pipe[0]); 
  }

}


void child_redirect_io(enum cmd_pos pos, int left_pipe[], int right_pipe[]) {
  
  // TODO 4: A child may need to redirect STDIN to read from the left
  // pipe and STDOUT to write to the right pipe depending on the
  // position in the pipeline.
  if(pos == first) {
    dup2(right_pipe[1],STDOUT_FILENO);
  }
  else if(pos ==  middle) {
    dup2(left_pipe[0],STDIN_FILENO);
    dup2(right_pipe[1],STDOUT_FILENO);
  }
  else if(pos == last) {
    dup2(left_pipe[0],STDIN_FILENO);
  }


}

void execute_command(char *argv[]) {
  if (execvp(argv[0], argv) == -1) {
    perror("No such command");
    exit(EXIT_FAILURE);
  }
}











