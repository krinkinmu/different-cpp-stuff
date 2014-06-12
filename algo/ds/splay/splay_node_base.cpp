#include <cassert>
#include "splay_node_base.hpp"

namespace
{

	bool IsLeftChild(SplayNodeBase const *node) throw()
	{
		return node->m_parent && (node->m_parent->m_left == node);
	}

	bool IsRightChild(SplayNodeBase const *node) throw()
	{
		return node->m_parent && (node->m_parent->m_right == node);
	}

	void RotateLeft(SplayNodeBase *node, SplayNodeBase *parent,
			SplayNodeBase *grand) throw()
	{
		SplayNodeBase *const child = node->m_left;
		SplaySetLeft(node, parent);
		SplaySetRight(parent, child);

		if (grand->m_left == parent)
			SplaySetLeft(grand, node);
		else
			SplaySetRight(grand, node);
	}

	void RotateRight(SplayNodeBase *node, SplayNodeBase *parent,
			SplayNodeBase *grand) throw()
	{
		SplayNodeBase *const child = node->m_right;
		SplaySetRight(node, parent);
		SplaySetLeft(parent, child);

		if (grand->m_left == parent)
			SplaySetLeft(grand, node);
		else
			SplaySetRight(grand, node);
	}

	void RotateLeftLeft(SplayNodeBase *node, SplayNodeBase *parent,
			SplayNodeBase *grand, SplayNodeBase *ggrand) throw()
	{
		RotateLeft(parent, grand, ggrand);
		RotateLeft(node, parent, ggrand);
	}

	void RotateLeftRight(SplayNodeBase *node, SplayNodeBase *parent,
			SplayNodeBase *grand, SplayNodeBase *ggrand) throw()
	{
		RotateLeft(node, parent, grand);
		RotateRight(node, grand, ggrand);
	}

	void RotateRightLeft(SplayNodeBase *node, SplayNodeBase *parent,
			SplayNodeBase *grand, SplayNodeBase *ggrand) throw()
	{
		RotateRight(node, parent, grand);
		RotateLeft(node, grand, ggrand);
	}

	void RotateRightRight(SplayNodeBase *node, SplayNodeBase *parent,
			SplayNodeBase *grand, SplayNodeBase *ggrand) throw()
	{
		RotateRight(parent, grand, ggrand);
		RotateRight(node, parent, ggrand);
	}

} /* anonymous namespace */


SplayNodeBase::SplayNodeBase() throw()
	: m_parent(0)
	, m_left(0)
	, m_right(0)
{ }

void SplaySetLeft(SplayNodeBase *parent, SplayNodeBase *child) throw()
{
	assert(parent);
	parent->m_left = child;
	if (child)
		child->m_parent = parent;
}

void SplaySetRight(SplayNodeBase *parent, SplayNodeBase *child) throw()
{
	assert(parent);
	parent->m_right = child;
	if (child)
		child->m_parent = parent;
}

SplayNodeBase const *SplayLeftMost(SplayNodeBase const *node) throw()
{
	assert(node);
	while (node->m_left)
		node = node->m_left;
	return node;
}

SplayNodeBase *SplayLeftMost(SplayNodeBase *node) throw()
{
	return const_cast<SplayNodeBase *>(
		SplayLeftMost(const_cast<SplayNodeBase const *>(node)));
}

SplayNodeBase const *SplayRightMost(SplayNodeBase const *node) throw()
{
	assert(node);
	while (node->m_right)
		node = node->m_right;
	return node;
}

SplayNodeBase *SplayRightMost(SplayNodeBase *node) throw()
{
	return const_cast<SplayNodeBase *>(
		SplayRightMost(const_cast<SplayNodeBase const *>(node)));
}

SplayNodeBase const *SplaySucc(SplayNodeBase const *node) throw()
{
	assert(node);

	if (node->m_right)
		return SplayLeftMost(node->m_right);

	while (IsRightChild(node))
		node = node->m_parent;

	return node ? node->m_parent : 0;
}

SplayNodeBase *SplaySucc(SplayNodeBase *node) throw()
{
	return const_cast<SplayNodeBase *>(
		SplaySucc(const_cast<SplayNodeBase const *>(node)));
}

SplayNodeBase const *SplayPred(SplayNodeBase const *node) throw()
{
	assert(node);

	if (node->m_left)
		return SplayRightMost(node->m_left);

	while (IsLeftChild(node))
		node = node->m_parent;

	return node ? node->m_parent : 0;
}

SplayNodeBase *SplayPred(SplayNodeBase *node) throw()
{
	return const_cast<SplayNodeBase *>(
		SplayPred(const_cast<SplayNodeBase const *>(node)));
}

void Splay(SplayNodeBase *node, SplayNodeBase *root) throw()
{
	while (node->m_parent != root)
	{
		SplayNodeBase *const parent = node->m_parent;
		SplayNodeBase *const grand = parent->m_parent;

		if (grand == root)
		{
			if (parent->m_left == node)
				RotateRight(node, parent, grand);
			else
				RotateLeft(node, parent, grand);
		}
		else
		{
			if (grand->m_left == parent)
			{
				if (parent->m_left == node)
					RotateRightRight(node, parent, grand,
							grand->m_parent);
				else
					RotateLeftRight(node, parent, grand,
							grand->m_parent);
			}
			else
			{
				if (parent->m_left == node)
					RotateRightLeft(node, parent, grand,
							grand->m_parent);
				else
					RotateLeftLeft(node, parent, grand,
							grand->m_parent);
			}
		}
	}
}

void SplayErase(SplayNodeBase *node, SplayNodeBase *root) throw()
{
	Splay(node, root);

	if (!node->m_left)
	{
		SplaySetLeft(root, node->m_right);
	}
	else if (!node->m_right)
	{
		SplaySetLeft(root, node->m_left);
	}
	else
	{
		SplayNodeBase *const right = SplayRightMost(node->m_left);
		Splay(right, node);
		SplaySetRight(right, node->m_right);
		SplaySetLeft(root, right);
	}
}
