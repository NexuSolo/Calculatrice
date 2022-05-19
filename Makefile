CC=gcc
CFLAGS=-Wall -g -pedantic -std=c99
LDLIBS = -lm

calc_unbounded_int: unbounded_int.o calc_unbounded_int.o

unbounded_int.o : unbounded_int.c unbounded_int.h
calc_unbounded_int.o : calc_unbounded_int.c

clean:
	rm -rf *.o *.a
cleanall:
	rm -rf *.o *.a calc_unbounded_int