CC=g++-7
CFLAGS=-c -Wall -Werror -std=c++17
LDFLAGS=-lm

all: ../mesherTool

../mesherTool: mesher.o
	$(CC) -o ../mesherTool ../IO/io.o $^ $(LDFLAGS)

mesher.o: mesher.cc
	$(CC) -c $(CFLAGS) $<

.PHONY: clean

clean:
	rm *.o
