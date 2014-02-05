CC=clang++
CFLAGS=-Weffc++ -Wall -Wextra -Werror -pedantic -std=c++03

default: convert

convert: main.o
	$(CC) main.o -o convert

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -rf *.o convert

.PHONY : clean
