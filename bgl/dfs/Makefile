CC=g++
CFLAGS=-Wall -Wextra -std=c++03 -pedantic 
LDFLAGS=-lboost_serialization

program: main.cpp
	$(CC) main.cpp $(CFLAGS) $(LDFLAGS) -o program

clean:
	rm -rf *.o program

.PHONY : clean
