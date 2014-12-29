CXX ?= g++
CPPFLAGS += -Wall -Werror -pedantic -std=c++11 -pthread

default: thread_group.o

thread_group.o : thread_group.cpp thread_group.hpp
	$(CXX) -c $(CPPFLAGS) thread_group.cpp -o thread_group.o

clean:
	rm -f *.o

.PHONY : clean
