## bfi, a simple Brainfuck interpreter

It's pretty much a right of passage to implement a Brainfuck interpreter as a programmer. Here's my attempt!

## Building

On linux, you'll need gcc at a minimum, the program can be run by:

    gcc bfi.c && ./a.out

I've also included a Makefile, which does much the same thing:

    make && ./bfi

## Modifying

Feel free to poke around the source, it's not too heavy on the macro magic.

The `run(const unsigned char *prgm)` will take a string as the program text, and run it to completion.

There's several example programs included in main(), including Hello World, Sierpinski's triangle, and Conway's Game of Life.

## Installing

why would you want to do this?!
