#ifndef __INCLUDE_EXTRACTOR_HPP__
#define __INCLUDE_EXTRACTOR_HPP__

#include <iterator>
#include <string>
#include <vector>

#include "dependencies.hpp"

class include_extractor
{
	template <typename Iterator>
	Iterator skip_whitespaces(Iterator it, Iterator end)
	{
		while (it != end && (*it == ' ' || *it == '\t'))
			++it;
		return it;
	}

	template <typename Iterator>
	Iterator finish_line(Iterator it, Iterator end)
	{
		while (it != end && *it++ != '\n');
		return it;
	}

	template <typename Iterator>
	Iterator parse_word(Iterator it, Iterator end, std::string & word)
	{
		while (it != end && isalnum(*it))
			word.push_back(*it++);
		return it;
	}

	template <typename Iterator>
	Iterator parse_comment(Iterator it, Iterator end)
	{
		switch (*it)
		{
		case '/':
			it = finish_line(++it, end);
			break;
		case '*':
			while (++it != end)
				if ((*it == '*' && ++it != end && *it == '/') || (it == end))
					break;
			if (it != end)
				++it;
			break;
		default:
			break;
		}
		return it;
	}

	template <typename Iterator>
	Iterator parse_literal(Iterator it, Iterator end)
	{
		while (it != end && *it != '"')
			if ((*it != '\\') || (*it == '\\' && ++it != end))
				++it;
		if (it != end)
			++it;
		return it;
	}

	template <typename Iterator, typename Output>
	Iterator parse_include_file_name(Iterator it, Iterator end, Output locals, Output globals)
	{
		if (it != end && (*it == '"' || *it == '<'))
		{
			typename std::iterator_traits<Iterator>::value_type c(*it++);
			typename std::iterator_traits<Iterator>::value_type e(c == '"' ? '"' : '>');
			bool local = (c == '"') ? true : false;

			std::string fname;
			while (it != end && *it != e && *it != '\n')
				fname.push_back(*it++);

			if (it != end && *it == e)
			{
				if (local)
					*locals++ = fname;
				else
					*globals++ = fname;
			}
			it = finish_line(it, end);
		}
		return it;
	}

	template <typename Iterator, typename Output>
	Iterator parse_directive(Iterator it, Iterator end, Output locals, Output globals)
	{
		std::string word;
		it = skip_whitespaces(parse_word(skip_whitespaces(it, end), end, word), end);
		if (word == "include")
			it = parse_include_file_name(it, end, locals, globals);
		return it;
	}
public:
	template <typename Iterator, typename Output>
	void operator ()(Iterator it, Iterator end, Output locals, Output globals)
	{
		while (it != end)
		{
			switch (*it)
			{
			case '/':
				it = parse_comment(++it, end);
				break;
			case '"':
				it = parse_literal(++it, end);
				break;
			case '#':
				it = parse_directive(++it, end, locals, globals);
				break;
			default:
				++it;
				break;
			}
		}
	}
};

#endif /*__INCLUDE_EXTRACTOR_HPP__*/
