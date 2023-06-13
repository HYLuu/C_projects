SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
SOURCE = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCE))

all: sorting

sorting: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c *.h
	$(CC) $(CFLAGS) $< -c

tidy: 
	$(RM) *.o

clean: tidy
	$(RM) sorting

format:
	clang-format -i -style=file *.[ch]
