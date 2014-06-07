#include <cassert>
#include "splay_node_base.hpp"

namespace
{

	bool IsChild(SplayNodeBase const *node) throw()
	{
		return node && node->m_parent;
	}

	bool IsLeftChild(SplayNodeBase const *node) throw()
	{
		return IsChild(node) && (node->m_parent->m_left == node);
	}

	bool IsRightChild(SplayNodeBase const *node) throw()
	{
		return IsChild(node) && (node->m_parent->m_right == node);
	}

} /* anonymous namespace */


SplayNodeBase::SplayNodeBase() throw()
	: m_parent(0)
	, m_left(0)
	, m_right(0)
{ }

void SplayNodeBase::SetLeft(SplayNodeBase *node) throw()
{
	m_left = node;
	if (node)
		node->m_parent = this;
}

void SplayNodeBase::SetRight(SplayNodeBase *node) throw()
{
	m_right = node;
	if (node)
		node->m_parent = this;
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
