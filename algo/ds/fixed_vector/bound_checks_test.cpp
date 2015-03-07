#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#	define BOOST_TEST_MODULE BoundChecks
#endif
#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <string>

#include "fixed_vector.hpp"

BOOST_AUTO_TEST_SUITE(BoundCheckTests)

BOOST_AUTO_TEST_CASE(testPushBackOverflowZeroCapacityInt)
{
	fixed_vector<int> zero(0);
	BOOST_CHECK_THROW(zero.push_back(0), std::length_error);
}

BOOST_AUTO_TEST_CASE(testPushBackOverflowZeroCapacityString)
{
	fixed_vector<std::string> zero(0);
	BOOST_CHECK_THROW(zero.push_back("test is failed"), std::length_error);
}

BOOST_AUTO_TEST_CASE(testPushBackOverflowFullCapacityInt)
{
	fixed_vector<int> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(i);
	BOOST_CHECK_THROW(full.push_back(100500), std::length_error);
}

BOOST_AUTO_TEST_CASE(testPushBackOverflowFullCapacityString)
{
	fixed_vector<std::string> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(std::to_string(i));
	BOOST_CHECK_THROW(full.push_back("exception"), std::length_error);
}

BOOST_AUTO_TEST_CASE(testAtOutOfRangeZeroCapacityInt)
{
	fixed_vector<int> zero(0);
	BOOST_CHECK_THROW(zero.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(testAtOutOfRangeZeroCapacityString)
{
	fixed_vector<std::string> zero(0);
	BOOST_CHECK_THROW(zero.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(testAtOutOfRangeFullCapacityInt)
{
	fixed_vector<int> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(i);
	BOOST_CHECK_THROW(full.at(full.size()), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(testAtOutOfRangeFullCapacityString)
{
	fixed_vector<std::string> full(42);
	for (size_t i = 0; i != full.capacity(); ++i)
		full.push_back(std::to_string(i));
	BOOST_CHECK_THROW(full.at(full.size()), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(testAtOutOfRangeNonEmptyInt)
{
	fixed_vector<int> nonempty(42);
	nonempty.push_back(0);
	BOOST_CHECK_THROW(nonempty.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(testAtOutOfRangeNonEmptyString)
{
	fixed_vector<std::string> nonempty(42);
	nonempty.push_back("last");
	BOOST_CHECK_THROW(nonempty.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(testPopBackZeroCapacityInt)
{
	fixed_vector<int> zero(0);
	BOOST_CHECK_THROW(zero.pop_back(), std::length_error);
}

BOOST_AUTO_TEST_CASE(testPopBackZeroCapacityString)
{
	fixed_vector<std::string> zero(0);
	BOOST_CHECK_THROW(zero.pop_back(), std::length_error);
}

BOOST_AUTO_TEST_CASE(testPopBackNonEmptyInt)
{
	fixed_vector<int> nonempty(42);
	nonempty.push_back(1);
	nonempty.pop_back();
	BOOST_CHECK_THROW(nonempty.pop_back(), std::length_error);
}

BOOST_AUTO_TEST_CASE(testPopBackNonEmptyString)
{
	fixed_vector<std::string> nonempty(42);
	nonempty.push_back("first");
	nonempty.pop_back();
	BOOST_CHECK_THROW(nonempty.pop_back(), std::length_error);
}

BOOST_AUTO_TEST_SUITE_END()
