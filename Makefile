# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=g++
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-c -Wall -I/usr/include/flycapture

LIBS=-lflycapture `pkg-config --libs opencv`

all: cam-test Tcp_image_streamer

cam-test: cam-test.o peek-mill-utils.o
	$(CC) cam-test.o peek-mill-utils.o -o cam-test $(LIBS)
	
cam-test.o: cam-test.cpp
	$(CC) $(CFLAGS) cam-test.cpp
	
peek-mill-utils.o: peek-mill-utils.cpp peek-mill-utils.hpp
	$(CC) $(CFLAGS) peek-mill-utils.cpp
	
Tcp_image_streamer: Tcp_image_streamer.o
	$(CC) Tcp_image_streamer.o -o Tcp_image_streamer $(LIBS)
	
Tcp_image_streamer.o: Tcp_image_streamer.cpp
	$(CC) $(CFLAGS) Tcp_image_streamer.cpp
    
clean:
	rm -rf *o cam-test Tcp_image_streamer


