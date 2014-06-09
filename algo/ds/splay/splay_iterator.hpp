#ifndef __SPLAY_ITERATOR_HPP__
#define __SPLAY_ITERATOR_HPP__

#include <iterator>

#include "splay_node.hpp"

template <typename T>
struct SplayIterator
		: public std::iterator<std::bidirectional_iterator_tag, T>
{
	typedef std::iterator<std::bidirectional_iterator_tag, T> BaseType;
	typedef SplayIterator<T> SelfType;
	typedef SplayNodeBase *NodeBasePtrType;
	typedef SplayNode<T> *NodePtrType;

	typedef typename std::iterator_traits<SelfType>::reference RefType;
	typedef typename std::iterator_traits<SelfType>::pointer PtrType;

	SplayIterator() throw()
		: BaseType()
		, m_node(0)
	{ }

	explicit SplayIterator(NodePtrType ptr) throw()
		: BaseType()
		, m_node(ptr)
	{ }

	RefType operator*() const throw()
	{
		return static_cast<NodePtrType>(m_node)->m_value;
	}

	PtrType operator->() const throw()
	{
		return &static_cast<NodePtrType>(m_node)->m_value;
	}

	SelfType &operator++() throw()
	{
		m_node = SplaySucc(m_node);
		return *this;
	}

	SelfType operator++(int) throw()
	{
		SelfType tmp = *this;
		m_node = SplaySucc(m_node);
		return tmp;
	}

	SelfType &operator--() throw()
	{
		m_node = SplayPred(m_node);
		return *this;
	}

	SelfType operator--(int) throw()
	{
		SelfType tmp = *this;
		m_node = SplayPred(m_node);
		return tmp;
	}

	bool operator==(SelfType const & other) const throw()
	{
		return m_node == other.m_node;
	}

	bool operator!=(SelfType const & other) const throw()
	{
		return m_node != other.m_node;
	}

	NodeBasePtrType m_node;
};

template <typename T>
struct SplayConstIterator
		: public std::iterator<std::bidirectional_iterator_tag, T const>
{	
	typedef std::iterator<std::bidirectional_iterator_tag, T const>
				BaseType;

	typedef SplayConstIterator<T> SelfType;
	typedef SplayNodeBase const *NodeBasePtrType;
	typedef SplayNode<T> const *NodePtrType;

	typedef typename std::iterator_traits<SelfType>::reference RefType;
	typedef typename std::iterator_traits<SelfType>::pointer PtrType;

	typedef SplayIterator<T> IteratorType;

	SplayConstIterator() throw()
		: BaseType()
		, m_node(0)
	{ }

	explicit SplayConstIterator(NodePtrType ptr) throw()
		: BaseType()
		, m_node(ptr)
	{ }

	SplayConstIterator(IteratorType const & it)
		: BaseType()
		, m_node(it.m_node)
	{ }

	RefType operator*() const throw()
	{
		return static_cast<NodePtrType>(m_node)->m_value;
	}

	PtrType operator->() const throw()
	{
		return &static_cast<NodePtrType>(m_node)->m_value;
	}

	SelfType &operator++() throw()
	{
		m_node = SplaySucc(m_node);
		return *this;
	}

	SelfType operator++(int) throw()
	{
		SelfType tmp = *this;
		m_node = SplaySucc(m_node);
		return tmp;
	}

	SelfType &operator--() throw()
	{
		m_node = SplayPred(m_node);
		return *this;
	}

	SelfType operator--(int) throw()
	{
		SelfType tmp = *this;
		m_node = SplayPred(m_node);
		return tmp;
	}

	bool operator==(SelfType const & other) const throw()
	{
		return m_node == other.m_node;
	}

	bool operator!=(SelfType const & other) const throw()
	{
		return m_node != other.m_node;
	}

	NodeBasePtrType m_node;
};

template <typename T>
inline bool operator==(SplayIterator<T> const & lhs,
			SplayConstIterator<T> const & rhs) throw()
{
	return lhs.m_node == rhs.m_node;
}

template <typename T>
inline bool operator!=(SplayIterator<T> const & lhs,
			SplayConstIterator<T> const & rhs) throw()
{
	return lhs.m_node != rhs.m_node;
}

#endif /*__SPLAY_ITERATOR_HPP__*/
