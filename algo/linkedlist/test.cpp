#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include "linkedlist.hpp"

void run_test(size_t size)
{
	std::vector<int> control;
	LinkedList<int> ints;

	for (size_t i = 0; i != size; ++i) {
		int const number = rand();
		control.push_back(number);
		ints.push_back(number);
	}

	assert(ints.size() == control.size());
	assert(std::equal(ints.begin(), ints.end(), control.begin()));
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "List of integerst expected" << std::endl;
		return 0;
	}

	for (int arg = 1; arg != argc; ++arg)
		run_test(atol(*(argv + arg)));

	return 0;
}
