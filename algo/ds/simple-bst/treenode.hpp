#ifndef __TREE_NODE_HPP__
#define __TREE_NODE_HPP__

struct TreeNode {
	struct TreeNode *parent;
	struct TreeNode *left;
	struct TreeNode *right;
};

void wrap_node(struct TreeNode *node);

void add_left(struct TreeNode *left, struct TreeNode *parent);

void add_right(struct TreeNode *right, struct TreeNode *parent);

int is_left(struct TreeNode const *node);

int is_right(struct TreeNode const *right);

struct TreeNode const *left_most(struct TreeNode const *node);

struct TreeNode const *right_most(struct TreeNode const *node);

struct TreeNode const *next_node(struct TreeNode const *node);

struct TreeNode const *prev_node(struct TreeNode const*node);

#endif /*__TREE_NODE_HPP__*/
