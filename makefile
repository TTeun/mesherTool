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

mesherTool: mesher.o aux
	$(CC) -o mesherTool $< io.o exc.o $(LFLAGS)

showTool: show.o aux point.o
	$(CC) -o showTool $< point.o io.o exc.o  $(SFMLFLAGS) -lm $(LFLAGS)

mesher.o: mesher/mesher.cc
	$(CC) -c $< $(CFLAGS)

show.o: show/show.cc
	$(CC) -c $< $(CFLAGS)

point.o: show/types/point.cc show/types/point.h
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm *.o mesherTool showTool
