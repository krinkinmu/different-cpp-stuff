#ifndef __SPLAY_NODE_HPP__
#define __SPLAY_NODE_HPP__

#include "splay_node_base.hpp"

template <typename T>
struct SplayNode : public SplayNodeBase
{
	T m_value;
};

#endif /*__SPLAY_NODE_HPP__*/
