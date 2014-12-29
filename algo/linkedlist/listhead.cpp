#include "listhead.hpp"

void init_list_head(struct ListHead *head)
{
	head->next = head;
	head->prev = head;
}


static inline void insert_between(struct ListHead *new_node,
					struct ListHead *before,
					struct ListHead *after)
{
	before->next = new_node;
	new_node->prev = before;
	after->prev = new_node;
	new_node->next = after;
}

void insert_before(struct ListHead *new_node, struct ListHead *pos)
{ insert_between(new_node, pos->prev, pos); }


static inline void remove_between(struct ListHead *before,
					struct ListHead *after)
{
	before->next = after;
	after->prev = before;
}

void remove_from_list(struct ListHead *node)
{ remove_between(node->prev, node->next); }
