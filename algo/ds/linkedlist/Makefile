CC = g++
CFLAGS = -g -Wall -Wextra -Werror -pedantic -std=c++11

all: test

test: test.o listhead.o
	$(CC) test.o listhead.o -o test

listhead.o: listhead.cpp listhead.hpp
	$(CC) $(CFLAGS) -c listhead.cpp -o listhead.o

test.o: test.cpp linkedlist.hpp
	$(CC) $(CFLAGS) -c test.cpp -o test.o

clean:
	rm -f *.o test

.PHONY: clean
