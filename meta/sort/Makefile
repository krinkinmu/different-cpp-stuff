CC=clang++
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c++11

default: program

program: main.o
	$(CC) main.o -o program

main.o: main.cpp Makefile
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -rf *.o program

.PHONY : clean
