#ifndef LEXER_INCLUDED
#define LEXER_INCLUDED

/* Perform lexical analysis on line.  Returns a List object containing
   all tokens in line, or NULL if line contains any lexical errors. */
List_T lex(const char *line);

#endif
