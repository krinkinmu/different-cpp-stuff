CC=clang++
CFLAGS=-Weffc++ -Wall -Wextra -Werror -pedantic -std=c++03

default: parse

parse: parse.o
	$(CC) parse.o -o parse

parse.o: parse.cpp
	$(CC) $(CFLAGS) -c parse.cpp -o parse.o

clean:
	rm -rf *.o parse

.PHONY: clean
