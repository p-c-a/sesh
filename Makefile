CC = gcc
CFLAGS = -g -ggdb -Wall -Werror
OBJECTS = linenoise.o list.o token.o cmd.o lexer.o parser.o execute.o sesh.o

.PHONY: all clean

%.o: %.c
	$(CC) $(CFLAGS) -c $<

all: sesh

clean:
	rm -f *~ \#*\#
	rm -f sesh *.o

sesh: $(OBJECTS) sesh.c
	$(CC) -o sesh $(OBJECTS)
