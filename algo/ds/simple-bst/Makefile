CC = g++
CFLAGS = -g -Wall -Wextra -Werror -pedantic -std=c++11

all: test

test: test.o treenode.o
	$(CC) test.o treenode.o -o test

treenode.o: treenode.cpp treenode.hpp
	$(CC) $(CFLAGS) -c treenode.cpp -o treenode.o

test.o: test.cpp bst.hpp
	$(CC) $(CFLAGS) -c test.cpp -o test.o

clean:
	rm -f *.o test

.PHONY: clean
