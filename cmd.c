#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "list.h"
#include "sesh.h"

/* A Cmd struct holds the values of a shell command. */
struct Cmd {
	char *name;		/* command name */
	List_T args;		/* command args */
	char *in; 		/* stdin  redirection (filename) */
	char *out;		/* stdout redirection (filename) */
};

/* Return a pointer to a newly malloc'd Cmd struct. */
Cmd_T Cmd_new(char *name)
{
	Cmd_T cmd;
	List_T args;
	
	cmd = (Cmd_T) malloc(sizeof(struct Cmd));
	if (cmd == NULL) {
	        QUIT(program_name);
	}
	
	args = List_new(0);
	if (args == NULL)
		QUIT(program_name);
	
	cmd->name = (char*) malloc(strlen(name) + 1);
	if (cmd->name == NULL)
	        QUIT(program_name);
	
	strcpy(cmd->name, name);
	
	cmd->args = args;
	cmd->in = NULL;
	cmd->out = NULL;
	
	return cmd;
}

/* Return the name of a given command. */
char *Cmd_get_name(Cmd_T cmd)
{
	assert(cmd != NULL);
	return cmd->name;
}

/* Return the arguments of a given command. */
List_T Cmd_get_args(Cmd_T cmd)
{
	assert(cmd != NULL);
	return cmd->args;
}

/* Add arg to cmd's arguments.  Return 1 if successful, or 0 if
   insufficient memory is available. */
int Cmd_add_arg(Cmd_T cmd, char *arg)
{
	List_T args;

	assert(cmd != NULL);
	assert(arg != NULL);

	args = cmd->args;

	if (! List_add(args, arg))
		return 0;

	return 1;
}

/* Return the stdin redirection of a given command. */
char *Cmd_get_stdin(Cmd_T cmd)
{
	assert(cmd != NULL);
	return cmd->in;
}

/* Redirect cmd's stdin to in. Return 1 if successful, or 0 if cmd
   already redirects stdin. */
int Cmd_set_stdin(Cmd_T cmd, char *in)
{
	assert(cmd != NULL);
	assert(in != NULL);

	if (cmd->in != NULL)
		return 0;

	cmd->in = (char*) malloc(strlen(in) + 1);
	if (cmd->in == NULL) {
		perror(program_name);
		exit(EXIT_FAILURE);
	}
	strcpy(cmd->in, in);
	return 1;
}

/* Return the stdout redirection of a given command. */
char *Cmd_get_stdout(Cmd_T cmd)
{
        assert(cmd != NULL);
	return cmd->out;
}

/* Redirect cmd's stdout to out. Return 1 if successful, or 0 if cmd
   already redirects stdout. */
int Cmd_set_stdout(Cmd_T cmd, char *out)
{
	assert(cmd != NULL);
	assert(out != NULL);

	if (cmd->out != NULL)
		return 0;

	cmd->out = (char*) malloc(strlen(out) + 1);
	if (cmd->out == NULL) { QUIT(program_name); }
	strcpy(cmd->out, out);
	return 1;
}

/* Print cmd to caller's stdout. */
void Cmd_print(Cmd_T cmd)
{
	List_T args;
	char *arg;
	int len;
	int i;

	assert(cmd != NULL);
	
	printf("Command name: %s\n", cmd->name);

	args = cmd->args;
	len = List_get_length(args);
  
	for (i = 0; i < len; i++) {
		arg = List_get(args, i);
		printf("Command arg: %s\n", arg);
	}

	printf("Command stdin: %s\n", cmd->in);
	printf("Command stdout: %s\n", cmd->out);
}

/* Return a null-terminated array of pointers to null-terminated
   strings, representing the command's name and argument list. */
char **Cmd_get_exec_args(Cmd_T cmd)
{
	char **args;
	int len;
	int i;

	assert(cmd != NULL);
	
	len = List_get_length(cmd->args);
	
	args = (char**) calloc((size_t) len + 2, sizeof(char*));
	if (args == NULL) {
		QUIT(program_name);
	}
	
	args[0] = cmd->name;

	for (i = 0; i < len; i++) {
		args[i + 1] = List_get(cmd->args, i);
	}
	args[len + 1] = NULL; 	/* execvp() requires the array of
				 * pointers to be terminated by a null
				 * pointer -- exec(3). */

	return args;
}


/* Free a given command. */
void Cmd_free(Cmd_T cmd)
{
	assert(cmd != NULL);

	List_free(cmd->args); /* Token_free free()s individual args. */
	
	free(cmd->name);

	if (cmd->in != NULL)
		free(cmd->in);
	if (cmd->out != NULL)
		free(cmd->out);

	free(cmd);
}
