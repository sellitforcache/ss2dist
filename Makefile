CC=g++
CFLAGS=-I.

ss2dist:  ss2dist.cpp
	$(CC) -o ss2dist ss2dist.cpp -I.
debug:  ss2dist.cpp
	$(CC) -g -o ss2dist ss2dist.cpp -I.
