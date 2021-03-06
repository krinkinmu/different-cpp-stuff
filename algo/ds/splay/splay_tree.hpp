#ifndef __SPLAY_TREE_HPP__
#define __SPLAY_TREE_HPP__

#include <iterator>
#include <memory>

#include "splay_iterator.hpp"
#include "splay_node.hpp"

template <typename KeyCmp, typename NodeAllocator>
struct SplayTreeImpl : public NodeAllocator
{
	SplayTreeImpl()
		: NodeAllocator()
		, m_cmp()
		, m_header()
		, m_size(0)
	{ Init(); }

	SplayTreeImpl(KeyCmp const & cmp, NodeAllocator const & a)
		: NodeAllocator(a)
		, m_cmp(cmp)
		, m_header()
		, m_size(0)
	{ Init(); }

	void Init() throw()
	{
		m_header.m_parent = 0;
		m_header.m_left = 0;
		m_header.m_right = 0;
	}

	KeyCmp m_cmp;
	SplayNodeBase m_header;
	size_t m_size;
};

template < typename Key, typename Val, typename KeyVal, typename Cmp,
			typename Alloc = std::allocator<Val> >
class SplayTree
{
	typedef typename Alloc::template rebind< SplayNode<Val> >::other
				NodeAllocator;
	typedef SplayTree<Key, Val, KeyVal, Cmp, Alloc> SelfType;

public:
	typedef Key key_type;
	typedef Val value_type;
	typedef value_type * pointer;
	typedef value_type const * const_pointer;
	typedef value_type & reference;
	typedef value_type const & const_reference;
	typedef size_t size_type;
	typedef Alloc allocator_type;

	typedef SplayIterator<value_type> iterator;
	typedef SplayConstIterator<value_type> const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	allocator_type get_allocator() const throw()
	{
		return allocator_type(GetAllocator());
	}

private:
	typedef SplayNodeBase * NodeBasePtr;
	typedef SplayNodeBase const * NodeBaseConstPtr;
	typedef SplayNode<Val> * NodePtr;
	typedef SplayNode<Val> const * NodeConstPtr;
	typedef SplayTreeImpl<Cmp, NodeAllocator> SplayImpl;

	NodeAllocator & GetAllocator() throw()
	{
		return *static_cast<NodeAllocator *>(&m_impl);
	}

	NodeAllocator const & GetAllocator() const throw()
	{
		return *static_cast<NodeAllocator const *>(&m_impl);
	}

	NodePtr GetNode()
	{
		NodePtr tmp = GetAllocator().allocate(1);
		tmp->m_left = tmp->m_right = tmp->m_parent = 0;
		return tmp;
	}

	void PutNode(NodePtr ptr) throw()
	{
		GetAllocator().deallocate(ptr, 1);
	}

	NodePtr CreateNode(value_type const &x)
	{
		NodePtr tmp = GetNode();
		try
		{
			get_allocator().construct(&tmp->m_value, x);
		}
		catch (...)
		{
			PutNode(tmp);
			throw;
		}
		return tmp;
	}

	void DestroyNode(NodePtr ptr)
	{
		get_allocator().destroy(&ptr->m_value);
		PutNode(ptr);
	}

	NodePtr CloneNode(NodeConstPtr ptr)
	{
		return CreateNode(ptr->m_value);
	}

	static const_reference GetValue(NodeConstPtr x) throw()
	{
		return x->m_value;
	}

	static const_reference GetValue(NodeBaseConstPtr x) throw()
	{
		return GetValue(static_cast<NodeConstPtr>(x));
	}

	static Key const & GetKey(NodeConstPtr x) throw()
	{
		return KeyVal()(GetValue(x));
	}

	static Key const & GetKey(NodeBaseConstPtr x) throw()
	{
		return GetKey(static_cast<NodeConstPtr>(x));
	}

	static NodePtr GetLeft(NodeBasePtr x) throw()
	{
		return static_cast<NodePtr>(x->m_left);
	}

	static NodeConstPtr GetLeft(NodeBaseConstPtr x) throw()
	{
		return static_cast<NodeConstPtr>(x->m_left);
	}

	static NodePtr GetRight(NodeBasePtr x) throw()
	{
		return static_cast<NodePtr>(x->m_right);
	}

	static NodeConstPtr GetRight(NodeBaseConstPtr x) throw()
	{
		return static_cast<NodeConstPtr>(x->m_right);
	}

	static NodeBasePtr GetMinimum(NodeBasePtr x) throw()
	{
		return SplayLeftMost(x);
	}

	static NodeBaseConstPtr GetMinimum(NodeBaseConstPtr x) throw()
	{
		return SplayLeftMost(x);
	}

	static NodeBasePtr GetMaximum(NodeBasePtr x) throw()
	{
		return SplayRightMost(x);
	}

	static NodeBaseConstPtr GetMaximum(NodeBaseConstPtr x) throw()
	{
		return SplayRightMost(x);
	}

	NodeBaseConstPtr LowerBound(Key const & k) const throw()
	{
		NodeBaseConstPtr parent = &m_impl.m_header;
		NodeConstPtr current = GetLeft(parent);

		while (current)
		{
			if (!m_impl.m_cmp(GetKey(current), k))
			{
				parent = current;
				current = GetLeft(current);
			}
			else
			{
				current = GetRight(current);
			}
		}

		return parent;
	}

	NodeBasePtr LowerBound(Key const & k) throw()
	{
		return const_cast<NodeBasePtr>(
			const_cast<SelfType const *>(this)->LowerBound(k));
	}

	NodeBaseConstPtr UpperBound(Key const & k) const throw()
	{
		NodeBaseConstPtr parent = &m_impl.m_header;
		NodeConstPtr current = GetLeft(parent);

		while (current)
		{
			if (m_impl.m_cmp(k, GetKey(current)))
			{
				parent = current;
				current = GetLeft(current);
			}
			else
			{
				current = GetRight(current);
			}
		}

		return parent;
	}

	NodeBasePtr UpperBound(Key const & k) throw()
	{
		return const_cast<NodeBasePtr>(
			const_cast<SelfType const *>(this)->UpperBound(k));
	}

	NodeBasePtr Lookup(Key const & k) throw()
	{
		NodeBasePtr node = LowerBound(k);

		if (node != &m_impl.m_header)
		{
			Splay(node, &m_impl.m_header);
			if (!m_impl.m_cmp(GetKey(node), k)
					&& !m_impl.m_cmp(k, GetKey(node)))
				return node;
		}

		return &m_impl.m_header;
	}

	NodePtr Insert(value_type const & v)
	{
		NodeBasePtr parent = &m_impl.m_header;
		NodePtr current = GetLeft(parent);
		bool insert_left = true;

		while (current)
		{
			parent = current;
			if (!m_impl.m_cmp(GetKey(current), KeyVal()(v)))
			{
				insert_left = true;
				current = GetLeft(current);
			}
			else
			{
				insert_left = false;
				current = GetRight(current);
			}
		}

		NodePtr node = CreateNode(v);
		if (insert_left)
			SplaySetLeft(parent, node);
		else
			SplaySetRight(parent, node);

		Splay(node, &m_impl.m_header);
		++m_impl.m_size;

		return node;
	}

	NodeBasePtr Erase(NodePtr node)
	{
		NodeBasePtr next = SplaySucc(node);
		SplayErase(node, &m_impl.m_header);
		DestroyNode(node);
		--m_impl.m_size;

		return next;
	}

public:
	template <typename InputIterator>
	SplayTree(InputIterator first, InputIterator last)
		: m_impl()
	{
		insert(first, last);
	}

	SplayTree() throw()
		: m_impl()
	{ }

	SplayTree(SplayTree const & other)
		: m_impl()
	{
		insert(other.begin(), other.end());
	}

	~SplayTree()
	{
		erase(begin(), end());
	}

	SplayTree & operator=(SplayTree const & other)
	{
		erase(begin(), end());
		insert(other.begin(), other.end());
		return *this;
	}

	iterator begin() throw()
	{
		return iterator(GetMinimum(&m_impl.m_header));
	}

	const_iterator begin() const throw()
	{
		return const_iterator(GetMinimum(&m_impl.m_header));
	}

	reverse_iterator rbegin() throw()
	{
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const throw()
	{
		return const_reverse_iterator(end());
	}

	iterator end() throw()
	{
		return iterator(&m_impl.m_header);
	}

	const_iterator end() const throw()
	{
		return const_iterator(&m_impl.m_header);
	}

	reverse_iterator rend() throw()
	{
		return reverse_iterator(begin());
	}

	const_reverse_iterator rend() const throw()
	{
		return const_reverse_iterator(begin());
	}

	iterator lower_bound(Key const & k) throw()
	{
		return iterator(LowerBound(k));
	}

	iterator upper_bound(Key const & k) throw()
	{
		return iterator(UpperBound(k));
	}

	std::pair<iterator, iterator> equal_range(Key const & k) throw()
	{
		return std::make_pair(lower_bound(k), upper_bound(k));
	}

	iterator find(Key const & k) throw()
	{
		return iterator(Lookup(k));
	}

	std::pair<iterator, bool> insert_unique(value_type const & val)
	{
		iterator it = find(KeyVal()(val));

		if (it == end())
			return std::make_pair(iterator(Insert(val)), true);

		return std::make_pair(it, false);
	}

	template <typename InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		for (; first != last; ++first)
			insert(*first);
	}

	iterator insert(value_type const & val)
	{
		return iterator(Insert(val));
	}

	iterator erase(iterator it)
	{
		return iterator(Erase(static_cast<NodePtr>(it.m_node)));
	}

	iterator erase(iterator first, iterator last)
	{
		iterator it = first;
		for (; it != last;)
			it = erase(it);
		return it;
	}

	size_t size() const throw()
	{
		return m_impl.m_size;
	}

private:
	SplayImpl m_impl;
};

#endif /*__SPLAY_TREE_HPP__*/
