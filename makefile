CC=g++-7
CFLAGS=-c -Wall -Werror -std=c++17

all: io.o exc.o showTool mesherTool

io.o: IO/io.cc IO/io.h
	 $(CC) -c $< $(CFLAGS)

exc.o: exc/exc.cc exc/exc.h
	$(CC) -c $< $(CFLAGS)

showTool: show.o point.o readfile.o
	$(CC) -o showTool $^ -lsfml-graphics -lsfml-window -lsfml-system -lm

show.o: show/show.cc
	$(CC) -c $(CFLAGS) $<

readfile.o: show/IO/readfile.cc show/IO/readfile.h
	$(CC) -c $(CFLAGS) $<

point.o: show/types/point.cc show/types/point.h
	$(CC) -c $(CFLAGS) $<

mesherTool: mesher.o IO
	$(CC) -o mesherTool io.o $< $(LDFLAGS)

mesher.o: mesher/mesher.cc
	$(CC) -c $(CFLAGS) $<

.PHONY: clean

clean:
	rm *.o
