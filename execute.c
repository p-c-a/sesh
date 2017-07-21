#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cmd.h"
#include "execute.h"
#include "sesh.h"

/* cd -- change current directory. */
static void builtin_cd(List_T args)
{
	int args_len;
	char *path;
	char *home;

	args_len = List_get_length(args);

	if (args_len > 1) {
		fprintf(stderr, "%s: cd: too many arguments\n",
			program_name);
		return;
	}

	if (args_len == 1) {
		path = List_get(args, 0);
		if (chdir(path) == -1) {
			fprintf(stderr, "%s: ", program_name);
			perror(path);
			return;
		}
	}

	if (args_len == 0) { /* cd to HOME, if it exists */
		home = getenv("HOME");
		if (home == NULL) {
			fprintf(stderr, "%s: cd: HOME not set\n",
				program_name);
			return;
		}
		if (chdir(home) == -1) {
			fprintf(stderr, "%s: ", program_name);
			perror(home);
			return;
		}
	}
}

/* setenv -- set environment variable. */
static void builtin_setenv(List_T args)
{
	int args_len;
	char *key;
	char *value;

	args_len = List_get_length(args);

	if (args_len > 2) {
		fprintf(stderr, "%s: setenv: too many arguments\n",
			program_name);
		return;
	}
	
	if (args_len == 0) {
		fprintf(stderr, "%s: setenv: missing variable\n",
			program_name);
		return;
	}	

	key = List_get(args, 0);

	if (args_len == 1) {
		if (setenv(key, "\0", 1) == -1) {
			perror(program_name);
		}
		return;
	}
	
	if (args_len == 2) {
		value = List_get(args, 1);
		if (setenv(key, value, 1) == -1) {
			perror(program_name);
		}
		return;
	}
}

/* unsetenv -- remove environment variable. */
static void builtin_unsetenv(List_T args)
{
	int args_len;
	char *key;

	args_len = List_get_length(args);

	if (args_len > 1) {
		fprintf(stderr, "%s: unsetenv: too many arguments\n",
			program_name);
		return;
	}

	if (args_len == 0) {
		fprintf(stderr, "%s: unsetenv: missing variable\n",
			program_name);
		return;
	}

	key = List_get(args, 0);
	
	if (unsetenv(key) == -1) {
		perror(program_name);
	}
	
	return;
}

/* Execute command, calling built-in handlers as required. */
void execute(Cmd_T cmd)
{
	char *name;
	List_T args;
	char *in;
	char *out;
	char **exec_args;
	pid_t pid;
	int fd;
	
	name = Cmd_get_name(cmd);
	args = Cmd_get_args(cmd);
	
	/* built-ins */
	if (strcmp(name, "exit") == 0) {
		printf("\n");
		exit(EXIT_SUCCESS);
	} else if (strcmp(name, "cd") == 0) {
		builtin_cd(args);
		return;
	} else if (strcmp(name, "setenv") == 0) {
		builtin_setenv(args);
		return;
	} else if (strcmp(name, "unsetenv") == 0) {
		builtin_unsetenv(args);
		return;
	}
	
	in = Cmd_get_stdin(cmd);
	out = Cmd_get_stdout(cmd);
	exec_args = Cmd_get_exec_args(cmd);

	fflush(NULL);
	pid = fork();

	if (pid == -1) {
		QUIT(program_name);
	}

	/* child process */
	if (pid == 0) {

		if (in != NULL) { /* redirect stdin */
			fd = open(in, O_RDONLY);
			if (fd == -1) {
				fprintf(stderr, "%s: ", program_name);
				QUIT(in);
			}
			if (close(0) == -1) {
				fprintf(stderr, "%s: ", program_name);
				QUIT(in);
			}
			if (dup(fd) == -1) {
				fprintf(stderr, "%s: ", program_name);
				QUIT(in);
			}
			if (close(fd) == -1) {
				fprintf(stderr, "%s: ", program_name);
				QUIT(in);
			}
		}

		if (out != NULL) { /* redirect stdout */
			fd = creat(out, NEW_FILE_PERMISSIONS);
			if (fd == -1) {
				fprintf(stderr, "%s: ", program_name);
				QUIT(out);
			}
			if (close(1) == -1) {
				fprintf(stderr, "%s: ", program_name);
				QUIT(out);
			}
			if (dup(fd) == -1) {
				fprintf(stderr, "%s: ", program_name);
				QUIT(out);
			}
			if (close(fd) == -1) {
				fprintf(stderr, "%s: ", program_name);
				QUIT(out);
			}
		}

		/* execute the command */
		execvp(exec_args[0], exec_args);

		fprintf(stderr, "%s: ", program_name);
		QUIT(exec_args[0]);
	}

	/* parent process */
	pid = wait(NULL);
	if (pid == -1) { QUIT(program_name); }

	free(exec_args);
}
