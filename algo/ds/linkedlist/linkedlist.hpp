#ifndef __LINKED_LIST_BASE_HPP__
#define __LINKED_LIST_BASE_HPP__

#include <memory>
#include "linkedlistnode.hpp"

/**
 * I hide LinkedListImpl in detail namespace, but more appropriate way is
 * to make LinkedListImpl inner class of LinkedListBase class.
 **/
namespace detail
{
	/**
	 * LinkedListImpl inherits Allocator to enable empty base class
	 * optimization (see
	 * http://www.stroustrup.com/bs_faq2.html#sizeof-empty).
	 * Note that Allocator is a ListNode allocator not T allocator.
	 **/
	template <typename T, typename Allocator>
	struct LinkedListImpl : public Allocator {
		ListHead head;

		LinkedListImpl()
		: Allocator()
		{ init_list_head(&head); }

		LinkedListImpl(Allocator const &a)
		: Allocator(a)
		{ init_list_head(&head); }

		LinkedListImpl(Allocator &&a)
		: Allocator(std::move(a))
		{ init_list_head(&head); }
	};

	template <typename T, typename Alloc>
	struct LinkedListBase {
		using Node = LinkedListNode<T>;
		using ValueAlloc = Alloc;
		using NodeAlloc = typename Alloc::template rebind<Node>::other;
		using Impl = LinkedListImpl<T, NodeAlloc>;

		LinkedListImpl<T, NodeAlloc> impl;

		Node *get_node()
		{ return impl.allocate(1); }

		void put_node(Node *node)
		{ impl.deallocate(node, 1); }

		NodeAlloc &node_allocator()
		{ return *static_cast<NodeAlloc *>(&impl); }

		NodeAlloc const &node_allocator() const
		{ return *static_cast<NodeAlloc const *>(&impl); }

		template <typename ... Args>
		Node *create_node(Args && ... args)
		{
			Node *node = get_node();

			try {
				node_allocator().construct(
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
			node_allocator().destroy(node);
			put_node(node);
		}

		/**
		 * Empty list - LinkedListImpl default constructor calls
		 * init_list_head.
		 **/
		LinkedListBase()
		: impl()
		{ }

		/**
		 * This is a main reason why i need intermediate LinkedListBase
		 * class - users supply T allocator, but i need ListNode
		 * allocator. So LinkedListBase translates T allocator to
		 * ListNode allocator.
		 **/
		LinkedListBase(NodeAlloc const &a)
		: impl(a)
		{ }

		/**
		 * First af all moves Allocator subobject from LinkedListBase
		 * then swaps heads.
		 **/
		LinkedListBase(LinkedListBase &&other) noexcept
		: impl(std::move(other.node_allocator()))
		{ std::swap(impl.head, other.impl.head); }
	};
}

template <typename T, typename Alloc = std::allocator<T>>
class LinkedList : private detail::LinkedListBase<T, Alloc> {
	using Base = detail::LinkedListBase<T, Alloc>;
	using ValueAlloc = typename Base::ValueAlloc;
	using NodeAlloc = typename Base::NodeAlloc;


	/**
	 * I can use this-> instead, but this way is a bit shorter
	 **/
	using Base::impl;
	using Base::node_allocator;
	using Base::create_node;
	using Base::destroy_node;

public:
	using Iterator = LinkedListIterator<T>;
	using ConstIterator = LinkedListConstIterator<T>;

	LinkedList()
	: Base()
	{ }

	explicit LinkedList(Alloc const &a)
	: Base(NodeAlloc(a))
	{ }

	LinkedList(LinkedList const &other)
	: Base(other.node_allocator())
	{ insert(begin(), other.begin(), other.end()); }

	LinkedList(std::initializer_list<T> other,
			ValueAlloc const &a = ValueAlloc())
	: Base(NodeAlloc(a))
	{ insert(begin(), other.begin(), other.end()); }

	/**
	 * LinkedListBase contains all the data, so it is safe to cast
	 * LinkedList to LinkedListBase.
	 **/
	LinkedList(LinkedList &&other) noexcept
	: Base(std::move(other))
	{ }

	~LinkedList()
	{ clear(); }

	/**
	 * It is extremely useful idom - copy and swap. It is universal way
	 * of assignment operator implementation (not only), if there is swap
	 * method that doesn't throw exceptions than such assignment opertor
	 * meets strong exception safety guarantee. More about exception safety
	 * guaranries here http://www.gotw.ca/gotw/082.htm.
	 **/
	LinkedList &operator=(LinkedList other)
	{
		swap(other);
		return *this;
	}

	LinkedList &operator=(LinkedList &&other) noexcept
	{
		clear();
		swap(other);
		return *this;
	}

	/**
	 * It is possible that there is swap function for NodeAllocator and if so
	 * we want to use it instead std::swap. For this reason i use using
	 * std::swap. More details in Scott Meyers, Effective C++ Third Edition,
	 * Item 25: "Consider support for a non-throwing swap".
	 **/
	void swap(LinkedList &other) noexcept
	{
		using std::swap;
		swap(impl.head, other.impl.head);
		swap(node_allocator(), other.node_allocator());
	}

	void clear()
	{ erase(begin(), end()); }

	bool empty() const
	{ return begin() == end(); }

	/**
	 * Attention time complexity of size method is O(n)
	 **/
	size_t size() const
	{ return static_cast<size_t>(std::distance(begin(), end())); }

	Iterator begin()
	{ return Iterator(impl.head.next); }

	ConstIterator begin() const
	{ return ConstIterator(impl.head.next); }

	Iterator end()
	{ return Iterator(&impl.head); }

	ConstIterator end() const
	{ return ConstIterator(&impl.head); }

	void push_back(T const &x)
	{ insert(end(), x); }

	void push_back(T &&x)
	{ insert(end(), std::forward<T>(x)); }

	template <typename ... Args>
	void emplace_back(Args && ... args)
	{ emplace(end(), std::forward<Args>(args)...); }

	void pop_back()
	{ erase(end()); }

	void push_front(T const &x)
	{ insert(begin(), x); }

	void push_front(T &&x)
	{ insert(begin(), std::forward<T>(x)); }

	template <typename ... Args>
	void emplace_front(Args && ... args)
	{ emplace(begin(), std::forward<Args>(args)...); }

	void pop_front()
	{ erase(begin()); }

	template <typename ... Args>
	Iterator emplace(ConstIterator pos, Args && ... args)
	{
		ListHead *node = create_node(std::forward<Args>(args)...);
		insert_before(node, const_cast<ListHead *>(pos.node));
		return Iterator(node);
	}

	template <typename It>
	Iterator insert(ConstIterator pos, It first, It last)
	{
		while (first != last)
			pos = insert(pos, *first++);
		return pos;
	}

	Iterator insert(ConstIterator pos, T const &x)
	{
		ListHead *node = create_node(x);
		insert_before(node, const_cast<ListHead *>(pos.node));
		return Iterator(node);
	}

	Iterator insert(ConstIterator pos, T &&x)
	{
		ListHead *node = create_node(std::forward<T>(x));
		insert_before(node, const_cast<ListHead *>(pos.node));
		return Iterator(node);
	}

	Iterator erase(ConstIterator pos)
	{
		ListHead *node = const_cast<ListHead *>(pos.node);
		ListHead *next = node->next;
		remove_from_list(node);
		destroy_node(static_cast<LinkedListNode<T> *>(node));
		return Iterator(next);
	}

	Iterator erase(ConstIterator first, ConstIterator last)
	{
		while (first != last)
			first = erase(first);
		return Iterator(const_cast<ListHead *>(first.node));
	}
};

#endif /*__LINKED_LIST_BASE_HPP__`*/
