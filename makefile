CC=g++-7
CFLAGS=-c  -pedantic -Werror -std=c++17
LFLAGS=-O3
SFMLFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

all: tool

tool: show.o mesher.o point.o io.o exc.o config.o
	$(CC) -o tool $< point.o mesher.o io.o exc.o config.o $(SFMLFLAGS) -lm $(LFLAGS)

io.o: IO/io.cc IO/io.h
	$(CC) $< $(CFLAGS)

exc.o: exc/exc.cc exc/exc.h
	$(CC) $< $(CFLAGS)

show.o: show/show.cc
	$(CC) $< $(CFLAGS)

config.o: config/config.cc config/config.h
	$(CC) $< $(CFLAGS)

mesher.o: mesher/mesher.cc mesher/mesher.h
	$(CC) $< $(CFLAGS)

point.o: show/types/point.cc show/types/point.h
	$(CC) $< $(CFLAGS)

.PHONY: clean
clean:
	rm *.o tool
