#ifndef __LINE_ITERATOR_HPP__
#define __LINE_ITERATOR_HPP__

#include <iterator>
#include <string>

template <typename Stream>
class line_iterator
	: public std::iterator<std::input_iterator_tag, std::string>
{
public:
	line_iterator()
		: inp_(NULL), buff_(NULL), delim_(0)
	{ }

	line_iterator(Stream & in, std::string & buff, char delim)
		: inp_(&in), buff_(&buff), delim_(delim)
	{ increment(); }

	line_iterator(line_iterator const & other)
		: inp_(other.inp_), buff_(other.buff_), delim_(other.delim_)
	{ }

	line_iterator & operator=(line_iterator const & other)
	{
		inp_ = other.inp_;
		buff_ = other.buff_;
		delim_ = other.delim_;
		return *this;
	}

	std::string const & operator*() const
	{ return *buff_; }

	std::string const * operator->() const
	{ return buff_; }

	line_iterator & operator++()
	{
		increment();
		return *this;
	}

	line_iterator operator++(int)
	{
		line_iterator it(*this);
		increment();
		return it;
	}

	friend bool operator==(line_iterator const & lhs, line_iterator const & rhs)
	{ return lhs.buff_ == rhs.buff_; }

	friend bool operator!=(line_iterator const & lhs, line_iterator const & rhs)
	{ return !(lhs == rhs); }

private:
	Stream *inp_;
	std::string *buff_;
	char delim_;

	void increment()
	{
		if (!std::getline(*inp_, *buff_, delim_))
		{
			inp_ = NULL;
			buff_ = NULL;
			delim_ = 0;
		}
	}
};

#endif /*__LINE_ITERATOR_HPP__*/
