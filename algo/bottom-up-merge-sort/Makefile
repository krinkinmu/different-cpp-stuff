CC=clang++
CFLAGS=-Weffc++ -Wall -Wextra -Werror -pedantic -std=c++03

default: sort

sort: main.o
	$(CC) main.o -o sort

main.o: main.cpp msort.hpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -rf *.o sort

.PHONY: clean
