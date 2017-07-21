#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "lexer.h"
#include "sesh.h"
#include "token.h"

/* The lexer is implemented as a DFA with five states: */
enum Lexer_State {LEX_START, LEX_NORMAL, LEX_IN_QUOTES, LEX_RETURN,
		  LEX_ERR_UNMATCHED_QUOTE};

/* Handle LEX_START state. */
enum Lexer_State handle_start(char c, char *buf, int *buf_i, List_T tokens)
{
	Token_T token;

	if (c == '\0') {
		return LEX_RETURN;
	} else if (isspace(c)) {
		return LEX_START;
	} else if (c == '\"') {
		return LEX_IN_QUOTES;
	} else if (c == '>') {
		buf[(*buf_i)++] = c;
		buf[*buf_i] = '\0';
		token = Token_new(TOKEN_SPECIAL_OUT, buf);
		if (! List_add(tokens, token))
			QUIT(program_name);
		*buf_i = 0;
		return LEX_START;
	} else if (c == '<') {
		buf[(*buf_i)++] = c;
		buf[*buf_i] = '\0';
		token = Token_new(TOKEN_SPECIAL_IN, buf);
		if (! List_add(tokens, token))
		        QUIT(program_name);
		*buf_i = 0;
		return LEX_START;
	} else {
		buf[(*buf_i)++] = c;
		return LEX_NORMAL;
	}
}

/* Handle LEX_NORMAL state. */
enum Lexer_State handle_normal(char c, char *buf, int *buf_i,
			       List_T tokens)
{
	Token_T token;

	if (c == '\0') {
		buf[*buf_i] = '\0';
		token = Token_new(TOKEN_REGULAR, buf);
		if (! List_add(tokens, token))
			QUIT(program_name);
		*buf_i = 0;
		return LEX_RETURN;
	} else if (isspace(c)) {
		buf[*buf_i] = '\0';
		token = Token_new(TOKEN_REGULAR, buf);
		if (! List_add(tokens, token))
			QUIT(program_name);
		*buf_i = 0;
		return LEX_START;
	} else if (c == '\"') {
		return LEX_IN_QUOTES;
	} else {
		buf[(*buf_i)++] = c;
		return LEX_NORMAL;
	}
}

/* Handle LEX_IN_QUOTES state. */
enum Lexer_State handle_quotes(char c, char *buf, int *buf_i)
{
	if (c == '\0') {
		return LEX_ERR_UNMATCHED_QUOTE;
	} else if (c == '\"') {
		return LEX_NORMAL;
	} else {
		buf[(*buf_i)++] = c;
		return LEX_IN_QUOTES;
	}
}

/* Perform lexical analysis on line.  Returns a List object containing
   all tokens in line, or NULL if line contains any lexical errors. */
List_T lex(const char *line)
{
	enum Lexer_State state = LEX_START;

	int line_i = 0;
	char c;

	char *buf;
	int buf_i = 0;

	List_T tokens;

	assert(line != NULL);

	tokens = List_new(0);
	if (tokens == NULL) {
	        QUIT(program_name);
	}

	buf = (char*) malloc(strlen(line) + 1);
	if (buf == NULL) {
	        QUIT(program_name);
	}

	while (state != LEX_RETURN && state != LEX_ERR_UNMATCHED_QUOTE) {
		c = line[line_i++];

		switch (state) {
		case LEX_START:
			state = handle_start(c, buf, &buf_i, tokens);
			break;
		case LEX_NORMAL:
			state = handle_normal(c, buf, &buf_i, tokens);
			break;
		case LEX_IN_QUOTES:
			state = handle_quotes(c, buf, &buf_i);
			break;
		default:
			break;
		}
	}

	free(buf);

	if (state == LEX_ERR_UNMATCHED_QUOTE) {
		printf("%s: unmatched quote\n", program_name);
		Token_free(tokens);
		return NULL;
	}

	return tokens;
}
