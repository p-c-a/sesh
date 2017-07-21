#ifndef PARSER_INCLUDED
#define PARSER_INCLUDED

/* Parse list of tokens.
   Returns Cmd object if successful, otherwise NULL. */
Cmd_T parse(List_T tokens);

#endif
