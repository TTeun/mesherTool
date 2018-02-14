CC=g++
CFLAGS=-pedantic -Wall -std=c++11
LFLAGS=-O3
SFMLFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

all: tool

tool: Mesh2D.o main.o config.o io.o
	$(CC) -o tool Mesh2D.o main.o io.o config.o $(SFMLFLAGS) $(CFLAGS) -lm $(LFLAGS)

Mesh2D.o: Mesh2D.cc Mesh2D.h
	$(CC) -c $< $(CFLAGS)

config.o: config.cc config.h
	$(CC) -c $< $(CFLAGS)

io.o: io.cc io.h
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm *.o tool
