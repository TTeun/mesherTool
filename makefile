CC=g++
CFLAGS=-std=c++11 -pedantic -Wall -pedantic 
LFLAGS=-O3
SFMLFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

all: tool

tool: Mesh2D.o config.o io.o CoordinateHelper.o main.o
	$(CC) *.o $(SFMLFLAGS) $(CFLAGS) -lm $(LFLAGS) -o tool

main.o: main.cc 
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

Mesh2D.o: Mesh2D.cc Mesh2D.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

config.o: config.cc config.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

io.o: io.cc io.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

CoordinateHelper.o: CoordinateHelper.cc CoordinateHelper.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

.PHONY: clean
clean:
	rm *.o tool
