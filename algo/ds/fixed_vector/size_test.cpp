#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#	define BOOST_TEST_MODULE Size
#endif
#include <boost/test/unit_test.hpp>
#include <string>

#include "fixed_vector.hpp"

BOOST_AUTO_TEST_SUITE(SizeTests)

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

BOOST_AUTO_TEST_CASE(testSizeNonEmptyInt)
{
	fixed_vector<int> nonempty(42);
	nonempty.push_back(1);
	BOOST_CHECK(nonempty.size() == 1);
	nonempty.push_back(42);
	BOOST_CHECK(nonempty.size() == 2);
}

BOOST_AUTO_TEST_CASE(testSizeNonEmptyString)
{
	fixed_vector<std::string> nonempty(42);
	nonempty.push_back("test");
	BOOST_CHECK(nonempty.size() == 1);
	nonempty.push_back("passed");
	BOOST_CHECK(nonempty.size() == 2);
}

BOOST_AUTO_TEST_CASE(testEmptyNonEmptyInt)
{
	fixed_vector<int> nonempty(42);
	nonempty.push_back(1);
	BOOST_CHECK(!nonempty.empty());
	nonempty.push_back(42);
	BOOST_CHECK(!nonempty.empty());
}

BOOST_AUTO_TEST_CASE(testEmptyNonEmptyString)
{
	fixed_vector<std::string> nonempty(42);
	nonempty.push_back("test");
	BOOST_CHECK(!nonempty.empty());
	nonempty.push_back("passed");
	BOOST_CHECK(!nonempty.empty());
}

BOOST_AUTO_TEST_CASE(testSizeFullInt)
{
	const size_t CAPACITY = 42;
	fixed_vector<int> full(CAPACITY);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(i);
	BOOST_CHECK(full.size() == full.capacity());
	BOOST_CHECK(full.size() == CAPACITY);
}

BOOST_AUTO_TEST_CASE(testSizeFullString)
{
	const size_t CAPACITY = 42;
	fixed_vector<std::string> full(CAPACITY);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(std::to_string(i));
	BOOST_CHECK(full.size() == full.capacity());
	BOOST_CHECK(full.size() == CAPACITY);
}

BOOST_AUTO_TEST_CASE(testEmptyFullInt)
{
	fixed_vector<int> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(i);
	BOOST_CHECK(!full.empty());
}

BOOST_AUTO_TEST_CASE(testEmptyFullString)
{
	fixed_vector<std::string> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(std::to_string(i));
	BOOST_CHECK(!full.empty());
}

BOOST_AUTO_TEST_CASE(testSizeZeroCapacityInt)
{
	fixed_vector<int> zero(0);
	BOOST_CHECK(zero.capacity() == 0);
	BOOST_CHECK(zero.size() == 0);
}

BOOST_AUTO_TEST_CASE(testSizeZeroCapacityString)
{
	fixed_vector<std::string> zero(0);
	BOOST_CHECK(zero.capacity() == 0);
	BOOST_CHECK(zero.size() == 0);
}

BOOST_AUTO_TEST_CASE(testEmptyZeroCapacityInt)
{
	fixed_vector<int> zero(0);
	BOOST_CHECK(zero.empty());
}

BOOST_AUTO_TEST_CASE(testEmptyZeroCapacityString)
{
	fixed_vector<std::string> zero(0);
	BOOST_CHECK(zero.empty());
}

BOOST_AUTO_TEST_SUITE_END()

