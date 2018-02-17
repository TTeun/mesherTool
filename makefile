CC=g++
CFLAGS=-std=c++11 -pedantic -Wall -pedantic 
LFLAGS=-O3
SFMLFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

all: tool

tool: Mesh2D.o Mesh2DBuilder.o config.o io.o CoordinateHelper.o Region1.o Region2.o Region3.o main.o
	$(CC) *.o $(SFMLFLAGS) $(CFLAGS) -lm $(LFLAGS) -o tool

main.o: main.cc 
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

Mesh2D.o: Mesh2D.cc Mesh2D.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

Mesh2DBuilder.o: Mesh2DBuilder.cc Mesh2DBuilder.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

config.o: config.cc config.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

io.o: io.cc io.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

CoordinateHelper.o: CoordinateHelper.cc CoordinateHelper.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

Region1.o: coordinateHelpers/Region1.cc coordinateHelpers/Region1.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

Region2.o: coordinateHelpers/Region2.cc coordinateHelpers/Region2.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

Region3.o: coordinateHelpers/Region3.cc coordinateHelpers/Region3.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

.PHONY: clean
clean:
	rm *.o tool
