#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>
#include "linkedlist.hpp"

template <typename Ct>
void fill_random(Ct &ct, size_t size)
{ generate_n(std::back_inserter(ct), size, &rand); }

template <typename Ct>
void print_container(Ct const &ct)
{
	using std::begin;
	using std::end;

	std::cout << "[";
	std::for_each(begin(ct), end(ct),
		[](typename Ct::const_reference v)
		{ std::cout << v << " "; });
	std::cout << "]";
}

#ifdef DEBUG
#define LOG_CONTAINER(ct)			\
	do {					\
		std::cout << #ct << ": ";	\
		print_container(ct);		\
		std::cout << std::endl;		\
	} while (0);
#else
#define LOG_CONTAINER(ct) do { } while(0);
#endif

template <typename Ct>
void log_container(Ct const &ct, char const *name)
{
	std::cout << name;
	print_container(ct);
	std::cout << std::endl;	
}

void run_fill_test(size_t size)
{
	using std::begin;
	using std::end;

	std::vector<int> source;
	fill_random(source, size);
	LOG_CONTAINER(source);

	LinkedList<int> ints;
	std::copy(begin(source), end(source), std::back_inserter(ints));
	LOG_CONTAINER(ints);

	assert(ints.size() == source.size());
	assert(std::equal(begin(ints), end(ints), begin(source)));
}

void run_range_construct_test(size_t size)
{
	std::vector<int> source;
	fill_random(source, size);
	LOG_CONTAINER(source);

	LinkedList<int> ints(begin(source), end(source));
	LOG_CONTAINER(ints);

	assert(source.size() == ints.size());
	assert(std::equal(begin(source), end(source), begin(ints)));
}

void run_copy_test(size_t size)
{
	LinkedList<int> source;
	fill_random(source, size);
	LOG_CONTAINER(source);

	LinkedList<int> copy(source);
	LOG_CONTAINER(copy);
	assert(source.size() == copy.size());
	assert(std::equal(begin(source), end(source), begin(copy)));
}

int main(void)
{
	for (size_t size : {0, 10, 100, 1000, 10000}) {
		run_fill_test(size);
		run_range_construct_test(size);
		run_copy_test(size);
	}

	return 0;
}
