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

	SplayTree<int, int, Ident<int>, std::less<int> > tree;
	assert(tree.begin() == tree.end());
	assert(tree.rbegin() == tree.rend());

	std::cout << "test is successfully passed" << std::endl;

	return 0;
}
