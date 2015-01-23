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

template <typename Val, typename Cmp, typename Allocator>
struct BinarySearchTreeBase {
	using Node = BSTNode<Val>;
	using ValueAlloc = Allocator;
	using ValueAllocTrait = std::allocator_traits<ValueAlloc>;
	using NodeAlloc = typename
		ValueAllocTrait::template rebind_alloc<Node>;
	using NodeAllocTrait = std::allocator_traits<NodeAlloc>;
	using Impl = BSTImpl<Cmp, NodeAlloc>;

	Impl impl;

	Node *get_node()
	{ return NodeAllocTrait::allocate(impl, 1); }

	void put_node(Node *node)
	{ NodeAllocTrait::deallocate(impl, node, 1); }

	NodeAlloc &node_allocator()
	{ return *static_cast<NodeAlloc *>(&impl); }

	NodeAlloc const &node_allocator() const
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
			wrap_node(node);
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

	void detach_leaf(TreeNode *node)
	{
		if (is_left(node))
			node->parent->left = node->parent;
		else
			node->parent->right = node->parent;
	}

	void detach_left_edge(TreeNode *node)
	{
		if (is_left(node))
			add_left(node->left, node->parent);
		else
			add_right(node->left, node->parent);
	}

	void detach_right_edge(TreeNode *node)
	{
		if (is_left(node))
			add_left(node->right, node->parent);
		else
			add_right(node->right, node->parent);
	}

	void detach_common(TreeNode *node)
	{
		TreeNode *prev = const_cast<TreeNode *>(right_most(node->left));

		detach_node(prev);
		add_right(node->right, prev);

		if (node->left != node)
			add_left(node->left, prev);

		if (is_left(node))
			add_left(prev, node->parent);
		else
			add_right(prev, node->parent);
	}

	void detach_node(TreeNode *node)
	{
		bool const has_left = node->left != node;
		bool const has_right = node->right != node;

		if (!has_left && !has_right) {
			detach_leaf(node);
			return;
		}

		if (!has_right) {
			detach_left_edge(node);
			return;
		}

		if (!has_left) {
			detach_right_edge(node);
			return;
		}

		detach_common(node);
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
	typename Allocator = std::allocator<Val>>
class BinarySearchTree : private BinarySearchTreeBase<Val, KeyCmp, Allocator> {
	using Base = BinarySearchTreeBase<Val, KeyCmp, Allocator>;
	using ValueAlloc = typename Base::ValueAlloc;
	using NodeAlloc = typename Base::NodeAlloc;
	using Node = typename Base::Node;
	using Self = BinarySearchTree<Key, Val, KeyOf, KeyCmp, Allocator>;

	using Base::impl;
	using Base::node_allocator;
	using Base::create_node;
	using Base::destroy_node;
	using Base::key_comparator;
	using Base::detach_node;

public:
	using iterator = TreeIterator<Val>;
	using const_iterator = TreeConstIterator<Val>;
	using value_type = typename iterator::value_type;
	using reference = typename iterator::reference;
	using const_reference = typename const_iterator::reference;

	BinarySearchTree()
	: Base()
	{ }

	explicit BinarySearchTree(KeyCmp const &cmp = KeyCmp(),
					Allocator const &a = Allocator())
	: Base(cmp, NodeAlloc(a))
	{ }

	explicit BinarySearchTree(Allocator const &a)
	: Base(NodeAlloc(a))
	{ }

	template <typename It>
	BinarySearchTree(It first, It last, KeyCmp const &cmp = KeyCmp(),
				Allocator const &a = Allocator())
	: Base(cmp, NodeAlloc(a))
	{ insert(first, last); }

	BinarySearchTree(Self &&other)
	: Base(std::move(other))
	{ }

	~BinarySearchTree()
	{ clear(); }

	BinarySearchTree &operator=(Self other)
	{
		swap(other);
		return *this;
	}

	BinarySearchTree &operator=(Self &&other)
	{
		clear();
		swap(other);
		return *this;
	}

	void swap(Self &other)
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
	{ return iterator(impl.head.right); }

	const_iterator begin() const
	{ return const_iterator(impl.head.right); }

	iterator end()
	{ return iterator(&impl.head); }

	const_iterator end() const
	{ return const_iterator(&impl.head); }

	iterator insert(Val const &x)
	{ return insert(Val(x)); }

	iterator insert(Val &&x)
	{ return emplace(std::forward<Val>(x)); }

	template <typename It>
	void insert(It first, It last)
	{
		while (first != last)
			insert(*first++);
	}

	template <typename ... Args>
	iterator emplace(Args && ... args)
	{
		BSTNode<Val> *new_node =
				create_node(std::forward<Args>(args)...);
		TreeNode *node = lower_bound(KeyOf()(new_node->data)).node;

		if (node->left != node) {
			node = const_cast<TreeNode *>(right_most(node->left));
			add_right(new_node, node);
		} else {
			add_left(new_node, node);
			impl.head.right = impl.head.right->left;
		}
		return iterator(new_node);
	}

	const_iterator lower_bound(Key const &key) const
	{
		TreeNode const *parent = &impl.head;
		TreeNode const *child = impl.head.left;
		TreeNode const *x = parent;

		KeyCmp const &keycmp = key_comparator();
		KeyOf keyof;

		while (child != parent) {
			BSTNode<Val> const *node =
				static_cast<BSTNode<Val> const *>(child);

			parent = child;
			if (!keycmp(keyof(node->data), key)) {
				child = node->left;
				x = node;
			} else {
				child = node->right;
			}
		}

		return const_iterator(x);
	}

	const_iterator upper_bound(Key const &key) const
	{
		TreeNode const *parent = &impl.head;
		TreeNode const *child = impl.head.left;
		TreeNode const *x = parent;

		KeyCmp const &keycmp = key_comparator();
		KeyOf keyof;

		while (child != parent) {
			BSTNode<Val> const *node =
				static_cast<BSTNode<Val> const *>(child);

			parent = child;
			if (keycmp(key, keyof(node->data))) {
				child = node->left;
				x = child;
			} else {
				child = node->right;
			}
		}

		return const_iterator(x);
	}

	iterator lower_bound(Key const &key)
	{
		Self const *self = const_cast<Self const *>(this);
		const_iterator it = self->lower_bound(key);

		return iterator(const_cast<TreeNode *>(it.node));
	}

	iterator upper_bound(Key const &key)
	{
		Self const *self = const_cast<Self const *>(this);
		const_iterator it = self->upper_bound(key);

		return iterator(const_cast<TreeNode *>(it.node));
	}

	const_iterator find(Key const &key) const
	{
		const_iterator it = lower_bound(key);
		KeyCmp const &keycmp = key_comparator();
		KeyOf keyof;

		if (it == end())
			return it;

		if (keycmp(keyof(*it), key) || keycmp(key, keyof(*it)))
			return end();

		return it;
	}

	iterator find(Key const &key)
	{
		Self const *self = const_cast<Self const *>(this);
		const_iterator it = self->find(key);

		return iterator(const_cast<TreeNode *>(it.node));
	}

	size_t count(Key const &key) const
	{ return std::distance(lower_bound(key), upper_bound(key)); }
	
	iterator erase(const_iterator it)
	{
		Node *node = const_cast<Node *>(
					static_cast<Node const *>(it.node));
		TreeNode *next = const_cast<TreeNode *>(next_node(node));

		detach_node(node);
		if (impl.head.right == node)
			impl.head.right = next;
		destroy_node(node);

		return iterator(next);		
	}

	iterator erase(const_iterator first, const_iterator last)
	{
		while (first != last)
			first = erase(first);
		return iterator(const_cast<TreeNode *>(first.node));
	}
};

#endif /*__BST_HPP__*/
