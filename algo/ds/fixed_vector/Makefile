LIBS = -lboost_unit_test_framework
CXX = clang++-3.5
CXXFLAGS = -Wall -Wextra -Werror -std=c++11 -pedantic -MMD

all: test

TESTS = access_test.o

test: $(TESTS)
	$(CXX) $(LIBS) $^ -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include: *.d

clean:
	rm -rf *.o *.d test

.PHONY: clean
