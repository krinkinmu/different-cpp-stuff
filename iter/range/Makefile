CC=clang++
CFLAGS=-Weffc++ -Wall -Wextra -Werror -pedantic -std=c++03

default: lines

lines: main.o
	$(CC) main.o -o lines

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -rf *.o lines

.PHONY : clean
