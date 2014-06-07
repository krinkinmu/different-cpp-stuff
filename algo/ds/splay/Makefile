CXX ?= g++
CPPFLAGS += -Wall -Werror -pedantic -std=c++03

default: splay_node_base.o

splay_node_base.o : splay_node_base.cpp splay_node_base.hpp
	$(CXX) -c $(CPPFLAGS) splay_node_base.cpp -o splay_node_base.o

clean:
	rm -f *.o

.PHONY : clean
