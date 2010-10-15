CC=gcc
CFLAGS=-Wall -g -std=c99
LDFLAGS=-lm

convert: convert.o

clean:
	-rm *.o convert