#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include <iostream>
#include <fstream>

#include "include_extractor.hpp"
#include "files_collector.hpp"
#include "dependencies.hpp"

namespace fs = boost::filesystem;

static void analyze_includes(fs::path const & f);

fs::path		root;
std::vector<fs::path>	dirs;
graph<fs::path>		dep;

template <typename Functor>
void iterate_over_filesystem(fs::path const & dir, Functor func)
{
	boost::filesystem::recursive_directory_iterator end;
	for (fs::recursive_directory_iterator it(dir); it != end; ++it)
		if (boost::filesystem::exists(*it))
			func(it->path());
}

static void default_cpp_extensions(std::vector<std::string> & extensions)
{
	extensions.push_back(".cpp");
	extensions.push_back(".cxx");
}

static void print_usage()
{
	std::cout << "usage:" << std::endl
		<< "\tanalyser <sources path> [options]" << std::endl << std::endl
		<< "\twhere [options]:" << std::endl
		<< "\t\t-I <path>     -- additional headers search path" << std::endl;
}

static void analyze_globals(fs::path const &f, std::vector<std::string> const & globals)
{
	for (std::vector<std::string>::const_iterator iit(globals.begin()); iit != globals.end(); ++iit)
	{
		bool found = false;
		for (std::vector<fs::path>::const_iterator dit(dirs.begin()); !found && (dit != dirs.end()); ++dit)
		{
			fs::path target(*dit / fs::path(*iit));
			if (fs::exists(target))
			{
				found = true;
				fs::path full(fs::canonical(target));
				bool appended = dep.add_node(full);
				dep.add_edge(f, full);
				if (appended)
					analyze_includes(full);
			}				
		}
		if (!found)
			dep.add_edge(f, *iit);
	}
}

static void analyze_locals(fs::path const &f, std::vector<std::string> const & locals)
{
	fs::path dir(f.parent_path());
	for (std::vector<std::string>::const_iterator it(locals.begin()); it != locals.end(); ++it)
	{
		fs::path target(dir / fs::path(*it));
		if (fs::exists(target))
		{
			fs::path full(fs::canonical(target));
			bool appended = dep.add_node(full);
			dep.add_edge(f, full);
			if (appended)
				analyze_includes(full);
		}
		else
			dep.add_edge(f, fs::path(*it));
	}
}

static void analyze_includes(fs::path const & f)
{
	std::vector<std::string> locals;
	std::vector<std::string> globals;

	std::ifstream ifs(f.string().c_str());
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	ifs.close();

	include_extractor extractor;
	extractor(content.begin(), content.end(), std::back_inserter(locals), std::back_inserter(globals));
	analyze_locals(f, locals);
	analyze_globals(f, globals);
}

static void print_dependencies(fs::path const & f, std::set<fs::path> & visited, size_t level = 0)
{
	std::pair<std::set<fs::path>::iterator, bool> result = visited.insert(f);
	if (!result.second)
		return;

	std::cout << std::string(level, '.') << f.filename().string();
	if (!fs::exists(f))
		std::cout << " (!)";
	std::cout << std::endl;

	graph<fs::path>::node_iterator nit(dep.find(f));
	if (nit != dep.node_end())
		for (graph<fs::path>::edge_iterator eit(dep.edge_begin(nit)); eit != dep.edge_end(nit); ++eit)
			print_dependencies(*eit, visited, level + 2);

	visited.erase(result.first);
}

struct entry_t {
	std::string	_path;
	size_t		_power;

	entry_t(std::string const &path, size_t power) : _path(path), _power(power) { }
	entry_t() { }

	bool operator<(entry_t const & other) const
	{ return (_power == other._power) ? (_path < other._path) : (_power > other._power); }
};

static void analyze()
{
	include_extractor extractor;

	std::vector<std::string> extensions;
	default_cpp_extensions(extensions);

	files_collector cpps(extensions.begin(), extensions.end());
	iterate_over_filesystem(root, boost::bind(boost::ref(cpps), _1));
	std::set<fs::path> const & root_set = cpps.get_files();

	for (std::set<fs::path>::const_iterator it(root_set.begin()); it != root_set.end(); ++it)
		analyze_includes(*it);

	for (std::set<fs::path>::const_iterator it(root_set.begin()); it != root_set.end(); ++it)
	{
		std::set<fs::path> visited;
		print_dependencies(*it, visited);
	}

	std::cout << std::endl;

	std::vector<entry_t> entries;
	for (graph<fs::path>::node_iterator it(dep.node_begin()); it != dep.node_end(); ++it)
		entries.push_back(entry_t(it->filename().string(), dep.in_degree(it)));
	std::sort(entries.begin(), entries.end());
	for (std::vector<entry_t>::const_iterator it(entries.begin()); it != entries.end(); ++it)
		std::cout << it->_path << " " << it->_power << std::endl;
}

static void print_dependencies()
{
	std::set<fs::path> visited;
	graph<fs::path>::node_iterator it(dep.node_begin());
}

int main(int argc, char **argv)
{
	if (argc < 2 || argc % 2 != 0)
	{
		print_usage();
		return 1;
	}

	root = fs::path(argv[1]);
	for (std::size_t i = 2; i < (std::size_t)argc; i += 2)
	{
		std::string option(argv[i]);
		if (option != "-I")
		{
			print_usage();
			return 1;
		}
		dirs.push_back(argv[i + 1]);
	}

	if (!fs::exists(root) || !fs::is_directory(root))
	{
		std::cout << "directory " << root << " doesn't exists or isn't directory" << std::endl;
		return 2;
	}

	for (std::vector<fs::path>::const_iterator it(dirs.begin()); it != dirs.end(); ++it)
	{
		if (!fs::exists(*it) || !fs::is_directory(*it))
		{
			std::cout << "directory " << *it << " doesn't exists or isn't directory" << std::endl;
			return 2;
		}
	}

	analyze();

	return 0;
}
