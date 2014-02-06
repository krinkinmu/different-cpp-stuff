CC=clang++
CFLAGS=-Weffc++ -Wall -Wextra -Werror -pedantic -std=c++03

default: size

size: main.o
	$(CC) main.o -o size

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -rf *.o size

.PHONY : clean
