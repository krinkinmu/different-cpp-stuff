CC=clang++
CFLAGS=-Weffc++ -Wall -Wextra -Werror -pedantic -std=c++03

default: auto

auto: main.o
	$(CC) main.o -o auto

main.o: main.cpp unique_ptr.hpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -rf *.o auto

.PHONY: clean
