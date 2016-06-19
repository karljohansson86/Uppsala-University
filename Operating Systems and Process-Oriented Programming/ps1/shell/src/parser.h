#ifndef PARSER_H_
#define PARSER_H_

/*****************************************************************************
 * PARSER API
 *****************************************************************************/

// Position of a command in a pipeline. 
enum cmd_pos {unknown, single, first, middle, last};

/*****************************************************************************
 * DESCRIPTION
 *
 * Parses a string looking for pipe characters '|'. Everything between
 * pipe characters is considered to be a bash command.  Each call to
 * this function will search the string for a new command (left to
 * right) and populate the provided argv with command data.
 * 
 * INPUT(S)
 *
 * line
 * 
 *      A string with bash like commands separated with pipe
 *	    characters. After calling this function, the line string will
 *	    be altered since terminating nulls are insterted (see argv
 *	    bellow).
 * 
 * argv 
 *    
 *      Array of string pointers. On return, this array will be
 *      populated with the argv for the current command. The argv will
 *      be populated with pointers within the orginal input line.
 *
 * RETURN VALUE
 *
 * The position of the returned command is returned.
 * 
 * single
 * 
 *      The value single is returne if there is a singel command in
 *      the parsed command line.
 *
 * first
 * 
 *		The value first is returned for the first command if there is
 *		at least two commands in the parsed command line.
 *
 * middle
 * 
 *      The value middle is returned for all but the first and the
 *      last command if there are at least three commands in the
 *      parsed command line.
 *
 * last
 * 
 *      The value last is returned for the last command if there are
 *      at least two commands in the parsed command line.
 *
 * EXAMPLE USAGE
 *
 * str = "  ls -i -l | grep foo " 
 * 
 * pos = next_command(str, argv) 
 * 
 *	   pos	 <--  first
 *	argv[0]	 <--  "ls"
 *	argv[1]	 <--  "-i"
 *	argv[2]	 <--   "-l"
 *	argv[1]	 <--   NULL
 *
 *  pos =  next_command(str, argv) 
 * 
 *	    pos  <--  last
 *	argv[0]  <--  "grep"
 *	argv[1]  <--  "foo"
 *	argv[2]  <--  NULL
 *****************************************************************************/
enum cmd_pos next_command(char* line, char* argv[]);


/*****************************************************************************
 * Internal function used by the parser. 
 *****************************************************************************/

char* ltrim(char* s);
char* rtrim(char* s);
char* trim(char* s);
int empty_line(char* str);


// #define __DBG__

// String representation of a command position.
char const* pos2str(enum cmd_pos pos, int format);

void dbg_print_string(char* ptr, const char* name, const char* callee, const char* msg);


#ifdef __DBG__
#define DBG_PRINT_STRING(ptr, msg) dbg_print_string(ptr, #ptr, __FUNCTION__, msg)
#else 
#define DBG_PRINT_STRING(ptr, msg) 
#endif


#endif // PARSER_H_
