#ifndef CMD_INCLUDED
#define CMD_INCLUDED

#include "list.h"

/* A Cmd struct holds the values of a shell command. */
typedef struct Cmd *Cmd_T;

/* Return a pointer to a newly malloc'd Cmd struct. */
Cmd_T Cmd_new();

/* Return the name of a given command. */
char *Cmd_get_name(Cmd_T cmd);

/* Return the arguments of a given command. */
List_T Cmd_get_args(Cmd_T cmd);

/* Add arg to cmd's arguments. Return 1 if successful, or 0 if
   insufficient memory is available. */
int Cmd_add_arg(Cmd_T cmd, char *arg);

/* Return the stdin redirection of a given command. */
char *Cmd_get_stdin(Cmd_T cmd);

/* Redirect cmd's stdin to in. Return 1 if successful, or 0 if cmd
   already redirects stdin. */
int Cmd_set_stdin(Cmd_T cmd, char *in);

/* Return the stdout redirection of a given command. */
char *Cmd_get_stdout(Cmd_T cmd);

/* Redirect cmd's stdout to pcStdOut. Return 1 if successful, or 0 if
   cmd already redirects stdout. */
int Cmd_set_stdout(Cmd_T cmd, char *out);

/* Write the command to stdout in the specified formatting. */
void Cmd_print(Cmd_T cmd);

/* Return a null-terminated array of pointers to null-terminated
   strings, representing the command's name and argument list. */
char **Cmd_get_exec_args(Cmd_T cmd);

/* Free a given command. */
void Cmd_free(Cmd_T cmd);

#endif
