CC=gcc
AS=gcc

main: main.c gt.o gtswitch.o
	$(CC) main.c gt.o gtswitch.o -o main

gt.o: gt.c gt.h
	$(CC) -c gt.c -o gt.o

gtswitch.o: gtswitch.s
	$(CC) -c gtswitch.s -o gtswitch.o