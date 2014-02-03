CC=clang++
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c++11 -pthread

default: main

main: main.o
	$(CC) $(CFLAGS) main.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -rf *.o main

.PHONY : clean
