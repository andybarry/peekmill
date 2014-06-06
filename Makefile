# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=g++
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-c -Wall -I/usr/include/flycapture

LIBS=-lflycapture `pkg-config --libs opencv`

all: cam-test

cam-test: cam-test.o
	$(CC) cam-test.o -o cam-test $(LIBS)
	
cam-test.o: cam-test.cpp
	$(CC) $(CFLAGS) cam-test.cpp
    
clean:
	rm -rf *o cam-test


