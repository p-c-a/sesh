#include <stdio.h>
#include <stdlib.h>

#include "cmd.h"
#include "execute.h"
#include "lexer.h"
#include "linenoise.h"
#include "list.h"
#include "parser.h"
#include "sesh.h"
#include "token.h"

/* Global variable to hold argv[0]. */
const char *program_name;

/* Unix shell. */
int main(int argc, char **argv)
{
	char *line;		/* user input as malloc'd string */
	List_T tokens;		/* tokenized input, returned by lexer */
	Cmd_T cmd;		/* command, returned by parser */

	program_name = argv[0];

	/* Load history. */
	linenoiseHistoryLoad(".sesh_history");

	/* Main loop. */
	for (;;) {
		line = linenoise("sesh $ ");
		if (line == NULL)
			break;
		if (line[0] == '\0')
			continue;

		tokens = lex(line);
		if (tokens == NULL) {
			free(line);
			continue;
		}
		
		cmd = parse(tokens);
		if (cmd != NULL) {
			execute(cmd);

			/* Update saved history. */
			linenoiseHistoryAdd(line);
			linenoiseHistorySave(".sesh_history");

			Cmd_free(cmd);
		}

		Token_free(tokens);
		
		free(line);
	}

	return 0;
}
