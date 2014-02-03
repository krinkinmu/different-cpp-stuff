#ifndef __DEPENDENCIES_HPP__
#define __DEPENDENCIES_HPP__

#include <iterator>
#include <vector>
#include <map>

template <typename Entry>
class graph
{
	typedef std::vector<size_t>		neighbours;
	typedef std::vector<size_t>		powers;
	typedef std::vector<neighbours>		edges;
	typedef std::map<Entry, size_t>		ids;
	typedef std::vector<Entry>		entries;

	ids		_ids;
	entries		_entries;
	edges		_edges;
	powers		_powers;

public:
	typedef typename entries::const_iterator		node_iterator;

	class edge_iterator : public std::iterator<std::bidirectional_iterator_tag, Entry>
	{
		friend class graph<Entry>;

		entries const *					_entries;
		neighbours::const_iterator			_iterator;

		edge_iterator(entries const * const nodes, neighbours::const_iterator const & it)
		: _entries(nodes), _iterator(it) { }
	public:
		edge_iterator() : _entries(NULL) { }
		edge_iterator(edge_iterator const & other) : _entries(other._entries), _iterator(other._iterator) { }

		edge_iterator & operator=(edge_iterator const & other)
		{
			_entries = other._entries;
			_iterator = other._iterator;
			return *this;
		}

		edge_iterator & operator--() { --_iterator; return *this; }
		edge_iterator const operator--(int) { edge_iterator it(*this); --_iterator; return it; }

		edge_iterator & operator++() { ++_iterator; return *this; }
		edge_iterator const operator++(int) { edge_iterator it(*this); ++_iterator; return it; }

		bool operator==(edge_iterator const & other) const { return _iterator == other._iterator; }
		bool operator!=(edge_iterator const & other) const { return !(*this == other); }

		Entry const & operator*() const { return _entries->at(*_iterator); }
		Entry const * operator->() const { return &(operator*()); }
	};

	bool add_node(Entry const & entry)
	{
		std::pair<typename ids::iterator, bool> result = _ids.insert(std::make_pair(entry, _ids.size()));
		if (result.second)
		{
			_entries.push_back(entry);
			_edges.push_back(neighbours());
			_powers.push_back(0);
		}
		return result.second;
	}

	void add_edge(Entry const & source, Entry const & destination)
	{
		add_node(source); add_node(destination);
		size_t src_id = _ids[source], dst_id = _ids[destination];
		_edges[src_id].push_back(dst_id);
		_powers[dst_id]++;
	}

	node_iterator const node_begin() const
	{ return _entries.begin(); }

	node_iterator const node_end() const
	{ return _entries.end(); }

	edge_iterator const edge_begin(node_iterator const &it) const
	{ return edge_iterator(&_entries, _edges[std::distance(_entries.begin(), it)].begin()); }

	edge_iterator const edge_end(node_iterator const &it) const
	{ return edge_iterator(&_entries, _edges[std::distance(_entries.begin(), it)].end()); }

	node_iterator const find(Entry const & entry) const
	{
		typename ids::const_iterator it(_ids.find(entry));
		if (it != _ids.end())
			return _entries.begin() + it->second;
		return node_end();
	}

	size_t in_degree(node_iterator const & it) const
	{ return _powers[std::distance(_entries.begin(), it)]; }

	size_t out_degree(node_iterator const & it) const
	{ return _edges[std::distance(_entries.begin(), it)].size(); }
};

#endif /*__DEPENDENCIES_HPP__*/
