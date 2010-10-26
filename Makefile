CC=gcc
CFLAGS=-Wall -std=c99
LDFLAGS=-lm

all: convert
convert: convert.o
	$(CC) $(LDFLAGS) -o $@  convert.o

clean:
	-rm *.o convert
