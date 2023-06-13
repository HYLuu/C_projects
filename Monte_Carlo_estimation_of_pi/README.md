# Assignment 1 - Monte Carlo Estimation of π

## Program Description
The purpose of this program is to produce plots using gnuplot based on the C program provided in the class resources repository.

The provided *monte\_carlo.c* program first produces uniformly scattered points in a square with an inscribed quadrant and then measures the number of points that fall within the quadrant and out of the quadrant. The ratio of the number of points in the quadrant to the number of points in the square can be used to estimate π. After each random point it tests, the *monte\_carlo.c* program will output the current iteration, the estimated value of π of the current point, its x and y coordinates, and a 1 or 0 value indicating if it is in the circle, where 1 means it is inside the circle.

The first plot will demonstrate the uniformly scattered points in a square and inscribed quadrant. The points in the quadrant have a distance less than or equal to 1 and are blue. And the points that are a part of the square but not the circle will be red.
The second plot will show the difference between the estimated π and “true” π as iteration increases under different seeds for the random number generator.

## Building monte\_carlo.c

```
$ make all
```
will create an executable binary file titled 'monte\_carlo', which will be used in the main *plot.sh* bash script.

## Running the bash script

```
./plot.sh
```
will execute the main *plot.sh* bash script to produce plots.

## Cleaning

```
$ make clean
```
will clean the directory after building. It will remove the executable 'monte\_carlo' and the .o files from the directory.
