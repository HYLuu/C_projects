# Assignment 3 - Comparison of Sorting Algorithms

## Program Description

We implement four different sorting programs and test their performance by writing a test harness using bit-wise Set operations. The four sorting methods are Shell sort, Heap sort, Quick sort and Batcher sort, respectively. The Set operation implementing functions and statistics module will be separate files that can be used in the main *sorting.c* testing program.

## Building sorting.c

```
$ make all
```
or
```
$ make sorting
```
will create an executable binary file titled 'sorting', which will print out the results of our four sorting algorihm implementation. These commands will as well compile and link all the small written sorting functions.

## Running sorting

```
$ ./sorting [-options]
```
This command without any argument will show the usage options of *sorting*. <br />

The options of *sorting*: <br />

-H              Display program help and usage. <br />
-a              Enable all sorts. <br />
-s              Enable Shell Sort. <br />
-b              Enable Batcher Sort. <br />
-h              Enable Heap Sort. <br />
-q              Enable Quick Sort. <br />
-n length       Specify number of array elements (default: 100). <br />
-p elements     Specify number of elements to print (default: 100).  <br />
-r seed         Specify random seed (default: 13371453).

## Cleaning

```
$ make clean
```
will clean the directory after building. It will remove the executable 'sorting' and the .o files from the directory.

## Formatting

```
$ make format
```
will format all the source code, including the header files.
