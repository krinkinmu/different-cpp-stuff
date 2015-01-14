#include "treenode.hpp"

void wrap_node(struct TreeNode *node)
{
	node->left = node;
	node->right = node;
	node->parent = 0;
}

void add_left(struct TreeNode *left, struct TreeNode *parent)
{
	left->parent = parent;
	parent->left = left;
}

void add_right(struct TreeNode *right, struct TreeNide *parent)
{
	right->parent = parent;
	parent->right = right;
}

struct TreeNode *left_most(struct TreeNode *node)
{
	while (node->left)
		node = node->left;
	return node;
}

struct TreeNode *right_most(struct TreeNode *node)
{
	while (node->right)
		node = node->right;
	return node;
}

static inline int is_left(struct TreeNode *node)
{ return node->parent && node->parent->left == node; }

static inline int is_right(struct TreeNode *node)
{ return node->parent && node->parent->right == node; }

struct TreeNode *next_node(struct TreeNode *node)
{
	if (node->right)
		return left_most(node->right);

	while (is_right(node))
		node = node->parent;

	return node->parent;
}

struct TreeNode *prev_node(struct TreeNode *node)
{
	if (node->left)
		return right_most(node->left);

	while (is_left(node))
		node = node->parent;

	return node->parent;
}
