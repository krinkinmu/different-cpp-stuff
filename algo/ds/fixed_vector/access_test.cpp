#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#	define BOOST_TEST_MODULE Access
#endif
#include <boost/test/unit_test.hpp>
#include <string>

#include "fixed_vector.hpp"

BOOST_AUTO_TEST_SUITE(AccessTests)

BOOST_AUTO_TEST_CASE(testSquareBracketsInt)
{
	fixed_vector<int> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(i);
	for (size_t i = 0; i != full.capacity(); ++i)
		BOOST_CHECK_EQUAL(full[i], i);
}

BOOST_AUTO_TEST_CASE(testSquareBracketsString)
{
	fixed_vector<std::string> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(std::to_string(i));
	for (size_t i = 0; i != full.capacity(); ++i)
		BOOST_CHECK_EQUAL(full[i], std::to_string(i));
}

BOOST_AUTO_TEST_CASE(testAtInt)
{
	fixed_vector<int> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(i);
	for (size_t i = 0; i != full.capacity(); ++i)
		BOOST_CHECK_EQUAL(full.at(i), i);
}

BOOST_AUTO_TEST_CASE(testAtString)
{
	fixed_vector<std::string> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(std::to_string(i));
	for (size_t i = 0; i != full.capacity(); ++i)
		BOOST_CHECK_EQUAL(full.at(i), std::to_string(i));
}

BOOST_AUTO_TEST_CASE(testBackInt)
{
	fixed_vector<int> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(i);
	BOOST_CHECK_EQUAL(full.back(), full.size() - 1);
	full.pop_back();
	BOOST_CHECK_EQUAL(full.back(), full.size() - 1);
}

BOOST_AUTO_TEST_CASE(testBackString)
{
	fixed_vector<std::string> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(std::to_string(i));
	BOOST_CHECK_EQUAL(full.back(), std::to_string(full.size() - 1));
	full.pop_back();
	BOOST_CHECK_EQUAL(full.back(), std::to_string(full.size() - 1));	
}

BOOST_AUTO_TEST_CASE(testFrontInt)
{
	fixed_vector<int> nonempty(42);
	nonempty.push_back(42);
	BOOST_CHECK_EQUAL(nonempty.front(), 42);
}

BOOST_AUTO_TEST_CASE(testFrontString)
{
	fixed_vector<std::string> nonempty(42);
	nonempty.push_back("test");
	BOOST_CHECK_EQUAL(nonempty.front(), "test");
}

BOOST_AUTO_TEST_CASE(testDataInt)
{
	fixed_vector<int> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(i);
	for (size_t i = 0; i != full.capacity(); ++i)
		BOOST_CHECK_EQUAL(*(full.data() + i), i);
}

BOOST_AUTO_TEST_CASE(testDataString)
{
	fixed_vector<std::string> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(std::to_string(i));
	for (size_t i = 0; i != full.capacity(); ++i)
		BOOST_CHECK_EQUAL(*(full.data() + i), std::to_string(i));
}

BOOST_AUTO_TEST_SUITE_END()
