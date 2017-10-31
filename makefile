CC=g++-7
CFLAGS=-c -Wall -pedantic -Werror -std=c++17
LFLAGS=-O3
SFMLFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

all: io.o exc.o showTool mesherTool

io.o: IO/io.cc IO/io.h
	$(CC) -c $< $(CFLAGS)

exc.o: exc/exc.cc exc/exc.h
	$(CC) -c $< $(CFLAGS)

mesherTool: mesher.o config.o meshbuilder.o exc.o io.o point.o
	$(CC) -o mesherTool $< config.o meshbuilder.o io.o exc.o point.o $(LFLAGS) $(SFMLFLAGS)

showTool: show.o meshbuilder.o point.o io.o exc.o config.o
	$(CC) -o showTool $< point.o meshbuilder.o io.o exc.o config.o $(SFMLFLAGS) -lm $(LFLAGS)

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
