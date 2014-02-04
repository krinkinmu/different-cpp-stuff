CC=clang++
CFLAGS=-Weffc++ -Wall -Wextra -Werror -pedantic -std=c++03

default: has

has: main.o
	$(CC) main.o -o has

main.o: main.cpp Makefile
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -rf *.o has

.PHONY : clean
