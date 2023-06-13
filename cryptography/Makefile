SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)
LIB = randstate.c numtheory.c ss.c
LIBOBJ = $(patsubst %.c,%.o,$(LIB))

all: keygen encrypt decrypt

keygen: keygen.o $(LIBOBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

encrypt: encrypt.o $(LIBOBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

decrypt: decrypt.o $(LIBOBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

%.o: %.c *.h
	$(CC) $(CFLAGS) $< -c

tidy: 
	$(RM) *.o

clean: tidy
	$(RM) keygen encrypt decrypt

format:
	clang-format -i -style=file *.[ch]
