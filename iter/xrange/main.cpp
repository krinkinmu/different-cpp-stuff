#include <type_traits>
#include <iostream>

#include <boost/iterator/iterator_facade.hpp>

template <typename ValTy, typename DiffTy = ValTy>
class range_iterator
	: public boost::iterator_facade<
	  		range_iterator<ValTy, DiffTy>,
			ValTy,
			boost::random_access_traversal_tag,
			ValTy,
			long>
{
public:
	range_iterator(ValTy value, DiffTy step)
		: value_(value), step_(step)
	{ }

	void increment()
	{ value_ += step_; }

	void decrement()
	{ value_ -= step_; }

	bool equal(range_iterator const & other) const
	{ return value_ == other.value_; }

	ValTy dereference() const
	{ return value_; }

	void advance(long n)
	{ value_ += n * step_; }

	DiffTy distance_to(range_iterator const & other) const
	{ return other.value_ - value_; }

private:
	ValTy value_;
	DiffTy step_;
};

namespace detail
{

	template <typename ValTy, typename DiffTy>
	struct xrange_storage
	{
		typedef range_iterator<ValTy, DiffTy> iterator;

		xrange_storage(ValTy begin, ValTy end, DiffTy step)
			: begin_(begin), end_(end), step_(step)
		{ }

		iterator begin()
		{ return range_iterator<ValTy, DiffTy>(begin_, step_); }

		iterator end()
		{ return range_iterator<ValTy, DiffTy>(end_, step_); }

		ValTy begin_;
		ValTy end_;
		DiffTy step_;
	};

}

template <typename ValTy, typename DiffTy>
detail::xrange_storage<ValTy, DiffTy> xrange(ValTy begin, ValTy end, DiffTy step)
{
	static_assert(std::is_integral<ValTy>::value && std::is_integral<DiffTy>::value, "integral type required");
	return detail::xrange_storage<ValTy, DiffTy>(begin, end, step);
}

template <typename ValTy>
detail::xrange_storage<ValTy, ValTy> xrange(ValTy begin, ValTy end)
{
	static_assert(std::is_integral<ValTy>::value, "integral type required");
	return detail::xrange_storage<ValTy, ValTy>(begin, end, static_cast<ValTy>(1));
}

int main()
{

	for (auto v : xrange(1, 10))
		std::cout << v << " ";
	std::cout << std::endl;

//	for (auto v : xrange(1.0, 10.0, 0.9))
//		std::cout << v << " ";
//	std::cout << std::endl;

	return 0;

}
