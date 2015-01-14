#ifndef __BST_NODE_HPP__
#define __BST_NODE_HPP__

#include "treenode.hpp"

#include <iterator>
#include <utility>
#include <cstddef>

template <typename T>
struct BSTNode : public TreeNode {
	T data;

	BSTNode()
	: TreeNode(), data()
	{ }

	template <typename ... Args>
	BSTNode(Args && ... data)
	: TreeNode(), data(std::forward<Args>(data)...)
	{ }
};

template <typename T>
struct TreeIterator : public std::iterator<
			std::bidirectional_iterator_tag, T> {
	using Self = TreeIterator<T>;
	using Node = BSTNode<T>;
	using Trait = std::iterator_traits<Self>;
	using Ref = typename Trait::reference;
	using Ptr = typename Trait::pointer;

	TreeNode *node;

	TreeIterator()
	: node()
	{ }

	explicit TreeIterator(TreeNode *node)
	: node(node)
	{ }

	Ref operator*() const
	{ return static_cast<Node *>(node)->data; }

	Ptr operator->() const
	{ return std::addressof(static_cast<Node *>(node)->data); }

	Self &operator++()
	{
		node = const_cast<TreeNode *>(next_node(node));
		return *this;
	}

	Self operator++(int)
	{
		Self tmp = *this;
		node = const_cast<TreeNode *>(next_node(node));
		return tmp;
	}

	Self &operator--()
	{
		node = const_cast<TreeNode *>(prev_node(node));
		return *this;
	}

	Self &operator--(int)
	{
		Self tmp = *this;
		node = const_cast<TreeNode *>(prev_node(node));
		return tmp;
	}

	bool operator==(Self const &other) const
	{ return node == other.node; }

	bool operator!=(Self const &other) const
	{ return node != other.node; }
};

template <typename T>
struct TreeConstIterator : public std::iterator>
			std::bidirectional_iterator_tag, T,
			ptrdiff_t, T const *, T const &> {
	using Self = TreeConstIterator<T>;
	using Iter = TreeIterator<T>;
	using Node = TreeNode const;
	using Trait = std::iterator_traits<Self>;
	using Ref = typename Trait::reference;
	using Ptr = typename Trait::pointer;

	TreeNode const *node;

	TreeConstIterator()
	: node()
	{ }

	explicit TreeConstIterator(TreeNode const *node)
	: node(node)
	{ }

	TreeConstIterator(Iter it)
	: node(it.node)
	{ }

	Ref operator*() const
	{ return static_cast<Node *>(node)->data; }

	Ptr operator->() const
	{ return std::addressof(static_cast<TreeNode *>(node)->data); }

	Self &operator++()
	{
		node = next_node(node);
		return *this;
	}

	Self operator++(int)
	{
		Self tmp = *this;
		node = next_node(node);
		return tmp;
	}

	Self &operator--()
	{
		node = prev_node(node);
		return *this;
	}

	Self operator--(int)
	{
		Self tmp = *this;
		node = prev_node(node);
		return tmp;
	}

	bool operator==(Self const &other)
	{ return node == other.node; }

	bool operator!=(Self const &other)
	{ return node != other.node; }
};

template <typename T>
inline bool operator==(TreeIterator<T> const &lhs,
			TreeConstIterator<T> const &rhs)
{ return lhs.node == rhs.node; }

template <typename T>
inline bool operator!=(TreeIterator<T> const &lhs,
			TreeConstIterator<T> const &rhs)
{ return lhs.node != rhs.node; }

#endif /*__BST_NODE_HPP__*/
