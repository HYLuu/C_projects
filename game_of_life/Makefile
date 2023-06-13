SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
SOURCE = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCE))

all: life

life: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lm -lncurses

%.o: %.c *.h
	$(CC) $(CFLAGS) $< -c -gdwarf-4

tidy: 
	$(RM) *.o

clean: tidy
	$(RM) life

format:
	clang-format -i -style=file *.[ch]
