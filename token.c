#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "sesh.h"
#include "token.h"

struct Token {
	enum TokenType type;
	char *value;
};

/* Write all tokens in list to stdout. */
void Token_print(List_T list)
{
	int i;
	int length;
	Token_T token;

	assert(list != NULL);

	length = List_get_length(list);

	for (i = 0; i < length; i++) {
		token = List_get(list, i);
		if (token->type == TOKEN_REGULAR)
			printf("Token: %s (ordinary)\n", token->value);
		else
			printf("Token: %s (special)\n", token->value);
	}
}

/* Free all of the tokens in list, and the list itself. */
void Token_free(List_T list)
{
	int i;
	int length;
	Token_T token;

	assert(list != NULL);

	length = List_get_length(list);

	for (i = 0; i < length; i++) {
		token = List_get(list, i);
		free(token->value);
		free(token);
	}

	List_free(list);
}

/* Create and return a new token. */
Token_T Token_new(enum TokenType type, char *value) {
	Token_T token;

	assert(value != NULL);

	token = (Token_T) malloc(sizeof(struct Token));
	if (token == NULL) {
		perror(program_name);
		exit(EXIT_FAILURE);
	}

	token->type = type;

	token->value = (char*) malloc(strlen(value) + 1);
	if (token->value == NULL) {
		perror(program_name);
		exit(EXIT_FAILURE);
	}
	strcpy(token->value, value);

	return token;
}

/* Return token's type. */
enum TokenType Token_get_type(Token_T token)
{
	assert(token != NULL);
	return token->type;
}

/* Return token's value. */
char *Token_get_value(Token_T token)
{
	assert(token != NULL);
	return token->value;
}
