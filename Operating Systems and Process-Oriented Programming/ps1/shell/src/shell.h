#ifndef SHELL_H_
#define SHELL_H_

#include "parser.h" // enum cmd_pos, next_command()

#define MAX_ARGV_SIZE 10
#define COMMAND_LINE_BUFFER_SIZE 1024


/* Infinite loop: Print prompt, read command line, execute command
 * line, start over.
 */
int  main(void);

/* Print prompt, read command line into buffer. 
 */
int  read_command_line(int n, char *buffer, int buffer_size);

/* For all but the last of the commands separated by '|', create a
 * pipe. For each command, fork a child to execute the command.
 */
int  execute_command_line(char *line, char *argv[]);

/* If there are more than one command, create a pipe for all but the
 * last command.
 *
 * SIDE EFFECT: On succes, the new_pipe array stores file descriptors
 * to the created pipe.
 */
void create_pipe(enum cmd_pos pos, int new_pipe[]);

/* Fork a child. 
 *
 * NOTE: Only parent will return from this function. Child
 * will do exec to execute the command.
 */
void fork_child(enum cmd_pos pos, int left_pipe[], int right_pipe[], char *argv[]);

/* As each command in the pipeline is handled, the previous right pipe
   becomes the next commands left pipe.
 */
void shift_pipes(int left_pipe[], int right_pipe[]);

/* The parent must close un-used pipe descritpors, otherwise children
 * may not detect enf-of-file (EOF).
 */
void parent_close_pipes(enum cmd_pos pos, int left_pipe[], int right_pipe[]);

/* A child may need to redirect STDIN to read from the left pipe and
 * STDOUT to write to the right pipe depending on the position in the
 * pipeline.
 */
void child_redirect_io(enum cmd_pos pos, int left_pipe[], int right_pipe[]);

/* A small wrapper around execvp() to replace the process executable.
 *
 * NOTE: On success, the process executalbe is replaced and the caller
 * will not return from this.
 */
void execute_command(char *argv[]);


#endif // SHELL_H_
