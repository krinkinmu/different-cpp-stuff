#ifndef __BST_HPP__
#define __BST_HPP__

#include "bstnode.hpp"

#include <algorithm>
#include <memory>
#include <cstddef>

template <typename KeyCmp, typename Allocator>
struct BSTImpl : public Allocator {
	TreeNode head;
	KeyCmp cmp;

	BSTImpl()
	: Allocator()
	, head()
	, cmp()
	{ wrap_node(&head); }

	BSTImpl(KeyCmp const &cmp, Allocator const &a)
	: Allocator(a), head(), cmp(cmp)
	{ wrap_node(&head); }

	BSTImpl(KeyCmp const &cmp, Allocator &&a)
	: Allocator(std::move(a)), head(), cmp(cmp)
	{ wrap_node(&head); }
};

template <typename Key, typename Val, typename KeyVal, typename Cmp,
	typename Allocator>
struct BinarySearchTreeBase {
	using Node = BSTNode<Val>;
	using ValueAlloc = Allocator;
	using ValueAllocTrait = std::allocator_traits<ValueAlloc>;
	using NodeAlloc = typename
		ValueAllocTrait::template rebind_alloc<Node>;
	using Impl = BSTImpl<Cmp, NodeAlloc>;

	Impl impl;

	Node *get_node()
	{ return NodeAllocTrait::allocate(impl, 1); }

	void put_node(Node *node)
	{ NodeAllocTrait::deallocate(impl, node, 1); }

	NodeAlloc &node_allocator()
	{ return *static_cast<NodeAlloc *>(&impl); }

	NodeAlloc const &node_allocator()
	{ return *static_cast<NodeAlloc const *>(&impl); }

	Cmp &key_comparator()
	{ return impl.cmp; }

	Cmp const &key_comparator() const
	{ return impl.cmp; }

	template <typename ... Args>
	Node *create_node(Args && ... args)
	{
		Node *node = get_node();

		try {
			NodeAllocTrait::construct(
				impl,
				node,
				std::forward<Args>(args)...
			);
		} catch (...) {
			put_node(node);
			throw;
		}

		return node;
	}

	void destroy_node(Node *node)
	{
		NodeAllocTrait::destroy(impl, node);
		put_node(node);
	}

	BinarySearchTreeBase()
	: impl()
	{ }

	BinarySearchTreeBase(Cmp const &cmp, NodeAlloc const &a)
	: impl(cmp, NodeAllocTrait::select_on_container_copy_construction(a))
	{ }

	BinarySearchTreeBase(BinarySearchTreeBase &&other)
	: impl(std::move(other.key_comparator()),
		std::move(other.node_allocator()))
	{ std::swap(impl.head, other.impl.head); }
};

template <typename Key, typename Val, typename KeyOf, typename KeyCmp,
	typename Allocator = std::allocator<T>>
class BinarySearchTree : private BinarySearchTreeBase<Key, Val, KeyCmp,
							Allocator> {
	using Base = BinarySearchTreeBase<Key, Val, KeyCmp, Allocator>;
	using ValueAlloc = typename Base::ValueAlloc;
	using NodeAlloc = typename Base::NodeAlloc;

	using Base::impl;
	using Base::node_allocator;
	using Base::create_node;
	using Base::destroy_node;
	using Base::key_comparator;

public:
	using iterator = TreeIterator<Val>;
	using const_iterator = TreeConstIterator<Val>;
	using value_type = typename iterator::value_type;
	using reference = typename iterator::reference;
	using const_reference = typename const_iterator::reference;

	BinarySearchTree()
	: Base()
	{ }

	explicit BinarySearchTree(KeyCmp const &cmp,
					Allocator const &a = Allocator())
	: Base(cmp, NodeAlloc(a))
	{ }

	explicit BinarySearchTree(Allocator const &a)
	: Base(NodeAlloc(a))
	{ }

	template <typename It>
	BinarySearchTree(It first, It last, KeyCmp const &cmp,
				Allocator const &a = Allocator())
	: Base(cmp, NodeAlloc(a))
	{ insert(first, last); }

	BinarySearchTree(BinarySearchTree &&other)
	: Base(std::move(other))
	{ }

	~BinarySearchTree()
	{ clear(); }

	BinarySearchTree &operator=(BinarySearchTree other)
	{
		swap(other);
		return *this;
	}

	BinarySearchTree &operator=(BinarySearchTree &&other)
	{
		clear();
		swap(other);
		return *this;
	}

	void swap(BinarySearchTree &other)
	{
		using std::swap;
		swap(impl.head, other.impl.head);
		swap(node_allocator(), other.node_allocator());
		swap(key_comparator(), other.key_comparator());
	}

	void clear()
	{ erase(begin(), end()); }

	bool empty() const
	{ return begin() == end(); }

	size_t size() const
	{ return static_cast<size_t>(std::distance(begin(), end())); }

	iterator begin()
	{ return iterator(impl.head.left); }

	const_iterator begin() const
	{ return const_iterator(impl.head.right); }

	iterator end()
	{ return iterator(&impl.head); }

	const_iterator end() const
	{ return const_iterator(&impl.head); }

	iterator erase(const_iterator it)
	{ return iterator(const_cast<TreeNode *>(it.node)); }

	iterator erase(const_iterator first, const_iterator last)
	{ return iterator(const_cast<TreeNode *>(first.node)); }

	iterator insert(Val const &x)
	{ return iterator(0); }

	iterator insert(Val &&x)
	{ return iterator(0); }

	template <typename It>
	void insert(It first, It last)
	{ }

	template <typename ... Args>
	iterator emplace(Args && ... args)
	{ return iterator(0); }

	iterator find(Key const &key)
	{ return iterator(0); }

	const_iterator find(Key const &key) const
	{ return iterator(0); }

	size_t count(Key const &key) const
	{ return 0; }
};

#endif /*__BST_HPP__*/
