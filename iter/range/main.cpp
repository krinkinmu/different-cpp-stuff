#include <algorithm>
#include <fstream>
#include <iostream>

#include "iterator_range.hpp"
#include "line_iterator.hpp"

namespace detail {
	struct lines_range_storage { std::string buff_; };
}

template <typename Stream>
class lines_range
		: private detail::lines_range_storage
		, public iterator_range< line_iterator<Stream> >
{
	typedef iterator_range< line_iterator<Stream> > Base;
public:
	lines_range(Stream & in, char delim)
		: Base(line_iterator<Stream>(in, buff_, delim), line_iterator<Stream>())
	{ }
};

template <typename Stream>
lines_range<Stream> getlines(Stream & in, char delim = '\n')
{ return lines_range<Stream>(in, delim); }

int main(int argc, char **argv)
{
	for (int it = 1; it < argc; ++it)
	{
		std::ifstream inp(*(argv + it));
		lines_range<std::ifstream> const range(getlines(inp));
		std::copy(range.begin(), range.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
	}

	return 0;
}
