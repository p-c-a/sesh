CC = gcc
CFLAGS = -g -ggdb -Wall -Werror
COMPILE = $(CC) $(CFLAGS)
OBJECTS = linenoise.o list.o token.o cmd.o lexer.o parser.o execute.o sesh.o

.PHONY: all clean tags

%.o: %.c
	$(COMPILE) -c $<

all: sesh

clean:
	rm -f *~ \#*\#
	rm -f sesh *.o
	rm -f TAGS

sesh: $(OBJECTS) sesh.c
	$(COMPILE) -o sesh $(OBJECTS)

tags:
	find . -name "*.[ch]" -print | etags -
