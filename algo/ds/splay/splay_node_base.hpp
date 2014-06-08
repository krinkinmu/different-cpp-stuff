#ifndef __SPLAY_HPP__
#define __SPLAY_HPP__

struct SplayNodeBase
{
	SplayNodeBase *m_parent;
	SplayNodeBase *m_left;
	SplayNodeBase *m_right;

	SplayNodeBase() throw();
};

void SplaySetLeft(SplayNodeBase *parent, SplayNodeBase *child) throw();
void SplaySetRight(SplayNodeBase *parent, SplayNodeBase *child) throw();

SplayNodeBase const *SplayLeftMost(SplayNodeBase const *node) throw();
SplayNodeBase *SplayLeftMost(SplayNodeBase *node) throw();
SplayNodeBase const *SplayRightMost(SplayNodeBase const *node) throw();
SplayNodeBase *SplayRightMost(SplayNodeBase *node) throw();

SplayNodeBase const *SplaySucc(SplayNodeBase const *node) throw();
SplayNodeBase *SplaySucc(SplayNodeBase *node) throw();
SplayNodeBase const *SplayPred(SplayNodeBase const *node) throw();
SplayNodeBase *SplayPred(SplayNodeBase *node) throw();

void Splay(SplayNodeBase *node, SplayNodeBase *root) throw();
void SplayErase(SplayNodeBase *node, SplayNodeBase *root) throw();

#endif /*__SPLAY_HPP__*/
