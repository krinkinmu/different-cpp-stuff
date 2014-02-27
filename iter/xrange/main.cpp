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
	range_iterator(ValTy value, DiffTy step, long num = 0)
		: value_(value), step_(step), step_num_(num)
	{ }

	void increment()
	{ ++step_num_; }

	void decrement()
	{ --step_num_; }

	bool equal(range_iterator const & other) const
	{ return step_num_ == other.step_num_; }

	ValTy dereference() const
	{ return value_ + step_ * step_num_; }

	void advance(long n)
	{ step_num_ += n; }

	long distance_to(range_iterator const & other) const
	{
		if (other.step_num_ < step_num_)
		   return -(step_num_ - other.step_num_);
		return other.step_num_ - step_num_;
	}

private:
	ValTy value_;
	DiffTy step_;
	long step_num_;
};

namespace detail
{

	template <typename ValTy, typename DiffTy>
	struct xrange_storage
	{
		typedef range_iterator<ValTy, DiffTy> iterator;

		xrange_storage(ValTy begin, ValTy end, DiffTy step)
			: begin_(begin), step_(step), steps_(static_cast<long>((end - begin) / step))
		{ }

		iterator begin()
		{ return range_iterator<ValTy, DiffTy>(begin_, step_, 0); }

		iterator end()
		{ return range_iterator<ValTy, DiffTy>(begin_, step_, steps_); }

		ValTy begin_;
		DiffTy step_;
		long steps_;
	};

}

template <typename ValTy, typename DiffTy>
detail::xrange_storage<ValTy, DiffTy> xrange(ValTy begin, ValTy end, DiffTy step)
{
	return detail::xrange_storage<ValTy, DiffTy>(begin, end, step);
}

template <typename ValTy>
detail::xrange_storage<ValTy, ValTy> xrange(ValTy begin, ValTy end)
{
	return detail::xrange_storage<ValTy, ValTy>(begin, end, static_cast<ValTy>(1));
}

int main()
{

	for (auto v : xrange(1, 10))
		std::cout << v << " ";
	std::cout << std::endl;

	for (auto v : xrange(1.0, 10.0, 0.9))
		std::cout << v << " ";
	std::cout << std::endl;

	return 0;

}
