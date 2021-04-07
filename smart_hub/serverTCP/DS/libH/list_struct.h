#ifndef __LIST_STRUCT_H__
#define __LIST_STRUCT_H__


typedef struct Node Node;

struct Node
{
	void* m_data;
	Node *m_next;
	Node *m_prev;
};

struct List
{
	Node m_head;
	Node m_tail;
};


#endif /* __LIST_STRUCT_H__ */

