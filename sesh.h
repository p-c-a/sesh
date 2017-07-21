#ifndef SESH_INCLUDED
#define SESH_INCLUDED

/* Global variable to hold argv[0]. */
extern const char *program_name;

/* Permissions when redirecting stdout to a new file. */
#define NEW_FILE_PERMISSIONS 0600 /* rw- --- --- */

/* Handy macro to save typing. */
#define QUIT(x) { perror(x); exit(EXIT_FAILURE); }

#endif
