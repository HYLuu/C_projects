# Assignment 6 - Lempel-Ziv Compression

## Program Description

The two main program will be the implementation of compression and decompression using Lempel-Ziv Compression algorithm. The *encode.c* is a compression program that compresses any file, text or binary. The *decode.c* program can decompress any file, text or binary, that was compressed with *encode*. <br />

The *trie.c* contains the implementation of the ADT and functions for tries. The *word.c* program contains the implementations of the ADT and functions for words and word tables. The *io.c* contains implementations of an I/O module to read or write through some buffer. The libraries and modules will be used in the two main programs to perform efficient compression and decompression.

## Building encode and decode executables

```
$ make all
```
or
```
$ make
```
will create executable binary files titled 'encode' and 'decode'. They will compress the input file and decompress the output from the *encode*, respectively. This commands will as well compile and link the three modules and libraries.

## Building encode

 ```
 $ make encode
 ```
will only create executable binary files titled 'encode'.

## Building decode

 ```
$ make decode
 ```
will only create executable binary files titled 'decode'.

## Running encode

 ```
$ ./encode [-options]
 ```
If no command-line argument is specified, the program will wait for user input data. <br />

The options of *encode*: <br />

-v          Display compression statistics <br />
-i input    Specify input to compress (stdin by default) <br />
-o output   Specify output of compressed input (stdout by default) <br />
-h          Display program help and usage <br />

## Running decode

 ```
$ ./decode [-options]
 ```
If no command-line argument is specified, the program will wait for user input data. <br />

The options of *decode*: <br />

-v          Display decompression statistics <br />
-i input    Specify input to decompress (stdin by default) <br />
-o output   Specify output of decompressed input (stdout by default) <br />
-h          Display program usage <br />

## Cleaning

```
$ make clean
```
will clean the directory after building. It will remove the executable 'encode', 'decode', and the .o files from the directory.

## Formatting

```
$ make format
```
will format all the source code, including the header files.
