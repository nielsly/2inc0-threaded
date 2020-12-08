#
#	Makefile for <2INC0> Interprocess Communication
#
#	(c) TUe 2016-2020, Joris Geurts
#

BINARIES = flip

CC = gcc
CFLAGS = -Wall -g -c
LDLIBS = -lpthread

all:	$(BINARIES)

clean:
	rm -f *.o $(BINARIES)

flip: flip.o

flip.o: flip.c flip.h

