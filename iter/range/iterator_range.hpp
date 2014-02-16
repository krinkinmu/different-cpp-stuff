#ifndef __ITERATOR_RANGE_HPP__
#define __ITERATOR_RANGE_HPP__

template <typename FwdIt>
class iterator_range
{
public:
	template <typename FwdIt2>
	iterator_range(FwdIt2 begin, FwdIt2 end = FwdIt2())
		: begin_(begin), end_(end)
	{ }

	FwdIt begin() const
	{ return begin_; }

	FwdIt end() const
	{ return end_; }

	template <typename OthRng>
	iterator_range(OthRng const & rng)
		: begin_(rng.begin()), end_(rng.end())
	{ }

	template <typename OthRng>
	iterator_range(OthRng & rng)
		: begin_(rng.begin()), end_(rng.end())
	{ }

	template <typename OthRng>
	iterator_range & operator=(OthRng const & rng)
	{
		begin_ = rng.begin();
		end_ = rng.end();
		return *this;
	}

	template <typename OthRng>
	iterator_range & operator=(OthRng & rng)
	{
		begin_ = rng.begin();
		end_ = rng.end();
		return *this;
	}

private:
	FwdIt	begin_;
	FwdIt	end_;
};

template <typename FwdIt>
iterator_range<FwdIt> make_range(FwdIt begin, FwdIt end = FwdIt())
{ return iterator_range<FwdIt>(begin, end); }

#endif /*__ITERATOR_RANGE_HPP__*/
