#include <iostream>
#include <string>
#include <cassert>

#include "splay_tree.hpp"

template <typename T>
struct Ident
{
	T const & operator()(T const & t) const
	{ return t; }
};

typedef SplayTree<int, int, Ident<int>, std::less<int> > TreeType;

int main()
{
	SplayNode<std::string> string_node;
	SplayNode<int> int_node;

	SplayIterator<std::string> string_iterator(&string_node);
	SplayConstIterator<std::string> string_const_iterator(&string_node);

	SplayIterator<int> int_iterator(&int_node);
	SplayConstIterator<int> int_const_iterator(&int_node);

	assert(string_iterator == string_const_iterator);
	assert(int_iterator == int_const_iterator);

	++string_iterator;
	string_const_iterator++;

	assert(string_iterator.m_node == 0);
	assert(string_const_iterator.m_node == 0);

	int_iterator++;
	++int_const_iterator;

	assert(int_iterator.m_node == 0);
	assert(int_const_iterator.m_node == 0);

	TreeType tree;
	assert(tree.begin() == tree.end());
	assert(tree.rbegin() == tree.rend());

	std::pair<TreeType::iterator, bool> res1 = tree.insert_unique(5);
	assert(res1.second);
	assert(res1.first == tree.begin());
	assert(*res1.first == 5);
	++res1.first;
	assert(res1.first == tree.end());

	std::pair<TreeType::iterator, bool> res2 = tree.insert_unique(4);
	assert(res2.second);
	assert(res2.first == tree.begin());
	assert(*res2.first == 4);
	++res2.first;
	assert(*res2.first == 5);
	++res2.first;
	assert(res2.first == tree.end());

	std::pair<TreeType::iterator, bool> res3 = tree.insert_unique(3);
	assert(res3.second);
	assert(res3.first == tree.begin());
	assert(*res3.first == 3);
	++res3.first;
	assert(*res3.first == 4);
	++res3.first;
	assert(*res3.first == 5);
	++res3.first;
	assert(res3.first == tree.end());

	std::pair<TreeType::iterator, bool> res4 = tree.insert_unique(4);
	assert(!res4.second);
	assert(*res4.first == 4);
	--res4.first;
	assert(*res4.first == 3);
	assert(res4.first == tree.begin());
	++res4.first;
	++res4.first;
	assert(*res4.first == 5);
	++res4.first;
	assert(res4.first == tree.end());

	std::pair<TreeType::iterator, bool> res5 = tree.insert_unique(2);
	assert(res5.second);
	assert(res5.first == tree.begin());
	assert(*res5.first == 2);
	++res5.first;
	assert(*res5.first == 3);
	++res5.first;
	assert(*res5.first == 4);
	++res5.first;
	assert(*res5.first == 5);
	++res5.first;
	assert(res5.first == tree.end());
	--res5.first;
	assert(*res5.first == 5);
	--res5.first;
	assert(*res5.first == 4);
	--res5.first;
	assert(*res5.first == 3);
	--res5.first;
	assert(*res5.first == 2);
	assert(res5.first == tree.begin());

	std::cout << "test is successfully passed" << std::endl;

	return 0;
}
