GCC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2

TARGET = cards.exe
OBJS = cards.o xorcrypt.o

.PHONY: all clean

all: cards.exe

cards.exe: cards.o xorcrypt.o
	gcc cards.o xorcrypt.o -o cards.exe

cards.o: cards.c xorcrypt.h
	gcc -c cards.c -o cards.o

xorcrypt.o: xorcrypt.c xorcrypt.h
	gcc -c xorcrypt.c -o xorcrypt.o

clean:
	-rm *.o *.exe NUL
