CC=clang++
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c++11

default: bind

bind: bind.o
	$(CC) bind.o -o bind

bind.o: bind.cpp
	$(CC) $(CFLAGS) -c bind.cpp -o bind.o

clean:
	rm -rf *.o bind

.PHONY : clean
