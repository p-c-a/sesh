# sesh: super easy shell

sesh is a minimal Unix shell written in the C programming language.

## Features

sesh contains some of the features you might expect from a shell, namely:

* input and output redirection
* builtins: `cd`, `setenv`, `unsetenv`
* line editing and history handling (implemented in the BSD licensed [linenoise.c](linenoise.c))

but is missing:

* piping
* scripting functionality (loops, conditionals, &c.)
* globbing
* many others!

## Building

```
$ make
```

## Running

```
$ ./sesh
```
