#ifndef __SPLAY_HPP__
#define __SPLAY_HPP__

struct SplayNodeBase
{
	SplayNodeBase *m_parent;
	SplayNodeBase *m_left;
	SplayNodeBase *m_right;

	SplayNodeBase() throw();
	void SetLeft(SplayNodeBase *child) throw();
	void SetRight(SplayNodeBase *child) throw();
};

SplayNodeBase const *SplayLeftMost(SplayNodeBase const *node) throw();
SplayNodeBase *SplayLeftMost(SplayNodeBase *node) throw();
SplayNodeBase const *SplayRightMost(SplayNodeBase const *node) throw();
SplayNodeBase *SplayRightMost(SplayNodeBase *node) throw();

SplayNodeBase const *SplaySucc(SplayNodeBase const *node) throw();
SplayNodeBase *SplaySucc(SplayNodeBase *node) throw();
SplayNodeBase const *SplayPred(SplayNodeBase const *node) throw();
SplayNodeBase *SplayPred(SplayNodeBase *node) throw();

#endif /*__SPLAY_HPP__*/
