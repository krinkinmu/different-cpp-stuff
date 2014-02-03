CC=clang++
CFLAGS=-Weffc++ -Wall -Wextra -Werror -pedantic -std=c++03

default: simple

simple: simple.o
	$(CC) simple.o -o simple

simple.o: simple.cpp
	$(CC) $(CFLAGS) -c simple.cpp -o simple.o

clean:
	rm -rf *.o simple

.PHONY: clean
