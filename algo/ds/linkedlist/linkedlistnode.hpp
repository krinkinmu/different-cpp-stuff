#ifndef __LINKED_LIST_NODE_HPP__
#define __LINKED_LIST_NODE_HPP__

/**
 * C++ linked list node.
 *
 * Note: links inhertited from ListHead struct, so next and prev are of type
 * struct ListHead *, thus type cast is required to get appropriate
 * LinkedListNode. Never cast dummy head element of linked list because it is
 * of type struct ListHead not of type LinkedListNode.
 **/

#include <cstddef>
#include <utility>
#include "listhead.hpp"

template <typename T>
struct LinkedListNode : public ListHead {
	T data;

	/**
	 * I want zero-initilize ListHead subobject due to debug reasons
	 * mostly. So call ListHead "constructor" in constructors initializer
	 * lists. Without this call ListHead part would be default initialized
	 * (see 12.6.2.8 in N3337 2012-01-16), so i use direct initialization
	 * (value initialization in this case, see 8.5.15-8.5.16 in N3337
	 * 2012-01-16) to zero-initialize ListHead struct (see 8.5.7 in N3337
	 * 2012-01-16, note that ListHead has no user defined constructor)
	 **/
	LinkedListNode()
	: ListHead(), data()
	{ }

	/**
	 * This template constructor covers both copy construction and
	 * "emplace" construction of data member
	 **/
	template <typename ... Args>
	LinkedListNode(Args && ... args)
	: ListHead(), data(std::forward<Args>(args)...)
	{ }
};

template <typename T>
struct LinkedListIterator : public std::iterator<
				std::bidirectional_iterator_tag, T> {
	using Self = LinkedListIterator<T>;
	using Node = LinkedListNode<T>;
	using Trait = std::iterator_traits<Self>;
	using Ref = typename Trait::reference;
	using Ptr = typename Trait::pointer;

	ListHead *node;

	LinkedListIterator()
	: node()
	{ }

	explicit LinkedListIterator(ListHead *node)
	: node(node)
	{ }

	Ref operator*() const
	{ return static_cast<Node *>(node)->data; }

	/**
	 * It is possible to overload operator& in C++, so i need
	 * addressof.
	 **/
	Ptr operator->() const
	{ return std::addressof(static_cast<Node *>(node))->data; }

	Self &operator++()
	{
		node = node->next;
		return *this;
	}

	Self operator++(int)
	{
		Self tmp = *this;
		node = node->next;
		return tmp;
	}

	Self &operator--()
	{
		node = node->prev;
		return *this;
	}

	Self operator--(int)
	{
		Self tmp = *this;
		node = node->prev;
		return tmp;
	}

	bool operator==(LinkedListIterator const &other) const
	{ return node == other.node; }

	bool operator!=(LinkedListIterator const &other) const
	{ return node != other.node; }
};

template <typename T>
struct LinkedListConstIterator : public std::iterator<
				std::bidirectional_iterator_tag, T,
				ptrdiff_t, T const *, T const &> {
	using Self = LinkedListConstIterator<T>;
	using Iter = LinkedListIterator<T>;
	using Node = LinkedListNode<T> const;
	using Trait = std::iterator_traits<Self>;
	using Ref = typename Trait::reference;
	using Ptr = typename Trait::pointer;

	ListHead const *node;

	LinkedListConstIterator()
	: node()
	{ }

	explicit LinkedListConstIterator(ListHead const *node)
	: node(node)
	{ }

	LinkedListConstIterator(Iter it)
	: node(it.node)
	{ }

	Ref operator*() const
	{ return static_cast<Node *>(node)->data; }

	Ptr operator->() const
	{ return std::addressof(static_cast<Node *>(node)->data); }

	Self &operator++()
	{
		node = node->next;
		return *this;
	}

	Self operator++(int)
	{
		Self tmp = *this;
		node = node->next;
		return tmp;
	}

	Self &operator--()
	{
		node = node->prev;
		return *this;
	}

	Self operator--(int)
	{
		Self tmp = *this;
		node = node->prev;
		return tmp;
	}

	bool operator==(LinkedListConstIterator const &other) const
	{ return node == other.node; }

	bool operator!=(LinkedListConstIterator const &other) const
	{ return node != other.node; }
};

/**
 * With template operator== type casts are very limited, for example, following
 * template operator:
 *
 * template <typename T>
 * inline bool operator==(LinkedListConstIterator<T> const &lhs,
 * 				LinkedListConstIterator<T> const &rhs);
 *
 * doesn't work for arguments of type LinkedListIterator and
 * LinkedListConstIterator for any template parameter in spite of there is
 * conversion from LinkedListIterator to LinkedListConstIterator because after
 * substitution of some concrete type X instead of T we get
 *
 * inline bool operator==(LinkedListIterator<X> const &lhs,
 * 				LinkedListConstIterator<X> const &rhs);
 *
 * not
 *
 * inline bool operator==(LinkedListConstIterator<X> const &lhs,
 * 				LinkedListConstIterator<X> const &rhs);
 *
 * So i need member function of operator== in LinkedListConstIterator<T> and
 * LinkedListIterator<T> this enables type casts for second argument of
 * operator, also i need following operator to cover one last case.
 **/
template <typename T>
inline bool operator==(LinkedListIterator<T> const &lhs,
			LinkedListConstIterator<T> const &rhs)
{ return lhs.node == rhs.node; }

template <typename T>
inline bool operator!=(LinkedListIterator<T> const &lhs,
			LinkedListConstIterator<T> const &rhs)
{ return lhs.node != rhs.node; }

#endif /*__LINKED_LIST_NODE_HPP__*/
