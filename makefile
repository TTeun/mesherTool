CC=g++
CFLAGS=-std=c++11 -pedantic -Wall -pedantic 
LFLAGS=-O3
SFMLFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

all: tool

tool: Mesh2D.o Mesh2DBuilder.o BaseConfig.o io.o CoordinateHelper.o main.o
	$(CC) *.o $(SFMLFLAGS) $(CFLAGS) -lm $(LFLAGS) -o tool

main.o: main.cpp 
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

Mesh2D.o: Mesh2D.cpp Mesh2D.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

Mesh2DBuilder.o: Mesh2DBuilder.cpp Mesh2DBuilder.h CoordinateHelper.o Mesh2D.o
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

BaseConfig.o: BaseConfig.cpp BaseConfig.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

io.o: io.cpp io.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

CoordinateHelper.o: CoordinateHelper.cpp CoordinateHelper.h NozzleCoordinates.o PipeCoordinates.o ExteriorCoordinates.o
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

NozzleCoordinates.o: coordinateHelpers/NozzleCoordinates.cpp coordinateHelpers/NozzleCoordinates.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

PipeCoordinates.o: coordinateHelpers/PipeCoordinates.cpp coordinateHelpers/PipeCoordinates.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

ExteriorCoordinates.o: coordinateHelpers/ExteriorCoordinates.cpp coordinateHelpers/ExteriorCoordinates.h
	$(CC) -c $< $(CFLAGS) $(LFLAGS) 

.PHONY: clean
clean:
	rm *.o tool
