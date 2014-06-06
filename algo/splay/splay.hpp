#include <iterator>
#include <memory>

namespace ds
{
	struct SplayTreeNodeBase
	{
		using	BasePtr		=	SplayTreeNodeBase *;
		using	ConstBasePtr	=	SplayTreeNodeBase const *;

		BasePtr	m_parent;
		BasePtr	m_left;
		BasePtr	m_right;
	};

	inline SplayTreeNodeBase const *
	LeftMost(SplayTreeNodeBase const *node) noexcept
	{
		while (node->m_left)
			node = node->m_left;
		return node;
	}

	inline SplayTreeNodeBase *
	LeftMost(SplayTreeNodeBase *node) noexcept
	{
		return const_cast<SplayTreeNodeBase *>(
			LeftMost(const_cast<SplayTreeNodeBase const *>(node)
		);
	}

	inline SplayTreeNodeBase const *
	RightMost(SplayTreeNodeBase const *node) noexcept
	{
		while (node->m_right)
			node = node->m_right;
		return node;
	}

	inline SplayTreeNodeBase *
	RightMost(SplayTreeNodeBase *node) noexcept
	{
		return const_cast<BasePtr>(
			RightMost(const_cast<ConstBasePtr>(node)
		);
	}

	inline SplayTreeNodeBase const *
	SplayTreeNext(SplayTreeNodeBase const *v) noexcept
	{
		if (v->m_right)
			return LeftMost(v->m_right);

		while (v->m_parent)
		{
			if (v->m_parent->m_left == v)
				return v->m_parent;
			v = v->m_parent;
		}

		return 0;
	}

	inline SplayTreeNodeBase *
	SplayTreeNext(SplayTreeNodeBase *v) noexcept
	{
		return const_cast<SplayTreeNodeBase *>(
			SplayTreeNext(const_cast<SplayTreeNodeBase const *>(v)
		);
	}

	inline SplayTreeNodeBase const *
	SplayTreePrev(SplayTreeNodeBase const *v) noexcept
	{
		if (v->m_left)
			return RightMost(v->m_left);

		while (v->m_parent)
		{
			if (v->m_parent->m_right == v)
				return v->m_parent;
			v = v->m_parent;
		}

		return 0;
	}

	inline SplayTreeNodeBase *
	SplayTreePrev(SplayTreeNodeBase *v) noexcept
	{
		return const_cast<SplayTreeNodeBase *>(
			SplayTreePrev(const_cast<SplayTreeNodeBase const *>(v)
		);
	}

	inline void
	RotateRight(SplayTreeNodeBase *x, SplayTreeNodeBase *p) noexcept
	{
		SplayTreeNodeBase *const grand = p->m_parent;
		SplayTreeNodeBase *const child = x->m_right;

		x->m_right = p;
		x->m_parent = grand;
		p->m_left = child;
		p->m_parent = x;

		if (child)
			child->m_parent = p;

		if (grand->m_left == p)
			grand->m_left = x;
		else
			grand->m_right = x;
	}

	inline void
	RotateLeft(SplayTreeNodeBase *x, SplayTreeNodeBase *p) noexcept
	{
		SplayTreeNodeBase *const grand = p->m_parent;
		SplayTreeNodeBase *const child = x->m_left;

		x->m_left = p;
		x->m_parent = grand;
		p->m_right = child;
		p->m_parent = x;

		if (child)
			child->m_parent = p;

		if (grand->m_left == p)
			grand->m_left = x;
		else
			grand->m_right = x;
	}

	inline void
	RotateRightRight(SplayTreeNodeBase *x, SplayTreeNodeBase *p,
				SplayTreeNodeBase *g) noexcept
	{
		RotateRight(p, g);
		RotateRight(x, p);
	}

	inline void
	RotateLeftLeft(SplayTreeNodeBase *x, SplayTreeNodeBase *p,
				SplayTreeNodeBase *g) noexcept
	{
		RotateLeft(p, g);
		RotateLeft(x, p);
	}

	inline void
	RotateLeftRight(SplayTreeNodeBase *x, SplayTreeNodeBase *p,
				SplayTreeNodeBase *g) noexcept
	{
		RotateLeft(x, p);
		RotateRight(x, g);
	}

	inline void
	RotateRightLeft(SplayTreeNodeBase *x, SplayTreeNodeBase *p,
				SplayTreeNodeBase *g) noexcept
	{
		RotateRight(x, p);
		RotateLeft(x, g);
	}

	inline void
	Splay(SplayTreeNodeBase *x, SplayTreeNodeBase *r)
	{
		while (x->m_parent != r)
		{
			SplayTreeNodeBase *const parent = x->m_parent;
			SplayTreeNodeBase *const grand = parent->m_parent;

			if (grand == r)
			{
				if (parent->m_left == x)
					RotateRight(x, parent);
				else
					RotateLeft(x, parent);
			}
			else if (grand->m_left == parent)
			{
				if (parent->m_left == x)
					RotateRightRight(x, parent, grand);
				else
					RotateLeftRight(x, parent, grand);
			}
			else
			{
				if (parent->m_left == x)
					RotateRightLeft(x, parent, grand);
				else
					RotateLeftLeft(x, parent, grand);
			}
		}
	}

	inline void
	Erase(SplayTreeNodeBase *x, SplayTreeNodeBase *r) noexcept
	{
		Splay(x, r);

		SplayTreeNodeBase *const left = x->m_left;
		SplayTreeNodeBase *const right = x->m_right;
		SplayTreeNodeBase *const next = RightMost(left);

		left->m_parent = r;
		r->m_left = left;
		Splay(next, r);
		next->m_right = right;
		right->m_parent = next;
	}

	template <typename V>
	struct SplayTreeNode : public SplayTreeNodeBase
	{
		using	LinkType	=	SplayTreeNode<V> *;
		using	ConstLinkType	=	SplayTreeNode<V> const *;

		V	m_value;

		template <typename ... Args>
		SplayTreeNode(Args&&... args)
			: SplayTreeNodeBase()
			, m_value(std::forward<Args>(args)...)
		{ }
	};

	template <typename T>
	struct SplayTreeIterator
		: public std::iterator<std::bidirectional_iterator_tag, T>
	{
		using	BaseType	=	std::iterator<
						std::bidirectional_iterator_tag,
						T>;
		using	SelfType	=	SplayTreeIterator<T>;
		using	BasePtr		=	SplayTreeNodeBase::BasePtr;
		using	LinkType	=	SplayTreeNode<T>::LinkType;

		BasePtr	m_node;

		SplayTreeIterator() noexcept
			: BaseType()
			, m_node()
		{ }

		explicit SplayTreeIterator(LinkType l) noexcept
			: BaseType()
			, m_node(l)
		{ }

		reference
		operator*() const noexcept
		{
			return static_cast<LinkType>(m_node)->m_value;
		}

		pointer
		operator->() const noexcept
		{
			return std::addressof(
				static_cast<LinkType>(m_node)->m_value
			);
		}

		SelfType &
		operator++() noexcept
		{
			m_node = SplayTreeNext(m_node);
			return *this;
		}

		SelfType
		operator++(int) noexcept
		{
			SelfType self = *this;
			m_node = SplayTreeNext(m_node);
			return self;
		}

		SelfType &
		operator--() noexcept
		{
			m_node = SplayTreePrev(m_node);
			return *this;
		}

		SelfType
		operator--(int) noexcept
		{
			SelfType self = *this;
			m_node = SplayTreePrev(m_node);
			return self;
		}

		bool
		operator==(Self const &rhs) const noexcept
		{
			return m_node == rhs.m_node;
		}

		bool
		operator!=(Self const &rhs) const noexcept
		{
			return !(*this == rhs);
		}
	};

	template <typename T>
	struct SplayTreeConstIterator :
		: public std::iterator<std::bidirectional_iterator_tag,
					T, ptrdiff_t, T const *, T const &>
	{
		using	BaseType	=	std::iterator<
						std::bidirectional_iterator_tag,
						T, ptrdiff_t, T const *,
						T const &>;
		using	SelfType	=	SplayTreeConstIterator<T>;
		using	BasePtr		=	SplayTreeNodeBase::ConstBasePtr;
		using	LinkType	=	SplayTreeNode<T>::ConstLinkType;
		using	Iterator	=	SplayTreeIterator<T>;

		BasePtr	m_node;

		SplayTreeConstIterator() noexcept
			: BaseType()
			, m_node()
		{ }

		explicit SplayTreeConstIterator(LinkType l) noexcept
			: BaseType()
			, m_node(l)
		{ }

		SplayTreeConstIterator(Iterator const & it) noexcept
			: BaseType()
			, m_node(it.m_node)
		{ }

		reference
		operator*() const noexcept
		{
			return static_cast<LinkType>(m_node)->m_value;
		}

		pointer
		operator->() const noexcept
		{
			return std::addressof(
				static_cast<LinkType>(m_node)->m_value
			);
		}

		SelfType &
		operator++() noexcept
		{
			m_node = SplayTreeNext(m_node);
			return *this;
		}

		SelfType
		operator++(int) noexcept
		{
			SelfType self = *this;
			m_node = SplayTreeNext(m_node);
			return self;
		}

		SelfType &
		operator--() noexcept
		{
			m_node = SplayTreePrev(m_node);
			return *this;
		}

		SelfType
		operator--(int) noexcept
		{
			SelfType self = *this;
			m_node = SplayTreePrev(m_node);
			return self;
		}

		bool operator==(SelfType const &rhs) const noexcept
		{
			return m_node == rhs.m_node;
		}

		bool operator!=(SelfType const &rhs) const noexcept
		{
			return !(*this == rhs);
		}
	};

	template <typename T>
	inline bool
	operator==(SplayTreeItertor<T> const &lhs,
			SplayTreeConstIterator<T> const &rhs) noexcept
	{
		return rhs == lhs;
	}

	template <typename T>
	inline bool
	operator!=(SplayTreeIterator<T> const &lhs,
			SplayTreeConstIterator<T> const &rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template < typename K, typename V, typename KV, typename Cmp,
				typename All = std::allocator<V> >
	class SplayTree
	{
	};
}
