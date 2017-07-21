#ifndef TOKEN_INCLUDED
#define TOKEN_INCLUDED

typedef struct Token *Token_T;

/* A Token object can be either ordinary or special.  Special tokens are
   those which redirect stdin or stdout. */
enum TokenType {TOKEN_REGULAR, TOKEN_SPECIAL_IN, TOKEN_SPECIAL_OUT};

/* Write all tokens in list to stdout. */
void Token_print(List_T list);

/* Free all of the tokens in list, and the list itself. */
void Token_free(List_T list);

/* Create and return a token with given type and value. */
Token_T Token_new(enum TokenType type, char *value);

/* Return token's type. */
enum TokenType Token_get_type(Token_T token);

/* Return token's value. */
char *Token_get_value(Token_T token);

#endif
