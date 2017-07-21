#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "list.h"
#include "sesh.h"
#include "token.h"

/* The parser is implemented as a DFA with four states: */
enum Parser_State {PARSE_ARGS, PARSE_STDIN, PARSE_STDOUT, PARSE_ERROR};

/* Handle PARSE_ARGS state. */
enum Parser_State handle_args(Token_T token, Cmd_T cmd)
{
	enum TokenType type;
	char *value;
	int success;

	type = Token_get_type(token);
	value = Token_get_value(token);

	if (type == TOKEN_REGULAR) {
		success = Cmd_add_arg(cmd, value);
		if (!success) { QUIT(program_name); }
		return PARSE_ARGS;
	} else if (type == TOKEN_SPECIAL_IN) {
		return PARSE_STDIN;
	} else {
		return PARSE_STDOUT;
	}
}

/* Handle PARSE_STDIN state. */
enum Parser_State handle_stdin(Token_T token, Cmd_T cmd)
{
	enum TokenType type;
	char *value;
	int success;

	type = Token_get_type(token);

	if (type != TOKEN_REGULAR) {
		fprintf(stderr,
			"%s: stdin: missing file name\n", program_name);
		return PARSE_ERROR;
	}

	value = Token_get_value(token);
	success = Cmd_set_stdin(cmd, value);
	if (! success) {
		fprintf(stderr,
			"%s: stdin: multiple redirection\n", program_name);
		return PARSE_ERROR;
	}

	return PARSE_ARGS;
}

/* Handle PARSE_STDOUT state. */
enum Parser_State handle_stdout(Token_T token, Cmd_T cmd)
{
	enum TokenType type;
	char *value;
	int success;

	type = Token_get_type(token);

	if (type != TOKEN_REGULAR) {
		fprintf(stderr,
			"%s: stdout: missing file name\n", program_name);
		return PARSE_ERROR;
	}

	value = Token_get_value(token);
	success = Cmd_set_stdout(cmd, value);
	if (! success) {
		fprintf(stderr,
			"%s: stdout: multiple redirection\n", program_name);
		return PARSE_ERROR;
	}

	return PARSE_ARGS;
}

/* Parse list of tokens.  Returns Cmd object if successful, or NULL if
   there is an error in the tokens list. */
Cmd_T parse(List_T tokens)
{
	Cmd_T cmd;
	char *name;
	int length;
	int i;
	Token_T tmp;
	
	enum Parser_State state = PARSE_ARGS;

	assert(tokens != NULL);

	length = List_get_length(tokens);
	if (length < 1) return NULL;
	
	/* Assume first token is command name. */
	tmp = List_get(tokens, 0);
	if (Token_get_type(tmp) != TOKEN_REGULAR) {
		fprintf(stderr, "%s: missing command name\n", program_name);
		return NULL;
	}
	name = Token_get_value(tmp);
	cmd = Cmd_new(name);
	
	/* Assign remaining tokens. */
	for (i = 1; i < length; i++) {
		tmp = List_get(tokens, i);

		switch (state) {
		case PARSE_ARGS:
			state = handle_args(tmp, cmd);
			break;
		case PARSE_STDIN:
			state = handle_stdin(tmp, cmd);
			break;
		case PARSE_STDOUT:
			state = handle_stdout(tmp, cmd);
			break;
		case PARSE_ERROR:
			goto free_cmd;
		}
	}

	if (state == PARSE_STDIN) {
		fprintf(stderr,
			"%s: stdin: missing file name\n", program_name);
		goto free_cmd;
	}

	if (state == PARSE_STDOUT) {
		fprintf(stderr,
			"%s: stdout: missing file name\n", program_name);
		goto free_cmd;
	}

	return cmd;

free_cmd:
	Cmd_free(cmd);
	return NULL;
}
