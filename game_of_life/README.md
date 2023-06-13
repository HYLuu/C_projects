# Assignment 4 - Game of Life

## Program Description

The *life.c* program implements the Game of Life evolution based on input files or STDIN and shows the animation of the evolution of the universe. The *universe.c* program includes the structure of *Universe* and the accessing and manipulating functions that will be used in *life.c*.

## Building life.c

```
$ make all
```
or
```
$ make sorting
```
will create an executable binary file titled 'life', which will display the evolution animation of the cells based on inputs and print out the final state of the universe. These commands will as well compile and link the universe module.

## Running life

```
$ ./life [-options]
```
If no command-line argument is specified, the program will wait for user input data.
This command without any argument will show the usage options of *life*. <br />

The options of *life*: <br />

-t             Create your universe as a toroidal <br />
-s             Silent - do not use animate the evolution <br />
-n {number}    Number of generations [default: 100] <br />
-i {file}      Input file [default: stdin] <br />
-o {file}      Output file [default: stdout] <br />

## Cleaning

```
$ make clean
```
will clean the directory after building. It will remove the executable 'life' and the .o files from the directory.

## Formatting

```
$ make format
```
will format all the source code, including the header files.
