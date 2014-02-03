#ifndef __FILES_COLLECTOR_HPP__
#define __FILES_COLLECTOR_HPP__

#include <boost/filesystem.hpp>
#include <functional>
#include <string>
#include <set>

namespace fs = boost::filesystem;

class files_collector : public std::unary_function<fs::path, void>
{
	std::set<std::string> const		_suffixes;
	std::set<fs::path>			_files;

public:
	template <typename Iterator>
	files_collector(Iterator const & begin, Iterator const & end) : _suffixes(begin, end) { }

	void operator()(fs::path const & path)
	{
		fs::path p(fs::canonical(path));
		if (fs::is_regular_file(p) && _suffixes.find(p.extension().string()) != _suffixes.end())
			_files.insert(p);
	}

	std::set<fs::path> const & get_files() const { return _files; }
};

#endif /*__FILES_COLLECTOR_HPP__*/
