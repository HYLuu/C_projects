SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
SOURCE = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCE))

all: mathlib-test

mathlib-test: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c mathlib.h
	$(CC) $(CFLAGS) $< -c

tidy: 
	$(RM) *.o

clean: tidy
	$(RM) mathlib-test

format:
	clang-format -i -style=file *.[ch]
