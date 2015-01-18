#include "treenode.hpp"

void wrap_node(struct TreeNode *node)
{
	node->left = node;
	node->right = node;
	node->parent = node;
}

void add_left(struct TreeNode *left, struct TreeNode *parent)
{
	left->parent = parent;
	parent->left = left;
}

void add_right(struct TreeNode *right, struct TreeNode *parent)
{
	right->parent = parent;
	parent->right = right;
}

struct TreeNode const *left_most(struct TreeNode const *node)
{
	while (node->left != node)
		node = node->left;
	return node;
}

struct TreeNode const *right_most(struct TreeNode const *node)
{
	while (node->right != node)
		node = node->right;
	return node;
}

static inline int is_left(struct TreeNode const *node)
{ return node->parent != node && node->parent->left == node; }

static inline int is_right(struct TreeNode const *node)
{ return node->parent != node && node->parent->right == node; }

struct TreeNode const *next_node(struct TreeNode const *node)
{
	if (node->right != node)
		return left_most(node->right);

	while (is_right(node))
		node = node->parent;

	return node->parent;
}

struct TreeNode const *prev_node(struct TreeNode const *node)
{
	if (node->left != node)
		return right_most(node->left);

	while (is_left(node))
		node = node->parent;

	return node->parent;
}
