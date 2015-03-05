#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Access
#include <boost/test/unit_test.hpp>

#include <string>

#include "fixed_vector.hpp"

BOOST_AUTO_TEST_CASE(testSizeEmptyInt)
{
	fixed_vector<int> empty(42);
	BOOST_CHECK(empty.size() == 0);
}

BOOST_AUTO_TEST_CASE(testSizeEmptyString)
{
	fixed_vector<std::string> empty(42);
	BOOST_CHECK(empty.size() == 0);
}

BOOST_AUTO_TEST_CASE(testEmptyEmptyInt)
{
	fixed_vector<int> empty(42);
	BOOST_CHECK(empty.empty());
}

BOOST_AUTO_TEST_CASE(testEmptyEmptyString)
{
	fixed_vector<std::string> empty(42);
	BOOST_CHECK(empty.empty());
}

