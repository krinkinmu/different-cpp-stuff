CC=clang++
CFLAGS=-Weffc++ -Wall -Wextra -Werror -pedantic -std=c++03

default: bind

bind: main.o
	$(CC) main.o -o bind

main.o: main.cpp mybind.hpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -rf *.o bind

.PHONY: clean
