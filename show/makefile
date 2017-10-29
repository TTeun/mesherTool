CC=g++-7
CFLAGS=-c -Wall -Werror -std=c++17
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lm

all: ../showTool

../showTool: show.o point.o readfile.o
	$(CC) -o ../showTool $^ $(LDFLAGS)

show.o: show.cc
	$(CC) -c $(CFLAGS) $<

readfile.o: IO/readfile.cc IO/readfile.h
	$(CC) -c $(CFLAGS) $<

point.o: types/point.cc types/point.h
	$(CC) -c $(CFLAGS) $<

.PHONY: clean

clean:
	rm *.o
