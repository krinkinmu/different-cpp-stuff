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
		m_header.m_left = &m_header;
		m_header.m_right = &m_header;
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

protected:
	typedef SplayNodeBase * NodeBasePtr;
	typedef SplayNodeBase const * NodeBaseConstPtr;
	typedef SplayNode<Val> * NodePtr;
	typedef SplayNode<Val> const * NodeConstPtr;

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
		return GetAllocator().allocate(1);
	}

	void PutNode(NodePtr ptr) throw()
	{
		GetAllocator().deallocate(ptr);
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
		NodePtr tmp = CreateNode(ptr->m_value);
		tmp->m_parent = 0;
		tmp->m_left = 0;
		tmp->m_right = 0;
		return tmp;
	}

	NodeBasePtr & Root() throw()
	{
		return m_impl.m_header.m_parent;
	}

	NodeBaseConstPtr Root() const throw()
	{
		return m_impl.m_header.m_parent;
	}

	NodeBasePtr & LeftMost() throw()
	{
		return m_impl.m_header.m_left;
	}

	NodeBaseConstPtr LeftMost() const throw()
	{
		return m_impl.m_header.m_left;
	}

	NodeBasePtr & RightMost() throw()
	{
		return m_impl.m_header.m_right;
	}

	NodeBaseConstPtr RightMost() const throw()
	{
		return m_impl.m_header.m_right;
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

public:
	iterator Insert(NodeBasePtr p, value_type const & v)
	{
		NodeBasePtr ptr = CreateNode(v);

		if (p == &m_impl.m_header)
			ptr->m_parent = p;
		else if (m_impl.m_cmp(GetKey(ptr), GetKey(p)))
			SplaySetLeft(p, ptr);
		else
			SplaySetRight(p, ptr);

		Splay(ptr, &m_impl.m_header);
		++m_impl.m_size;

		return iterator(ptr);
	}

	iterator LowerBound(Key const & k) throw()
	{
		NodeBasePtr parent = &m_impl.m_header;
		NodePtr current = GetLeft(parent);

		while (current)
		{
			parent = current;
			if (!m_impl.m_cmp(GetKey(current), k))
				current = GetLeft(current);
			else
				current = GetRight(current);
		}

		Splay(parent, &m_impl.m_header);

		return iterator(parent);
	}

	iterator UpperBound(Key const & k) throw()
	{
		NodeBasePtr parent = &m_impl.m_header;
		NodePtr current = GetLeft(parent);

		while (current)
		{
			parent = current;
			if (m_impl.m_cmp(k, GetKey(current)))
				current = GetLeft(current);
			else
				current = GetRight(current);
		}

		Splay(parent, &m_impl.m_header);

		return iterator(parent);
	}

private:
	SplayTreeImpl<Cmp, NodeAllocator> m_impl;
};

#endif /*__SPLAY_TREE_HPP__*/
