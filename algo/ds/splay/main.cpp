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
	TreeType tree;
	assert(tree.begin() == tree.end());
	assert(tree.rbegin() == tree.rend());
	assert(tree.size() == 0);

	std::pair<TreeType::iterator, bool> res1 = tree.insert_unique(5);
	assert(res1.second);
	assert(res1.first == tree.begin());
	assert(*res1.first == 5);
	++res1.first;
	assert(res1.first == tree.end());
	assert(tree.size() == 1);

	std::pair<TreeType::iterator, bool> res2 = tree.insert_unique(4);
	assert(res2.second);
	assert(res2.first == tree.begin());
	assert(*res2.first == 4);
	++res2.first;
	assert(*res2.first == 5);
	++res2.first;
	assert(res2.first == tree.end());
	assert(tree.size() == 2);

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
	assert(tree.size() == 3);

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
	assert(tree.size() == 3);

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
	assert(tree.size() == 4);

	TreeType::iterator it1 = tree.insert(1);
	assert(it1 == tree.begin());
	assert(*it1 == 1);
	++it1;
	assert(*it1 == 2);
	++it1;
	assert(*it1 == 3);
	it1++;
	assert(*it1 == 4);
	it1++;
	assert(*it1 == 5);
	it1++;
	assert(it1 == tree.end());
	assert(tree.size() == 5);

	TreeType::iterator it2 = tree.insert(1);
	assert(*it2 == 1);
	assert(it2 == tree.begin() || --it2 == tree.begin());
	assert(tree.size() == 6);

	std::pair<TreeType::iterator, TreeType::iterator> its = tree.equal_range(1);
	assert(its.first != its.second);
	assert(its.first == tree.begin());
	assert(*its.first == 1);
	++its.first;
	assert(its.first != its.second);
	assert(*its.first == 1);
	its.first++;
	assert(its.first == its.second);
	assert(*its.first == 2);
	assert(*its.second == 2);

	TreeType::iterator it3 = tree.find(5);
	assert(*it3 == 5);
	++it3;
	assert(it3 == tree.end());
	--it3;
	TreeType::iterator it4 = tree.erase(it3);
	assert(tree.size() == 5);
	assert(it4 == tree.end());
	TreeType::iterator it5 = tree.find(5);
	assert(it5 == tree.end());

	std::pair<TreeType::iterator, TreeType::iterator> its2 = tree.equal_range(1);
	TreeType::iterator it6 = tree.erase(its2.first, its2.second);
	assert(tree.size() == 3);
	assert(it6 == its2.second);
	assert(*it6 == 2);

	TreeType::iterator it7 = tree.find(1);
	assert(it7 == tree.end());

	TreeType other = tree;
	assert(other.size() == tree.size());
	other.insert(20);
	assert(other.size() == tree.size() + 1);
	tree = other;
	assert(other.size() == tree.size());

	std::cout << "test is successfully passed" << std::endl;

	return 0;
}
