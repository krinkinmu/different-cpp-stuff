#include <algorithm>
#include <iostream>

#include "msort.hpp"

template <typename T>
void out(T v)
{ std::cout << v << " "; }

int main()
{
	size_t values[] = {5, 4, 3, 1, 10, 12, 11, 9, 8, 7, 6, 2, 13};
	size_t const size = sizeof(values)/sizeof(values[0]);

	std::for_each(values, values + size, &out<size_t>);
	std::cout << std::endl;

	msort(values, values + size);

	std::for_each(values, values + size, &out<size_t>);
	std::cout << std::endl;

	return 0;
}
