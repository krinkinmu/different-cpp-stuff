#include "bst.hpp"

#include <algorithm>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <iterator>

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
	} while (0)
#else
#define LOG_CONTAINER(ct) do { } while(0)
#endif

template <typename T>
struct Id {
	T operator()(T x) const
	{ return x; }
};

using Tree = BinarySearchTree<int, int, Id<int>, std::less<int>>;

void run_range_construct_test(size_t size)
{
	using std::begin;
	using std::end;

	std::vector<int> source;
	fill_random(source, size);
	LOG_CONTAINER(source);

	Tree tree(begin(source), end(source));
	LOG_CONTAINER(tree);

	std::sort(begin(source), end(source));
	LOG_CONTAINER(source);

	assert(source.size() == tree.size());
	assert(std::equal(begin(tree), end(tree), begin(source)));
}

void run_clear_test(size_t size)
{
	using std::begin;
	using std::end;

	std::vector<int> source;
	fill_random(source, size);
	LOG_CONTAINER(source);

	Tree tree(begin(source), end(source));
	LOG_CONTAINER(tree);

	tree.clear();
	LOG_CONTAINER(tree);

	assert(tree.size() == 0);
}

int main()
{
	for (size_t size : {0, 1, 10, 100, 1000, 10000}) {
		run_clear_test(size);
		run_range_construct_test(size);
	}

	return 0;
}
