CXX ?= g++
CPPFLAGS += -g -Wall -Werror -pedantic -std=c++03

SRCS = main.cpp splay_node_base.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

.PHONY : clean all

all: test

test: $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(OBJS) -o test

.cpp.o:
	$(CXX) $(CPPFLAGS) -c $< -o $@

%.d: %.cpp
	$(CXX) $(CPPFLAGS) -M $< > $@

clean:
	rm -f *.o *.d test

-include $(DEPS)
