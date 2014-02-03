#ifndef __BOTTOM_UP_MERGE_SORT_HPP__
#define __BOTTOM_UP_MERGE_SORT_HPP__

#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>

template < typename IT, typename LN >
IT safe_advance(IT b, IT e, LN adv)
{
	std::advance(b, std::min(static_cast<LN>(std::distance(b, e)), adv));
	return b;
}

template < typename IT, typename CP >
void msort(IT b, IT e, CP cmp)
{
	typedef std::vector<typename std::iterator_traits<IT>::value_type> buffer_t;
	typedef typename std::iterator_traits<IT>::difference_type length_t;

	length_t const len = std::distance(b, e);
	buffer_t buffer(b, e);

	for (length_t blen = 1; blen < len; blen *= 2)
	{
		for (IT it = b; it != e;)
		{
			IT first = it;
			IT middle = safe_advance(first, e, blen);
			IT last = safe_advance(middle, e, blen);

			typename buffer_t::iterator buffer_end = std::copy(first, middle, buffer.begin());
			it = std::merge(buffer.begin(), buffer_end, middle, last, it, cmp);
		}
	}
}

template < typename IT >
void msort(IT b, IT e)
{
	msort(b, e, std::less<typename std::iterator_traits<IT>::value_type>());
}

#endif /*__BOTTOM_UP_MERGE_SORT_HPP__*/
