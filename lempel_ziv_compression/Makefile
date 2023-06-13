SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LIB = trie.c word.c io.c
LIBOBJ = $(patsubst %.c,%.o,$(LIB))

all: encode decode

encode: encode.o $(LIBOBJ)
	$(CC) $(CFLAGS) -o $@ $^

decode: decode.o $(LIBOBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c *.h
	$(CC) $(CFLAGS) $< -c

tidy:
	$(RM) *.o

clean: tidy
	$(RM) encode decode

format:
	clang-format -i -style=file *.[ch]
