# Assignment 2 - Approximation of Irrational Numbers

## Program Description

There are seven C programs. Six of them are implementation of different mathematical formulas which are used to compute the fundamental constants *e* and π. The *mathlib-test.c* is a test harness that compares the created implemented functions with that of the math library’s.

## Building mathlib-test.c

```
$ make all
```
or
```
$ make mathlib-test
```
will create an executable binary file titled 'mathlib-test', which will print out the results of comparison of our written functions with that of the math library’s. These commands will as well compile and link all the small written math functions.

## Running mathlib-test

```
$ ./mathlib-test [-options]
```
This command without any argument will show the usage options of *mathlib-test*. <br />

The options of *mathlib-test*: <br />

-a         Runs all tests. <br />
-e         Runs e test. <br />
-b         Runs BBP pi test. <br />
-m         Runs Madhava pi test. <br />
-r         Runs Euler pi test. <br />
-v         Runs Viete pi test. <br />
-n         Runs Newton square root tests. <br />
-s         Print verbose statistics. <br />
-h         Display program synopsis and usage.

## Cleaning

```
$ make clean
```
will clean the directory after building. It will remove the executable 'mathlib-test' and the .o files from the directory.

## Formatting

```
$ make format
```
will format all the source code, including the header files.
