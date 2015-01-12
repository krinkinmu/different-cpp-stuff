#ifndef __LIST_HEAD_HPP__
#define __LIST_HEAD_HPP__

/**
 * Circular doubly linked list with a dummy head element.
 *
 * Notes: this part is extracted from template to reduce generated code size,
 * also this is usefull C language library (very minimalistic library).
 **/

struct ListHead {
	struct ListHead *next;
	struct ListHead *prev;
};

/**
 * Initialize empty circular dobly linked list head.
 **/
void init_list_head(struct ListHead *head);

/**
 * Inserts new_node before pos in a linked list.
 **/
void insert_before(struct ListHead *new_node, struct ListHead *pos);

/**
 * Removes node from linked list. node must not be a dummy head element.
 **/
void remove_from_list(struct ListHead *node);

/**
 * Reverses linked list with specified node as a dummy head element.
 **/
void reverse_list(struct ListHead *head);

#endif /*__LIST_HEAD_HPP__*/
