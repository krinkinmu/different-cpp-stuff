CC=clang++
CFLAGS=-Weffc++ -Wall -Wextra -Werror -pedantic -std=c++03

default: same

same: same.o
	$(CC) same.o -o same

same.o: same.cpp
	$(CC) $(CFLAGS) -c same.cpp -o same.o

clean:
	rm -rf *.o same

.PHONY : clean
