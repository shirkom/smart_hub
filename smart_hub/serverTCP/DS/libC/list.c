#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../libH/list.h"
#include "../libH/list_struct.h"


static void Push(Node *_back, Node *_node);
static ListResult ParaCheckPush(List *_list, void* _item);
static void Pop(Node *_node);
static ListResult ParaCheckPop(List *_list, void** _pItem);

List *ListCreate()
{
	List *list;
	
	if (NULL == (list = (List *) malloc(sizeof(List))))
	{
		return NULL;
	}
	list->m_head.m_next=&list->m_tail;
	list->m_head.m_prev=&list->m_head;
	list->m_tail.m_next=&list->m_tail;
	list->m_tail.m_prev=&list->m_head;
	
	return list;
}

void ListDestroy(List** _list, void (*_elementDestroy)(void* _item))
{
	Node *temp,*temp1;
	
	if(NULL == _list  || NULL == *_list )
	{
		return;
	}
	
	temp =(*_list)->m_head.m_next;
	while (temp != &((*_list)->m_tail))
	{
		temp1=temp->m_next;
		if (NULL != (*_elementDestroy))
		{
			(*_elementDestroy)(temp->m_data);
		}
		free(temp);
		temp = temp1;
	}
	free(*_list);
	*_list = NULL;
}

ListResult ListPushHead(List *_list, void* _item)
{
	Node *node;
	ListResult check;
	
	if (LIST_SUCCESS != (check = ParaCheckPush(_list, _item)))
	{
		return check;
	}

	if (NULL == (node = (Node*) malloc(sizeof(Node))))
	{
		return LIST_ALLOCATION_ERROR;
	}
	Push(&(_list->m_head),node);
	node->m_data = _item;
	
	return LIST_SUCCESS;
}

ListResult ListPushTail(List *_list, void* _item)
{
	Node *node;
	ListResult check;
	
	if (LIST_SUCCESS != (check = ParaCheckPush(_list, _item)))
	{
		return check;
	}

	if (NULL == (node = (Node*) malloc(sizeof(Node))))
	{
		return LIST_ALLOCATION_ERROR;
	}
	Push(_list->m_tail.m_prev,node);
	node->m_data = _item;
	
	return LIST_SUCCESS;
}

ListResult ListPopHead(List* _list, void** _pItem)
{
	ListResult check;
	
	if(LIST_SUCCESS != (check = ParaCheckPop(_list, _pItem)))
	{
		return check;
	}
	
	*_pItem=_list->m_head.m_next->m_data;
	Pop(_list->m_head.m_next);
	
	return LIST_SUCCESS;	
}

ListResult ListPopTail(List *_list, void** _pItem)
{
	ListResult check;
	
	if(LIST_SUCCESS != (check = ParaCheckPop(_list, _pItem)))
	{
		return check;
	}
	
	*_pItem=_list->m_tail.m_prev->m_data;
	Pop(_list->m_tail.m_prev);
	
	return LIST_SUCCESS;
}

size_t ListSize(const List* _list)
{
	size_t count = 0;
	Node *temp;
	
	if (_list == NULL)
	{
		return 0;
	}
	
	temp=_list->m_head.m_next;
	while (temp != &(_list->m_tail))
	{
		count++;
		temp=temp->m_next;
	}
	
	return count;
}
static ListResult ParaCheckPop(List *_list, void** _pItem)
{
	if (_list == NULL)
	{
		return LIST_UNINITIALIZED_ERROR;
	}
	if (_pItem == NULL)
	{
		return LIST_UNINITIALIZED_ITEM_ERROR;
	}
	if (_list->m_head.m_next == &(_list->m_tail))
	{
		return LIST_UNDERFLOW_ERROR;
	}
	
	return LIST_SUCCESS;
}


static ListResult ParaCheckPush(List *_list, void* _item)
{
	if (_list == NULL)
	{
		return LIST_UNINITIALIZED_ERROR;
	}
	if (_item == NULL)
	{
		return LIST_UNINITIALIZED_ITEM_ERROR;
	}
	
	return LIST_SUCCESS;
}

static void Push(Node *_back, Node *_node)
{
	_node->m_next = _back->m_next;
	_node->m_prev = _back;
	_back->m_next->m_prev = _node;
	_back->m_next = _node;
}

static void Pop(Node *_node)
{
	_node->m_prev->m_next=_node->m_next;
	_node->m_next->m_prev=_node->m_prev;
	free(_node);
}


