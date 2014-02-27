CC=clang++
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c++11

default: range

range: main.o
	$(CC) main.o -o range

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -rf *.o range

.PHONY : clean
