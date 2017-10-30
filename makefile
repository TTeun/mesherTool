CC=g++-7
CFLAGS=-c -Wall -Werror -std=c++17
LFLAGS=-O3
SFMLFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

all: aux showTool mesherTool

aux: io.o exc.o

io.o: IO/io.cc IO/io.h
	$(CC) -c $< $(CFLAGS)

exc.o: exc/exc.cc exc/exc.h
	$(CC) -c $< $(CFLAGS)

mesherTool: mesher.o config.o meshbuilder.o 
	$(CC) -o mesherTool $< config.o meshbuilder.o io.o exc.o $(LFLAGS)

showTool: show.o  point.o
	$(CC) -o showTool $< point.o io.o exc.o  $(SFMLFLAGS) -lm $(LFLAGS)

mesher.o: mesher/mesher.cc config.o meshbuilder.o
	$(CC) -c $< $(CFLAGS)

show.o: show/show.cc
	$(CC) -c $< $(CFLAGS)

config.o: mesher/config.cc mesher/config.h
	$(CC) -c $< $(CFLAGS)

meshbuilder.o: mesher/meshbuilder.cc mesher/meshbuilder.h
	$(CC) -c $< $(CFLAGS)

point.o: show/types/point.cc show/types/point.h
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm *.o mesherTool showTool
